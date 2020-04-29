#include "stream.h"
#include "stb_sprintf.h"

Istream istream(void *ptr, size_t length) {
	Istream res;
	res.base    = (u8 *)ptr;
	res.end     = res.base + length;
	res.current = res.base;
	return res;
}

Istream istream(Array_View<u8> buffer) {
	Istream result;
	result = istream(buffer.data, buffer.count);
	return result;
}

void *istream_consume_size(Istream *b, size_t size) {
	assert(b->current + size <= b->end);
	void *ptr = b->current;
	b->current += size;
	return ptr;
}

String istream_consume_line(Istream *b) {
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

bool istream_eof(Istream *b) {
	return b->current >= b->end;
}

//
//
//

void ostream_write_buffer(Ostream *stream, const void *ptr, s64 size) {
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

void ostream_write_formatted(Ostream *stream, const char *fmt, ...) {
	char working_buffer[STB_SPRINTF_MIN];

	va_list ap;
	va_start(ap, fmt);
	stbsp_vsprintfcb([](char const *buf, void *user, int len) -> char * {
		ostream_write_buffer((Ostream *)user, buf, len);
		return (char *)buf;
	},
					 stream, working_buffer, fmt, ap);
	va_end(ap);
}

String ostream_build_string(Ostream *stream, bool null_terminate) {
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

void ostream_free(Ostream *stream) {
	assert(stream->tail->next == 0);

	for (auto buk = stream->tail; buk != &stream->head;) {
		auto prev = buk->prev;
		mfree(buk, stream->allocator);
		buk = prev;
	}

	stream->tail        = &stream->head;
	stream->head.filled = 0;
}
