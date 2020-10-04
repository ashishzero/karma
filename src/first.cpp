#include "modules/core/systems.h"

int karma_user_zero();
int karma_user_atish();
int karma_user_shankar();
int karma_user_pujan();

int main() {
	auto user = system_read_entire_file("dev/local.karma");

	if (!user.data) {
		user = system_get_user_name();
	}

	if (string_match(user, "user")|| string_match(user, "zeroa")) {
		return karma_user_zero();
	} else if (string_match(user, "atish")) {
		return karma_user_atish();
	} else if (string_match(user, "shankar")) {
		return karma_user_shankar();
	} else if (string_match(user, "leopujan")) {
		return karma_user_pujan();
	}

	return 0;
}

Builder system_builder() {
	Builder builder;
	builder.allocator = system_create_heap_allocator();
	builder.entry = main;
	builder.temporary_buffer_size = mega_bytes(128);
	builder.flags = Builder_ALL;
	return builder;
}
