#define _CRT_SECURE_NO_WARNINGS
#include "modules/core/stream.cpp"
#include "modules/core/karma_crt_impl.hpp"
#include "modules/core/tokenizer.cpp"

#include <clang-c/Index.h>

#ifdef TARGET_WINDOWS
#	pragma comment(lib, "libclang.lib")
#else
#	error "Platform currently not supported"
#endif

#if 0
CXChildVisitResult cursor_type_printer(CXCursor cursor, CXCursor parent, CXClientData client_data) {
	int * ntabs = (int *)client_data;

	auto kind_spelling = clang_getCursorKindSpelling(clang_getCursorKind(cursor));
	auto name_spelling = clang_getCursorSpelling(cursor);

	for (int i = 0; i < *ntabs; ++i) printf("\t");
	printf("%s (%s)\n", clang_getCString(name_spelling), clang_getCString(kind_spelling));

	*ntabs += 1;
	clang_visitChildren(cursor, cursor_type_printer, ntabs);
	*ntabs -= 1;

	clang_disposeString(kind_spelling);
	clang_disposeString(name_spelling);

	return CXChildVisit_Continue;
}
#endif

void reflection_of_enum_info(Ostream *stream, CXCursor cursor, const char *name, Array_View<CXCursor> enums, s64 min, s64 max, bool sequential) {
	auto type_decl          = clang_getEnumDeclIntegerType(cursor);
	auto type_decl_spelling = clang_getTypeSpelling(type_decl);
	defer {
		clang_disposeString(type_decl_spelling);
	};

	ostream_write_formatted(stream, "template <> struct Enum_Info<%s> {\n", name);
	ostream_write_formatted(stream, "\tstatic constexpr size_t get_count() { return %u; }\n", enums.count);
	ostream_write_formatted(stream, "\tstatic constexpr s64 get_min_value() { return %zd; }\n", min);
	ostream_write_formatted(stream, "\tstatic constexpr s64 get_max_value() { return %zd; }\n", max);

	if (sequential) {
		ostream_write_formatted(stream, "\tstatic const %s index_value(s64 index) { return (%s)(index + %zd); }\n", name, name, clang_getEnumConstantDeclValue(enums[0]));
	} else {
		ostream_write_formatted(stream, "\tstatic const %s index_value(s64 index) {\n", name);
		ostream_write_formatted(stream, "\t\tstatic const %s enum_values[] = {\n", name);
		for (s64 index = 0; index < enums.count; ++index) {
			ostream_write_formatted(stream, "\t\t\t(%s)%zd,\n", name, clang_getEnumConstantDeclValue(enums[index]));
		}
		ostream_write_formatted(stream, "\t\t};\n");
		ostream_write_formatted(stream, "\t\tassert(index >= 0 && index < static_count(enum_values));\n");
		ostream_write_formatted(stream, "\t\treturn enum_values[index];\n\t}\n", name);
	}

	if (sequential) {
		ostream_write_formatted(stream, "\tstatic const String string(%s value) {\n", name);
		ostream_write_formatted(stream, "\t\tstatic const String strings[] = {\n");
		for (s64 index = 0; index < enums.count; ++index) {
			auto spelling = clang_getCursorSpelling(enums[index]);
			ostream_write_formatted(stream, "\t\t\t\"%s\",\n", clang_getCString(spelling));
			clang_disposeString(spelling);
		}
		ostream_write_formatted(stream, "\t\t};\n");
		ostream_write_formatted(stream, "\t\treturn strings[value - %d];\n\t}\n", clang_getEnumConstantDeclValue(enums[0]));
	} else {
		ostream_write_formatted(stream, "\tstatic const String string(%s value) {\n", name);
		for (s64 index = 0; index < enums.count; ++index) {
			auto        spelling = clang_getCursorSpelling(enums[index]);
			const char *mem      = clang_getCString(spelling);
			ostream_write_formatted(stream, "\t\tif (%d == value) return \"%s\";\n", clang_getEnumConstantDeclValue(enums[index]), mem);
			clang_disposeString(spelling);
		}
		ostream_write_formatted(stream, "\n\t\tinvalid_code_path();\n");
		ostream_write_formatted(stream, "\t\treturn \"\";\n\t}\n");
	}

	ostream_write_formatted(stream, "\tstatic const Array_View<const char *> string_array() {\n");
	ostream_write_formatted(stream, "\t\tstatic const char * strings[] = {\n");
	for (s64 index = 0; index < enums.count; ++index) {
		auto spelling = clang_getCursorSpelling(enums[index]);
		ostream_write_formatted(stream, "\t\t\t\"%s\",\n", clang_getCString(spelling));
		clang_disposeString(spelling);
	}
	ostream_write_formatted(stream, "\t\t};\n");
	ostream_write_formatted(stream, "\t\treturn Array_View<const char *>(strings, %zd);\n\t}\n", enums.count);

	ostream_write_formatted(stream, "\tstatic const Array_View<%s> item_array() {\n", name);
	ostream_write_formatted(stream, "\t\tstatic const %s items[] = {\n", name);
	for (s64 index = 0; index < enums.count; ++index) {
		auto spelling = clang_getCursorSpelling(enums[index]);
		ostream_write_formatted(stream, "\t\t\t%s,\n", clang_getCString(spelling));
		clang_disposeString(spelling);
	}
	ostream_write_formatted(stream, "\t\t};\n");
	ostream_write_formatted(stream, "\t\treturn Array_View<%s>((%s *)items, %zd);\n\t}\n", name, name, enums.count);

	ostream_write_formatted(stream, "};\n\n");
}

void reflection_of_enum(Ostream *stream, const char *name, const char *element_type_name, bool is_seq) {
	ostream_write_formatted(stream, "template <> struct Reflect<%s> {\n", name);
	ostream_write_formatted(stream, "\tstatic constexpr Type_Id id = Type_Id_ENUM;\n");

	ostream_write_formatted(stream, "\tstatic const Type_Info * const info() {\n");
	ostream_write_formatted(stream, "\t\tstatic const Type_Info_Enum i(sizeof(%s), \"%s\", reflect_info<%s>(),"
										"%s, enum_string_array<%s>(), enum_item_array<%s>().data, enum_item_array<%s>().count);\n", 
							name, name, element_type_name, is_seq ?"true":"false", name, name, name);
	ostream_write_formatted(stream, "\t\treturn &i;\n\t}\n};\n\n");
}

void reflection_of_attrs(Ostream *stream, CXCursor cursor) {
	auto        spelling = clang_getCursorSpelling(cursor);
	const char *name     = clang_getCString(spelling);
	defer {
		clang_disposeString(spelling);
	};

	ostream_write_formatted(stream, "\t\tstatic const String attrs_%s[] = {\n", name);

	clang_visitChildren(
		cursor,
		[](CXCursor cursor, CXCursor parent, CXClientData stream) -> CXChildVisitResult {
			if (clang_getCursorKind(cursor) == CXCursor_AnnotateAttr) {
				auto attrs = clang_getCursorSpelling(cursor);
				ostream_write_formatted((Ostream *)stream, "\t\t\t%s\n", clang_getCString(attrs));
				clang_disposeString(attrs);
			}
			return CXChildVisit_Continue;
		},
		stream);

	ostream_write_formatted(stream, "\t\t};\n\n");
}

CXChildVisitResult visit_members_of_anonymous_type(CXCursor cursor, CXCursor parent, CXClientData client_data) {
	auto kind = clang_getCursorKind(cursor);

	if (kind == CXCursor_FieldDecl) {
		auto members = (Array<CXCursor> *)client_data;
		array_add(members, cursor);
	}

	return CXChildVisit_Continue;
}

void reflection_of_anonymous_struct(Ostream *stream, CXCursor cursor, const char *name, int type_index) {
	Array<CXCursor> members;
	clang_visitChildren(cursor, visit_members_of_anonymous_type, &members);
	defer {
		array_free(&members);
	};

	auto size = clang_Type_getSizeOf(clang_getCursorType(cursor));

	if (members.count == 0) {
		ostream_write_formatted(stream, "\t\tstatic const Type_Info_Struct _i_anonymous_struct_%d(%zd, \"%s::anonymous struct\", 0, 0, 0);\n",
								type_index, size, name);
		return;
	}

	Array<bool> has_attrs;
	array_resize(&has_attrs, members.count);
	defer {
		array_free(&has_attrs);
	};

	for (s64 index = 0; index < members.count; ++index) {
		if (clang_Cursor_hasAttrs(members[index])) {
			array_add(&has_attrs, true);
			reflection_of_attrs(stream, members[index]);
		} else {
			array_add(&has_attrs, false);
		}
	}

	// We don't do recursions!

	ostream_write_formatted(stream, "\t\tstatic const Struct_Member _anonymous_struct_members%d[] = {\n", type_index);

	for (s64 index = 0; index < members.count; ++index) {
		auto spelling = clang_getCursorSpelling(members[index]);
		auto member   = clang_getCString(spelling);

		auto member_type   = clang_getCursorType(members[index]);
		auto type_spelling = clang_getTypeSpelling(member_type);
		auto type_string   = clang_getCString(type_spelling);

		auto offset = clang_Cursor_getOffsetOfField(members[index]) / 8;
		ostream_write_formatted(stream, "\t\t\t{ \"%s\", %zd, ", member, offset);

		if (has_attrs[index]) {
			ostream_write_formatted(stream, "static_count(attrs_%s), attrs_%s, ", member, member);
		} else {
			ostream_write_formatted(stream, "0, 0, ");
		}

		ostream_write_formatted(stream, "Reflect<%s>::info() },\n", type_string);

		clang_disposeString(type_spelling);
		clang_disposeString(spelling);
	}

	ostream_write_formatted(stream, "\t\t};\n");

	ostream_write_formatted(stream, "\t\tstatic const Type_Info_Struct _i_anonymous_struct_%d(%zd, \"%s::anonymous struct\", static_count(_anonymous_struct_members%d), _anonymous_struct_members%d, 0);\n",
							type_index, size, name, type_index, type_index);
}

void reflection_of_anonymous_union(Ostream *stream, CXCursor cursor, const char *name, int type_index) {
	Array<CXCursor> members;
	clang_visitChildren(cursor, visit_members_of_anonymous_type, &members);
	defer {
		array_free(&members);
	};

	auto size = clang_Type_getSizeOf(clang_getCursorType(cursor));

	if (members.count == 0) {
		ostream_write_formatted(stream, "\t\tstatic const Type_Info_Union _i_anonymous_union_%d(%zd, \"%s::anonymous union\", 0, 0);\n",
								type_index, size, name);
		return;
	}

	Array<bool> has_attrs;
	array_resize(&has_attrs, members.count);
	defer {
		array_free(&has_attrs);
	};

	for (s64 index = 0; index < members.count; ++index) {
		if (clang_Cursor_hasAttrs(members[index])) {
			array_add(&has_attrs, true);
			reflection_of_attrs(stream, members[index]);
		} else {
			array_add(&has_attrs, false);
		}
	}

	// We don't do recursions!

	ostream_write_formatted(stream, "\t\tstatic const Union_Member _anonymous_union_members_%d[] = {\n", type_index);

	for (s64 index = 0; index < members.count; ++index) {
		auto spelling = clang_getCursorSpelling(members[index]);
		auto member   = clang_getCString(spelling);

		auto member_type   = clang_getCursorType(members[index]);
		auto type_spelling = clang_getTypeSpelling(member_type);
		auto type_string   = clang_getCString(type_spelling);

		ostream_write_formatted(stream, "\t\t\t{ \"%s\", ", member);

		if (has_attrs[index]) {
			ostream_write_formatted(stream, "static_count(attrs_%s), attrs_%s, ", member, member);
		} else {
			ostream_write_formatted(stream, "0, 0, ");
		}

		ostream_write_formatted(stream, "Reflect<%s>::info() },\n", type_string);

		clang_disposeString(type_spelling);
		clang_disposeString(spelling);
	}

	ostream_write_formatted(stream, "\t\t};\n");

	ostream_write_formatted(stream, "\t\tstatic const Type_Info_Union _i_anonymous_union_%d(%zd, \"%s::anonymous union\", static_count(_anonymous_union_members_%d), _anonymous_union_members_%d);\n",
							type_index, size, name, type_index, type_index);
}

void reflection_of_struct(Ostream *stream, CXCursor cursor, Array_View<CXCursor> members, const char *base) {
	auto type = clang_getCursorType(cursor);

	auto        spelling = clang_getTypeSpelling(type);
	const char *name     = clang_getCString(spelling);
	defer {
		clang_disposeString(spelling);
	};

	auto size = clang_Type_getSizeOf(type);

	ostream_write_formatted(stream, "template <> struct Reflect<%s> {\n", name);
	ostream_write_formatted(stream, "\tstatic constexpr Type_Id id = Type_Id_STRUCT;\n");

	ostream_write_formatted(stream, "\tstatic const Type_Info * const info() {\n");

	if (members.count == 0) {
		if (base == 0) {
			ostream_write_formatted(stream, "\t\tstatic const Type_Info_Struct i(%zd, \"%s\", 0, 0, 0);\n", size, name);
		} else {
			ostream_write_formatted(stream, "\t\tstatic const Type_Info_Struct i(%zd, \"%s\", 0, 0, Reflect<%s>::info());\n", size, name, base);
		}
		ostream_write_formatted(stream, "\t\treturn &i;\n\t}\n};\n\n");
		return;
	}

	Array<bool> has_attrs;
	array_resize(&has_attrs, members.count);
	defer {
		array_free(&has_attrs);
	};

	for (s64 index = 0; index < members.count; ++index) {
		if (clang_Cursor_isAnonymous(members[index])) {
			if (clang_getCursorKind(members[index]) == CXCursor_StructDecl) {
				reflection_of_anonymous_struct(stream, members[index], name, (int)index);
			} else if (clang_getCursorKind(members[index]) == CXCursor_UnionDecl) {
				reflection_of_anonymous_union(stream, members[index], name, (int)index);
			}
		}

		if (clang_Cursor_hasAttrs(members[index])) {
			array_add(&has_attrs, true);
			reflection_of_attrs(stream, members[index]);
		} else {
			array_add(&has_attrs, false);
		}
	}

	ostream_write_formatted(stream, "\t\tstatic const Struct_Member struct_members[] = {\n");

	for (s64 index = 0; index < members.count; ++index) {
		auto is_anonymous = clang_Cursor_isAnonymous(members[index]);

		auto spelling = clang_getCursorSpelling(members[index]);
		auto member   = clang_getCString(spelling);
		if (is_anonymous) member = "anonymous";

		auto member_type   = clang_getCursorType(members[index]);
		auto type_spelling = clang_getTypeSpelling(member_type);
		auto type_string   = clang_getCString(type_spelling);

		auto offset = clang_Cursor_getOffsetOfField(members[index]) / 8;
		ostream_write_formatted(stream, "\t\t\t{ \"%s\", %zd, ", member, offset);

		if (has_attrs[index]) {
			ostream_write_formatted(stream, "static_count(attrs_%s), attrs_%s, ", member, member);
		} else {
			ostream_write_formatted(stream, "0, 0, ");
		}

		if (!is_anonymous) {
			ostream_write_formatted(stream, "Reflect<%s>::info() },\n", type_string);
		} else {
			if (clang_getCursorKind(members[index]) == CXCursor_UnionDecl)
				ostream_write_formatted(stream, "&_i_anonymous_union_%d },\n", (int)index);
			else if (clang_getCursorKind(members[index]) == CXCursor_StructDecl)
				ostream_write_formatted(stream, "&_i_anonymous_struct_%d },\n", (int)index);
			else
				ostream_write_formatted(stream, "&_i_anonymous_uknown_%d },\n", (int)index); // We never reach here
		}

		clang_disposeString(type_spelling);
		clang_disposeString(spelling);
	}

	ostream_write_formatted(stream, "\t\t};\n");

	if (base == 0) {
		ostream_write_formatted(stream,
								"\t\tstatic const Type_Info_Struct i(%zd, \"%s\", static_count(struct_members), struct_members, 0);\n",
								size, name);
	} else {
		ostream_write_formatted(stream,
								"\t\tstatic const Type_Info_Struct i(%zd, \"%s\", static_count(struct_members), struct_members, Reflect<%s>::info());\n",
								size, name, base);
	}
	ostream_write_formatted(stream, "\t\treturn &i;\n\t}\n};\n\n");
}

void reflection_of_union(Ostream *stream, CXCursor cursor, Array_View<CXCursor> members) {
	auto        spelling = clang_getCursorSpelling(cursor);
	const char *name     = clang_getCString(spelling);
	defer {
		clang_disposeString(spelling);
	};

	auto type = clang_getCursorType(cursor);
	auto size = clang_Type_getSizeOf(type);

	ostream_write_formatted(stream, "template <> struct Reflect<%s> {\n", name);
	ostream_write_formatted(stream, "\tstatic constexpr Type_Id id = Type_Id_UNION;\n");

	ostream_write_formatted(stream, "\tstatic const Type_Info * const info() {\n");

	if (members.count == 0) {
		ostream_write_formatted(stream, "\t\tstatic const Type_Info_Union i(%zd, \"%s\", 0, 0);\n", size, name);
		ostream_write_formatted(stream, "\t\treturn &i;\n\t}\n};\n\n");
		return;
	}

	Array<bool> has_attrs;
	array_resize(&has_attrs, members.count);
	defer {
		array_free(&has_attrs);
	};

	for (s64 index = 0; index < members.count; ++index) {
		if (clang_Cursor_isAnonymous(members[index])) {
			if (clang_getCursorKind(members[index]) == CXCursor_StructDecl) {
				reflection_of_anonymous_struct(stream, members[index], name, (int)index);
			} else if (clang_getCursorKind(members[index]) == CXCursor_UnionDecl) {
				reflection_of_anonymous_union(stream, members[index], name, (int)index);
			}
		}

		if (clang_Cursor_hasAttrs(members[index])) {
			array_add(&has_attrs, true);
			reflection_of_attrs(stream, members[index]);
		} else {
			array_add(&has_attrs, false);
		}
	}

	ostream_write_formatted(stream, "\t\tstatic const Union_Member union_members[] = {\n");

	for (s64 index = 0; index < members.count; ++index) {
		auto is_anonymous = clang_Cursor_isAnonymous(members[index]);

		auto spelling = clang_getCursorSpelling(members[index]);
		auto member   = clang_getCString(spelling);
		if (is_anonymous) member = "anonymous";

		auto member_type   = clang_getCursorType(members[index]);
		auto type_spelling = clang_getTypeSpelling(member_type);
		auto type_string   = clang_getCString(type_spelling);

		ostream_write_formatted(stream, "\t\t\t{ \"%s\", ", member);

		if (has_attrs[index]) {
			ostream_write_formatted(stream, "static_count(attrs_%s), attrs_%s, ", member, member);
		} else {
			ostream_write_formatted(stream, "0, 0, ");
		}

		if (!is_anonymous) {
			ostream_write_formatted(stream, "Reflect<%s>::info() },\n", type_string);
		} else {
			if (clang_getCursorKind(members[index]) == CXCursor_UnionDecl)
				ostream_write_formatted(stream, "&_i_anonymous_union_%d }, \n", (int)index);
			else if (clang_getCursorKind(members[index]) == CXCursor_StructDecl)
				ostream_write_formatted(stream, "&_i_anonymous_struct_%d }, \n", (int)index);
			else
				ostream_write_formatted(stream, "&_i_anonymous_uknown_%d }, \n", (int)index); // We never reach here
		}

		clang_disposeString(type_spelling);
		clang_disposeString(spelling);
	}

	ostream_write_formatted(stream, "\t\t};\n");

	ostream_write_formatted(stream,
							"\t\tstatic const Type_Info_Union i(%zd, \"%s\", static_count(union_members), union_members);\n", size, name);
	ostream_write_formatted(stream, "\t\treturn &i;\n\t}\n};\n\n");
}

void reflection_of_templated_struct(Ostream *stream, CXCursor cursor, Array_View<CXCursor> members, const char *base) {
	auto type = clang_getCursorType(cursor);

	auto        spelling = clang_getCursorSpelling(cursor);
	const char *name     = clang_getCString(spelling);
	defer {
		clang_disposeString(spelling);
	};

	const char *t_type = 0;
	const char *t_call = 0;

	{
		Array<CXCursor> template_type_params;
		defer {
			array_free(&template_type_params);
		};

		clang_visitChildren(
			cursor,
			[](CXCursor cursor, CXCursor parent, CXClientData client_data) -> CXChildVisitResult {
				if (clang_getCursorKind(cursor) == CXCursor_TemplateTypeParameter) {
					auto arr = (Array<CXCursor> *)client_data;
					array_add(arr, cursor);
					return CXChildVisit_Continue;
				}
				return CXChildVisit_Break;
			},
			&template_type_params);

		Ostream t_type_stream, t_call_stream;
		defer {
			ostream_free(&t_type_stream);
			ostream_free(&t_call_stream);
		};

		for (s64 i = 0; i < template_type_params.count; ++i) {
			auto        s      = clang_getCursorSpelling(template_type_params[i]);
			const char *string = clang_getCString(s);
			ostream_write_formatted(&t_type_stream, "typename %s", string);
			ostream_write_formatted(&t_call_stream, "%s", string);
			if (i < template_type_params.count - 1) {
				ostream_write_formatted(&t_type_stream, ", ");
				ostream_write_formatted(&t_call_stream, ", ");
			}
			clang_disposeString(s);
		}

		t_type = (char *)ostream_build_string(&t_type_stream, true).data;
		t_call = (char *)ostream_build_string(&t_call_stream, true).data;
	}

	if (t_type == 0 || t_call == 0) return;
	defer {
		memory_free(t_type);
		memory_free(t_call);
	};

	ostream_write_formatted(stream, "template <%s> struct Reflect<%s<%s>> {\n", t_type, name, t_call);
	ostream_write_formatted(stream, "\tstatic constexpr Type_Id id = Type_Id_STRUCT;\n");

	ostream_write_formatted(stream, "\tstatic const Type_Info * const info() {\n");
	ostream_write_formatted(stream, "\t\tusing Templated_Type = %s<%s>;\n", name, t_call);

	if (members.count == 0) {
		if (base == 0) {
			ostream_write_formatted(stream, "\t\tstatic const Type_Info_Struct i(sizeof(Templated_Type), \"%s\", 0, 0, 0);\n", name);
		} else {
			ostream_write_formatted(stream, "\t\tstatic const Type_Info_Struct i(sizeof(Templated_Type), \"%s\", 0, 0, Reflect<%s>::info());\n", name, base);
		}
		ostream_write_formatted(stream, "\t\treturn &i;\n\t}\n};\n\n");
		return;
	}

	Array<bool> has_attrs;
	array_resize(&has_attrs, members.count);
	defer {
		array_free(&has_attrs);
	};

	for (s64 index = 0; index < members.count; ++index) {
		if (clang_Cursor_hasAttrs(members[index])) {
			array_add(&has_attrs, true);
			reflection_of_attrs(stream, members[index]);
		} else {
			array_add(&has_attrs, false);
		}
	}

	ostream_write_formatted(stream, "\t\tstatic const Struct_Member struct_members[] = {\n");

	for (s64 index = 0; index < members.count; ++index) {
		auto spelling = clang_getCursorSpelling(members[index]);
		auto member   = clang_getCString(spelling);

		auto member_type   = clang_getCursorType(members[index]);
		auto type_spelling = clang_getTypeSpelling(member_type);
		auto type_string   = clang_getCString(type_spelling);

		ostream_write_formatted(stream, "\t\t\t{ \"%s\", offsetof(Templated_Type, %s), ", member, member);

		if (has_attrs[index]) {
			ostream_write_formatted(stream, "static_count(attrs_%s), attrs_%s, ", member, member);
		} else {
			ostream_write_formatted(stream, "0, 0, ");
		}

		ostream_write_formatted(stream, "Reflect<%s>::info() },\n", type_string);

		clang_disposeString(type_spelling);
		clang_disposeString(spelling);
	}

	ostream_write_formatted(stream, "\t\t};\n");

	if (base == 0) {
		ostream_write_formatted(stream,
								"\t\tstatic const Type_Info_Struct i(sizeof(Templated_Type), \"%s<%s>\", static_count(struct_members), struct_members, 0);\n",
								name, t_call);
	} else {
		ostream_write_formatted(stream,
								"\t\tstatic const Type_Info_Struct i(sizeof(Templated_Type), \"%s<%s>\", static_count(struct_members), struct_members, Reflect<%s>::info());\n",
								name, t_call, base);
	}
	ostream_write_formatted(stream, "\t\treturn &i;\n\t}\n};\n\n");
}

struct Output_Info {
	Ostream reflected_enum;
	Ostream reflected_struct;
};

void ast_visit_enum(CXCursor cursor);
void ast_visit_struct(CXCursor cursor);
void ast_visit_union(CXCursor cursor);
void ast_visit_tempalted_struct(CXCursor cursor);

CXChildVisitResult visit_enum_members(CXCursor cursor, CXCursor parent, CXClientData client_data) {
	if (clang_getCursorKind(cursor) == CXCursor_EnumConstantDecl) {
		auto members = (Array<CXCursor> *)client_data;
		array_add(members, cursor);
	}
	return CXChildVisit_Continue;
}

void ast_visit_enum(CXCursor cursor) {
	auto out = (Output_Info *)context.data;

	Array<CXCursor> members;
	auto            spelling = clang_getTypeSpelling(clang_getCursorType(cursor));
	CXType item_type		 = clang_getEnumDeclIntegerType(cursor);
	auto item_type_spelling  = clang_getTypeSpelling(item_type);

	defer {
		clang_disposeString(spelling);
		clang_disposeString(item_type_spelling);
		array_free(&members);
	};

	clang_visitChildren(cursor, visit_enum_members, &members);

	bool sequential = true;
	s64  min = 0, max = 0;

	if (members.count) {
		auto a = clang_getEnumConstantDeclValue(members[0]);
		max    = a;
		min    = a;
		if (sequential) {
			for (s64 index = 1; index < members.count; ++index) {
				auto b = clang_getEnumConstantDeclValue(members[index]);
				min    = b < min ? b : min;
				max    = b > max ? b : max;
				if (a + 1 != b) sequential = false;
				a = b;
			}
		}
	}

	const char *name	  = clang_getCString(spelling);
	const char *type_name = clang_getCString(item_type_spelling);
	reflection_of_enum_info(&out->reflected_enum, cursor, name, members, min, max, sequential);
	reflection_of_enum(&out->reflected_enum, name, type_name, sequential);
}

bool struct_check_for_base(CXCursor derived, CXCursor *out) {
	struct Result {
		bool     found;
		CXCursor cursor;
	};

	Result result;
	result.found = false;

	clang_visitChildren(
		derived,
		[](CXCursor cursor, CXCursor parent, CXClientData client_data) -> CXChildVisitResult {
			if (clang_getCursorKind(cursor) == CXCursor_CXXBaseSpecifier) {
				auto out    = (Result *)client_data;
				out->cursor = cursor;
				out->found  = true;
			}
			return CXChildVisit_Continue;
		},
		&result);

	if (result.found) *out = result.cursor;
	return result.found;
}

CXChildVisitResult visit_members(CXCursor cursor, CXCursor parent, CXClientData client_data) {
	auto kind = clang_getCursorKind(cursor);

	if (kind == CXCursor_FieldDecl) {
		auto members = (Array<CXCursor> *)client_data;
		array_add(members, cursor);
	} else if (kind == CXCursor_EnumDecl) {
		ast_visit_enum(cursor);
	} else if (kind == CXCursor_StructDecl) {
		if (clang_Cursor_isAnonymous(cursor)) {
			auto members = (Array<CXCursor> *)client_data;
			array_add(members, cursor);
		} else {
			ast_visit_struct(cursor);
		}
	} else if (kind == CXCursor_UnionDecl) {
		if (clang_Cursor_isAnonymous(cursor)) {
			auto members = (Array<CXCursor> *)client_data;
			array_add(members, cursor);
		} else {
			ast_visit_union(cursor);
		}
	} else if (kind == CXCursor_ClassTemplate) {
		ast_visit_tempalted_struct(cursor);
	}

	return CXChildVisit_Continue;
}

void ast_visit_struct(CXCursor cursor) {
	auto out = (Output_Info *)context.data;

	auto struct_spelling = clang_getCursorSpelling(cursor);
	const char *struct_name = clang_getCString(struct_spelling);
	defer {
		clang_disposeString(struct_spelling);
	};

	static const char* ignored_structs[] = {
		"Compile_Info",
		"String",
		"Buffer",
	};

	for (int index = 0; index < static_count(ignored_structs); ++index) {
		if (strcmp(struct_name, ignored_structs[index]) == 0) {
			return;
		}
	}

	Array<CXCursor> members;
	defer {
		array_free(&members);
	};

	clang_visitChildren(cursor, visit_members, &members);

	CXCursor    base_cursor;
	CXString    base_spelling;
	const char *base = 0;
	if (struct_check_for_base(cursor, &base_cursor)) {
		base_spelling = clang_getCursorSpelling(base_cursor);
		base          = clang_getCString(base_spelling);
	}

	reflection_of_struct(&out->reflected_struct, cursor, members, base);

	if (base) clang_disposeString(base_spelling);
}

void ast_visit_union(CXCursor cursor) {
	auto out = (Output_Info *)context.data;

	Array<CXCursor> members;
	defer {
		array_free(&members);
	};
	clang_visitChildren(cursor, visit_members, &members);

	reflection_of_union(&out->reflected_struct, cursor, members);
}

void ast_visit_tempalted_struct(CXCursor cursor) {
	auto out = (Output_Info *)context.data;

	auto struct_spelling = clang_getCursorSpelling(cursor);
	const char *struct_name = clang_getCString(struct_spelling);
	defer {
		clang_disposeString(struct_spelling);
	};

	static const char* ignored_structs[] = {
		"ExitScope",
		"Array",
		"Array_View",
	};

	for (int index = 0; index < static_count(ignored_structs); ++index) {
		if (strcmp(struct_name, ignored_structs[index]) == 0) {
			return;
		}
	}

	Array<CXCursor> members;
	defer {
		array_free(&members);
	};

	clang_visitChildren(cursor, visit_members, &members);

	CXCursor    base_cursor;
	CXString    base_spelling;
	const char *base = 0;
	if (struct_check_for_base(cursor, &base_cursor)) {
		base_spelling = clang_getCursorSpelling(base_cursor);
		base          = clang_getCString(base_spelling);
	}

	reflection_of_templated_struct(&out->reflected_struct, cursor, members, base);

	if (base) clang_disposeString(base_spelling);
}

CXChildVisitResult ast_visitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
	auto kind = clang_getCursorKind(cursor);
	auto out  = (Output_Info *)context.data;

	// Ignore Global ananymous structs/unions
	if (clang_Cursor_isAnonymous(cursor)) return CXChildVisit_Continue;
	// Ignore declarations
	if (!clang_isCursorDefinition(cursor)) return CXChildVisit_Continue;

	switch (kind) {
		case CXCursor_EnumDecl: {
			ast_visit_enum(cursor);

			return CXChildVisit_Continue;
		} break;

		case CXCursor_StructDecl: {
			ast_visit_struct(cursor);

			return CXChildVisit_Continue;
		} break;

		case CXCursor_UnionDecl: {
			ast_visit_union(cursor);

			return CXChildVisit_Continue;
		} break;

		case CXCursor_ClassTemplate: {
			ast_visit_tempalted_struct(cursor);

			return CXChildVisit_Continue;
		} break;

		default: {
		} break;
	}

	return CXChildVisit_Continue;
}

/*
 * CommandLine Options:
 *		-include:'{include_directory_path}'					optional
 *		-defination:'{PREPROCESSOR_DEFINATION} [VALUE]'		optional
 *		-build:'{path_to_file_to_reflect}'					required
 *		-output:'{reflected_source_code}'					one required
 *		-help: Displays usage information
*/

constexpr int KARMA_REFLECTION_VERSION_MAJOR = 1;
constexpr int KARMA_REFLECTION_VERSION_MINOR = 0;
constexpr int KARMA_REFLECTION_VERSION_PATCH = 0;

void command_line_print_help() {
	auto spelling = clang_getClangVersion();
	printf("Karma Reflection (version %d.%d.%d) ",
		   KARMA_REFLECTION_VERSION_MAJOR, KARMA_REFLECTION_VERSION_MINOR, KARMA_REFLECTION_VERSION_PATCH);
	printf("(%s)\n", clang_getCString(spelling));
	printf("\t-include:'{include_directory_path}'                optional\n");
	printf("\t-defination:'{PREPROCESSOR_DEFINATION [VALUE]}'    optional\n");
	printf("\t-build:'{path_to_file_to_reflect}'                 required\n");
	printf("\t-output:'{reflected_source_code}'                  one required\n");
	printf("\t-help: Displays usage information\n");
	clang_disposeString(spelling);
}

void command_line_error_print_location(String string, ptrsize location) {
	printf("\t");
	for (s64 i = 0; i < string.count; ++i)
		printf("%c", string[i]);
	printf("\n\t");
	for (ptrsize i = 0; i < location - 1; ++i)
		printf(" ");
	printf("^\n");
}

bool parse_command_line(String command_line, Array<char *> *options, Array<char *> *files, char **output) {
	// Skipping first string because it is path to executable
	// In windows if path to executable has space then the path is inclosed in quotes
	String search_for;
	if (command_line[0] == '"')
		search_for = "\"";
	else
		search_for = " ";

	auto find_res = string_isearch(command_line, search_for);
	if (!find_res.found) {
		command_line_print_help();
		return false;
	}

	String string     = string_substring(command_line, find_res.start_index, command_line.count - find_res.start_index);
	String executable = string_substring(command_line, 0, find_res.start_index + 1);

	Tokenization_Status status;
	auto                tokens = tokenize(string, &status);
	defer {
		memory_free(tokens.data);
	};

	if (status.result != Tokenization_Result_SUCCESS) {
		fprintf(stderr, "Error parsing command line arguments at %zd\n", status.column);
		command_line_error_print_location(string, status.column);
		if (status.result == Tokenization_Result_ERROR_NO_MATCHING_PARENTHESIS)
			fprintf(stderr, "Matching parenthesis not found!");
		else
			fprintf(stderr, "Unrecognized token!");
		return false;
	}

	if (!tokens.count) {
		fprintf(stderr, "Invalid usage!\n");
		printf("USAGE: %s help", tto_cstring(executable));
		return 0;
	}

	if (tokens.count == 1 && string_match(tokens[0].content, "help")) {
		command_line_print_help();
		return false;
	}

	// Default options
	array_add(options, (char *)"-x");
	array_add(options, (char *)"c++");
	array_add(options, (char *)"-std=c++17");
	array_add(options, (char *)"-Wno-pragma-once-outside-header");
	array_add(options, (char *)"-DRUN_REFLECTION");
	*output = 0;

	s64 i = 0;
	for (i = 0; i + 2 < tokens.count; i += 3) {
		auto key   = tokens[i + 0];
		auto colon = tokens[i + 1];
		auto value = tokens[i + 2];

		if (key.kind != Token_Kind_IDENTIFIER) {
			fprintf(stderr, "Expected identifier, found %s\n", tto_cstring(key.content));
			command_line_error_print_location(string, key.column);
			printf("USAGE: %s help", tto_cstring(executable));
			return false;
		}

		if (colon.kind != Token_Kind_COLON) {
			fprintf(stderr, "Unrecognized syntax, expected key:value pairs\n");
			command_line_error_print_location(string, colon.column);
			printf("USAGE: %s help", tto_cstring(executable));
			return false;
		}

		if (value.kind != Token_Kind_SQ_STRING) {
			fprintf(stderr, "Unrecognized syntax, expected single quoted string as value\n");
			command_line_error_print_location(string, value.column);
			printf("USAGE: %s help", tto_cstring(executable));
			return false;
		}

		if (string_match(key.content, "include")) {
			String option = string_tconcat("-I", value.value.string);
			array_add(options, tto_cstring(option));
		} else if (string_match(key.content, "defination")) {
			String option = string_tconcat("-D", value.value.string);
			array_add(options, tto_cstring(option));
		} else if (string_match(key.content, "build")) {
			array_add(files, tto_cstring(value.value.string));
		} else if (string_match(key.content, "output")) {
			if (*output == 0) {
				*output = tto_cstring(value.value.string);
			} else {
				fprintf(stderr, "Only output file is accepted!\n");
				command_line_error_print_location(string, key.column);
				return false;
			}
		} else if (string_match(key.content, "help")) {
			fprintf(stderr, "Unexpected option 'help'. 'help' can only be used alone.\n");
			command_line_error_print_location(string, key.column);
			printf("USAGE: %s help", tto_cstring(executable));
			return false;
		} else {
			fprintf(stderr, "Unrecognized identifier '%s'\n", tto_cstring(key.content));
			command_line_error_print_location(string, key.column);
			printf("USAGE: %s help", tto_cstring(executable));
			return false;
		}
	}

	if (i < tokens.count && tokens[i].kind != Token_Kind_END_OF_STREAM) {
		fprintf(stderr, "Unexpected end of arguments\n");
		command_line_error_print_location(string, tokens[i].column);
		printf("USAGE: %s help", tto_cstring(executable));
		return false;
	}

	bool success = true;

	if (files->count == 0) {
		fprintf(stderr, "Missing *build* option. At least one build is required. Specify \"build:'path_to_source_code'\"\n");
		success = false;
	}

	if (*output == 0) {
		fprintf(stderr, "Missing *output* option. Specify \"header:'reflected_code_file'\"\n");
		success = false;
	}

	return success;
}

int system_main() {
	Array<char *> options, files;
	char *        out_file = 0;
	if (!parse_command_line(get_command_line(), &options, &files, &out_file)) {
		return -1;
	}

	CXIndex index = clang_createIndex(0, 0);

	u32 flags = CXTranslationUnit_SkipFunctionBodies;

	Output_Info out = {};
	context.data    = &out;

	for (s64 file_index = 0; file_index < files.count; ++file_index) {
		printf("Reflecting '%s'...", files[file_index]);

		CXTranslationUnit unit = clang_parseTranslationUnit(index, files[file_index], options.data, (int)options.count, 0, 0, flags);

		bool error = false;
		for (unsigned i = 0, n = clang_getNumDiagnostics(unit); i != n; ++i) {
			CXDiagnostic diag = clang_getDiagnostic(unit, i);

			auto location = clang_getDiagnosticLocation(diag);

			CXFile       file;
			unsigned int line, column, offset;
			clang_getSpellingLocation(location, &file, &line, &column, &offset);
			auto file_spelling = clang_File_tryGetRealPathName(file);

			u32 diag_flags = CXDiagnostic_DisplaySourceRanges | CXDiagnostic_DisplayOption;
			diag_flags |= CXDiagnostic_DisplayCategoryId | CXDiagnostic_DisplayCategoryName;
			auto diag_spelling = clang_formatDiagnostic(diag, diag_flags);

			fprintf(stderr, "%s(%u, %u) : %s\n", clang_getCString(file_spelling), line, column, clang_getCString(diag_spelling));

			clang_disposeString(file_spelling);
			clang_disposeString(diag_spelling);
			error = true;
		}
		if (error) return -1;

		if (unit == 0) {
			printf("Unable to parse translation unit. Quitting.");
			return -1;
		}

		clang_visitChildren(clang_getTranslationUnitCursor(unit), ast_visitor, 0);

		printf("done.\n");
		clang_disposeTranslationUnit(unit);
	}

	String reflected_enum   = ostream_build_string(&out.reflected_enum, true);
	String reflected_struct = ostream_build_string(&out.reflected_struct, true);

	ostream_free(&out.reflected_enum);
	ostream_free(&out.reflected_struct);

	defer {
		memory_free(reflected_enum.data);
		memory_free(reflected_struct.data);
	};

	printf("Exporting '%s'...", out_file);

	FILE *header = fopen(out_file, "wb");
	defer {
		fclose(header);
	};

	fprintf(header, "#pragma once\n");
	fprintf(header, "\n%s", reflected_enum.data);
	fprintf(header, "\n%s", reflected_struct.data);

	printf("done.\n");

	clang_disposeIndex(index);
	return 0;
}
