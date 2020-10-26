#include "utility.h"

u32 murmur3_32(const void *ptr, size_t len, u32 seed) {
	const u8 *key = (u8 *)ptr;
	u32       h = seed;
	u32       k;
	/* Read in groups of 4. */
	for (size_t i = len >> 2; i; i--) {
		// Here is a source of differing results across endiannesses.
		// A swap here has no effects on hash properties though.
		k = *((u32 *)key);
		key += sizeof(u32);

		k *= 0xcc9e2d51;
		k = (k << 15) | (k >> 17);
		k *= 0x1b873593;

		h ^= k;
		h = (h << 13) | (h >> 19);
		h = h * 5 + 0xe6546b64;
	}
	/* Read the rest. */
	k = 0;
	for (size_t i = len & 3; i; i--) {
		k <<= 8;
		k |= key[i - 1];
	}
	// A swap is *not* necessary here because the preceeding loop already
	// places the low bytes in the low places according to whatever endianness
	// we use. Swaps only apply when the memory is copied in a chunk.

	k *= 0xcc9e2d51;
	k = (k << 15) | (k >> 17);
	k *= 0x1b873593;
	h ^= k;
	/* Finalize. */
	h ^= len;
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;
	return h;
}

//
//
//

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

bool parse_unsigned_integer(const String string, u64 *out) {
	if (!string.count) return false;

	s64 index = 0;
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

	u64 value = 0;
	u8 code;
	for (; index < string.count; index += 1) {
		code = string[index];

		if (is_numeral(code)) {
			if ((base == 10 || base == 16) ||
				(is_oct_numeral(code) && base == 8) ||
				(is_binary_numeral(code) && base == 2))
				value = value * base + (code - '0');
			else
				break;
		}
		else if (base == 16) {
			if (code >= 'a' && code <= 'f') {
				value = value * base + (code - 'a') + 10;
			}
			else if (code >= 'A' && code <= 'F') {
				value = value * base + (code - 'A') + 10;
			}
			else {
				break;
			}
		}
		else {
			break;
		}
	}

	*out = value;
	return index >= string.count;
}

bool parse_integer(const String string, s64 *out, bool *is_signed) {
	if (!string.count) return false;

	s64 index = 0;

	u8  code = string[index];
	s64 sign = code == '-' ? -1 : 1;
	*is_signed = (sign == -1);
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
		}
		else if (base == 16) {
			if (code >= 'a' && code <= 'f') {
				value = value * base + (code - 'a') + 10;
			}
			else if (code >= 'A' && code <= 'F') {
				value = value * base + (code - 'A') + 10;
			}
			else {
				break;
			}
		}
		else {
			break;
		}
	}

	*out = sign * value;
	return index >= string.count;
}

bool parse_real(const String string, r64 *out, bool *is_signed) {
	if (!string.count) return false;

	s64 index = 0;

	u8  code = string[index];
	r64 sign = code == '-' ? -1 : 1;
	*is_signed = (code == '-');
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

			if (negative)
				value /= p;
			else
				value *= p;
		}
		else {
			exponent_fail = true;
		}
	}

	if (index < string.count && (code == 'f' || code == 'F')) index += 1;

	*out = sign * value;
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
	tokenizer->status.row = row;
	tokenizer->status.column = column;
	tokenizer->status.offset = offset;
	tokenizer->can_continue = false;
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
		}
		else if (tokenizer->at[1].codepoint.code == '*') {
			tokenizer_advance(tokenizer, 2);

			ptrsize err_row = tokenizer->row;
			ptrsize err_col = tokenizer->column;
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
			token.kind = Token_Kind_NONE;
			token.row = tokenizer->row;
			token.column = tokenizer->column;
			token.content = String(tokenizer->content.data + tokenizer->at[0].index, tokenizer->content.count - tokenizer->at[0].index);
			token.value = Value();

			if (!comment_ended) tokenizer_error(tokenizer, Tokenization_Result_ERROR_COMMENT_BLOCK_NO_END, err_row, err_col, err_offset);

			return true;
		}
	}

	return false;
}

Token tokenizer_next_token(Tokenizer_State *tokenizer) {
	Token token;
	token.kind = Token_Kind_NONE;

	s64 index = tokenizer->at[0].index;
	token.column = tokenizer->column;
	token.row = tokenizer->row;
	s64 count = 0;

	while (tokenizer->can_continue) {
		if (tokenizer_check_and_advance_comments(tokenizer) || tokenizer_check_and_advance_newline(tokenizer)) {
			if (count) break;

			index = tokenizer->at[0].index;
			token.column = tokenizer->column;
			token.row = tokenizer->row;
			continue;
		}

		auto a = tokenizer->at[0].codepoint.code;
		auto b = tokenizer->at[1].codepoint.code;
		auto c = tokenizer->at[2].codepoint.code;

		if (tokenizer->at[0].codepoint.size_in_bytes == 1) {
			if (a == '\f' || a == ' ' || a == '\t') {
				tokenizer_advance(tokenizer, 1);

				if (count) break;

				index = tokenizer->at[0].index;
				token.column = tokenizer->column;
				token.row = tokenizer->row;
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

				}
				else if (a == '&' && b == '&') {
					token.kind = Token_Kind_LOGICAL_AND;
					tok_counts = 2;
				}
				else if (a == '|' && b == '|') {
					token.kind = Token_Kind_LOGICAL_OR;
					tok_counts = 2;
				}
				else if (a == '<' && b == '<') {
					token.kind = Token_Kind_LEFT_SHIFT;
					tok_counts = 2;
				}
				else if (a == '>' && b == '>') {
					token.kind = Token_Kind_RIGHT_SHIFT;
					tok_counts = 2;
				}
				else if (a == '+' && b == '+') {
					token.kind = Token_Kind_PLUS_PLUS;
					tok_counts = 2;
				}
				else if (a == '-' && b == '-') {
					token.kind = Token_Kind_MINUS_MINUS;
					tok_counts = 2;
				}
				else if (a == '-' && b == '>') {
					token.kind = Token_Kind_DASH_ARROW;
					tok_counts = 2;
				}
				else if (a == '=' && b == '>') {
					token.kind = Token_Kind_EQUAL_ARROW;
					tok_counts = 2;
				}
				else if (a == ':' && b == ':') {
					token.kind = Token_Kind_DOUBLE_COLON;
					tok_counts = 2;
				}
				else {
					if (a == '+' || a == '-' || a == '.') {
						if (is_numeral(b)) {
							count = 2;
							tokenizer_advance(tokenizer, 2);
							continue;
						}
						else if (a != '.' && b == '.' && is_numeral(c)) {
							count = 3;
							tokenizer_advance(tokenizer, 3);
							continue;
						}
					}
				}

				if (tok_counts) {
					count = tok_counts;
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
				}
				else if (!count) {
					count = 1;
					token.kind = kind;
					token.value = tokenizer->at[0].codepoint;
					tokenizer_advance(tokenizer, 1);
					break;
				}
				else {
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
				count = 1;
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
			u64 uval;
			bool is_signed;
			if (parse_integer(token.content, &ival, &is_signed)) {
				token.kind = Token_Kind_INTEGER_LITERAL;
				if (is_signed)
					uval = (u64)ival;
				else
					parse_unsigned_integer(token.content, &uval);
				token.value = Value(ival, uval, is_signed);
			}
			else if (parse_real(token.content, &rval, &is_signed)) {
				token.kind = Token_Kind_REAL_LITERAL;
				token.value = Value(rval, is_signed);
			}
			else {
				if (string_match(token.content, "true")) {
					token.kind = Token_Kind_INTEGER_LITERAL;
					token.value = Value(1, 1, false);
				}
				else if (string_match(token.content, "false")) {
					token.kind = Token_Kind_INTEGER_LITERAL;
					token.value = Value(0, 0, false);
				}
				else {
					token.kind = Token_Kind_IDENTIFIER;
					token.value = token.content;
				}
			}
		}
		else {
			token.kind = Token_Kind_END_OF_STREAM;
			token.content = String(tokenizer->content.data + tokenizer->content.count, 0);
			token.value = token.content;
		}
	}

	return token;
}

Array_View<Token> tokenize(String string, Tokenization_Status *status) {
	Tokenizer_State tokenizer = {};
	tokenizer.content = string;
	tokenizer.row = 1;
	tokenizer.column = 1;

	string_iter_next(string, tokenizer.at + 0);
	tokenizer.at[1] = tokenizer.at[0];
	string_iter_next(string, tokenizer.at + 1);
	tokenizer.at[2] = tokenizer.at[1];
	string_iter_next(string, tokenizer.at + 2);

	tokenizer.can_continue = true;
	tokenizer.status.result = Tokenization_Result_SUCCESS;

	Array<Token> tokens;
	while (tokenizer.can_continue) {
		auto tkn = tokenizer_next_token(&tokenizer);
		array_add(&tokens, tkn);
	}

	*status = tokenizer.status;
	return tokens;
}

//
//
//

template <typename Type, typename Cast>
void serialize_text_basic(Ostream *out, Type *data, s64 count, const char *fmt) {
	if (count == -1) {
		ostream_write_formatted(out, fmt, (Cast)*data);
	} else {
		ostream_write_formatted(out, "[ ");
		for (s64 i = 0; i < count - 1; ++i) {
			ostream_write_formatted(out, fmt, (Cast)*data);
			ostream_write_formatted(out, ", ");
			data += i;
		}
		ostream_write_formatted(out, fmt, (Cast)*data);
		ostream_write_formatted(out, " ]");
	}
}

void serialize_text_basic(Ostream *out, String *string, s64 count) {
	if (count == -1) {
		ostream_write_formatted(out, "\"");
		ostream_write_buffer(out, string->data, string->count);
		ostream_write_formatted(out, "\"");
	} else {
		ostream_write_formatted(out, "[ ");
		for (s64 i = 0; i < count - 1; ++i) {
			ostream_write_formatted(out, "\"");
			ostream_write_buffer(out, string->data, string->count);
			ostream_write_formatted(out, "\"");
			ostream_write_formatted(out, ", ");
			string += i;
		}
		ostream_write_formatted(out, "\"");
		ostream_write_buffer(out, string->data, string->count);
		ostream_write_formatted(out, "\"");
		ostream_write_formatted(out, " ]");
	}
}

void serialize_fmt_text(Ostream *out, String name, const Type_Info *info, char *data, s64 array_count, int tab_count);

void serialize_text_pointer(Ostream *out, const Type_Info_Pointer *info, char *data, int tab_count) {
	serialize_fmt_text(out, "", info->pointer_to, (char *)(*(ptrsize *)data), -1, tab_count);
}

void serialize_text_enum(Ostream *out, const Type_Info_Enum *info, char *data, int tab_count) {
	serialize_fmt_text(out, "", info->item_type, data, -1, tab_count);
}

void serialize_text_struct(Ostream *out, const Type_Info_Struct *info, char *data, int tab_count) {
	if (info->base) {
		serialize_fmt_text(out, "", info->base, data, -1, tab_count);
	}

	for (ptrsize mem_index = 0; mem_index < info->member_count; ++mem_index) {
		auto mem = info->members + mem_index;
		bool no_serialize = false;

		for (ptrsize attr_index = 0; attr_index < mem->attribute_count; ++attr_index) {
			if (string_match(mem->attributes[attr_index], "no-serialize")) {
				no_serialize = true;
				break;
			}
		}

		if (!no_serialize) {
			if (string_match(mem->name, "anonymous")) {
				serialize_fmt_text(out, "", mem->info, data + mem->offset, -1, tab_count);
			} else {
				ostream_write_formatted(out, "\n");
				serialize_fmt_text(out, mem->name, mem->info, data + mem->offset, -1, tab_count);
			}
		}
	}
}

void serialize_text_union(Ostream *out, const Type_Info_Union *info, char *data, int tab_count) {
	if (info->member_count == 0) return;

	const Union_Member *write_mem = nullptr;

	for (ptrsize mem_index = 0; mem_index < info->member_count; ++mem_index) {
		auto mem = info->members + mem_index;
		for (u64 i = 0; i < mem->attribute_count; ++i) {
			if (string_match(mem->attributes[i], "use")) {
				write_mem = mem;
				break;
			}
		}

		if (write_mem) break;
	}

	if (!write_mem)
		write_mem = info->members;

	if (string_match(write_mem->name, "anonymous")) {
		serialize_fmt_text(out, "", write_mem->info, data, -1, tab_count);
	} else {
		ostream_write_formatted(out, "\n");
		serialize_fmt_text(out, write_mem->name, write_mem->info, data, -1, tab_count);
	}
}

void serialize_text_static_array(Ostream *out, const Type_Info_Static_Array *info, char *data, int tab_count) {
	serialize_fmt_text(out, "", info->type, data, info->count, tab_count);
}

void serialize_text_dynamic_array(Ostream *out, const Type_Info_Dynamic_Array *info, char *data, int tab_count) {
	Array<char> *array = (Array<char> *)data;
	ostream_write_formatted(out, "{ %zd, ", array->count);
	serialize_fmt_text(out, "", info->type, array->data, array->count, tab_count);
	ostream_write_formatted(out, " }");
}

void serialize_text_array_view(Ostream *out, const Type_Info_Array_View *info, char *data, int tab_count) {
	Array_View<char> *array = (Array_View<char> *)data;
	ostream_write_formatted(out, "{ %zd, ", array->count);
	serialize_fmt_text(out, "", info->type, array->data, array->count, tab_count);
	ostream_write_formatted(out, " }");
}

template <typename Cast_Info, typename Proc>
void serialize_text_recursive(Proc proc, Ostream *out, const Type_Info *info, char *data, s64 array_count, int tab_count) {
	if (array_count == -1) {
		proc(out, (Cast_Info *)info, data, tab_count);
	} else {
		ostream_write_formatted(out, "[ ");
		for (s64 i = 0; i < array_count - 1; ++i) {
			proc(out, (Cast_Info *)info, data, tab_count);
			ostream_write_formatted(out, ", ");
			data += info->size;
		}
		proc(out, (Cast_Info *)info, data, tab_count);
		ostream_write_formatted(out, " ]");
	}
}

void serialize_fmt_text(Ostream *out, String name, const Type_Info *info, char *data, s64 array_count, int tab_count) {
	if (name.count) {
		ostream_write_formatted(out, "%*s", tab_count, "");
		ostream_write_buffer(out, name.data, name.count);
		ostream_write_formatted(out, ": ");
		tab_count += 3;
	}

	switch (info->id) {
	case Type_Id_S8 :  serialize_text_basic<s8 , s32>  (out,  (s8   *)data, array_count, "%d");  break;
	case Type_Id_S16:  serialize_text_basic<s16, s32>  (out,  (s16  *)data, array_count, "%d");  break;
	case Type_Id_S32:  serialize_text_basic<s32, s32>  (out,  (s32  *)data, array_count, "%d");  break;
	case Type_Id_S64:  serialize_text_basic<s64, s64>  (out,  (s64  *)data, array_count, "%zd"); break;
	case Type_Id_U8 :  serialize_text_basic<u8 , u32>  (out,  (u8   *)data, array_count, "%u");  break;
	case Type_Id_U16:  serialize_text_basic<u16, u32>  (out,  (u16  *)data, array_count, "%u");  break;
	case Type_Id_U32:  serialize_text_basic<u32, u32>  (out,  (u32  *)data, array_count, "%u");  break;
	case Type_Id_U64:  serialize_text_basic<u64, s64>  (out,  (u64  *)data, array_count, "%zu"); break;
	case Type_Id_R32:  serialize_text_basic<r32, r32>  (out,  (r32  *)data, array_count, "%f");  break;
	case Type_Id_R64:  serialize_text_basic<r64, r64>  (out,  (r64  *)data, array_count, "%lf"); break;
	case Type_Id_CHAR: serialize_text_basic<char, s32> (out,  (char *)data, array_count, "%d");  break;

	case Type_Id_STRING: serialize_text_basic(out, (String *)data, array_count); break;

	case Type_Id_VOID: invalid_code_path(); break;
	case Type_Id_FUNCTION: break;

	case Type_Id_POINTER: {
		serialize_text_recursive<Type_Info_Pointer>(serialize_text_pointer, out, info, data, array_count, tab_count);
	} break;

	case Type_Id_ENUM: {
		serialize_text_recursive<Type_Info_Enum>(serialize_text_enum, out, info, data, array_count, tab_count);
	} break;

	case Type_Id_STRUCT: {
		serialize_text_recursive<Type_Info_Struct>(serialize_text_struct, out, info, data, array_count, tab_count);
	} break;

	case Type_Id_UNION: {
		serialize_text_recursive<Type_Info_Union>(serialize_text_union, out, info, data, array_count, tab_count);
	} break;

	case Type_Id_STATIC_ARRAY: {
		serialize_text_recursive<Type_Info_Static_Array>(serialize_text_static_array, out, info, data, array_count, tab_count);
	} break;

	case Type_Id_DYNAMIC_ARRAY: {
		serialize_text_recursive<Type_Info_Dynamic_Array>(serialize_text_dynamic_array, out, info, data, array_count, tab_count);
	} break;

	case Type_Id_ARRAY_VIEW: {
		serialize_text_recursive<Type_Info_Array_View>(serialize_text_array_view, out, info, data, array_count, tab_count);
	} break;

	invalid_default_case();

	}

	static const Type_Id IDS_NO_NEWLINES[] = {
		Type_Id_ENUM,
		Type_Id_POINTER,
	};

	bool add_new_line = true;
	for (ptrsize index = 0; index < static_count(IDS_NO_NEWLINES); ++index) {
		if (IDS_NO_NEWLINES[index] == info->id) {
			add_new_line = false;
			break;
		}
	}

	if (add_new_line) {
		//ostream_write_formatted(out, "\n");
	}
}

void serialize_fmt_text(Ostream *out, String name, const Type_Info *info, char *data) {
	serialize_fmt_text(out, name, info, data, -1, 0);
}

inline void parsing_error(Deserialize_State *w, Token_Kind kind) {
	w->error.token = *(w->current - 1);
	w->error.expected = kind;
	memset(w->error.string, 0, sizeof(w->error.string));
}

inline void parsing_error(Deserialize_State *w, String string) {
	w->error.token = *(w->current - 1);
	w->error.expected = Token_Kind_IDENTIFIER;
	memcpy(w->error.string, string.data, minimum(string.count, sizeof(w->error.string) - 1));
	w->error.string[minimum(string.count, sizeof(w->error.string))] = 0;
}

inline bool parsing(Deserialize_State *w) {
	return w->current < w->end;
}

inline Token *parse_next(Deserialize_State *w) {
	if (parsing(w)) {
		w->current += 1;
		return w->current - 1;
	}
	return nullptr;
}

inline bool parse_require_token(Deserialize_State *w, Token_Kind kind) {
	auto t = parse_next(w);
	if (t && t->kind == kind)
		return true;
	parsing_error(w, kind);
	return false;
}

inline bool parse_require_token(Deserialize_State *w, String string) {
	auto t = parse_next(w);
	if (t && t->kind == Token_Kind_IDENTIFIER && string_match(string, t->content))
		return true;
	parsing_error(w, string);
	return false;
}

template <typename T>
inline bool parse_require_integral(Deserialize_State *w, T *d, bool is_signed) {
	auto t = parse_next(w);
	if (t && t->kind == Token_Kind_INTEGER_LITERAL) {
		if (is_signed) {
			*d = (T)t->value.integer;
		} else {
			if (!t->value.is_signed) {
				*d = (T)t->value.uinteger;
			} else {
				parsing_error(w, Token_Kind_INTEGER_LITERAL);
				return false;
			}
		}
		return true;
	}
	parsing_error(w, Token_Kind_INTEGER_LITERAL);
	return false;
}

template <typename T>
inline bool parse_require_real(Deserialize_State *w, T *d) {
	auto t = parse_next(w);
	if (t) {
		if (t->kind == Token_Kind_INTEGER_LITERAL) {
			*d = (T)t->value.integer;
			return true;
		}
		else if (t->kind == Token_Kind_REAL_LITERAL) {
			*d = (T)t->value.real;
			return true;
		}
	}
	parsing_error(w, Token_Kind_REAL_LITERAL);
	return false;
}

inline bool parse_require_string(Deserialize_State *w, String *d) {
	auto t = parse_next(w);
	if (t && t->kind == Token_Kind_DQ_STRING) {
		*d = t->value.string;
		return true;
	}
	parsing_error(w, Token_Kind_DQ_STRING);
	return false;
}

template <typename Type, typename Proc, typename ...ExtraParams>
bool parse_basic_array(Proc proc, Deserialize_State *w, Type *data, s64 count, ExtraParams... params) {
	if (count == -1) {
		return proc(w, data, params...);
	} else {
		if (parse_require_token(w, Token_Kind_OPEN_SQUARE_BRACKET)) {
			for (s64 i = 0; i < count - 1; ++i) {
				if (!proc(w, data, params...) ||
					!parse_require_token(w, Token_Kind_COMMA)) {
					return false;
				}
				data += 1;
			}
			return proc(w, data, params...) && parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET);
		}
	}
	return false;
}

bool deserialize_fmt_text(Deserialize_State *w, String name, const Type_Info *info, char *data, s64 array_count);

bool parse_pointer(Deserialize_State *w, const Type_Info_Pointer *info, char *data) {
	(*(ptrsize *)(data)) = (ptrsize)memory_allocate(info->pointer_to->size);
	return deserialize_fmt_text(w, "", info->pointer_to, (char *)(*(ptrsize *)(data)), -1);
}

bool parse_enum(Deserialize_State *w, const Type_Info_Enum *info, char *data) {
	return deserialize_fmt_text(w, "", info->item_type, data, -1);
}

bool parse_struct(Deserialize_State *w, const Type_Info_Struct *info, char *data) {
	if (info->base) {
		if (!deserialize_fmt_text(w, "", info->base, data, -1))
			return false;
	}

	for (ptrsize mem_index = 0; mem_index < info->member_count; ++mem_index) {
		auto mem = info->members + mem_index;
		bool no_serialize = false;
		for (u64 k = 0; k < mem->attribute_count; ++k) {
			if (string_match(mem->attributes[k], "no-serialize")) {
				no_serialize = true;
				break;
			}
		}
		if (!no_serialize) {
			if (string_match(mem->name, "anonymous")) {
				if (!deserialize_fmt_text(w, "", mem->info, data + mem->offset, -1))
					return false;
			} else {
				if (!deserialize_fmt_text(w, mem->name, mem->info, data + mem->offset, -1))
					return false;
			}
		}
	}

	return true;
}

bool parse_union(Deserialize_State *w, const Type_Info_Union *info, char *data) {
	const Union_Member *parse_mem = nullptr;
	for (int mem_index = 0; mem_index < info->member_count; ++mem_index) {
		auto mem = info->members + mem_index;
		for (u64 i = 0; i < mem->attribute_count; ++i) {
			if (string_match(mem->attributes[i], "use")) {
				parse_mem = mem;
				break;
			}
		}

		if (parse_mem) break;
	}

	if (!parse_mem) {
		parse_mem = info->members;
	}

	if (string_match(parse_mem->name, "anonymous")) {
		return deserialize_fmt_text(w, "", parse_mem->info, data, -1);
	} else {
		return deserialize_fmt_text(w, parse_mem->name, parse_mem->info, data, -1);
	}

	return true;
}

bool parse_static_array(Deserialize_State *w, const Type_Info_Static_Array *info, char *data) {
	return deserialize_fmt_text(w, "", info->type, data, info->count);
}

bool parse_dynamic_array(Deserialize_State *w, const Type_Info_Dynamic_Array *info, char *data) {
	Array<char> *array = (Array<char> *)data;
	*array = Array<char>(context.allocator);
	array->data = nullptr;

	if (!parse_require_token(w, Token_Kind_OPEN_CURLY_BRACKET) ||
		!parse_require_integral<s64>(w, &array->count, true) ||
		!parse_require_token(w, Token_Kind_COMMA)) {
		return false;
	}

	array->capacity = array->count;
	array->data = (char *)memory_reallocate(array->data, array->capacity * info->type->size, array->allocator);

	return deserialize_fmt_text(w, "", info->type, array->data, array->count) && parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET);
}

bool parse_array_view(Deserialize_State *w, const Type_Info_Array_View *info, char *data) {
	Array_View<char> *array = (Array_View<char> *)data;
	array->data = nullptr;

	if (!parse_require_token(w, Token_Kind_OPEN_CURLY_BRACKET) ||
		!parse_require_integral<s64>(w, &array->count, true) ||
		!parse_require_token(w, Token_Kind_COMMA)) {
		return false;
	}
	array->data = (char *)memory_reallocate(array->data, array->count * info->type->size);

	return deserialize_fmt_text(w, "", info->type, array->data, array->count) && parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET);
}

template <typename Cast_Info, typename Proc>
bool parse_recursive_array(Proc proc, Deserialize_State *w, const Type_Info *info, char *data, s64 array_count) {
	if (array_count == -1) {
		return proc(w, (const Cast_Info *)info, data);
	} else {
		if (parse_require_token(w, Token_Kind_OPEN_SQUARE_BRACKET)) {
			for (s64 i = 0; i < array_count - 1; ++i) {
				if (!proc(w, (const Cast_Info *)info, data) ||
					!parse_require_token(w, Token_Kind_COMMA)) {
					return false;
				}
				data += 1;
			}
			return proc(w, (const Cast_Info *)info, data) && parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET);
		}
	}
	return false;
}

bool deserialize_fmt_text(Deserialize_State *w, String name, const Type_Info *info, char *data, s64 array_count) {
	if (name.count) {
		if (!parse_require_token(w, name) || !parse_require_token(w, Token_Kind_COLON)) {
			return false;
		}
	}

	switch (info->id) {
	case Type_Id_S8:  if (!parse_basic_array<s8 >(parse_require_integral<s8 >, w, (s8  *)data, array_count, true)) return false; break;
	case Type_Id_S16: if (!parse_basic_array<s16>(parse_require_integral<s16>, w, (s16 *)data, array_count, true)) return false; break;
	case Type_Id_S32: if (!parse_basic_array<s32>(parse_require_integral<s32>, w, (s32 *)data, array_count, true)) return false; break;
	case Type_Id_S64: if (!parse_basic_array<s64>(parse_require_integral<s64>, w, (s64 *)data, array_count, true)) return false; break;
	case Type_Id_U8:  if (!parse_basic_array<u8 >(parse_require_integral<u8 >, w, (u8  *)data, array_count, false)) return false; break;
	case Type_Id_U16: if (!parse_basic_array<u16>(parse_require_integral<u16>, w, (u16 *)data, array_count, false)) return false; break;
	case Type_Id_U32: if (!parse_basic_array<u32>(parse_require_integral<u32>, w, (u32 *)data, array_count, false)) return false; break;
	case Type_Id_U64: if (!parse_basic_array<u64>(parse_require_integral<u64>, w, (u64 *)data, array_count, false)) return false; break;
	case Type_Id_R32: if (!parse_basic_array<r32>(parse_require_real<r32>, w, (r32 *)data, array_count)) return false; break;
	case Type_Id_R64: if (!parse_basic_array<r64>(parse_require_real<r64>, w, (r64 *)data, array_count)) return false; break;
	case Type_Id_CHAR: if (!parse_basic_array<char>(parse_require_integral<char>, w, (char *)data, array_count, true)) return false; break;
	case Type_Id_STRING: if (!parse_basic_array(parse_require_string, w, (String *)data, array_count)) return false; break;

	case Type_Id_VOID: invalid_code_path(); break;
	case Type_Id_FUNCTION: break;

	case Type_Id_POINTER: {
		if (!parse_recursive_array<Type_Info_Pointer>(parse_pointer, w, info, data, array_count))
			return false;
	} break;

	case Type_Id_ENUM: {
		if (!parse_recursive_array<Type_Info_Enum>(parse_enum, w, info, data, array_count))
			return false;
	} break;

	case Type_Id_STRUCT: {
		if (!parse_recursive_array<Type_Info_Struct>(parse_struct, w, info, data, array_count))
			return false;
	} break;

	case Type_Id_UNION: {
		if (!parse_recursive_array<Type_Info_Union>(parse_union, w, info, data, array_count))
			return false;
	} break;

	case Type_Id_STATIC_ARRAY: {
		if (!parse_recursive_array<Type_Info_Static_Array>(parse_static_array, w, info, data, array_count))
			return false;
	} break;

	case Type_Id_DYNAMIC_ARRAY: {
		if (!parse_recursive_array<Type_Info_Dynamic_Array>(parse_dynamic_array, w, info, data, array_count))
			return false;
	} break;

	case Type_Id_ARRAY_VIEW: {
		if (!parse_recursive_array<Type_Info_Array_View>(parse_array_view, w, info, data, array_count))
			return false;
	} break;
	}

	return true;
}

bool deserialize_fmt_text(Deserialize_State *w, String name, const Type_Info *info, char *data) {
	return deserialize_fmt_text(w, name, info, data, -1);
}
