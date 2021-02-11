#include "modules/core/systems.h"
#include "karma_config.h"

int karma_client();
int karma_server() { return 0; }

int karma_user_zero();
int karma_user_atish();
int karma_user_shankar();
int karma_user_pujan();
int karma_main_template();

int main() {
	#ifdef BUILD_CLIENT_APP
	return karma_client();
	#else

	auto user = system_read_entire_file("dev/local.karma");

	if (!user.data) {
		user = system_get_user_name();
	}

	if (string_match(user, "user") || string_match(user, "zeroa")) {
		return karma_user_zero();
	} else if (string_match(user, "atish")) {
		return karma_user_atish();
	} else if (string_match(user, "sankarg5")) {
		return karma_user_shankar();
	} else if (string_match(user, "leopujan")) {
		return karma_user_pujan();
	} else if (string_match(user, "template")) {
		return karma_main_template();
	}
	#endif

	return 0;
}

Builder system_builder() {
	Builder builder;
	builder.allocator = system_default_heap_allocator();
	builder.entry = main;
	builder.temporary_buffer_size = mega_bytes(128);
	builder.flags = Builder_ALL;

	return builder;
}
