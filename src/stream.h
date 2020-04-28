#pragma once

#include "karma.h"
#include "string.h"
#include "stb_sprintf.h"

inline u8 *move_ahead(u8 **ptr, int bytes) {
	u8 *r = *ptr;
	*ptr  = *ptr + bytes;
	return r;
}
inline u8 get_u8(u8 **ptr) {
	return *move_ahead(ptr, 1);
}
inline s8 get_s8(u8 **ptr) {
	return *((s8 *)move_ahead(ptr, 1));
}
inline u16 get_u16_be(u8 **ptr) {
	return bswap16p_be(move_ahead(ptr, 2));
}
inline u32 get_u32_be(u8 **ptr) {
	return bswap32p_be(move_ahead(ptr, 4));
}
inline u64 get_u64_be(u8 **ptr) {
	return bswap64p_be(move_ahead(ptr, 8));
}
inline s16 get_s16_be(u8 **ptr) {
	auto r = bswap16p_be(move_ahead(ptr, 2));
	return *((s16 *)&r);
}
inline s32 get_s32_be(u8 **ptr) {
	auto r = bswap32p_be(move_ahead(ptr, 4));
	return *((s32 *)&r);
}
inline s64 get_s64_be(u8 **ptr) {
	auto r = bswap64p_be(move_ahead(ptr, 8));
	return *((s64 *)&r);
}

inline u16 get_u16_le(u8 **ptr) {
	return bswap16p_le(move_ahead(ptr, 2));
}
inline u32 get_u32_le(u8 **ptr) {
	return bswap32p_le(move_ahead(ptr, 4));
}
inline u64 get_u64_le(u8 **ptr) {
	return bswap64p_le(move_ahead(ptr, 8));
}
inline s16 get_s16_le(u8 **ptr) {
	auto r = bswap16p_le(move_ahead(ptr, 2));
	return *((s16 *)&r);
}
inline s32 get_s32_le(u8 **ptr) {
	auto r = bswap32p_le(move_ahead(ptr, 4));
	return *((s32 *)&r);
}
inline s64 get_s64_le(u8 **ptr) {
	auto r = bswap64p_le(move_ahead(ptr, 8));
	return *((s64 *)&r);
}

//
//
//

struct Istream {
	u8 *base;
	u8 *end;
	u8 *current;
};

inline Istream istream(void *ptr, size_t length) {
	Istream res;
	res.base    = (u8 *)ptr;
	res.end     = res.base + length;
	res.current = res.base;
	return res;
}

inline Istream istream(Array_View<u8> buffer) {
	Istream result;
	result = istream(buffer.data, buffer.count);
	return result;
}

inline void *istream_consume_size(Istream *b, size_t size) {
	assert(b->current + size <= b->end);
	void *ptr = b->current;
	b->current += size;
	return ptr;
}
#define istream_consume(stream, type) (type *)istream_consume_size(stream, sizeof(type))

inline bool istream_eof(Istream *b) {
	return b->current >= b->end;
}

inline String istream_consume_line(Istream *b) {
	String string;

	u8 *ptr    = b->current;
	s64 length = 0;
	while (!istream_eof(b)) {
		auto c = *istream_consume(b, u8);
		if (c == '\n' || c == '\r') break;
		length += 1;
	}

	string.data  = ptr;
	string.count = length;

	return string;
}

//
//
//

constexpr int OSTREAM_BUCKET_SIZE = 4096;

struct Ostream {
	struct Bucket {
		u8  data[OSTREAM_BUCKET_SIZE];
		s64 filled = 0;

		Bucket *prev = 0;
		Bucket *next = 0;
	};

	Bucket  head;
	Bucket *tail = &head;

	Allocator allocator = context.allocator;
};

inline void ostream_write_buffer(Ostream *stream, const void *ptr, s64 size) {
	s64 left = size;
	u8 *data = (u8 *)ptr;

	while (left >= OSTREAM_BUCKET_SIZE - stream->tail->filled) {
		auto write = OSTREAM_BUCKET_SIZE - stream->tail->filled;
		memcpy(stream->tail->data + stream->tail->filled, data + size - left, write);
		stream->tail->filled += write;
		left -= write;
		stream->tail->next       = new (stream->allocator) Ostream::Bucket;
		stream->tail->next->prev = stream->tail;
		stream->tail             = stream->tail->next;
	}

	if (left) {
		memcpy(stream->tail->data + stream->tail->filled, data + size - left, left);
		stream->tail->filled += left;
	}
}

template <typename T>
inline void ostream_write(Ostream *stream, const T &v) {
	ostream_write_buffer(stream, (void *)&v, sizeof(T));
}

inline void ostream_write_formatted(Ostream *stream, const char *fmt, ...) {
	char working_buffer[512]; // TODO: This MUST equal STB_SPRINTF_MIN, move this to length_string.cpp

	va_list ap;
	va_start(ap, fmt);
	stbsp_vsprintfcb([](char const *buf, void *user, int len) -> char * {
		ostream_write_buffer((Ostream *)user, buf, len);
		return (char *)buf;
	},
					 stream, working_buffer, fmt, ap);
	va_end(ap);
}

inline String ostream_build_string(Ostream *stream, bool null_terminate = false) {
	s64 count = 0;

	for (auto buk = &stream->head; buk != 0; buk = buk->next) {
		count += buk->filled;
	}

	String string;
	string.data  = new u8[count + (s64)(null_terminate)];
	string.count = 0;

	for (auto buk = &stream->head; buk != 0; buk = buk->next) {
		memcpy(string.data + string.count, buk->data, buk->filled);
		string.count += buk->filled;
	}

	if (null_terminate) string.data[count] = 0;

	return string;
}

inline void ostream_free(Ostream *stream) {
	assert(stream->tail->next == 0);

	for (auto buk = stream->tail; buk != &stream->head;) {
		auto prev = buk->prev;
		mfree(buk, stream->allocator);
		buk = prev;
	}

	stream->tail        = &stream->head;
	stream->head.filled = 0;
}
