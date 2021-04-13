#include "modules/core/systems.h"

int karma_client();
int karma_main_template();

int main() {
	//return karma_main_template(); // uncomment this line to run the template file
	return karma_client();
}

Builder system_builder() {
	Builder builder;
	builder.allocator = system_default_heap_allocator();
	builder.entry = main;
	builder.temporary_buffer_size = mega_bytes(128);
	builder.flags = Builder_ALL;

	return builder;
}
