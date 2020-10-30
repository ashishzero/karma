#include "modules/core/systems.h"

int karma_user_zero();
int karma_user_atish();
int karma_user_shankar();
int karma_user_pujan();

int system_main() {
	auto user = system_read_entire_file("dev/local.karma");

	if (!user.data) {
		user = system_get_user_name();
	}
				
	if (string_match(user, "user")) {
		return karma_user_zero();
	} else if (string_match(user, "atish")) {
		return karma_user_atish();
	} else if (string_match(user, "sankarg5")) {
		return karma_user_shankar();
	} else if (string_match(user, "leopujan")) {
		return karma_user_pujan();
	}

	return 0;
}
