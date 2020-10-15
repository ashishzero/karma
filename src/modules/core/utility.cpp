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

bool parse_real(const String string, r64 *out) {
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
			if (parse_integer(token.content, &ival)) {
				token.kind = Token_Kind_INTEGER_LITERAL;
				token.value = ival;
			}
			else if (parse_real(token.content, &rval)) {
				token.kind = Token_Kind_REAL_LITERAL;
				token.value = rval;
			}
			else {
				if (string_match(token.content, "true")) {
					token.kind = Token_Kind_INTEGER_LITERAL;
					token.value = (s64)1;
				}
				else if (string_match(token.content, "false")) {
					token.kind = Token_Kind_INTEGER_LITERAL;
					token.value = (s64)0;
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

void iserialize_fmt_text(Ostream *out, String name, const Type_Info *info, char *data, s64 num_of_elements, bool is_array, const char *tab_count) {
	if (name.count) {
		ostream_write_formatted(out, "%*s", tab_count, "");
		ostream_write_buffer(out, name.data, name.count);
		ostream_write_formatted(out, " : ");
	}

	switch (info->id) {
	case Type_Id_S8: {
		for (s64 i = 0; i < num_of_elements; ++i) {
			ostream_write_formatted(out, "%d, ", (int)*(s8 *)(data + i * sizeof(s8)));
		}
		if (!is_array) ostream_write_formatted(out, "\n");
	} break;

	case Type_Id_S16:
	{
		for (s64 i = 0; i < num_of_elements; ++i) {
			ostream_write_formatted(out, "%d, ", (int)*(s16 *)(data + i * sizeof(s16)));
		}
		if (!is_array)
			ostream_write_formatted(out, "\n");
	}
	break;
	case Type_Id_S32:
	{
		for (s64 i = 0; i < num_of_elements; ++i) {
			ostream_write_formatted(out, "%d, ", *(s32 *)(data + i * sizeof(s32)));
		}
		if (!is_array)
			ostream_write_formatted(out, "\n");
	}
	break;
	case Type_Id_S64:
	{
		for (s64 i = 0; i < num_of_elements; ++i) {
			ostream_write_formatted(out, "%zd, ", *(s64 *)(data + sizeof(s64)));
		}
		if (!is_array)
			ostream_write_formatted(out, "\n");
	}
	break;
	case Type_Id_U8:
		for (s64 i = 0; i < num_of_elements; ++i) {
			ostream_write_formatted(out, "%u, ", (u32) * (u8 *)(data + i * sizeof(u8)));
		}
		if (!is_array)
			ostream_write_formatted(out, "\n");
		break;
	case Type_Id_U16:
		for (s64 i = 0; i < num_of_elements; ++i) {
			ostream_write_formatted(out, "%u, ", (u32) * (u16 *)(data + i * sizeof(u16)));
		}
		if (!is_array)
			ostream_write_formatted(out, "\n");
		break;
	case Type_Id_U32:
		for (s64 i = 0; i < num_of_elements; ++i) {
			ostream_write_formatted(out, "%u, ", (u32) * (u32 *)(data + i * sizeof(u32)));
		}
		if (!is_array)
			ostream_write_formatted(out, "\n");
		break;
	case Type_Id_U64:
		for (s64 i = 0; i < num_of_elements; ++i) {
			ostream_write_formatted(out, "%zu, ", *(u64 *)(data + i * sizeof(u64)));
		}
		if (!is_array)
			ostream_write_formatted(out, "\n");
		break;
	case Type_Id_R32:
		for (s64 i = 0; i < num_of_elements; ++i) {
			ostream_write_formatted(out, "%f, ", *(r32 *)(data + i * sizeof(r32)));
		}
		if (!is_array)
			ostream_write_formatted(out, "\n");
		break;
	case Type_Id_R64:
		for (s64 i = 0; i < num_of_elements; ++i) {
			ostream_write_formatted(out, "%lf, ", *(r64 *)(data + i * sizeof(r64)));
		}
		if (!is_array)
			ostream_write_formatted(out, "\n");
		break;
	case Type_Id_CHAR:
		for (s64 i = 0; i < num_of_elements; ++i) {
			ostream_write_formatted(out, "%d, ", (s32) * (char *)(data + i * sizeof(char)));
		}
		if (!is_array)
			ostream_write_formatted(out, "\n");
		break;
	case Type_Id_VOID:// invalid_path();
	{
		invalid_code_path();
	}
	break;
	case Type_Id_POINTER:// deference 
	{
		auto ptr_info = (Type_Info_Pointer *)info;
		for (s64 i = 0; i < num_of_elements; ++i) {
			iserialize_fmt_text(out, "", ptr_info->pointer_to, (char *)(*(ptrsize *)(data + i * sizeof(ptrsize))), 1, false, tab_count);
		}
	}
	break;
	case Type_Id_FUNCTION:// skip
		break;
	case Type_Id_ENUM:// s64 
	{
		auto enum_info = (Type_Info_Enum *)info;
		for (s64 i = 0; i < num_of_elements; ++i) {
			iserialize_fmt_text(out, "", enum_info->item_type, data + i * enum_info->size, 1, false, tab_count);
		}
	}
	break;
	case Type_Id_STRUCT:
	{
		auto struct_info = (Type_Info_Struct *)info;
		//TODO  : need optimization
		for (s64 j = 0; j < num_of_elements; ++j) {
			ostream_write_formatted(out, "%*s", tab_count, "");
			ostream_write_formatted(out, "{\n");
			if (struct_info->base) {
				iserialize_fmt_text(out, ".base", struct_info->base, data + j * struct_info->size, 1, false, tab_count + 3);
			}
			for (int i = 0; i < struct_info->member_count; ++i) {
				auto mem = struct_info->members + i;
				bool no_serialize = false;
				for (u64 k = 0; k < mem->attribute_count; ++k) {
					if (string_match(mem->attributes[k], "no-serialize")) {
						no_serialize = true;
						break;
					}
				}
				if (!no_serialize)
					iserialize_fmt_text(out, mem->name, mem->info, data + j * struct_info->size + mem->offset, 1, false, tab_count + 3);
			}
			ostream_write_formatted(out, "%*s", tab_count, "");
			ostream_write_formatted(out, "},\n");
		}
	}
	break;
	case Type_Id_UNION:
	{
		auto union_info = (Type_Info_Union *)info;
		for (s64 k = 0; k < num_of_elements; ++k) {
			bool write = false;
			for (int j = 0; j < union_info->member_count; ++j) {
				auto mem = union_info->members + j;
				for (u64 i = 0; i < mem->attribute_count; ++i) {
					if (string_match(mem->attributes[i], "write")) {
						write = true;
						break;
					}
				}
				if (write) {
					ostream_write_formatted(out, "{\n");
					iserialize_fmt_text(out, mem->name, mem->info, data + k * union_info->size, 1, false, tab_count + 3);
					ostream_write_formatted(out, "%*s", tab_count, "");
					ostream_write_formatted(out, "},\n");
					break;
				}
			}
			if (!write) {
				ostream_write_formatted(out, "{\n");
				iserialize_fmt_text(out, union_info->members->name, union_info->members->info, k * union_info->size + data, 1, false, tab_count + 3);
				ostream_write_formatted(out, "%*s", tab_count, "");
				ostream_write_formatted(out, "},\n");
			}
		}
	}
	break;
	case Type_Id_STRING:
	{
		for (s64 i = 0; i < num_of_elements; ++i) {
			String *string_data = (String *)(data + i * sizeof(String));
			ostream_write_formatted(out, "\"");
			ostream_write_buffer(out, string_data->data, string_data->count);
			ostream_write_formatted(out, "\"");
		};
		if (!is_array)
			ostream_write_formatted(out, "\n");
	}
	break;
	case Type_Id_STATIC_ARRAY:
	{
		if (num_of_elements > 1)
			ostream_write_formatted(out, "{ %zd, [ ", num_of_elements);

		s64 count = ((Type_Info_Static_Array *)info)->count;
		const Type_Info *elem_type_info = ((Type_Info_Static_Array *)info)->type;

		for (s64 i = 0; i < num_of_elements; ++i) {
			ostream_write_formatted(out, "[N] : ");
			ostream_write_formatted(out, "{ %zd, [ ", count);
			iserialize_fmt_text(out, "", elem_type_info, data + i * elem_type_info->size * count, count, true, tab_count + 3);
			ostream_write_formatted(out, " ] },\n");
		}
		if (num_of_elements > 1) {
			ostream_write_formatted(out, "%*s", tab_count, "");
			ostream_write_formatted(out, " ] },\n");
		}
	}
	break;
	case Type_Id_DYNAMIC_ARRAY:
	{
		if (num_of_elements > 1)
			ostream_write_formatted(out, "{ %zd, [ ", num_of_elements);

		for (s64 i = 0; i < num_of_elements; ++i) {
			Array<char> *array = (Array<char> *)(data + i * sizeof(Array<char>));
			s64 count = array->count;

			ostream_write_formatted(out, "[..] : ");
			ostream_write_formatted(out, "{ %zd, [ ", count);
			iserialize_fmt_text(out, "", ((Type_Info_Dynamic_Array *)info)->type, array->data, count, true, tab_count + 3);
			ostream_write_formatted(out, " ] },\n");
		}
		if (num_of_elements > 1) {
			ostream_write_formatted(out, "%*s", tab_count, "");
			ostream_write_formatted(out, " ] },\n");
		}
	}
	break;
	case Type_Id_ARRAY_VIEW:
	{
		if (num_of_elements > 1)
			ostream_write_formatted(out, "{ %zd, [ ", num_of_elements);

		const Type_Info *elem_type_info = ((Type_Info_Array_View *)info)->type;
		for (s64 i = 0; i < num_of_elements; ++i) {
			Array_View<char> *view = (Array_View<char> *)(data + i * sizeof(Array_View<char>));
			s64 count = view->count;

			ostream_write_formatted(out, "[] : ");
			ostream_write_formatted(out, "{ %zd, [ ", count);
			iserialize_fmt_text(out, "", elem_type_info, view->data, count, true, tab_count + 3);
			ostream_write_formatted(out, " ] },\n");
		}
		if (num_of_elements > 1) {
			ostream_write_formatted(out, "%*s", tab_count, "");
			ostream_write_formatted(out, " ] },\n");
		}
	} break;
	}
}

void serialize_fmt_text(Ostream *out, String name, const Type_Info *info, char *data, s64 num_of_elements, bool is_array) {
	iserialize_fmt_text(out, name, info, data, num_of_elements, is_array, 0);
}

struct Parse_State {
	Token *start;
	Token *end;
	Token *current;
	Deserialize_Error_Info *error;
};

inline void parsing_error(Parse_State *w, Token_Kind kind) {
	w->error->token = *(w->current - 1);
	w->error->expected = kind;
	memset(w->error->string, 0, sizeof(w->error->string));
}

inline void parsing_error(Parse_State *w, String string) {
	w->error->token = *(w->current - 1);
	w->error->expected = Token_Kind_IDENTIFIER;
	memcpy(w->error->string, string.data, minimum(string.count, sizeof(w->error->string) - 1));
	w->error->string[minimum(string.count, sizeof(w->error->string))] = 0;
}

inline bool parsing(Parse_State *w) {
	return w->current < w->end;
}

inline Token *parse_next(Parse_State *w) {
	if (parsing(w)) {
		w->current += 1;
		return w->current - 1;
	}
	return nullptr;
}

inline bool parse_require_token(Parse_State *w, String string) {
	auto t = parse_next(w);
	if (t && t->kind == Token_Kind_IDENTIFIER && string_match(string, t->content))
		return true;
	parsing_error(w, string);
	return false;
}

template <typename T>
inline bool parse_require_integer(Parse_State *w, T *d) {
	auto t = parse_next(w);
	if (t && t->kind == Token_Kind_INTEGER_LITERAL) {
		*d = (T)t->value.integer;
		return true;
	}
	parsing_error(w, Token_Kind_INTEGER_LITERAL);
	return false;
}

template <typename T>
inline bool parse_require_real(Parse_State *w, T *d) {
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

inline bool parse_require_token(Parse_State *w, Token_Kind kind) {
	auto t = parse_next(w);
	if (t && t->kind == kind)
		return true;
	parsing_error(w, kind);
	return false;
}

inline bool parse_require_string(Parse_State *w, String *d) {
	auto t = parse_next(w);
	if (t && t->kind == Token_Kind_DQ_STRING) {
		*d = t->value.string;
		return true;
	}
	parsing_error(w, Token_Kind_DQ_STRING);
	return false;
}

template <typename T>
inline bool parse_integer_array(Parse_State *w, s64 num_of_elements, T *data) {
	for (s64 i = 0; i < num_of_elements; ++i) {
		if (!parse_require_integer<T>(w, data + i) ||
			!parse_require_token(w, Token_Kind_COMMA)) {
			return false;
		}
	}
	if (num_of_elements > 1) {
		if (!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
			!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET) ||
			!parse_require_token(w, Token_Kind_COMMA)) {
			return false;
		}
	}
	return true;
}

inline bool parse_char_array(Parse_State *w, s64 num_of_elements, char *data) {
	s32 temp;
	for (s64 i = 0; i < num_of_elements; ++i) {
		if (!parse_require_integer<s32>(w, &temp) ||
			!parse_require_token(w, Token_Kind_COMMA)) {
			return false;
		}
		*(data + i) = temp;
	}
	if (num_of_elements > 1) {
		if (!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
			!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET) ||
			!parse_require_token(w, Token_Kind_COMMA)) {
			return false;
		}
	}
	return true;
}

bool ideserialize_fmt_text(Parse_State *w, String name, const Type_Info *info, char *data, s64 num_of_elements) {
	if (name.count) {
		if (!parse_require_token(w, name) || !parse_require_token(w, Token_Kind_COLON)) {
			return false;
		}
	}

	switch (info->id) {
	case Type_Id_S8:
	{
		if (!parse_integer_array<s8>(w, num_of_elements, (s8 *)data))
			return false;
	}
	break;
	case Type_Id_S16:
	{
		if (!parse_integer_array<s16>(w, num_of_elements, (s16 *)data))
			return false;
	}
	break;
	case Type_Id_S32:
	{
		if (!parse_integer_array<s32>(w, num_of_elements, (s32 *)data))
			return false;
	}
	break;
	case Type_Id_S64:
	{
		if (!parse_integer_array<s64>(w, num_of_elements, (s64 *)data))
			return false;
	}
	break;
	case Type_Id_U8:
	{
		if (!parse_integer_array<u8>(w, num_of_elements, (u8 *)data))
			return false;
	}
	break;
	case Type_Id_U16:
	{
		if (!parse_integer_array<u16>(w, num_of_elements, (u16 *)data))
			return false;
	}
	break;
	case Type_Id_U32:
	{
		if (!parse_integer_array<u32>(w, num_of_elements, (u32 *)data))
			return false;
	}
	break;
	case Type_Id_U64:
	{
		if (!parse_integer_array<u64>(w, num_of_elements, (u64 *)data))
			return false;
	}
	break;
	case Type_Id_R32:
	{
		for (s64 i = 0; i < num_of_elements; ++i) {
			if (!parse_require_real<r32>(w, (r32 *)data + i) ||
				!parse_require_token(w, Token_Kind_COMMA)) {
				return false;
			}
		}
		if (num_of_elements > 1) {
			if (!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET) ||
				!parse_require_token(w, Token_Kind_COMMA)) {
				return false;
			}
		}
	}
	break;
	case Type_Id_R64:
	{
		for (s64 i = 0; i < num_of_elements; ++i) {
			if (!parse_require_real<r32>(w, (r32 *)data + i) ||
				!parse_require_token(w, Token_Kind_COMMA)) {
				return false;
			}
		}
		if (num_of_elements > 1) {
			if (!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET) ||
				!parse_require_token(w, Token_Kind_COMMA)) {
				return false;
			}
		}
	}
	break;
	case Type_Id_CHAR:
	{
		if (!parse_char_array(w, num_of_elements, (char *)data))
			return false;
	}
	break;
	case Type_Id_VOID:// invalid_path();
	{
		invalid_code_path();
	}
	break;
	case Type_Id_POINTER:// deference 
	{
		auto ptr_info = (Type_Info_Pointer *)info;
		for (s64 i = 0; i < num_of_elements; ++i) {
			(*(ptrsize *)(data + i * sizeof(ptrsize))) = (ptrsize)memory_allocate(ptr_info->pointer_to->size);
			if (!ideserialize_fmt_text(w, "", ptr_info->pointer_to, (char *)(*(ptrsize *)(data + i * sizeof(ptrsize))), 1))
				return false;

		}
		if (num_of_elements > 1) {
			if (!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET) ||
				!parse_require_token(w, Token_Kind_COMMA)) {
				return false;
			}
		}
	}
	break;
	case Type_Id_FUNCTION:// skip
		break;
	case Type_Id_ENUM:// s64 
	{
		auto enum_info = (Type_Info_Enum *)info;
		for (s64 i = 0; i < num_of_elements; ++i) {
			if (!ideserialize_fmt_text(w, "", enum_info->item_type, data + i * enum_info->size, 1))
				return false;
		}
		if (num_of_elements > 1) {
			if (!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET) ||
				!parse_require_token(w, Token_Kind_COMMA)) {
				return false;
			}
		}
	}
	break;
	case Type_Id_STRUCT:
	{
		auto struct_info = (Type_Info_Struct *)info;
		//TODO  : need optimization
		for (s64 j = 0; j < num_of_elements; ++j) {
			if (!parse_require_token(w, Token_Kind_OPEN_CURLY_BRACKET))
				return false;
			if (struct_info->base) {
				if (!parse_require_token(w, Token_Kind_PERIOD))
					return false;
				if (!ideserialize_fmt_text(w, "base", struct_info->base, data + j * struct_info->size, 1))
					return false;
			}
			for (int i = 0; i < struct_info->member_count; ++i) {
				auto mem = struct_info->members + i;
				bool no_serialize = false;
				for (u64 k = 0; k < mem->attribute_count; ++k) {
					if (string_match(mem->attributes[k], "no-serialize")) {
						no_serialize = true;
						break;
					}
				}
				if (!no_serialize)
					if (!ideserialize_fmt_text(w, mem->name, mem->info, data + j * struct_info->size + mem->offset, 1))
						return false;
			}
			if (!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET) || !parse_require_token(w, Token_Kind_COMMA)) {
				return false;
			}
		}
		if (num_of_elements > 1) {
			if (!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET) ||
				!parse_require_token(w, Token_Kind_COMMA)) {
				return false;
			}
		}
	}
	break;
	case Type_Id_UNION:
	{
		auto union_info = (Type_Info_Union *)info;
		for (s64 k = 0; k < num_of_elements; ++k) {
			bool write = false;
			for (int j = 0; j < union_info->member_count; ++j) {
				auto mem = union_info->members + j;
				for (u64 i = 0; i < mem->attribute_count; ++i) {
					if (string_match(mem->attributes[i], "write")) {
						write = true;
						break;
					}
				}
				if (write) {
					if (!parse_require_token(w, Token_Kind_OPEN_CURLY_BRACKET))
						return false;
					if (!ideserialize_fmt_text(w, mem->name, mem->info, data + k * union_info->size, 1))
						return false;
					if (!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET) || !parse_require_token(w, Token_Kind_COMMA)) {
						return false;
					}
					break;
				}
			}
			if (!write) {
				if (!parse_require_token(w, Token_Kind_OPEN_CURLY_BRACKET))
					return false;
				if (!ideserialize_fmt_text(w, union_info->members->name, union_info->members->info, k * union_info->size + data, 1))
					return false;
				if (!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET) || !parse_require_token(w, Token_Kind_COMMA)) {
					return false;
				}
			}
		}
		if (num_of_elements > 1) {
			if (!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET) ||
				!parse_require_token(w, Token_Kind_COMMA)) {
				return false;
			}
		}
	}
	break;
	case Type_Id_STATIC_ARRAY:
	{
		if (num_of_elements > 1) {
			if (!parse_require_token(w, Token_Kind_OPEN_CURLY_BRACKET) ||
				!parse_require_integer<s64>(w, &num_of_elements) ||
				!parse_require_token(w, Token_Kind_COMMA) ||
				!parse_require_token(w, Token_Kind_OPEN_SQUARE_BRACKET))
				return false;
		}

		const Type_Info *elem_type_info = ((Type_Info_Static_Array *)info)->type;
		s64 count = ((Type_Info_Static_Array *)info)->count;
		for (s64 i = 0; i < num_of_elements; ++i) {
			if (!parse_require_token(w, Token_Kind_OPEN_SQUARE_BRACKET) ||
				!parse_require_token(w, "N") ||
				!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_COLON)) {
				return false;
			}

			if (!parse_require_token(w, Token_Kind_OPEN_CURLY_BRACKET) ||
				!parse_require_integer<s64>(w, &count) ||
				!parse_require_token(w, Token_Kind_COMMA) ||
				!parse_require_token(w, Token_Kind_OPEN_SQUARE_BRACKET))
				return false;

			if (!ideserialize_fmt_text(w, "", elem_type_info, data + i * elem_type_info->size * count, count))
				return false;
		}
		if (num_of_elements > 1) {
			if (!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET) ||
				!parse_require_token(w, Token_Kind_COMMA)) {
				return false;
			}
		}
	}
	break;
	case Type_Id_STRING:
	{
		{
			for (s64 i = 0; i < num_of_elements; ++i) {
				String *string_data = (String *)(data + i * sizeof(String));
				String temp;
				if (!parse_require_string(w, &temp) || !parse_require_token(w, Token_Kind_COMMA))
					return false;
				string_data->count = temp.count;
				string_data->data = new u8[string_data->count];
				memcpy(string_data->data, temp.data, string_data->count);
			}
			if (num_of_elements > 1) {
				if (!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
					!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET) ||
					!parse_require_token(w, Token_Kind_COMMA)) {
					return false;
				}
			}
		}
	}
	break;
	case Type_Id_DYNAMIC_ARRAY:
	{
		if (num_of_elements > 1) {
			if (!parse_require_token(w, Token_Kind_OPEN_CURLY_BRACKET) ||
				!parse_require_integer<s64>(w, &num_of_elements) ||
				!parse_require_token(w, Token_Kind_COMMA) ||
				!parse_require_token(w, Token_Kind_OPEN_SQUARE_BRACKET))
				return false;
		}

		s64 count;
		for (s64 i = 0; i < num_of_elements; ++i) {
			Array<char> *array = (Array<char> *)(data + i * sizeof(Array<char>));
			*array = Array<char>();
			array->data = nullptr;

			if (!parse_require_token(w, Token_Kind_OPEN_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_PERIOD) ||
				!parse_require_token(w, Token_Kind_PERIOD) ||
				!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_COLON)) {
				return false;
			}

			if (!parse_require_token(w, Token_Kind_OPEN_CURLY_BRACKET) ||
				!parse_require_integer<s64>(w, &count) ||
				!parse_require_token(w, Token_Kind_COMMA) ||
				!parse_require_token(w, Token_Kind_OPEN_SQUARE_BRACKET))
				return false;

			array->capacity = count;
			array->count = count;
			array->data = (char *)memory_reallocate(array->data, array->capacity * ((Type_Info_Dynamic_Array *)info)->type->size, array->allocator);
			if (!ideserialize_fmt_text(w, "", ((Type_Info_Dynamic_Array *)info)->type, array->data, count))
				return false;
		}
		if (num_of_elements > 1) {
			if (!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET) ||
				!parse_require_token(w, Token_Kind_COMMA)) {
				return false;
			}
		}
	}
	break;
	case Type_Id_ARRAY_VIEW:
	{
		if (num_of_elements > 1) {
			if (!parse_require_token(w, Token_Kind_OPEN_CURLY_BRACKET) ||
				!parse_require_integer<s64>(w, &num_of_elements) ||
				!parse_require_token(w, Token_Kind_COMMA) ||
				!parse_require_token(w, Token_Kind_OPEN_SQUARE_BRACKET))
				return false;
		}

		s64 count;
		for (s64 i = 0; i < num_of_elements; ++i) {
			Array_View<char> *view = (Array_View<char> *)(data + i * sizeof(Array_View<char>));

			if (!parse_require_token(w, Token_Kind_OPEN_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_COLON))
				return false;

			if (!parse_require_token(w, Token_Kind_OPEN_CURLY_BRACKET) ||
				!parse_require_integer<s64>(w, &count) ||
				!parse_require_token(w, Token_Kind_COMMA) ||
				!parse_require_token(w, Token_Kind_OPEN_SQUARE_BRACKET))
				return false;

			view->count = count;
			view->data = (char *)memory_allocate(view->count * ((Type_Info_Array_View *)info)->type->size);
			if (!ideserialize_fmt_text(w, "", ((Type_Info_Array_View *)info)->type, view->data, count))
				return false;
		}
		if (num_of_elements > 1) {
			if (!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET) ||
				!parse_require_token(w, Token_Kind_COMMA)) {
				return false;
			}
		}
	} break;
	}

	return true;
}

bool deserialize_fmt_text(Array_View<Token> &tokens, String name, const Type_Info *info, char *data, s64 num_of_elements, Deserialize_Error_Info *error) {
	if (tokens.count == 0)
		return false;

	Parse_State w;
	w.start = tokens.data;
	w.current = tokens.data;
	w.end = tokens.data + tokens.count;
	w.error = error;

	return ideserialize_fmt_text(&w, name, info, data, num_of_elements);
}
