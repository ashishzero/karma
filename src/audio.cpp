#include "audio.h"
#include "modules/core/stream.h"

Audio_Stream load_wave(String content) {
	Istream in				= istream(content);
	Riff_Header *wav_header = istream_consume(&in, Riff_Header);
	Wave_Fmt	*wav_fmt	= istream_consume(&in, Wave_Fmt);
	Wave_Data	*wav_data	= istream_consume(&in, Wave_Data);
	s16			*data		= (s16 *)istream_consume_size(&in, wav_data->size);

	Audio_Stream stream;
	stream.header		= wav_header;
	stream.fmt			= wav_fmt;
	stream.data			= wav_data;
	stream.samples		= data;
	stream.sample_count	= wav_data->size / sizeof(s16) / wav_fmt->channels_count;

	return stream;
}
