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

bool parse_integer(const String string, s64 *out) {
	if (!string.count) return false;

	s64 index = 0;

	u8  code = string[index];
	s64 sign = code == '-' ? -1 : 1;
	if (code == '-' || code == '+') index += 1;

	if (index >= string.count) return false;

	s32 base = 10;

	if (string[index] == '0') {
		index += 1;

		if (index < string.count) {
			switch (string[index]) {
				case 'x':
					index += 1;
					base = 16;
					break;
				case 'h':
					index += 1;
					base = 16;
					break;
				case 'd':
					index += 1;
					base = 10;
					break;
				case 'o':
					index += 1;
					base = 8;
					break;
				case 'b':
					index += 1;
					base = 2;
					break;
			}
		}
	}

	s64 value = 0;
	s32 one   = 0;

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

bool parse_real(const String string, r64 *out, bool *found_period, bool *found_exponential) {
	*found_period = false;
	*found_exponential = false;

	if (!string.count) return false;

	s64 index = 0;

	u8  code = string[index];
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
		*found_period = true;
		s32 loop_count = 0;

		for (index += 1; index < string.count; index += 1) {
			code = string[index];
			if (code >= '0' && code <= '9')
				s = s * 10 + (code - '0');
			else
				break;
			p *= 10;
			loop_count += 1;
		}

		value += s / p;
		period_fail = (loop_count == 0);
	}

	bool exponent_fail = false;
	if (index < string.count && (code == 'e' || code == 'E')) {
		*found_exponential = true;
		index += 1;

		if (index < string.count) {
			code          = string[index];
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

			if (negative)
				value /= p;
			else
				value *= p;
		} else {
			exponent_fail = true;
		}
	}

	if (index < string.count && (code == 'f' || code == 'F')) index += 1;

	*out        = sign * value;
	bool result = !(invalid_num || period_fail || exponent_fail || (index < string.count));
	return result;
}

struct Tokenizer_State {
	String              content;
	ptrsize             row;
	ptrsize             column;
	String_Iter         at[3];
	bool                can_continue;
	Tokenization_Status status;
};

void tokenizer_error(Tokenizer_State *tokenizer, Tokenization_Result result, ptrsize row, ptrsize column, ptrsize offset) {
	tokenizer->status.result = result;
	tokenizer->status.row    = row;
	tokenizer->status.column = column;
	tokenizer->status.offset = offset;
	tokenizer->can_continue  = false;
}

void tokenizer_advance(Tokenizer_State *tokenizer, int count) {
	if (tokenizer->can_continue) {
		for (int i = 0; i < count; ++i) {
			tokenizer->column += 1;
			tokenizer->at[0] = tokenizer->at[1];
			tokenizer->at[1] = tokenizer->at[2];
			if (!string_iter_next(tokenizer->content, tokenizer->at + 2))
				tokenizer->at[2] = {};

			if (tokenizer->at[0].codepoint.size_in_bytes == 0) tokenizer->can_continue = false;
		}
	}
}

bool tokenizer_check_and_advance_newline(Tokenizer_State *tokenizer) {
	if (tokenizer->at[0].codepoint.code == '\r' || tokenizer->at[0].codepoint.code == '\n') {
		if (tokenizer->at[0].codepoint.code == '\r' && tokenizer->at[1].codepoint.code == '\n') {
			tokenizer_advance(tokenizer, 1);
		}
		tokenizer_advance(tokenizer, 1);

		tokenizer->column = 1;
		tokenizer->row += 1;

		return true;
	}

	return false;
}

bool tokenizer_check_and_advance_comments(Tokenizer_State *tokenizer) {
	if (tokenizer->at[0].codepoint.code == '/') {
		if (tokenizer->at[1].codepoint.code == '/') {
			tokenizer_advance(tokenizer, 2);

			// single line comment
			while (tokenizer->can_continue) {
				if (tokenizer_check_and_advance_newline(tokenizer)) break;
				tokenizer_advance(tokenizer, 1);
			}

			return true;
		} else if (tokenizer->at[1].codepoint.code == '*') {
			tokenizer_advance(tokenizer, 2);

			ptrsize err_row    = tokenizer->row;
			ptrsize err_col    = tokenizer->column;
			s64     err_offset = tokenizer->at[0].index;

			// comment block
			bool comment_ended = false;
			while (tokenizer->can_continue) {
				if (tokenizer_check_and_advance_newline(tokenizer)) continue;

				if (tokenizer->at[0].codepoint.code == '*' && tokenizer->at[1].codepoint.code == '/') {
					tokenizer_advance(tokenizer, 2);
					comment_ended = true;
					break;
				}

				tokenizer_advance(tokenizer, 1);
			}

			Token token;
			token.kind    = Token_Kind_NONE;
			token.row     = tokenizer->row;
			token.column  = tokenizer->column;
			token.content = String(tokenizer->content.data + tokenizer->at[0].index, tokenizer->content.count - tokenizer->at[0].index);
			token.value   = Value();

			if (!comment_ended) tokenizer_error(tokenizer, Tokenization_Result_ERROR_COMMENT_BLOCK_NO_END, err_row, err_col, err_offset);

			return true;
		}
	}

	return false;
}

Token tokenizer_next_token(Tokenizer_State *tokenizer) {
	Token token;
	token.kind = Token_Kind_NONE;

	s64 index    = tokenizer->at[0].index;
	token.column = tokenizer->column;
	token.row    = tokenizer->row;
	s64 count    = 0;

	while (tokenizer->can_continue) {
		if (tokenizer_check_and_advance_comments(tokenizer) || tokenizer_check_and_advance_newline(tokenizer)) {
			if (count) break;

			index        = tokenizer->at[0].index;
			token.column = tokenizer->column;
			token.row    = tokenizer->row;
			continue;
		}

		auto a = tokenizer->at[0].codepoint.code;
		auto b = tokenizer->at[1].codepoint.code;
		auto c = tokenizer->at[2].codepoint.code;

		if (tokenizer->at[0].codepoint.size_in_bytes == 1) {
			if (a == '\f' || a == ' ' || a == '\t') {
				tokenizer_advance(tokenizer, 1);

				if (count) break;

				index        = tokenizer->at[0].index;
				token.column = tokenizer->column;
				token.row    = tokenizer->row;
				continue;
			}

			if (count == 0) {
				int tok_counts = 0;

				if (b == '=') {
					switch (a) {
						case '=':
							token.kind = Token_Kind_COMPARE_EQUALS;
							tok_counts = 2;
							break;
						case '!':
							token.kind = Token_Kind_COMPARE_NOT_EQUALS;
							tok_counts = 2;
							break;
						case '<':
							token.kind = Token_Kind_COMPARE_LESS_THAN_EQUALS;
							tok_counts = 2;
							break;
						case '>':
							token.kind = Token_Kind_COMPARE_GREATER_THAN_EQUALS;
							tok_counts = 2;
							break;
						case '+':
							token.kind = Token_Kind_PLUS_EQUALS;
							tok_counts = 2;
							break;
						case '-':
							token.kind = Token_Kind_MINUS_EQUALS;
							tok_counts = 2;
							break;
						case '*':
							token.kind = Token_Kind_MUL_EQUALS;
							tok_counts = 2;
							break;
						case '/':
							token.kind = Token_Kind_DIV_EQUALS;
							tok_counts = 2;
							break;
						case '%':
							token.kind = Token_Kind_MOD_EQUALS;
							tok_counts = 2;
							break;
						case '&':
							token.kind = Token_Kind_AND_EQUALS;
							tok_counts = 2;
							break;
						case '|':
							token.kind = Token_Kind_OR_EQUALS;
							tok_counts = 2;
							break;
						case '^':
							token.kind = Token_Kind_XOR_EQUALS;
							tok_counts = 2;
							break;
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
							tokenizer_advance(tokenizer, 2);
							continue;
						} else if (a != '.' && b == '.' && is_numeral(c)) {
							count = 3;
							tokenizer_advance(tokenizer, 3);
							continue;
						}
					}
				}

				if (tok_counts) {
					count       = tok_counts;
					token.value = String(tokenizer->content.data + index, count);
					tokenizer_advance(tokenizer, tok_counts);
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
				case '\0': kind = Token_Kind_NULL; break;
			}

			if (kind != Token_Kind_NONE) {
				if (kind == Token_Kind_PERIOD && is_numeral(b)) {
					kind = Token_Kind_NONE;
				} else if (!count) {
					count       = 1;
					token.kind  = kind;
					token.value = tokenizer->at[0].codepoint;
					tokenizer_advance(tokenizer, 1);
					break;
				} else {
					break;
				}
			}

			if (a == '"' || a == '\'') {
				if (count) {
					break;
				}

				token.kind = ((a == '"') ? Token_Kind_DQ_STRING : Token_Kind_SQ_STRING);

				s64 err_offset = tokenizer->at[0].index;

				bool end_found = false;
				count          = 1;
				tokenizer_advance(tokenizer, 1);

				while (tokenizer->can_continue) {
					if (tokenizer_check_and_advance_newline(tokenizer)) break;
					count += 1;

					if (tokenizer->at[0].codepoint.code == '\\') {
						count += 1;
						tokenizer_advance(tokenizer, 2);
						continue;
					}

					if (tokenizer->at[0].codepoint.code == a) {
						tokenizer_advance(tokenizer, 1);
						end_found = true;
						break;
					}

					tokenizer_advance(tokenizer, 1);
				}

				if (!end_found) tokenizer_error(tokenizer, Tokenization_Result_ERROR_NO_MATCHING_PARENTHESIS, token.row, token.column, err_offset);
				token.value = String(tokenizer->content.data + index + 1, count - 2);

				break;
			}
		}

		count += tokenizer->at[0].codepoint.size_in_bytes;
		tokenizer_advance(tokenizer, 1);
	}

	token.content = String(tokenizer->content.data + index, count);

	if (token.kind == Token_Kind_NONE) {
		if (count) {
			r64 rval;
			s64 ival;
			bool found_period, found_exponential;
			if (parse_real(token.content, &rval, &found_period, &found_exponential)) {
				if (!found_period && !found_exponential) {
					if (parse_integer(token.content, &ival)) {
						token.kind = Token_Kind_INTEGER_LITERAL;
						token.value = ival;
					} else {
						token.kind = Token_Kind_REAL_LITERAL;
						token.value = rval;
					}
				} else {
					token.kind = Token_Kind_REAL_LITERAL;
					token.value = rval;
				}
			} else if (parse_integer(token.content, &ival)) {
				token.kind = Token_Kind_INTEGER_LITERAL;
				token.value = ival;
			} else {
				if (string_match(token.content, "true")) {
					token.kind  = Token_Kind_INTEGER_LITERAL;
					token.value = (s64)1;
				} else if (string_match(token.content, "false")) {
					token.kind  = Token_Kind_INTEGER_LITERAL;
					token.value = (s64)0;
				} else {
					token.kind  = Token_Kind_IDENTIFIER;
					token.value = token.content;
				}
			}
		} else {
			token.kind    = Token_Kind_END_OF_STREAM;
			token.content = String(tokenizer->content.data + tokenizer->content.count, 0);
			token.value   = token.content;
		}
	}

	return token;
}

Array_View<Token> tokenize(String string, Tokenization_Status *status) {
	Tokenizer_State tokenizer = {};
	tokenizer.content         = string;
	tokenizer.row             = 1;
	tokenizer.column          = 1;

	string_iter_next(string, tokenizer.at + 0);
	tokenizer.at[1] = tokenizer.at[0];
	string_iter_next(string, tokenizer.at + 1);
	tokenizer.at[2] = tokenizer.at[1];
	string_iter_next(string, tokenizer.at + 2);

	tokenizer.can_continue  = true;
	tokenizer.status.result = Tokenization_Result_SUCCESS;

	Array<Token> tokens;
	while (tokenizer.can_continue) {
		auto tkn = tokenizer_next_token(&tokenizer);
		array_add(&tokens, tkn);
	}

	*status = tokenizer.status;
	return tokens;
}
