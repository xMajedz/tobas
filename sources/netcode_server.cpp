#include "netcode_server.h"
#include "netcode_common.h"
#include "game.h"

#include <thread>

static ENetAddress address = { 0 };
static ENetHost* host = { 0 };

static std::thread game_thread;

enum Status { INACTIVE = 0, ACTIVE, STARTING };

static Status status = INACTIVE;

static bool running = false;

static Config cfg = NetCommon::LoadConfig();

struct NetPlayer {
	uint8_t id = -1;
	uint8_t j_state = 0;
	uint8_t j_state_alt = 0;
	bool occupied = false;
	bool ready = false;
	ENetPeer* peer = nullptr;
};

NetPlayer* players;

int p_count = 0;

double last_t = 0;

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

	running = true;

	std::cout << "Server: Started a server at " << cfg.host << ":" << cfg.port << std::endl;

	return 0;
}

static void SendTo(NetPlayer player, ENetPacket* p)
{
	if (player.occupied) {
		enet_peer_send(player.peer, 0, p);
	}
};

static void SendToAllExcept(NetPlayer player, ENetPacket* p)
{
	for (int i = 0; i < cfg.max_clients; i += 1) {
		if (players[i].id != player.id) {
			SendTo(players[i], p);
		}
	}
};

static void SendToAll(ENetPacket* p)
{
	for (int i = 0; i < cfg.max_clients; i += 1) {
		SendTo(players[i], p);
	}
};

static void Receive(ENetPacket* packet)
{
	auto rules = Game::GetGamerules();
	uint8_t* data = (uint8_t*)packet->data;
	using namespace NetCommon;
	switch((Client::Command::Type)data[0]) {
		case Client::Command::Type::Join: {
			std::cout <<
				"Server: received join command from a client." <<
			std::endl;
		} break;
		case Client::Command::Type::Ready: {
			uint8_t p_id = data[1];
	
			std::cout <<
				"Server: player with ID: " << (int)p_id << " is ready." <<
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

				using namespace NetCommon::Server::Command;
				size_t b_size = 2;
				uint8_t b[b_size];
				b[0] = (uint8_t)Type::Step;
				b[1] = (uint8_t)rules.turnframes;
				ENetPacket* p = enet_packet_create(b, b_size, ENET_PACKET_FLAG_RELIABLE);

				SendToAll(p);

				for (int i = 0; i < cfg.max_clients; i += 1) {
					players[i].ready = false;
				}

				Game::Step(rules.turnframes);
			}
		} break;
	}
};

static uint8_t PeerWho(ENetPeer* peer)
{
	for (int i = 0; i < cfg.max_clients; i += 1) {
		if (players[i].peer == peer) {
			return players[i].id;
		}
	}
	return (uint8_t)-1;
};

void Server::HostGame()
{
	if (Init() > 0) {
		return;
	}

	Game::Init();
	Game::NewGame();

	NetPlayer _players[cfg.max_clients];
	players = _players;

	auto rules = Game::GetGamerules();

	ENetEvent event;
	while (running) {
		if (p_count >= rules.numplayers && 0 < rules.reaction_time) {
			double t = Game::GetTime();
			if (t - last_t >= rules.reaction_time) {
				std::cout <<
					"Server: time is up, taking a step ..." <<
				std::endl;

				using namespace NetCommon::Server::Command;
				size_t b_size = 2;
				uint8_t b[b_size];
				b[0] = (uint8_t)Type::Step;
				b[1] = (uint8_t)rules.turnframes;
				ENetPacket* p = enet_packet_create(b, b_size, ENET_PACKET_FLAG_RELIABLE);

				SendToAll(p);

				for (int i = 0; i < cfg.max_clients; i += 1) {
					players[i].ready = false;
				}

				Game::Step(rules.turnframes);

				last_t = t;
			}
		}

		while (enet_host_service(host, &event, 1000) > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT: {
				uint8_t p_id = (uint8_t)-1;
				for (int i = 0; i < cfg.max_clients; i += 1) {
					if (players[i].occupied == false) {
						p_id = (uint8_t)i;
						players[i].id = p_id;
						p_count += 1;
						break;
					}
				}

				if (p_id == (uint8_t)-1) {
					enet_peer_disconnect(event.peer, 0);
					break;
				}

				/*if (p_count >= rules.numplayers) {
					last_t = Game::GetTime();
				}*/

				std::cout <<
					"Server: A new client connected. ID: " << (int)p_id <<
				std::endl;

				players[p_id].id = p_id;
				players[p_id].peer = event.peer;
				players[p_id].occupied = true;
				players[p_id].ready = false;
				players[p_id].j_state = 0;
				players[p_id].j_state_alt = 0;

				using namespace NetCommon::Server::Command;
				size_t b_size = 2;
				uint8_t b[b_size];
				b[0] = (uint8_t)Type::Accept;
				b[1] = (uint8_t)p_id;
				ENetPacket* p = enet_packet_create(b, b_size, ENET_PACKET_FLAG_RELIABLE);
				SendTo(players[p_id], p);

				b_size = 2;
				b[b_size];
				b[0] = (uint8_t)Type::P_Connect;
				b[1] = (uint8_t)p_id;
				p = enet_packet_create(b, b_size, ENET_PACKET_FLAG_RELIABLE);
				SendToAllExcept(players[p_id], p);
			} break;
			case ENET_EVENT_TYPE_RECEIVE: {
				Receive(event.packet);
	                	enet_packet_destroy(event.packet);
			} break;
			case ENET_EVENT_TYPE_DISCONNECT: {
				uint8_t id = PeerWho(event.peer);
				if (id == (uint8_t)-1) {
					break;
				}
				NetPlayer& player = players[id];
				std::cout <<
					"Server: A client disconnected. ID:" << (int)player.id <<
				std::endl;

				using namespace NetCommon::Server::Command;
				size_t b_size = 2;
				uint8_t b[b_size];
				b[0] = (uint8_t)Type::P_Disconnect;
				b[1] = (uint8_t)player.id;
				ENetPacket* p = enet_packet_create(b, b_size, ENET_PACKET_FLAG_RELIABLE);

				player.id = -1;
				player.occupied = false;
				player.peer = nullptr;
		        	event.peer = nullptr;
				p_count -= 1;
				SendToAll(p);
			} break;
			case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT: {
				uint8_t id = PeerWho(event.peer);
				if (id == (uint8_t)-1) {
					break;
				}
				NetPlayer& player = players[id];
				std::cout <<
					"Server: A client disconnected. ID:" << (int)player.id << "(timeout)" <<
				std::endl;
				
				using namespace NetCommon::Server::Command;
				size_t b_size = 2;
				uint8_t b[b_size];
				b[0] = (uint8_t)Type::P_Timeout;
				b[1] = (uint8_t)player.id;
				ENetPacket* p = enet_packet_create(b, b_size, ENET_PACKET_FLAG_RELIABLE);

				player.id = -1;
				player.occupied = false;
				player.peer = nullptr;
		        	event.peer = nullptr;
				p_count -= 1;
				SendToAll(p);
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
	while(status != ACTIVE) {}
}

void Server::CloseThread()
{
	running = false;
	game_thread.join();
	Close();
}
