#include "modules/core/systems.h"

#define SOCKET_BUFFER_SIZE kilo_bytes(1)

int system_main();

Builder system_builder() {
	Builder builder;
	builder.allocator = system_create_heap_allocator();
	builder.entry = system_main;
	builder.temporary_buffer_size = mega_bytes(128);
	builder.flags = Builder_NETWORK;
	return builder;
}

int system_main() {	
	system_net_startup();

	Socket_Address address = socket_address_local(9999);
	Socket socket = system_net_open_udp_server(address);

	if (socket != SOCKET_INVALID) {
		int d = (address.address & 0x000000ff) >> 0;
		int c = (address.address & 0x0000ff00) >> 8;
		int b = (address.address & 0x00ff0000) >> 16;
		int a = (address.address & 0xff000000) >> 24;
		printf("Server Launched: %d.%d.%d.%d:%d\n", a, b, c, d, address.port);
	}

	char buffer[SOCKET_BUFFER_SIZE];
	s32 player_x = 0;
	s32 player_y = 0;
	bool running = true;

	while (running) {
		Socket_Address from;
		int bytes_received = system_net_receive_from(socket, buffer, SOCKET_BUFFER_SIZE, &from);

		if(bytes_received < 0) {
			printf("Failed to receive data\n");
			break;
		}

		int d = (from.address & 0x000000ff) >> 0;
		int c = (from.address & 0x0000ff00) >> 8;
		int b = (from.address & 0x00ff0000) >> 16;
		int a = (from.address & 0xff000000) >> 24;

		char client_input = buffer[0];
		printf("%d.%d.%d.%d:%d - %c\n", a, b, c, d, from.port, client_input);

		switch (client_input) {
		case 'w': player_y += 1; break;
		case 's': player_y -= 1; break;
		case 'a': player_x -= 1; break;
		case 'd': player_x += 1; break;
		case 'q': running = false; break;
		}

		int bytes_written = 0;
		memcpy(&buffer[bytes_written], &player_x, sizeof(player_x));
		bytes_written += sizeof(player_x);

		memcpy(&buffer[bytes_written], &player_y, sizeof(player_y));
		bytes_written += sizeof(player_y);

		memcpy(&buffer[bytes_written], &running, sizeof(running));
		bytes_written += sizeof(running);

		int bytes_send = system_net_send_to(socket, buffer, bytes_written, from);
		if (bytes_send < 0) {
			printf("Bytes failed to send\n");
			break;
		}

	}

	return 0;
}
