#include "audio.h"
#include "modules/core/stream.h"
#include "modules/imgui/dev.h"

void audio_mixer(Audio_Mixer *mixer) {
	circular_linked_list_init(&mixer->audio_list);

	mixer->samples_rate = system_audio_sample_rate();
	mixer->channel_count = system_audio_channel_count();

	// TODO: We only support these currently
	assert(mixer->samples_rate == 48000);
	assert(mixer->channel_count == 2);

	mixer->mutex = system_create_mutex();
	mixer->buffer_size_in_sample_count = (mixer->samples_rate * SYSTEM_AUDIO_BUFFER_SIZE_IN_MILLISECS) / 1000;
	mixer->buffer_size_in_bytes = sizeof(r32) * mixer->buffer_size_in_sample_count * mixer->channel_count;
	mixer->buffer = (r32 *)tallocate(mixer->buffer_size_in_bytes);
	mixer->buffer_consumed_bytes = 0;

	mixer->volume_a = 0.5f;
	mixer->volume_b = 0.5f;
	mixer->volume_position = 1;
	mixer->volume_span = 1;
	mixer->pitch_factor = 1;
}

void audio_mixer_set_volume(Audio_Mixer *mixer, r32 volume) {
	mixer->volume_a = mixer->volume_b = volume;
}

void audio_mixer_animate_volume_to(Audio_Mixer *mixer, r32 volume, r32 secs) {
	mixer->volume_a = lerp(mixer->volume_a, mixer->volume_b, clamp01(mixer->volume_position / mixer->volume_span));
	mixer->volume_b = volume;
	mixer->volume_position = 0;
	mixer->volume_span = mixer->samples_rate * secs;
}

void audio_mixer_animate_volume(Audio_Mixer *mixer, r32 volume_a, r32 volume_b, r32 secs) {
	mixer->volume_a = volume_a;
	mixer->volume_b = volume_b;
	mixer->volume_position = 0;
	mixer->volume_span = mixer->samples_rate * secs;
}

Audio *audio_mixer_add_audio(Audio_Mixer *mixer, Audio_Stream *stream, bool start_playing, bool loop) {
	auto node = circular_linked_list_add(&mixer->audio_list);

	node->data.stream = stream;
	node->data.sample_count = stream->data.size / sizeof(s16) / stream->fmt.channels_count;
	node->data.remove = false;
	node->data.playing = start_playing;
	node->data.loop = loop;
	node->data.attenuation = 0;
	node->data.pitch_factor = 1;
	node->data.volume = 1;
	node->data.dvolume = 0;
	node->data.samples_played = 0;
	node->data.buffered_pitch_factor = 1;

	return &node->data;
}

void audio_mixer_remove_audio(Audio_Mixer *mixer, Audio *audio) {
	audio->remove = true;
}

void system_refresh_audio_device(u32 sample_rate, u32 channel_count, void *user_data) {
	Audio_Mixer *mixer = (Audio_Mixer *)user_data;

	system_lock_mutex(mixer->mutex, WAIT_INFINITE);

	mixer->samples_rate = sample_rate;
	mixer->channel_count = channel_count;

	// TODO: We only support these currently
	assert(mixer->samples_rate == 48000);
	assert(mixer->channel_count == 2);

	mixer->buffer_size_in_sample_count = (mixer->samples_rate * SYSTEM_AUDIO_BUFFER_SIZE_IN_MILLISECS) / 1000;
	mixer->buffer_size_in_bytes = sizeof(r32) * mixer->buffer_size_in_sample_count * mixer->channel_count;
	mixer->buffer = (r32 *)tallocate(mixer->buffer_size_in_bytes);
	mixer->buffer_consumed_bytes = 0;

	system_unlock_mutex(mixer->mutex);
}

void system_update_audio(const System_Audio &sys_audio, void *user_data) {
	Audio_Mixer *mixer = (Audio_Mixer *)user_data;
	system_lock_mutex(mixer->mutex, WAIT_INFINITE);

	// TODO: Don't hard-code this!, Query and use!!
	u32 channel_count = 2;

	u32 sample_count;
	u8 *sys_buffer = sys_audio.lock(sys_audio.handle, &sample_count);
	if (sys_buffer) {
		u32 buffer_size = sizeof(r32) * channel_count * sample_count;
		u8 *src_ptr = (u8 *)mixer->buffer + mixer->buffer_consumed_bytes;
		u8 *one_past_end = (u8 *)mixer->buffer + mixer->buffer_size_in_bytes;
		assert(one_past_end >= src_ptr);
		u32 copy_size = minimum((u32)(one_past_end - src_ptr), buffer_size);
		memcpy(sys_buffer, src_ptr, copy_size);
		memset(sys_buffer + copy_size, 0, buffer_size - copy_size);
		sys_audio.unlock(sys_audio.handle, sample_count);

		Dev_AudioFeedback((r32 *)src_ptr, copy_size, channel_count, buffer_size - copy_size);

		mixer->buffer_consumed_bytes += copy_size;
		mixer->volume_position += sample_count * mixer->pitch_factor;

		for_list(Audio, ptr, &mixer->audio_list) {
			Audio &audio = ptr->data;

			if (audio.playing) {
				audio.volume -= audio.dvolume * (r32)sample_count;
				if (audio.volume <= 0) {
					audio.playing = false;
					audio.samples_played = 0;
				} else {
					audio.samples_played += audio.buffered_pitch_factor * (r32)sample_count;
					while ((u32)lroundf(audio.samples_played) >= audio.sample_count) {
						audio.samples_played -= audio.sample_count;
						audio.playing = audio.loop && !audio.remove;
					}
					if (!audio.playing) {
						audio.samples_played = 0;
					}
				}
			}
		}
	}

	system_unlock_mutex(mixer->mutex);
}

void audio_mixer_update(Audio_Mixer *mixer) {
	constexpr r32 INVERSE_RANGE_S16 = 2.0f / (r32)((r32)MAX_INT16 - (r32)MIN_INT16);

	system_lock_mutex(mixer->mutex, WAIT_INFINITE);

	// TODO: Fix me!! Don't hardcode this!!!!!!
	u32 channel_count = 2;
	memset(mixer->buffer, 0, sizeof(r32) * channel_count * mixer->buffer_size_in_sample_count);
	mixer->buffer_consumed_bytes = 0;

	for_list(Audio, ptr, &mixer->audio_list) {
		assert(ptr->data.stream->fmt.channels_count == 2);

		if (ptr->data.playing) {
			Audio &audio = ptr->data;
			audio.buffered_pitch_factor = audio.pitch_factor * mixer->pitch_factor;

			r32 attenuation = 1.0f / (1.0f + audio.attenuation);

			r32 *write_ptr = mixer->buffer;
			r32  read_cursor = audio.samples_played;
			r32  volume_position = mixer->volume_position;

			for (u32 sample_counter = 0; sample_counter < mixer->buffer_size_in_sample_count;) {
				u32 more_samples_required = (mixer->buffer_size_in_sample_count - sample_counter);
				u32 samples_available = (u32)floorf(((r32)audio.sample_count - read_cursor) / audio.buffered_pitch_factor);
				u32 samples_to_mix = minimum(samples_available, more_samples_required);

				r32 effective_volume;

				for (u32 sample_mix_index = 0; sample_mix_index < samples_to_mix; ++sample_mix_index) {
					// TODO: Here we expect both input audio and output buffer to be stero audio

					//#define AUDIO_APPLY_PITCH_FILTERING

					#ifdef AUDIO_APPLY_PITCH_FILTERING

					r32 real_sample_index = read_cursor + sample_mix_index * audio.buffered_pitch_factor;
					u32 sample_index_0 = (u32)(real_sample_index);
					assert(sample_index_0 < audio.stream->sample_count);
					u32 sample_index_1 = (sample_index_0 + 1 == audio.sample_count) ? sample_index_0 : sample_index_0 + 1;

					r32 sampled_left_0 = INVERSE_RANGE_S16 * ((r32)audio.stream->samples[2 * sample_index_0 + 0] - (r32)MIN_INT16) - 1.0f;
					r32 sampled_right_0 = INVERSE_RANGE_S16 * ((r32)audio.stream->samples[2 * sample_index_0 + 1] - (r32)MIN_INT16) - 1.0f;

					r32 sampled_left_1 = INVERSE_RANGE_S16 * ((r32)audio.stream->samples[2 * sample_index_1 + 0] - (r32)MIN_INT16) - 1.0f;
					r32 sampled_right_1 = INVERSE_RANGE_S16 * ((r32)audio.stream->samples[2 * sample_index_1 + 1] - (r32)MIN_INT16) - 1.0f;

					r32 sample_t = real_sample_index - (r32)sample_index_0;

					effective_volume = attenuation * maximum(audio.volume - audio.dvolume * (r32)sample_counter, 0.0f) * lerp(mixer->volume_a, mixer->volume_b, clamp01((volume_position + sample_mix_index * mixer->pitch_factor) / mixer->volume_span));

					write_ptr[0] += lerp(sampled_left_0, sampled_left_1, sample_t) * effective_volume;
					write_ptr[1] += lerp(sampled_right_0, sampled_right_1, sample_t) * effective_volume;

					#else

					u32 sample_index = lroundf(read_cursor + sample_mix_index * audio.buffered_pitch_factor);
					assert(sample_index < audio.sample_count);

					r32 sampled_left = INVERSE_RANGE_S16 * ((r32)audio.stream->samples[2 * sample_index + 0] - (r32)MIN_INT16) - 1.0f;
					r32 sampled_right = INVERSE_RANGE_S16 * ((r32)audio.stream->samples[2 * sample_index + 1] - (r32)MIN_INT16) - 1.0f;

					effective_volume = attenuation * maximum(audio.volume - audio.dvolume * sample_counter, 0.0f) * lerp(mixer->volume_a, mixer->volume_b, clamp01((volume_position + sample_mix_index * mixer->pitch_factor) / mixer->volume_span));

					write_ptr[0] += sampled_left * effective_volume;
					write_ptr[1] += sampled_right * effective_volume;

					#endif

					write_ptr[0] = mmclamp(-1.0f, 1.0f, write_ptr[0]);
					write_ptr[1] = mmclamp(-1.0f, 1.0f, write_ptr[1]);

					write_ptr += channel_count;
				}

				r32 samples_progressed = samples_to_mix * audio.buffered_pitch_factor;

				sample_counter += samples_to_mix;
				read_cursor += samples_progressed;
				volume_position += samples_progressed;

				if (!audio.loop) {
					break;
				} else {
					while ((u32)lroundf(read_cursor) >= audio.sample_count) {
						read_cursor -= audio.sample_count;
					}
				}
			}
		} else if (ptr->data.remove) {
			ptr = circular_linked_list_loop_remove(&mixer->audio_list, ptr);
		}
	}

	system_unlock_mutex(mixer->mutex);
}

void audio_pitch_factor(Audio *audio, r32 pitch) {
	audio->pitch_factor = pitch;
}

void audio_pause(Audio *audio) {
	audio->playing = false;
}

void audio_stop(Audio *audio) {
	audio->playing = false;
	audio->samples_played = 0;
}

void audio_vanish(Audio *audio, r32 secs) {
	if (audio->playing) {
		r32 samples = (r32)audio->stream->fmt.sample_rate * secs;
		audio->dvolume = 1.0f / samples;
	}
}

void audio_play(Audio *audio) {
	audio->playing = true;
}

void audio_play(Audio *audio, r32 volume) {
	audio->playing = true;
	audio->volume = volume;
}

void audio_reset(Audio *audio) {
	audio->samples_played = 0;
}
