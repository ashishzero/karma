#include "modules/core/systems.h"
#include "modules/core/utility.h"
#include <winsock2.h>
#pragma comment(lib, "Winmm.lib")

#define SOCKET_BUFFER_SIZE kilo_bytes(1)


const r32 TURN_SPEED = 0.01f;	// how fast player turns
const r32 ACCELERATION = 0.01f;
const r32 MAX_SPEED = 1.0f;

const u32	TICKS_PER_SECOND = 60;
const r32	SECONDS_PER_TICK = 1.0f / s32(TICKS_PER_SECOND);

const u16	MAX_CLIENTS = 2;
const r32	CLIENT_TIMEOUT = 5.0f;



enum  Client_Message : u8
{
	JOIN,		// tell server we're new here
	LEAVE,		// tell server we're leaving
	INPUTS 		// tell server our user input
};

enum  Server_Message : u8
{
	JOIN_RESULT,// tell client they're accepted/rejected
	STATE 		// tell client game state
};

bool operator==(const Ip_Endpoint& a, const Ip_Endpoint& b) { return a.address == b.address && a.port == b.port; }

struct Player_State
{
	r32 x, y, facing, speed;
};

struct Player_Input
{
	booli up, down, left, right;
};

static s32 time_since(LARGE_INTEGER t, LARGE_INTEGER frequency)
{
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);

	return s32(now.QuadPart - t.QuadPart) / s32(frequency.QuadPart);
}


int system_main() {	
	Ip_Endpoint address = ip_endpoint_local(9999);
	Socket socket = system_net_open_udp_server(address);


	if (socket != SOCKET_INVALID) {
		int d = (address.address & 0x000000ff) >> 0;
		int c = (address.address & 0x0000ff00) >> 8;
		int b = (address.address & 0x00ff0000) >> 16;
		int a = (address.address & 0xff000000) >> 24;
		printf("Server Launched: %d.%d.%d.%d:%d\n", a, b, c, d, address.port);
	}

	system_net_set_socket_nonblocking(socket);


	UINT sleep_granularity_ms = 1;
	bool sleep_granularity_was_set = timeBeginPeriod(sleep_granularity_ms) == TIMERR_NOERROR;  //bool 32
	LARGE_INTEGER clock_frequency;
	QueryPerformanceFrequency(&clock_frequency);


	u8 buffer[SOCKET_BUFFER_SIZE];
	Ip_Endpoint client_endpoints[MAX_CLIENTS];
	r32 time_since_heard_from_clients[MAX_CLIENTS];
	Player_State client_objects[MAX_CLIENTS];
	Player_Input client_inputs[MAX_CLIENTS];


	for (u16 i = 0; i < MAX_CLIENTS; ++i)
	{
		client_endpoints[i] = {};
	}


	//char buffer[SOCKET_BUFFER_SIZE];
	bool running = true;
	int flags;

	while (running) {


		LARGE_INTEGER tick_start_time;
		QueryPerformanceCounter(&tick_start_time);

		Ip_Endpoint from;
		int bytes_received = system_net_receive_from(socket, buffer, SOCKET_BUFFER_SIZE, &from);


		if(bytes_received < 0) {
			continue;
		}
		else
		{
			printf("sth received\n");
		}

		Ip_Endpoint from_endpoint;
		from_endpoint.address = from.address;
		from_endpoint.port = (from.address & 0xff000000) >> 24;

		int d = (from.address & 0x000000ff) >> 0;
		int c = (from.address & 0x0000ff00) >> 8;
		int b = (from.address & 0x00ff0000) >> 16;
		int a = (from.address & 0xff000000) >> 24;

		char client_input = buffer[0];
		//printf("%d.%d.%d.%d:%d - %c\n", a, b, c, d, from.port, client_input);
		printf("client %d.%d.%d.%d:%d \n", a, b, c, d, from.port);


		switch (buffer[0])
		{
		case Client_Message::JOIN:
		{
			system_trace("Client_Message::Join from %u:%hu\n", from_endpoint.address, from_endpoint.port);

			u16 slot = u16(-1);
			for (u16 i = 0; i < MAX_CLIENTS; ++i)
			{
				if (client_endpoints[i].address == 0)
				{
					slot = i;
					break;
				}
			}

			buffer[0] = (s8)Server_Message::JOIN_RESULT;
			if (slot != u16(-1))
			{
				printf("client will be assigned to slot %hu\n", slot);
				buffer[1] = 1;
				memcpy(&buffer[2], &slot, 2);

				 flags = 0;
				if (system_net_send_to(socket, buffer, sizeof(buffer), from) != SOCKET_ERROR)
				{
					client_endpoints[slot] = from_endpoint;
					time_since_heard_from_clients[slot] = 0.0f;
					client_objects[slot] = {};
					client_inputs[slot] = {};
				}
				else
				{
					printf("sendto failed: %d\n", WSAGetLastError());
				}
			}
			else
			{
				printf("could not find a slot for player\n");
				buffer[1] = 0;

				flags = 0;
				if (system_net_send_to(socket, buffer, sizeof(buffer), from) != SOCKET_ERROR)
				{
					printf("sendto failed: %d\n", WSAGetLastError());
				}
			}
		}
		break;

		case Client_Message::LEAVE:
		{
			u16 slot;
			memcpy(&slot, &buffer[1], 2);

			if (client_endpoints[slot] == from_endpoint)
			{
				client_endpoints[slot] = {};
				printf("Client_Message::Leave from %hu(%u:%hu)\n", slot, from_endpoint.address, from_endpoint.port);
			}
		}
		break;

		case Client_Message::INPUTS:
		{
			u32 received_crc, generated_crc;
			char compare[SOCKET_BUFFER_SIZE];

			memcpy(&compare, &buffer[0], 5);
			memcpy(&received_crc, &buffer[5], 4);
			generated_crc = murmur3_32(compare, 5, 0);

			if (generated_crc != received_crc)
			{
				system_trace("hash discarded\n");
				continue;
			}
			else
			{
				system_trace("hash matched\n");

			}
			u16 slot;
			memcpy(&slot, &buffer[2], 2);

			printf("%d %hu\n", bytes_received, slot);

			if (client_endpoints[slot] == from_endpoint)
			{
				u8 input = buffer[4];

				client_inputs[slot].up = input & 0x1;
				client_inputs[slot].down = input & 0x2;
				client_inputs[slot].left = input & 0x4;
				client_inputs[slot].right = input & 0x8;

				time_since_heard_from_clients[slot] = 0.0f;

				printf("Client_Message::Input from %hu:%d\n", slot, (input));
			}
			else
			{
				printf("Client_Message::Input discarded, was from %u:%hu but expected %u:%hu\n", from_endpoint.address, from_endpoint.port, client_endpoints[slot].address, client_endpoints[slot].port);
			}
		}
		break;
		}



		// process input and update state
		for (u16 i = 0; i < MAX_CLIENTS; ++i)
		{
			if (client_endpoints[i].address)
			{
				if (client_inputs[i].up)
				{
					printf("key :w\n");
					client_objects[i].speed += ACCELERATION * SECONDS_PER_TICK;
					if (client_objects[i].speed > MAX_SPEED)
					{
						client_objects[i].speed = MAX_SPEED;
					}
				}
				if (client_inputs[i].down)
				{
					printf("key :s\n");
					client_objects[i].speed -= ACCELERATION * SECONDS_PER_TICK;
					if (client_objects[i].speed < 0.0f)
					{
						client_objects[i].speed = 0.0f;
					}
				}
				if (client_inputs[i].left)
				{
					printf("key :a\n");
					client_objects[i].facing -= TURN_SPEED * SECONDS_PER_TICK;
				}
				if (client_inputs[i].right)
				{
					printf("key :d\n");
					client_objects[i].facing += TURN_SPEED * SECONDS_PER_TICK;
				}

				client_objects[i].x += client_objects[i].speed * SECONDS_PER_TICK * sinf(client_objects[i].facing);
				client_objects[i].y += client_objects[i].speed * SECONDS_PER_TICK * cosf(client_objects[i].facing);

				time_since_heard_from_clients[i] += SECONDS_PER_TICK;
				if (time_since_heard_from_clients[i] > CLIENT_TIMEOUT)
				{
					printf("client %hu timed out\n", i);
					client_endpoints[i] = {};
				}
			}
		}

				// create state packet
		buffer[0] = (s8)Server_Message::STATE;
		s32 bytes_written = 1;
		for (u16 i = 0; i < MAX_CLIENTS; ++i)
		{
			if (client_endpoints[i].address)
			{
				memcpy(&buffer[bytes_written], &i, sizeof(i));
				bytes_written += sizeof(i);

				memcpy(&buffer[bytes_written], &client_objects[i].x, sizeof(client_objects[i].x));
				bytes_written += sizeof(client_objects[i].x);

				memcpy(&buffer[bytes_written], &client_objects[i].y, sizeof(client_objects[i].y));
				bytes_written += sizeof(client_objects[i].y);

				memcpy(&buffer[bytes_written], &client_objects[i].facing, sizeof(client_objects[i].facing));
				bytes_written += sizeof(client_objects[i].facing);
			}
		}


		// send back to clients
		int flags = 0;
		Ip_Endpoint to;
		//to.sin_family = AF_INET;
		//to.sin_port = htons(9999);
		//int to_length = sizeof(to);

		for (u16 i = 0; i < MAX_CLIENTS; ++i)
		{
			if (client_endpoints[i].address)
			{
				to.address = client_endpoints[i].address;
				to.port = client_endpoints[i].port;

				if (system_net_send_to(socket, buffer, bytes_written, to) == SOCKET_ERROR)
				{
					printf("sendto failed: %d\n", WSAGetLastError());
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

	return 0;
}

Builder system_builder() {
	Builder builder;
	builder.allocator = system_create_heap_allocator();
	builder.entry = system_main;
	builder.temporary_buffer_size = mega_bytes(128);
	builder.flags = Builder_NETWORK;
	return builder;
}
