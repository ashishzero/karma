#include "modules/core/lin_maths.h"
#include "entity.h"
struct Grid {
	Vec2 position;
	r32 dimension;
	int no_of_object;
	Rigid_Body *next_member;
};

struct HGrid {
	Grid *grids;
	u32 count;
	r32 size;
	u32 level;
};

HGrid hgrid_create(u32 level, float dimension);
void hgrid_add_body_to_grid(HGrid *hgrid, Rigid_Body *body);
void hgrid_remove_body_from_grid(HGrid *hgrid, Rigid_Body *body);
void hgrid_check_collision(HGrid *start, Rigid_Body *body);
void hgrid_move_body(HGrid *start, Rigid_Body *rigid_body);
bool hgrid_test_collision(HGrid *hgrid, Rigid_Body *a, Rigid_Body *b);
