#include "netcode_client.h"
#include "netcode_common.h"
#define LOCALHOST "127.0.0.1"

static ENetAddress address = { 0 };

static ENetPeer* host = { 0 };

static ENetHost* client = { 0 };

void client_connect() {
	enet_initialize();

	client = enet_host_create(NULL, 1, 1, 0, 0);

	enet_address_set_host(&address, LOCALHOST);
	address.port = 4545;

	host = enet_host_connect(client, &address, 1, 0);
}
