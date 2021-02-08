#include "broad_phase.h"

bool hgrid_fully_fits(Vec2 R1, r32 dimension, Mm_Rect rect) {
	if (R1.x <= rect.min.x && R1.y <= rect.min.y && (R1.x + dimension) >= rect.max.x && (R1.y + dimension) >= rect.max.y)
		return true;
	return false;
}

void hgrid_add_body_to_grid(HGrid *hgrid, Rigid_Body *body) {
	auto grids = hgrid->grids;
	auto count = hgrid->count;
	assert(hgrid_fully_fits(grids->position, grids->dimension, body->bounding_box));
	bool collided_with_parent = true;
	u32 parent_index = 0;
	u32 my_grid_level = 0;

	while ((parent_index * 4 + 1) < count && collided_with_parent) {
		collided_with_parent = false;
		for (int i = 1; i <= 4; i++) {
			auto child = grids + 4 * parent_index + i;
			if (hgrid_fully_fits(child->position, child->dimension, body->bounding_box)) {
				collided_with_parent = true;
				parent_index = 4 * parent_index + i;
				my_grid_level++;
				break;
			}
		}
	}

	struct Rigid_Body *temp;
	auto grid = grids + parent_index;
	temp = grid->next_member;
	if (temp == NULL) {
		temp = body;
		grid->no_of_object++;
		grid->next_member = body;
		body->grid_index = parent_index;
		body->next = NULL;
		body->level = my_grid_level;
	} else {
		while (temp->next != NULL) {
			temp = (Rigid_Body *)temp->next;
		}
		temp->next = body;
		grid->no_of_object++;
		body->grid_index = parent_index;
		body->next = NULL;
		body->level = my_grid_level;
	}

}

void iterate_and_remove_linklist(Grid *start, Rigid_Body *body) {
	struct Rigid_Body *temp, *prev = NULL;
	temp = start->next_member;
	start->no_of_object--;
	while (temp != body) {
		prev = temp;
		temp = (Rigid_Body *)temp->next;
	}
	if (prev == NULL) {
		start->next_member = (Rigid_Body *)temp->next;
	} else {
		prev->next = temp->next;
	}
}

void hgrid_remove_body_from_grid(HGrid *hgrid, Rigid_Body *body) {
	assert(body->grid_index < hgrid->count);
	iterate_and_remove_linklist(hgrid->grids + body->grid_index, body);
}

void hgrid_collision_check_objects(HGrid *hgrid, Rigid_Body *body, u32 index) {
	auto start = hgrid->grids;
	auto count = hgrid->count;

	auto grid = start + index;
	auto temp = grid->next_member;

	u32 indexes[64];
	int top = 0;
	indexes[top] = index;

	while (top >= 0) {
		u32 child_index = indexes[top];
		if ((child_index * 4 + 1) < count) {
			indexes[top] = child_index * 4 + 4;
			top++;
			indexes[top] = child_index * 4 + 3;
			top++;
			indexes[top] = child_index * 4 + 2;
			top++;
			indexes[top] = child_index * 4 + 1;
		} else {
			top--;
		}

		grid = start + child_index;
		auto temp = grid->next_member;

		while (temp != NULL) {
			if (temp == body) {
				temp = (Rigid_Body *)temp->next;
				continue;
			}
			if (test_mmrect_vs_mmrect(temp->bounding_box, body->bounding_box)) {
				set_bit(temp->flags, Rigid_Body_BOUNDING_BOX_COLLIDING);
				set_bit(body->flags, Rigid_Body_BOUNDING_BOX_COLLIDING);

			}
			temp = (Rigid_Body *)temp->next;
		}
	}

}

void hgrid_check_collision(HGrid *start, Rigid_Body *body) {
	hgrid_collision_check_objects(start, body, body->grid_index);
}

void initialize_grid(Grid *grids, u32 index, u32 count, r32 dimension, Vec2 position) {
	auto grid = grids + index;

	grid->no_of_object = 0;
	grid->position = position;
	grid->dimension = dimension;
	grid->next_member = NULL;

	if ((index * 4 + 1) < count) {
		dimension *= 0.5;
		initialize_grid(grids, index * 4 + 1, count, dimension, position + vec2(0, dimension));
		initialize_grid(grids, index * 4 + 2, count, dimension, position + vec2(dimension, dimension));
		initialize_grid(grids, index * 4 + 3, count, dimension, position);
		initialize_grid(grids, index * 4 + 4, count, dimension, position + vec2(dimension, 0));
	}
}

HGrid hgrid_create(u32 level, float dimension) {
	u32 total_grids = 0;
	for (u32 i = 0; i < level; i++)
		total_grids += (int)powf(4.0f, (float)i);

	struct Grid *start = new Grid[total_grids];
	initialize_grid(&start[0], 0, total_grids, dimension, vec2(-dimension / 2));
	return { start,total_grids,dimension,level };
}

void hgrid_move_body(HGrid *start, Rigid_Body *rigid_body) {
	hgrid_remove_body_from_grid(start, rigid_body);
	hgrid_add_body_to_grid(start, rigid_body);
}

bool hgrid_test_collision(HGrid *hgrid, Rigid_Body *a, Rigid_Body *b) {
	u32 min_level;
	u32 max_level;

	Rigid_Body *temp = nullptr;
	
	u32 left_most_index;
	u32 right_most_index;
	
	if (a->level == b->level) {
		if (a->grid_index != b->grid_index)
			return false;
		else
			return true;
	}

	if (a->level < b->level) {
		min_level = a->level;
		temp = b;
		left_most_index = a->grid_index;
		max_level = b->level;
	} else {
		min_level = b->level;
		temp = a;
		left_most_index = b->grid_index;
		max_level = a->level;
	}

	right_most_index = left_most_index;
	
	for (u32 i = min_level; i <= max_level; i++) {
		if (temp->grid_index >= (4 * left_most_index + 1) && temp->grid_index <= (4 * right_most_index + 4))
			return true;
		left_most_index = 4 * left_most_index + 1;
		right_most_index = 4 * right_most_index + 4;
	}

	return false;
}
