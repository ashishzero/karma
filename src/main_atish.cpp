#pragma once

#include "modules/core/reflection.h"
#include "modules/core/karma.h"
#include "modules/core/systems.h"
#include "modules/core/lin_maths.h"
#include "modules/core/utility.h"
#include "modules/core/serialize.h"
#include "atish.h"
#include ".generated/atish.typeinfo"

int karma_user_atish() {
	FILE *fp;
	fp = fopen("temp/savegame.txt", "w+");

	if (fp == NULL) {
		printf("file not opened");
		exit(1);
	}

	Array<Vector2> a1, a2;
	array_add(&a1, Vector2{ 1, 2 });
	array_add(&a1, Vector2{ 3, 3 });
	array_add(&a1, Vector2{ 4, 3 });

	array_add(&a2, Vector2{ 9, 9 });

	Hard_Block sample;
	sample.placements = a1;

	serialize_to_file(fp, "sample", reflect_info(sample), (char *)&sample);

	rewind(fp);

	Hard_Block *retrieve = (Hard_Block *)memory_allocate(sizeof(sample));
	deserialize_from_file(fp, "sample", reflect_info(sample), (char *)retrieve);

	fclose(fp);
	return 0;
}
