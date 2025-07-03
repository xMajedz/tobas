#include "netcode_server.h"
#define ENET_IMPLEMENTATION
#include "netcode_common.h"
#include <cstdio>
#include <thread>
#define MAX_CLIENTS 20

static ENetAddress address = { 0 };

static ENetHost* host = { 0 };

static bool host_active = false;

static std::thread game_thread;

int host_game()
{
	if (enet_initialize() != 0) return 1;

	address = (ENetAddress){ .host = ENET_HOST_ANY, .port = 4545 };

	host = enet_host_create(&address, MAX_CLIENTS, 1, 0, 0);

	if (host == NULL) return 1;

	ENetEvent event;

	host_active = true;

	while (enet_host_service(host, &event, 1000)) {
		switch (event.type) {
		case ENET_EVENT_TYPE_CONNECT: {
		std::printf("A new client connected from %x:%u.\n",
			event.peer->address.host,
			event.peer->address.port
		);
		s_data data = { .content = "Client information" };
		event.peer->data = &data;
		} break;
		case ENET_EVENT_TYPE_RECEIVE: {
		s_data* data = (s_data*)event.peer->data;
		std::printf("A packet of length %lu containing %s was received from %s on channel %u.\n",
                       	event.packet->dataLength,
                       	event.packet->data,
                        data->content,
                        event.channelID
		);
                enet_packet_destroy(event.packet);
		} break;
		case ENET_EVENT_TYPE_DISCONNECT: {
	        event.peer->data = NULL;
		} break;
		case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT: {
	        event.peer->data = NULL;
		} break;
		case ENET_EVENT_TYPE_NONE: {
		} break;
		}

		if (!host_active) break;
	}

	enet_host_destroy(host);
	enet_deinitialize();
	return 0;
}

int host_game_thread()
{
	game_thread = std::thread(host_game);
	return 0;
}

int host_close()
{
	host_active = false;
	return 0;
}

int host_close_thread()
{
	game_thread.join();
	host_close();
	return 0;
}

bool host_status()
{
	return host_active;
}

