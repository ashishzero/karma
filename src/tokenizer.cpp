#include "tokenizer.h"

bool is_numeral(u32 codepoint) {
	return codepoint >= '0' && codepoint <= '9';
}

bool is_hex_numeral(u32 codepoint) {
	return (codepoint >= '0' && codepoint <= '9') || (codepoint >= 'a' && codepoint <= 'f') || (codepoint >= 'A' && codepoint <= 'F');
}

bool is_oct_numeral(u32 codepoint) {
	return (codepoint >= '0' && codepoint <= '7');
}

bool is_binary_numeral(u32 codepoint) {
	return codepoint == '0' || codepoint == '1';
}

bool parse_integer(const String string, s64* out) {
	if (!string) return false;

	s64 index = 0;

	u8 code = string[index];
	s64 sign = code == '-' ? -1 : 1;
	if (code == '-' || code == '+') index += 1;

	if (index >= string.count) return false;

	s32 base = 10;

	if (string[index] == '0') {
		index += 1;

		if (index < string.count) {
			switch (string[index]) {
				case 'x': index += 1; base = 16; break;
				case 'h': index += 1; base = 16; break;
				case 'd': index += 1; base = 10; break;
				case 'o': index += 1; base = 8; break;
				case 'b': index += 1; base = 2; break;
			}
		}
	}

	s64 value = 0;
	s32 one = 0;

	for (; index < string.count; index += 1) {
		code = string[index];

		if (is_numeral(code)) {
			if ((base == 10 || base == 16) || 
				(is_oct_numeral(code) && base == 8) || 
				(is_binary_numeral(code) && base == 2))
				value = value * base + (code - '0');
			else
				break;
		} else if (base == 16) {
			if (code >= 'a' && code <= 'f') {
				value = value * base + (code - 'a') + 10;
			} else if (code >= 'A' && code <= 'F') {
				value = value * base + (code - 'A') + 10;
			} else {
				break;
			}
		} else {
			break;
		}

	}

	*out = sign * value;
	return index >= string.count;
}

bool parse_real(const String string, r64* out) {
	if (!string) return false;

	s64 index = 0;

	u8 code = string[index];
	r64 sign = code == '-' ? -1 : 1;
	if (code == '-' || code == '+') index += 1;

	bool invalid_num = (index >= string.count);

	r64 value = 0;

	for (; index < string.count; index += 1) {
		code = string[index];

		if (code >= '0' && code <= '9')
			value = value * 10 + (code - '0');
		else
			break;
	}

	bool period_fail = false;
	if (index < string.count && code == '.') {
		r64 p = 1, s = 0;
		
		for (index += 1; index < string.count; index += 1) {
			code = string[index];
			if (code >= '0' && code <= '9')
				s = s * 10 + (code - '0');
			else
				break;
			p *= 10;
		}

		value += s / p;
		period_fail = (s == 0);
	}

	bool exponent_fail = false;
	if (index < string.count && (code == 'e' || code == 'E')) {
		index += 1;

		if (index < string.count) {
			code = string[index];
			bool negative = (code == '-');

			if (code == '-' || code == '+') index += 1;

			exponent_fail = (index >= string.count);

			r64 exp = 0;
			for (; index < string.count; index += 1) {
				code = string[index];
				if (code >= '0' && code <= '9')
					exp = exp * 10 + (code - '0');
				else
					break;
			}

			r64 p = pow(10, exp);

			if (negative) value /= p; else value *= p;
		} else {
			exponent_fail = true;
		}
	}

	if (index < string.count && (code == 'f' || code == 'F')) index += 1;

	*out = sign * value;
	bool result = !(invalid_num || period_fail || exponent_fail || (index < string.count));
	return result;
}



const char* token_kind_string(Token_Kind kind) {
	static const char* strings[] = {
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

Lexer lexer_init(String content) {
	Lexer lexer = {};
	lexer.content = content;
	lexer.row = 1;
	lexer.column = 1;

	string_iter_next(content, lexer.at + 0);
	lexer.at[1] = lexer.at[0];
	string_iter_next(content, lexer.at + 1);
	lexer.at[2] = lexer.at[1];
	string_iter_next(content, lexer.at + 2);

	lexer.processing = true;
	lexer.error_count = 0;

	return lexer;
}

bool lexer_can_continue(Lexer* lexer) {
	return lexer->processing;
}

bool lexer_pass(Lexer* lexer) {
	return lexer->error_count == 0;
}

int lexer_errors(Lexer* lexer) {
	return lexer->error_count;
}

bool lexer_max_error_reached(Lexer* lexer) {
	return lexer->error_count == LEXER_MAX_ERROR;
}

void lexer_clear_errors(Lexer* lexer) {
	lexer->error_count = 0;
}

void lexer_error(Lexer* lexer, Lexer_Error_Kind kind, Token_Kind expected, String content, Token got) {
	if (lexer->error_count != LEXER_MAX_ERROR) {
		lexer->error[lexer->error_count].kind = kind;
		lexer->error[lexer->error_count].expected = expected;
		lexer->error[lexer->error_count].content = content;
		lexer->error[lexer->error_count].got = got;
		lexer->error_count += 1;
	}
}

void lexer_error(Lexer* lexer, Lexer_Error_Kind kind, Token_Kind expected, Token got) {
	static String content("", 0);
	lexer_error(lexer, kind, expected, content, got);
}

void lexer_advance(Lexer* lexer, int count) {
	if (lexer->processing) {
		for (int i = 0; i < count; ++i) {
			lexer->column += 1;
			lexer->at[0] = lexer->at[1];
			lexer->at[1] = lexer->at[2];
			if (!string_iter_next(lexer->content, lexer->at + 2))
				lexer->at[2] = {};

			if (lexer->at[0].codepoint.size_in_bytes == 0) lexer->processing = false;
		}
	}
}

bool lexer_check_and_advance_newline(Lexer* lexer) {
	if (lexer->at[0].codepoint.code == '\r' || lexer->at[0].codepoint.code == '\n') {
		if (lexer->at[0].codepoint.code == '\r' && lexer->at[1].codepoint.code == '\n') {
			lexer_advance(lexer, 1);
		}
		lexer_advance(lexer, 1);

		lexer->column = 1;
		lexer->row += 1;

		return true;
	}

	return false;
}

bool lexer_check_and_advance_comments(Lexer* lexer) {
	if (lexer->at[0].codepoint.code == '/') {
		if (lexer->at[1].codepoint.code == '/') {
			lexer_advance(lexer, 2);

			// single line comment
			while (lexer_can_continue(lexer)) {
				if (lexer_check_and_advance_newline(lexer)) break;
				lexer_advance(lexer, 1);
			}

			return true;
		} else if (lexer->at[1].codepoint.code == '*') {
			lexer_advance(lexer, 2);

			// comment block
			bool comment_ended = false;
			while (lexer_can_continue(lexer)) {
				if (lexer_check_and_advance_newline(lexer)) continue;

				if (lexer->at[0].codepoint.code == '*' && lexer->at[1].codepoint.code == '/') {
					lexer_advance(lexer, 2);
					comment_ended = true;
					break;
				}

				lexer_advance(lexer, 1);
			}

			Token token;
			token.kind = Token_Kind_NONE;
			token.row = lexer->row;
			token.column = lexer->column;
			token.content = String(lexer->content.data + lexer->at[0].index, lexer->content.count - lexer->at[0].index);
			token.value = Value();

			if (!comment_ended) lexer_error(lexer, Lexer_Error_Kind_COMMENT_BLOCK_NO_END, Token_Kind_NONE, token);

			return true;
		}
	}

	return false;
}

Token lexer_next_token(Lexer* lexer) {
	Token token;
	token.kind = Token_Kind_NONE;

	s64 index = lexer->at[0].index;
	token.column = lexer->column;
	token.row = lexer->row;
	s64 count = 0;

	while (lexer_can_continue(lexer)) {
		if (lexer_check_and_advance_comments(lexer) || lexer_check_and_advance_newline(lexer)) {
			if (count) break;

			index = lexer->at[0].index;
			token.column = lexer->column;
			token.row = lexer->row;
			continue;
		}

		auto a = lexer->at[0].codepoint.code;
		auto b = lexer->at[1].codepoint.code;
		auto c = lexer->at[2].codepoint.code;

		if (lexer->at[0].codepoint.size_in_bytes == 1) {

			if (a == '\f' || a == ' ' || a == '\t') {
				lexer_advance(lexer, 1);
				
				if (count) break;

				index = lexer->at[0].index;
				token.column = lexer->column;
				token.row = lexer->row;
				continue;
			}

			if (count == 0) {
				int tok_counts = 0;

				if (b == '=') {

					switch (a) {
						case '=': token.kind = Token_Kind_COMPARE_EQUALS; tok_counts = 2; break;
						case '!': token.kind = Token_Kind_COMPARE_NOT_EQUALS; tok_counts = 2; break;
						case '<': token.kind = Token_Kind_COMPARE_LESS_THAN_EQUALS; tok_counts = 2; break;
						case '>': token.kind = Token_Kind_COMPARE_GREATER_THAN_EQUALS; tok_counts = 2; break;
						case '+': token.kind = Token_Kind_PLUS_EQUALS; tok_counts = 2; break;
						case '-': token.kind = Token_Kind_MINUS_EQUALS; tok_counts = 2; break;
						case '*': token.kind = Token_Kind_MUL_EQUALS; tok_counts = 2; break;
						case '/': token.kind = Token_Kind_DIV_EQUALS; tok_counts = 2; break;
						case '%': token.kind = Token_Kind_MOD_EQUALS; tok_counts = 2; break;
						case '&': token.kind = Token_Kind_AND_EQUALS; tok_counts = 2; break;
						case '|': token.kind = Token_Kind_OR_EQUALS; tok_counts = 2; break;
						case '^': token.kind = Token_Kind_XOR_EQUALS; tok_counts = 2; break;
					}

				} else if (a == '&' && b == '&') {
					token.kind = Token_Kind_LOGICAL_AND;
					tok_counts = 2;
				} else if (a == '|' && b == '|') {
					token.kind = Token_Kind_LOGICAL_OR;
					tok_counts = 2;
				} else if (a == '<' && b == '<') {
					token.kind = Token_Kind_LEFT_SHIFT;
					tok_counts = 2;
				} else if (a == '>' && b == '>') {
					token.kind = Token_Kind_RIGHT_SHIFT;
					tok_counts = 2;
				} else if (a == '+' && b == '+') {
					token.kind = Token_Kind_PLUS_PLUS;
					tok_counts = 2;
				} else if (a == '-' && b == '-') {
					token.kind = Token_Kind_MINUS_MINUS;
					tok_counts = 2;
				} else if (a == '-' && b == '>') {
					token.kind = Token_Kind_DASH_ARROW;
					tok_counts = 2;
				} else if (a == '=' && b == '>') {
					token.kind = Token_Kind_EQUAL_ARROW;
					tok_counts = 2;
				} else if (a == ':' && b == ':') {
					token.kind = Token_Kind_DOUBLE_COLON;
					tok_counts = 2;
				} else {
					if (a == '+' || a == '-' || a == '.') {
						if (is_numeral(b)) {
							count = 2;
							lexer_advance(lexer, 2);
							continue;
						} else if (b == '.' && is_numeral(c)) {
							count = 3;
							lexer_advance(lexer, 3);
							continue;
						}
					}

				}

				if (tok_counts) {
					count = tok_counts;
					token.value = String(lexer->content.data + index, count);
					lexer_advance(lexer, tok_counts);
					break;
				}

			}

			Token_Kind kind = Token_Kind_NONE;

			switch (a) {
				case '@': kind = Token_Kind_AT; break;
				case '#': kind = Token_Kind_HASH; break;
				case '$': kind = Token_Kind_DOLLAR; break;
				case '(': kind = Token_Kind_OPEN_BRACKET; break;
				case ')': kind = Token_Kind_CLOSE_BRACKET; break;
				case '{': kind = Token_Kind_OPEN_CURLY_BRACKET; break;
				case '}': kind = Token_Kind_CLOSE_CURLY_BRACKET; break;
				case '[': kind = Token_Kind_OPEN_SQUARE_BRACKET; break;
				case ']': kind = Token_Kind_CLOSE_SQUARE_BRACKET; break;
				case ':': kind = Token_Kind_COLON; break;
				case ';': kind = Token_Kind_SEMICOLON; break;
				case ',': kind = Token_Kind_COMMA; break;
				case '.': kind = Token_Kind_PERIOD; break;
				case '?': kind = Token_Kind_QUESTION_MARK; break;
				case '~': kind = Token_Kind_TILDE; break;
				case '`': kind = Token_Kind_BACK_TICK; break;
				case '!': kind = Token_Kind_EXCLAMATION; break;
				case '%': kind = Token_Kind_PERCENT; break;
				case '^': kind = Token_Kind_CARET; break;
				case '|': kind = Token_Kind_PIPE; break;
				case '&': kind = Token_Kind_AMPERSAND; break;
				case '*': kind = Token_Kind_ASTRICK; break;
				case '=': kind = Token_Kind_EQUALS; break;
				case '<': kind = Token_Kind_LESS_THAN; break;
				case '>': kind = Token_Kind_GREATER_THAN; break;
				case '+': kind = Token_Kind_PLUS; break;
				case '-': kind = Token_Kind_MINUS; break;
				case '/': kind = Token_Kind_SLASH; break;
				case '\\': kind = Token_Kind_BACK_SLASH; break;
			}

			if (kind != Token_Kind_NONE) {
				if (!count) {
					count = 1;
					token.kind = kind;
					token.value = lexer->at[0].codepoint;
					lexer_advance(lexer, 1);
				}
				break;
			}

			if (a == '"' || a == '\'') {
				if (count) {
					break;
				}

				token.kind = ((a == '"') ? Token_Kind_DQ_STRING : Token_Kind_SQ_STRING);

				bool end_found = false;
				count = 1;
				lexer_advance(lexer, 1);

				while (lexer_can_continue(lexer)) {
					if (lexer_check_and_advance_newline(lexer)) break;
					count += 1;

					if (lexer->at[0].codepoint.code == '\\') {
						count += 1;
						lexer_advance(lexer, 2);
						continue;
					}

					if (lexer->at[0].codepoint.code == a) {
						lexer_advance(lexer, 1);
						end_found = true;
						break;
					}

					lexer_advance(lexer, 1);
				}

				if (!end_found) lexer_error(lexer, Lexer_Error_Kind_NO_MATCHING_PARENTHESIS, token.kind, token);
				token.value = String(lexer->content.data + index + 1, count - 2);

				break;
			}

		}

		count += lexer->at[0].codepoint.size_in_bytes;
		lexer_advance(lexer, 1);
	}

	token.content = String(lexer->content.data + index, count);

	if (token.kind == Token_Kind_NONE) {
		
		if (count) {
			r64 rval; s64 ival;
			if (parse_integer(token.content, &ival)) {
				token.kind = Token_Kind_INTEGER_LITERAL;
				token.value = ival;
			} else if (parse_real(token.content, &rval)) {
				token.kind = Token_Kind_REAL_LITERAL;
				token.value = rval;
			} else {
				if (string_match(token.content, "true")) {
					token.kind = Token_Kind_INTEGER_LITERAL;
					token.value = (s64)1;
				} else if (string_match(token.content, "false")) {
					token.kind = Token_Kind_INTEGER_LITERAL;
					token.value = (s64)0;
				} else {
					token.kind = Token_Kind_IDENTIFIER;
					token.value = token.content;
				}
			}
		} else {
			token.kind = Token_Kind_END_OF_STREAM;
			token.content = String(lexer->content.data + lexer->content.count, 0);
			token.value = token.content;
		}

	}

	return token;
}

Token lexer_require_token(Lexer* lexer, Token_Kind kind) {
	Token token = lexer_next_token(lexer);
	if (token.kind != kind) {
		lexer_error(lexer, Lexer_Error_Kind_INVALID_TOKEN_KIND, kind, token);
	}
	return token;
}

Token lexer_require_identifier(Lexer* lexer, String id) {
	auto token = lexer_require_token(lexer, Token_Kind_IDENTIFIER);

	bool fail = true;
	if (lexer_pass(lexer)) {
		if (string_match(token.content, id))
			fail = false;
	}

	if (fail) {
		lexer_error(lexer, Lexer_Error_Kind_INVALID_TOKEN_KIND, Token_Kind_IDENTIFIER, id, token);
	}

	return token;
}
