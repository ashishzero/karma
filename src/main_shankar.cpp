#include "modules/core/systems.h"
#include <winsock2.h>
//#include <time.h>
#include "modules/core/utility.h"

#define SOCKET_BUFFER_SIZE kilo_bytes(1)


enum  Client_Message : u8
{
	JOIN,		// tell server we're new here
	LEAVE,		// tell server we're leaving
	INPUTS,	// tell server our user input
	READY
};

enum  Server_Message : u8
{
	JOIN_RESULT,// tell client they're accepted/rejected
	STATE,		// tell client game state
	START
};



struct Player_State
{
	r32 x, y, facing;
};

const u32	TICKS_PER_SECOND = 60;
const r32	SECONDS_PER_TICK = 1.0f / s32(TICKS_PER_SECOND);

static s32 time_since(LARGE_INTEGER t, LARGE_INTEGER frequency)
{
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);

	return s32(now.QuadPart - t.QuadPart) / s32(frequency.QuadPart);
}


int karma_user_shankar() {
	Ip_Endpoint to = ip_endpoint_local(9999);

	auto socket = system_net_open_udp_client();

	if (socket != SOCKET_INVALID) {
		int d = (to.address & 0x000000ff) >> 0;
		int c = (to.address & 0x0000ff00) >> 8;
		int b = (to.address & 0x00ff0000) >> 16;
		int a = (to.address & 0xff000000) >> 24;
		system_trace("Client Connected to: %d.%d.%d.%d:%d", a, b, c, d, to.port);
	}


	system_net_set_socket_nonblocking(socket);

	char buffer[SOCKET_BUFFER_SIZE];
	u8 send_packet_id = 0;
	s32 player_x = 0;
	s32 player_y = 0;

	bool running = true;


	Player_State objects;
	u32 num_objects = 0;
	u16 slot = 0xFFFF;


	buffer[0] = (u8)Client_Message::JOIN;
	bool fail = true;

	if (system_net_send_to(socket, buffer, sizeof(buffer[0]), to) < 0)
	{
		system_trace("join message failed to send\n");
		return 0;
	}
	else
	{
		system_trace("join message sent\n");
	}

	//	Timing_Info timing_info = timing_info_create();

	UINT sleep_granularity_ms = 1;
	bool sleep_granularity_was_set = timeBeginPeriod(sleep_granularity_ms) == TIMERR_NOERROR;  //bool 32
	LARGE_INTEGER clock_frequency;
	QueryPerformanceFrequency(&clock_frequency);
	bool received_ready_once = true;
	bool connected_to_server = false;


	while (running)
	{

		Ip_Endpoint from;
		int ready_received = 1;
		while (ready_received){
			ready_received = system_net_receive_from(socket, buffer, SOCKET_BUFFER_SIZE, &from);
			if (ready_received < 0) {
				break;
			}
			else {
				system_trace("%s", buffer);
			}
			system_trace("loop\n");

		}

		if (system_key(Key_Q)) {
			running = false;
			break;
		}

		if (buffer[0] == (u8)Server_Message::JOIN_RESULT) {
			if (buffer[1]){
				memcpy(&slot, &buffer[2], sizeof(slot));
				system_trace("server let us in\n");
			}
			else{
				system_trace("server didn't let us in\n");
			}
		}

		if (system_key(Key_R) && slot>=0 && slot <10 &&  received_ready_once){
			buffer[0] = (u8)Client_Message::READY;
			int bytes_written = 1;
			memcpy(&buffer[1],&slot, 2);
			bytes_written += sizeof(slot);
			if (system_net_send_to(socket, buffer,bytes_written, to) < 0)
			{
				system_trace("READY message failed to send\n");
				return 0;
			}
			else {
				system_trace("READY message sent\n");
				received_ready_once = false;
			}
		}


		while (buffer[0] == (u8)Server_Message::START) {
			system_trace("READY message received\n");

			LARGE_INTEGER tick_start_time;
			QueryPerformanceCounter(&tick_start_time);

			int bytes_received = 1;

			while (bytes_received) {
				bytes_received = system_net_receive_from(socket, buffer, SOCKET_BUFFER_SIZE, &from);


				if (bytes_received < 0) {
					break;
				}
				else
				{
					system_trace("%s", buffer);
				}
				system_trace("loop\n");

				switch (buffer[0])
				{
				//case Server_Message::JOIN_RESULT:
				//{
				//	if (buffer[1])
				//	{
				//		memcpy(&slot, &buffer[2], sizeof(slot));
				//	}
				//	else
				//	{
				//		system_trace("server didn't let us in\n");
				//	}
				//}
				//break;

				case Server_Message::STATE:
				{
					num_objects = 0;
					int  bytes_read = 1;
					while (bytes_read < bytes_received)
					{
						u16 id; // unused
						memcpy(&id, &buffer[bytes_read], sizeof(id));
						bytes_read += sizeof(id);

						memcpy(&objects.x, &buffer[bytes_read], sizeof(objects.x));
						bytes_read += sizeof(objects.x);

						memcpy(&objects.y, &buffer[bytes_read], sizeof(objects.y));
						bytes_read += sizeof(objects.y);

						memcpy(&objects.facing, &buffer[bytes_read], sizeof(objects.facing));
						bytes_read += sizeof(objects.facing);

						//	++num_objects;
					}
				}
				break;
				}
			}


			// Send input
			if (slot != 0xFFFF)
			{
				buffer[0] = (u8)Client_Message::INPUTS;
				int bytes_written = 1;

				memcpy(&buffer[bytes_written], &send_packet_id, sizeof(send_packet_id));
				bytes_written += sizeof(send_packet_id);

				memcpy(&buffer[bytes_written], &slot, sizeof(slot));
				bytes_written += sizeof(slot);

				u8 input = 0;
				bool key_pressed = false;

				if (system_key(Key_W) || system_key(Key_UP)) {
					input |= 0x1;
					key_pressed = true;
					system_trace("key :w\n");

				}

				if (system_key(Key_S) || system_key(Key_DOWN)) {
					input |= 0x2;
					key_pressed = true;
					system_trace("key :s\n");
				}

				if (system_key(Key_A) || system_key(Key_LEFT)) {
					input |= 0x4;
					key_pressed = true;
					system_trace("key :a\n");
				}

				if (system_key(Key_D) || system_key(Key_RIGHT)) {
					input |= 0x8;
					key_pressed = true;
					system_trace("key :d\n");
				}

				if (system_key(Key_Q)) {
					running = false;
					break;
				}

				if (system_key(Key_ESCAPE)) {
					running = false;
					break;
				}


				memcpy(&buffer[bytes_written], &input, sizeof(input));
				bytes_written += sizeof(input);

				u32 crc = murmur3_32(buffer, bytes_written, 0);

				memcpy(&buffer[bytes_written], &crc, sizeof(crc));
				bytes_written += sizeof(crc);


				if (key_pressed) {

					send_packet_id = (send_packet_id + 1) % 250;
					if (system_net_send_to(socket, buffer, bytes_written, to) < 0) {
						system_trace("Falied to send!");
						break;
					}
				}

			}


			s32 time_taken_s = time_since(tick_start_time, clock_frequency);

			while (time_taken_s < SECONDS_PER_TICK)
			{
				if (sleep_granularity_was_set)
				{
					DWORD time_to_wait_ms = DWORD((SECONDS_PER_TICK - time_taken_s) * 1000);
					if (time_to_wait_ms > 0)
					{
						Sleep(time_to_wait_ms);
					}
				}

				time_taken_s = time_since(tick_start_time, clock_frequency);
			}


		}
	}

	buffer[0] = (u8)Client_Message::LEAVE;
	int bytes_written = 1;
	memcpy(&buffer[bytes_written], &slot, sizeof(slot));
	if (system_net_send_to(socket, buffer, bytes_written, to) < 0) {
		system_trace("falied to send!");
	}

	//u32 crc = murmur3_32(buffer, bytes_written, 0);

	return 0;
}
