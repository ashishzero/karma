#pragma once
#include "modules/core/length_string.h"
#include "modules/core/reflection.h"

// Example struct

struct asset_info {
    String name;
    u64 size_of_file;
    u64 file_location;
};

struct Vector2 {
    float x=1;
    float y=2;
};

enum Block_Type :unsigned int {
    Block_Type_INVALID,
    Block_Type_HARD_BLOCK,
    Block_Type_SOFT_BLOCK,
};

struct Base {
    unsigned int id=3;
    Block_Type type= Block_Type_SOFT_BLOCK;
    Block_Type type4= Block_Type_INVALID;
    Base() {}
    Base(Block_Type _type) : type(_type) {}
};

struct Block_Data {
    int index;
    unsigned int flags;
};

struct World {
    int x;
    int y;
};

struct Region {
    int x;
    int y;
    Region(int a, int b)
    {
        x = a;
        y = b;
    }
};

struct Hard_Block:public Base {
    Vector2 position = {3,6.9f};
    Vector2 size = {2.1f,4.5f};
    Block_Data data[2] = { {1,9},{3,5} };
    String name="barud";
    Array<Vector2> placements;
    //Array_View<int> placements_2;
    attribute(no_serialize) World* world;
    Region* region=new Region(3,2);

    Hard_Block() : Base(Block_Type_HARD_BLOCK) {}
};

// Input to serialize function is Hard_Block, the output should be
// The output text file should look the following way
