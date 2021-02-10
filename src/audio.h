#pragma once
#include "modules/core/karma.h"
#include "modules/core/data_structures.h"
#include "modules/core/systems.h"

#pragma pack(push, 1)

struct Riff_Header {
	u8	id[4];
	u32 size;
	u8	format[4];
};

struct Wave_Fmt {
	u8	id[4];
	u32 size;
	u16 audio_format;
	u16 channels_count;
	u32 sample_rate;
	u32 byte_rate;
	u16 block_align;
	u16 bits_per_sample;
};

struct Wave_Data {
	u8 id[4];
	u32 size;
};

#pragma pack(pop)

//
//
//

struct Audio_Stream {
	Riff_Header header;
	Wave_Fmt	fmt;
	Wave_Data	data;
	s16			samples[1];
};

//
//
//

struct Audio {
	Audio_Stream *stream;
	u32			  sample_count;
	booli		  remove;
	booli		  playing;
	booli         loop;
	r32			  attenuation;
	r32           pitch_factor;
	r32           volume;
	r32			  dvolume;
	r32           samples_played;
	r32           buffered_pitch_factor;
};

using Audio_List = Circular_Linked_List<Audio>;

struct Audio_Mixer {
	u32 samples_rate;
	u32 channel_count;

	Handle     mutex;
	Audio_List audio_list;
	u32        buffer_size_in_sample_count;
	u32        buffer_size_in_bytes;
	u32        buffer_consumed_bytes;
	r32 *buffer;

	r32 volume_a;
	r32 volume_b;
	r32 volume_position; // in samples
	r32 volume_span;     // in samples
	r32 pitch_factor;
};

void audio_mixer(Audio_Mixer *mixer);
void audio_mixer_set_volume(Audio_Mixer *mixer, r32 volume);
void audio_mixer_animate_volume_to(Audio_Mixer *mixer, r32 volume, r32 secs);
void audio_mixer_animate_volume(Audio_Mixer *mixer, r32 volume_a, r32 volume_b, r32 secs);
Audio *audio_mixer_add_audio(Audio_Mixer *mixer, Audio_Stream *stream, bool start_playing, bool loop);
void audio_mixer_remove_audio(Audio_Mixer *mixer, Audio *audio);
void system_refresh_audio_device(u32 sample_rate, u32 channel_count, void *user_data);
void system_update_audio(const System_Audio &sys_audio, void *user_data);
void audio_mixer_update(Audio_Mixer *mixer);

void audio_pitch_factor(Audio *audio, r32 pitch);

void audio_pause(Audio *audio);
void audio_stop(Audio *audio);
void audio_vanish(Audio *audio, r32 secs);
void audio_play(Audio *audio);
void audio_play(Audio *audio, r32 volume);
void audio_reset(Audio *audio);
