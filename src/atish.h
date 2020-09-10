#pragma once
#include "modules/core/length_string.h"
#include "modules/core/reflection.h"

struct Atishs {
	int a;
	int b;
	int c;
};

struct Atish {
	enum Number {
		ONE,
		TWO,
		THREE,
		FOUR,
		FIVE,
	};
	Number n = ONE;
	int a=2;
	int b=10;
	String name = "Ashish";
	attribute ("no-serialize") Atishs att = {1,2,3};
	Atishs x[3] = { { 56,78,34 },{ 56,78,34 },{ 56,78,34 } };
	float h=34.987f;
	int* a_ptr;
	Array<String> strings;
	Array_View<int> values;
};

