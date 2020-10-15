#pragma once
#include "karma.h"
#include "reflection.h"
#include "length_string.h"
#include "stream.h"

#define swap_by_exchange(a, b, t) (t = a, a = b, b = t)

template <typename Type>
inline void swap(Type *a, Type *b) {
	Type t = *a;
	*a     = *b;
	*b     = t;
}

template <typename Type>
inline void swap_vec(Type *a, Type *b, s64 n) {
	if (n > 0) {
		Type t;
		for (; n > 0; a += 1, b += 1, n -= 1)
			swap_by_exchange(*a, *b, t);
	}
}

template <typename Type, typename Compare>
inline void sort_insert(Type *a, s64 n, Compare cmp) {
	int i, j;
	for (i = 1; i < n; ++i) {
		Type t = a[i];
		j      = i - 1;
		for (; j >= 0 && cmp(a[j], t) > 0; --j) {
			swap(a + j, a + j + 1);
		}
		a[j + 1] = t;
	}
}

// NOTE: *left* is inclusive but *right* is exclusive
template <typename T, typename Compare>
void merge_buttom_up(T *src, s64 left, s64 mid, s64 right, T *dst, Compare comp) {
	s64 i = left, j = mid;
	for (s64 k = left; k < right; k++) {
		if (i < mid && (j >= right || comp(src[i], src[j]) <= 0)) {
			dst[k] = src[i];
			i += 1;
		} else {
			dst[k] = src[j];
			j += 1;
		}
	}
}

template <typename T, typename Compare>
void sort_merge(T *src, T *aux, s64 n, Compare comp) {
	T *t;
	for (s64 width = 1; width < n; width = 2 * width) {
		for (s64 i = 0; i < n; i = i + 2 * width) {
			merge_buttom_up(src, i, minimum(i + width, n), minimum(i + 2 * width, n), aux, comp);
		}
		swap_by_exchange(src, aux, t);
	}
}

template <typename T, typename Compare>
void heap_make(T *root, s64 height, Compare comp) {
	while (height != 0) {
		int parent = (height - 1) / 2;
		if (comp(root[parent], root[height]) > 0) {
			swap(root + parent, root + height);
			height = parent;
		} else {
			break;
		}
	}
}

template <typename T, typename Compare>
void heap_push(T *root, s64 height, const T &node, Compare comp) {
	root[height] = node;
	heap_make(root, height, comp);
}

template <typename T, typename Compare>
void heap_pop(T *root, s64 height, Compare comp) {
	assert(height != 0);

	height -= 1;
	root[0] = root[height];

	s64 current = 0;
	s64 left    = 2 * current + 1;

	while (left < height) {
		s64 index = left;

		s64 right = 2 * current + 2;
		if (right < height) {
			if (comp(root[index], root[right]) > 0) {
				index = right;
			}
		}

		if (comp(root[current], root[index]) > 0) {
			swap(root + current, root + index);
			current = index;
		} else {
			break;
		}

		left = 2 * current + 1;
	}
}

template <typename T, typename Compare>
void sort_heap(T *a, s64 n, Compare comp) {
	for (s64 i = 1; i < n; ++i) {
		heap_make(a, i, comp);
	}
}

//
//
//

u32 murmur3_32(const void *ptr, size_t len, u32 seed);

//
//
//

enum Value_Kind {
	Value_Kind_NONE,
	Value_Kind_REAL,
	Value_Kind_INTEGER,
	Value_Kind_CODEPOINT,
	Value_Kind_STRING,
};

struct Value {
	Value_Kind kind;

	r64             real;
	s64             integer;
	Utf32_Codepoint codepoint;
	String          string;

	inline Value() {
		kind = Value_Kind_NONE;
	}

	inline Value(r64 data) {
		kind = Value_Kind_REAL;
		real = data;
	}

	inline Value(s64 data) {
		kind = Value_Kind_INTEGER;
		integer = data;
	}

	inline Value(Utf32_Codepoint data) {
		kind = Value_Kind_CODEPOINT;
		codepoint = data;
	}

	inline Value(String data) {
		kind = Value_Kind_STRING;
		string = data;
	}
};

bool is_numeral(u32 codepoint);
bool is_hex_numeral(u32 codepoint);
bool is_oct_numeral(u32 codepoint);
bool is_binary_numeral(u32 codepoint);
bool parse_integer(const String string, s64 *out);
bool parse_real(const String string, r64 *out);

enum Token_Kind {
	Token_Kind_NONE = 0,

	Token_Kind_AT,
	Token_Kind_HASH,
	Token_Kind_DOLLAR,
	Token_Kind_OPEN_BRACKET,
	Token_Kind_CLOSE_BRACKET,
	Token_Kind_SLASH,
	Token_Kind_OPEN_CURLY_BRACKET,
	Token_Kind_CLOSE_CURLY_BRACKET,
	Token_Kind_OPEN_SQUARE_BRACKET,
	Token_Kind_CLOSE_SQUARE_BRACKET,
	Token_Kind_COLON,
	Token_Kind_SEMICOLON,
	Token_Kind_COMMA,
	Token_Kind_PERIOD,
	Token_Kind_QUESTION_MARK,
	Token_Kind_TILDE,
	Token_Kind_BACK_TICK,

	Token_Kind_EXCLAMATION,
	Token_Kind_PERCENT,
	Token_Kind_CARET,
	Token_Kind_AMPERSAND,
	Token_Kind_ASTRICK,
	Token_Kind_PLUS,
	Token_Kind_MINUS,
	Token_Kind_EQUALS,
	Token_Kind_UNDERSCORE,
	Token_Kind_PIPE,
	Token_Kind_BACK_SLASH,
	Token_Kind_NULL,
	Token_Kind_LESS_THAN,
	Token_Kind_GREATER_THAN,

	Token_Kind_COMPARE_EQUALS,
	Token_Kind_COMPARE_NOT_EQUALS,
	Token_Kind_COMPARE_LESS_THAN_EQUALS,
	Token_Kind_COMPARE_GREATER_THAN_EQUALS,
	Token_Kind_LOGICAL_AND,
	Token_Kind_LOGICAL_OR,
	Token_Kind_LEFT_SHIFT,
	Token_Kind_RIGHT_SHIFT,
	Token_Kind_PLUS_PLUS,
	Token_Kind_MINUS_MINUS,
	Token_Kind_PLUS_EQUALS,
	Token_Kind_MINUS_EQUALS,
	Token_Kind_MUL_EQUALS,
	Token_Kind_DIV_EQUALS,
	Token_Kind_MOD_EQUALS,
	Token_Kind_AND_EQUALS,
	Token_Kind_OR_EQUALS,
	Token_Kind_XOR_EQUALS,
	Token_Kind_DASH_ARROW,
	Token_Kind_EQUAL_ARROW,
	Token_Kind_DOUBLE_COLON,

	Token_Kind_DQ_STRING,
	Token_Kind_SQ_STRING,

	Token_Kind_REAL_LITERAL,
	Token_Kind_INTEGER_LITERAL,

	Token_Kind_IDENTIFIER,

	Token_Kind_END_OF_STREAM
};

struct Token {
	Token_Kind kind;
	String     content;
	ptrsize    row;
	ptrsize    column;
	Value      value;
};

enum Tokenization_Result {
	Tokenization_Result_SUCCESS,
	Tokenization_Result_ERROR_COMMENT_BLOCK_NO_END,
	Tokenization_Result_ERROR_NO_MATCHING_PARENTHESIS
};

struct Tokenization_Status {
	Tokenization_Result result;
	ptrsize             row;
	ptrsize             column;
	s64                 offset;
};

Array_View<Token> tokenize(String string, Tokenization_Status *status);

//
//
//

struct Deserialize_Error_Info {
	Token token;
	Token_Kind expected;
	char string[512];
};

void serialize_fmt_text(Ostream *out, String name, const Type_Info *info, char *data, s64 num_of_elements = 1, bool is_array = false);
bool deserialize_fmt_text(Array_View<Token> &tokens, String name, const Type_Info *info, char *data, s64 num_of_elements, Deserialize_Error_Info *error);
