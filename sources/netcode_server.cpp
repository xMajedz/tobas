#include "netcode_server.h"
#include "netcode_common.h"
#include "game.h"

#include <thread>

static ENetAddress address = { 0 };
static ENetHost* host = { 0 };

static std::thread game_thread;

enum Status { INACTIVE = 0, ACTIVE, STARTING };

static Status status = INACTIVE;

static bool host_active = false;

static Config cfg = NetCommon::LoadConfig();

struct NetPlayer {
	uint8_t id;
	uint8_t j_state;
	uint8_t j_state_alt;
	bool active = false;
	bool ready = false;
	ENetPeer* peer;
};

NetPlayer* players;

int p_count = 0;

int Init()
{
	status = STARTING;
	using namespace NetCommon::Server;
	NetCommon::log(Message::Get(Message::Type::Starting));

	if (enet_initialize() != 0) {
		NetCommon::log(Message::Get(Message::Type::InitError));
		return 1;
	}

	enet_address_set_host(&address, cfg.host.c_str());
	address.port = cfg.port;

	host = enet_host_create(&address, cfg.max_clients, 1, 0, 0);

	if (host == nullptr) {
		NetCommon::log(Message::Get(Message::Type::ServerError));
		return 1;
	}
	
	status = ACTIVE;

	host_active = true;

	std::cout << "Server: Started a server at " << cfg.host << ":" << cfg.port << std::endl;

	return 0;
}

static void Send(NetCommon::Server::Command::Type CMD, NetPlayer player)
{
	using namespace NetCommon::Server::Command;
	switch(CMD) {
		case Type::Accept: {
			size_t b_size = 2;
			uint8_t b[b_size];
			b[0] = (uint8_t)Type::Accept;
			b[1] = (uint8_t)player.id;
			ENetPacket* p = enet_packet_create(b, b_size, ENET_PACKET_FLAG_RELIABLE);

			enet_peer_send(player.peer, 0, p);
		} break;
		case Type::Step: {
			size_t b_size = 2;
			uint8_t b[b_size];
			b[0] = (uint8_t)Type::Step;
			b[1] = (uint8_t)1;
			ENetPacket* p = enet_packet_create(b, b_size, ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(player.peer, 0, p);
		} break;
	}
};

static void SendToAllExcept(NetCommon::Server::Command::Type CMD, NetPlayer player)
{
	for (int i = 0; i <= p_count; i += 1) {
		if (player.id != players[i].id) {
			Send(CMD, players[i]);
		}

		if (CMD == NetCommon::Server::Command::Type::Step) {
			players[i].ready = false;
		}
	}
};

static void SendToAll(NetCommon::Server::Command::Type CMD)
{
	for (int i = 0; i <= p_count; i += 1) {
		Send(CMD, players[i]);
		if (CMD == NetCommon::Server::Command::Type::Step) {
			players[i].ready = false;
		}
	}
};

static void Receive(NetCommon::Client::Command::Type CMD, ENetPacket* packet)
{
	auto rules = Game::GetGamerules();
	uint8_t* data = (uint8_t*)packet->data;
	using namespace NetCommon::Client::Command;
	switch(CMD) {
		case Type::Join: {
			std::cout <<
				"Server: received join command from a client." <<
			std::endl;
		} break;
		case Type::Ready: {
			int p_id = (int)data[1];
	
			std::cout <<
				"Server: player with id " << p_id << " is ready." <<
			std::endl;

			players[p_id].ready = true;
	
			bool step = true;
	
			int i = 0;
	
			while (step && i < p_count) {
				step = step && players[i].ready;
				i += 1;
			}
	
			if (p_count < rules.numplayers) {
				std::cout <<
					"Server: not enough players." <<
				std::endl;
			} else if (step) {
				std::cout <<
					"Server: players are ready, taking a step ..." <<
				std::endl;

				SendToAll(NetCommon::Server::Command::Type::Step);
				Game::Step(rules.turnframes);
			}
		} break;
	}
};

void Server::HostGame()
{
	if (Init() > 0) {
		return;
	}

	Game::Init();
	Game::NewGame();

	ENetEvent event;
	NetPlayer _players[cfg.max_clients];
	players = _players;

	while (host_active) {
		while (enet_host_service(host, &event, 1000) > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT: {
				uint8_t p_id = p_count;

				std::cout <<
					"Server: A new client connected. ID: " << (int)p_id <<
				std::endl;

				p_count += 1;

				players[p_id].id = p_id;
				players[p_id].peer = event.peer;
				players[p_id].ready = false;
				players[p_id].j_state = 0;
				players[p_id].j_state_alt = 0;

				Send(NetCommon::Server::Command::Type::Accept, players[p_id]);
			} break;
			case ENET_EVENT_TYPE_RECEIVE: {
				uint8_t* data = (uint8_t*)event.packet->data;
				
				using namespace NetCommon::Client;
				Receive((Command::Type)data[0], event.packet);

	                	enet_packet_destroy(event.packet);
			} break;
			case ENET_EVENT_TYPE_DISCONNECT: {
				std::cout <<
					"Server: A client disconnected." <<
				std::endl;

		        	event.peer->data = nullptr;
				p_count -= 1;
			} break;
			case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT: {
				std::cout <<
					"Server: A client disconnected. (timeout)" <<
				std::endl;

		        	event.peer->data = nullptr;
				p_count -= 1;
			} break;
			case ENET_EVENT_TYPE_NONE: {}
			}
	
			if (status == INACTIVE) break;
		}
	}
}

void Server::Close()
{
	status = INACTIVE;
	enet_host_destroy(host);
	enet_deinitialize();
	Game::Quit();
}

void Server::HostGameThread()
{
	game_thread = std::thread(HostGame);
}

void Server::WaitGameThread()
{
	while(status == STARTING || status == INACTIVE) {}
}

void Server::CloseThread()
{
	game_thread.join();
	Close();
}
