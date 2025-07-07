#include "netcode_server.h"
#define ENET_IMPLEMENTATION
#include "netcode_common.h"

#include <cstdio>
#include <thread>
#include <iostream>

#define MAX_CLIENTS 20

static ENetAddress address = { 0 };

static ENetHost* host = { 0 };

static std::thread game_thread;

enum Status {
	INACTIVE = 0,
	ACTIVE,
	STARTING,
};

static Status status = INACTIVE;

static bool host_active = false;

int host_init()
{
	std::puts("Server: Starting ...");

	status = STARTING;

	if (enet_initialize() != 0) {
		std::fprintf(stderr, "Server: An error occurred while initializing ENet.\n");
		return 1;
	}

	address.host = ENET_HOST_ANY;
	address.port = 4545;

	host = enet_host_create(&address, MAX_CLIENTS, 1, 0, 0);

	if (host == NULL) {
		std::printf("Server: An error occurred while trying to create an ENet server host.\n");
		return 1;
	}
	
	std::puts("Server: Started a server at localhost:4545");

	status = ACTIVE;

	host_active = true;

	return 0;
}

int host_game()
{
	host_init();

	ENetEvent event;

	while (host_active) {
		while (enet_host_service(host, &event, 1000) > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT: {
				std::printf("Server: A new client connected.\n");
	
				size_t buf_size = 1;
				uint8_t buf[buf_size] = { 5 };
				ENetPacket* p = enet_packet_create(buf, buf_size, ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(event.peer, 0, p);
			} break;
			case ENET_EVENT_TYPE_RECEIVE: {
			std::printf("Server: A packet of length %lu containing %lu was received on channel %lu.\n",
	                       	event.packet->dataLength,
				*((uint8_t*)event.packet->data),
	                        event.channelID
				);
	                	enet_packet_destroy(event.packet);
			} break;
			case ENET_EVENT_TYPE_DISCONNECT: {
				std::printf("Server: A client disconnected.\n");
		        	event.peer->data = NULL;
			} break;
			case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT: {
		        	event.peer->data = NULL;
			} break;
			case ENET_EVENT_TYPE_NONE: {}
			}
	
			if (status == INACTIVE) break;
		}
	}
	return 0;
}

int host_game_thread()
{
	game_thread = std::thread(host_game);
	return 0;
}

int host_wait_thread()
{
	while(status == STARTING || status == INACTIVE) {}
	return 0;
}

int host_close()
{
	status = INACTIVE;
	enet_host_destroy(host);
	enet_deinitialize();
	return 0;
}

int host_close_thread()
{
	game_thread.join();
	host_close();
	return 0;
}
