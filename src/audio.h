#pragma once
#include "karma.h"

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

enum Audio_Channel {
	Audio_Channel_LEFT,
	Audio_Channel_RIGHT,

	Audio_Channel_COUNT
};


//
//
//

struct Audio_Stream {
	Riff_Header *header; // TODO: do we need this?
	Wave_Fmt	*fmt;	 //	TODO: do we need this?
	Wave_Data	*data;	 // TODO: do we need this?
	s16			*samples;
	u32			sample_count;
};

Audio_Stream load_wave(String content);

//
//
//