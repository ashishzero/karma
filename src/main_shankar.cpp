#include "modules/core/systems.h"

#define SOCKET_BUFFER_SIZE kilo_bytes(1)

int karma_user_shankar() {
	system_net_startup();

	Socket_Address to = socket_address_local(9999);

	auto socket = system_net_open_udp_client();

	if (socket != SOCKET_INVALID) {
		int d = (to.address & 0x000000ff) >> 0;
		int c = (to.address & 0x0000ff00) >> 8;
		int b = (to.address & 0x00ff0000) >> 16;
		int a = (to.address & 0xff000000) >> 24;
		system_trace("Client Connected to: %d.%d.%d.%d:%d", a, b, c, d, to.port);
	}

	char buffer[SOCKET_BUFFER_SIZE];
	s32 player_x = 0;
	s32 player_y = 0;

	bool running = true;

	while (running) {
		buffer[0] = 0;

		if (system_key(Key_W) || system_key(Key_UP)) {
			buffer[0] = 'w';
		}
		
		if (system_key(Key_A) || system_key(Key_LEFT)) {
			buffer[0] = 'a';
		}
		
		if (system_key(Key_S) || system_key(Key_DOWN)) {
			buffer[0] = 's';
		}
		
		if (system_key(Key_D) || system_key(Key_RIGHT)) {
			buffer[0] = 'd';
		}

		if (system_key(Key_Q)) {
			buffer[0] = 'q';
			running = false;
		}
		
		if (system_key(Key_ESCAPE)) {
			running = false;
		}

		if (buffer[0]) {
			int buffer_length = 1;
			if (system_net_send_to(socket, buffer, buffer_length, to) < 0) {
				system_trace("Falied to send!");
				break;
			}

			Socket_Address from;
			int bytes_received = system_net_receive_from(socket, buffer, SOCKET_BUFFER_SIZE, &from);

			if (bytes_received < 0) {
				system_trace("Failed to receive data");
				break;
			}

			// grab data from packet
			int read_index = 0;

			memcpy(&player_x, &buffer[read_index], sizeof(player_x));
			read_index += sizeof( player_x );

			memcpy(&player_y, &buffer[read_index], sizeof(player_y));
			read_index += sizeof( player_y );

			memcpy(&running, &buffer[read_index], sizeof(running));

			system_trace("x:%d, y:%d, is_running:%d", player_x, player_y, running);
		}
	}

	return 0;
}
