#pragma once
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
	Atishs x = { 56,78,34 };
	float h=34.987f;
	int* a_ptr;
};

