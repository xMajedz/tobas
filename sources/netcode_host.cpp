#include "netcode_host.h"
#define ENET_IMPLEMENTATION
#include "netcode_common.h"
#define MAX_CLIENTS 20

ENetAddress address = { 0 };

ENetHost* host = { 0 }:

bool active = false;

void client_connected ()
{
}

void client_receive ()
{
}

void client_dsconnected ()
{
}

void client_timeout ()
{
}

int host_game()
{
	if (enet_initialize() != 0) return 1;

	address = (ENetAddress){ .host = ENET_HOST_ANY, .port = 4545 };

	host = enet_host_create(&address, MAX_CLIENTS, 1, 0, 0);

	active = true;

	while (active) {
		ENetEvent event = { 0 };
		if (enet_host_service(host, &event, 1000) > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT: {
				client_connected();
			} break;
			case ENET_EVENT_TYPE_RECEIVE: {
				client_receive();
			} break;
			case ENET_EVENT_TYPE_DISCONNECT: {
				client_disconnected();
			} break;
			case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT: {
				client_timeout();
			} break;
			case ENET_EVENT_TYPE_NONE: {
			} break;
		}
	}

	enet_host_destroy(host);
	enet_deinitialize();
	return 0;
}


