#include "netcode_client.h"
#include "netcode_common.h"
#include <cstdio>

static ENetAddress address = { 0 };

static ENetPeer* host = { 0 };

static ENetHost* client = { 0 };
	
static bool disconnected = true;

static bool connected = false;

enum Status {
	DISCONNECTED = 0,
	CONNECTED,
	CONNECTING,
};

static Status status = DISCONNECTED;

int client_connect()
{
	std::puts("Client: Connecting ...");	

	status = CONNECTING;

	if (enet_initialize() != 0) {
		std::fprintf(stderr, "Client: An error occurred while initializing ENet.\n");
    		return 1;
  	}

	client = enet_host_create(NULL, 1, 1, 0, 0);

	if (client == NULL) {
		std::fprintf(stderr, "Client: An error occurred while trying to create an ENet client host.\n");
		return 1;
	}

	//enet_address_set_host(&address, "::1");
	enet_address_set_host(&address, "127.0.0.1");
	address.port = 4545;

	host = enet_host_connect(client, &address, 1, 0);

	if (host == NULL) {
    		std::fprintf(stderr, "Client: No available peers for initiating an ENet connection.\n");
		return 1;
	}

	ENetEvent event;

	if (enet_host_service(client, &event, 1000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
		std::puts("Client: Connection to localhost:4545 succeeded.");	

		size_t buf_size = 1;
		uint8_t buf[buf_size] = { 2 };
		ENetPacket* p = enet_packet_create(buf, buf_size, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(host, 0, p);
		connected = true;
		status = CONNECTED;
	} else {
		std::puts("Client: Connection to localhost:4545 failed.");
		enet_peer_reset(host);
		return 1;
	}

	return 0;
}

int client_update()
{
	ENetEvent event;

  	while (enet_host_service(client, &event, 0) > 0) {
      		switch (event.type) {
		case ENET_EVENT_TYPE_RECEIVE: {
			std::printf(
				"Client: A packet of length %lu "
				"containing %lu was received on channel %lu .\n",
				event.packet->dataLength,
				*((uint8_t*)event.packet->data),
				event.channelID
			);
        		enet_packet_destroy(event.packet);
		} break;
		case ENET_EVENT_TYPE_DISCONNECT: {
			std::puts("Client: Disconnection succeeded.\n");
        		disconnected = true;
		} break;
		}
	}

	if (!disconnected) {
		enet_peer_reset(host);
		client_close();
	}

	return 0;
}

int client_disconnect()
{
	disconnected = true;
	return 0;
}

int client_close()
{
	enet_host_destroy(client);
	enet_deinitialize();
	return 0;
}
