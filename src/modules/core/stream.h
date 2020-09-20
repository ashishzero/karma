#pragma once

#include "karma.h"
#include "systems.h"

#if 0
// TODO: remove or clean this up!
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
#endif

//
//
//

struct Istream {
	u8 *base;
	u8 *end;
	u8 *current;
};

Istream istream(void *ptr, size_t length);
Istream istream(Array_View<u8> buffer);
void *  istream_consume_size(Istream *b, size_t size);
String  istream_consume_line(Istream *b);
#define istream_consume(stream, type) (type *)istream_consume_size(stream, sizeof(type))
bool istream_eof(Istream *b);

//
//
//

constexpr int OSTREAM_BUCKET_SIZE = 16384;

struct Ostream {
	struct Bucket {
		u8  data[OSTREAM_BUCKET_SIZE];
		s64 filled = 0;

		Bucket *prev = 0;
		Bucket *next = 0;
	};

	Bucket  head;
	Bucket *tail = &head;
	s64     size = 0;

	Allocator allocator = context.allocator;
};

void ostream_write_buffer(Ostream *stream, const void *ptr, s64 size);
template <typename T>
void ostream_write(Ostream *stream, const T &v) {
	ostream_write_buffer(stream, (void *)&v, sizeof(T));
}
void   ostream_write_formatted(Ostream *stream, const char *fmt, ...);
s64    ostream_get_size(Ostream *stream);
template <typename Procedure>
void ostream_builder(Ostream *stream, const Procedure &procedure, void *user_context) {
	for (auto buk = &stream->head; buk != 0; buk = buk->next) {
		procedure(buk->data, buk->filled, user_context);
	}
}
String ostream_build_string(Ostream *stream, bool null_terminate = false);
void   ostream_build_out_file(Ostream *stream, System_File *file);
void   ostream_free(Ostream *stream);
