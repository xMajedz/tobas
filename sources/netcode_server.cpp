#include "netcode_server.h"
#define ENET_IMPLEMENTATION
#include "netcode_common.h"

#include <thread>

#define MAX_CLIENTS 20

static ENetAddress address = { 0 };

static ENetHost* host = { 0 };

static std::thread game_thread;

enum Status { INACTIVE = 0, ACTIVE, STARTING };

static Status status = INACTIVE;

static bool host_active = false;

int host_init()
{
	status = STARTING;

	std::cout << "Server: Starting ..." << std::endl;

	if (enet_initialize() != 0) {
		std::cout <<
			"Server: An error occurred while initializing ENet." <<
		std::endl;
		return 1;
	}

	Config cfg = parse_cfg();

	enet_address_set_host(&address, cfg.host.c_str());
	address.port = cfg.port;

	host = enet_host_create(&address, cfg.max_clients, 1, 0, 0);

	if (host == NULL) {
		std::cout <<
			"Server: An error occurred while trying to create an ENet server host." <<
		std::endl;
		return 1;
	}
	
	status = ACTIVE;

	host_active = true;

	std::cout << "Server: Started a server at " << cfg.host << ":" << cfg.port << std::endl;

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
				std::cout <<
					"Server: A new client connected." <<
				std::endl;
	
				size_t buf_size = 1;
				uint8_t buf[buf_size] = { 5 };
				ENetPacket* p = enet_packet_create(buf, buf_size, ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(event.peer, 0, p);
			} break;
			case ENET_EVENT_TYPE_RECEIVE: {
				std::cout <<
					"Server: A packet of length " << event.packet->dataLength << " " <<
					"containing " << (int)*((uint8_t*)event.packet->data) << " " <<
					"was received on channel " << (int)event.channelID <<
				std::endl;

	                	enet_packet_destroy(event.packet);
			} break;
			case ENET_EVENT_TYPE_DISCONNECT: {
				std::cout <<
					"Server: A client disconnected." <<
				std::endl;

		        	event.peer->data = NULL;
			} break;
			case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT: {
				std::cout <<
					"Server: A client disconnected. (timeout)" <<
				std::endl;

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
