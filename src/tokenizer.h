#pragma once

#include "shared.h"
#include "string.h"

enum Value_Kind {
	Value_Kind_NONE,
	Value_Kind_REAL,
	Value_Kind_INTEGER,
	Value_Kind_CODEPOINT,
	Value_Kind_STRING,
};

struct Value {
	Value_Kind kind;

	union {
		r64 real;
		s64 integer;
		Utf32_Codepoint codepoint;
		String string;
	};

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
bool parse_integer(const String string, s64 * out);
bool parse_real(const String string, r64 * out);

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
	Token_Kind	kind;
	String		content;
	ptrsize		row;
	ptrsize		column;
	Value	value;
};

enum Tokenization_Result {
	Tokenization_Result_SUCCESS,
	Tokenization_Result_ERROR_COMMENT_BLOCK_NO_END,
	Tokenization_Result_ERROR_NO_MATCHING_PARENTHESIS
};

struct Tokenization_Status {
	Tokenization_Result result;
	ptrsize				row;
	ptrsize				column;
	s64					offset;
};

Array_View<Token> tokenize(String string, Tokenization_Status * status);

// TODO: Remove this!
inline String enum_string(Token_Kind kind) {
	static String strings[] = {
		"Token_Kind_NONE",

		"Token_Kind_AT",
		"Token_Kind_HASH",
		"Token_Kind_DOLLAR",
		"Token_Kind_OPEN_BRACKET",
		"Token_Kind_CLOSE_BRACKET",
		"Token_Kind_SLASH",
		"Token_Kind_OPEN_CURLY_BRACKET",
		"Token_Kind_CLOSE_CURLY_BRACKET",
		"Token_Kind_OPEN_SQUARE_BRACKET",
		"Token_Kind_CLOSE_SQUARE_BRACKET",
		"Token_Kind_COLON",
		"Token_Kind_SEMICOLON",
		"Token_Kind_COMMA",
		"Token_Kind_PERIOD",
		"Token_Kind_QUESTION_MARK",
		"Token_Kind_TILDE",
		"Token_Kind_BACK_TICK",

		"Token_Kind_EXCLAMATION",
		"Token_Kind_PERCENT",
		"Token_Kind_CARET",
		"Token_Kind_AMPERSAND",
		"Token_Kind_ASTRICK",
		"Token_Kind_PLUS",
		"Token_Kind_MINUS",
		"Token_Kind_EQUALS",
		"Token_Kind_UNDERSCORE",
		"Token_Kind_PIPE",
		"Token_Kind_BACK_SLASH",
		"Token_Kind_LESS_THAN",
		"Token_Kind_GREATER_THAN",

		"Token_Kind_COMPARE_EQUALS",
		"Token_Kind_COMPARE_NOT_EQUALS",
		"Token_Kind_COMPARE_LESS_THAN_EQUALS",
		"Token_Kind_COMPARE_GREATER_THAN_EQUALS",
		"Token_Kind_LOGICAL_AND",
		"Token_Kind_LOGICAL_OR",
		"Token_Kind_LEFT_SHIFT",
		"Token_Kind_RIGHT_SHIFT",
		"Token_Kind_PLUS_PLUS",
		"Token_Kind_MINUS_MINUS",
		"Token_Kind_PLUS_EQUALS",
		"Token_Kind_MINUS_EQUALS",
		"Token_Kind_MUL_EQUALS",
		"Token_Kind_DIV_EQUALS",
		"Token_Kind_MOD_EQUALS",
		"Token_Kind_AND_EQUALS",
		"Token_Kind_OR_EQUALS",
		"Token_Kind_XOR_EQUALS",
		"Token_Kind_DASH_ARROW",
		"Token_Kind_EQUAL_ARROW",
		"Token_Kind_DOUBLE_COLON",

		"Token_Kind_DQ_STRING",
		"Token_Kind_SQ_STRING",

		"Token_Kind_REAL_LITERAL",
		"Token_Kind_INTEGER_LITERAL",

		"Token_Kind_IDENTIFIER",

		"Token_Kind_END_OF_STREAM"
	};

	static_assert(static_count(strings) == Token_Kind_END_OF_STREAM + 1, "Token_Kind and strings array count don't match!");

	return strings[kind];
}
