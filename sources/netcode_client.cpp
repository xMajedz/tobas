#include "netcode_client.h"
#include "netcode_common.h"
#include "game.h"

static ENetAddress address = { 0 };
static ENetHost* client = { 0 };
static ENetPeer* host = { 0 };
	
static bool connected = false;
static bool disconnected = true;
static bool disconnecting = false;

static bool skip_local_sim = false;
static bool ready = false;

static int p_id = -1;

enum Status { DISCONNECTED = 0, CONNECTED, CONNECTING };

static Status status = DISCONNECTED;

static f64_t update_interval = 1.00 / 20.00;
static f64_t last_update_t = 0;
static f64_t last_t = 0;

static Config cfg = NetCommon::LoadConfig();

static void Send(NetCommon::Client::Command::Type CMD)
{
	using namespace NetCommon::Client::Command;
	switch (CMD) {
		case Type::Join: {
			size_t b_size = 2;
			uint8_t b[b_size];
			b[0] = (uint8_t)Type::Join;
			b[1] = (uint8_t)0;
			ENetPacket* p = enet_packet_create(b, b_size, ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(host, 0, p);
		} break;
		case Type::Ready: {
			auto game_player = Game::GetPlayer((int)p_id);
			size_t j_count = game_player.joint.size();
			size_t b_size = 2 + j_count;
			uint8_t b[b_size];
			b[0] = (uint8_t)Type::Ready;
			b[1] = (uint8_t)p_id;
			for (int i = 0; i < j_count; i += 1) {
				uint8_t j_state = (uint8_t)game_player.GetJointState(i);
				uint8_t j_state_alt = (uint8_t)game_player.GetJointStateAlt(i);
				b[2 + i] = j_state + j_state_alt << 2; 
			}
			ENetPacket* p = enet_packet_create(b, b_size, ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(host, 0, p);
		} break;
	}
};

static void Receive(NetCommon::Server::Command::Type CMD, uint8_t* data)
{
	auto rules = Game::GetGamerules();
	using namespace NetCommon::Server::Command;
	switch(CMD) {
		case Type::Accept: {
			p_id = (int)data[1];
			std::cout <<
				"Client: received player id from server. ID: " << p_id <<
			std::endl;
			Game::SetSelectedPlayer(p_id);
		} break;
		case Type::Step: {
			std::cout <<
				"Client: received step command from server." <<
			std::endl;
			if (!skip_local_sim) {
				Game::Step(rules.turnframes);
			}
		} break;
	}

};

int Client::Connect()
{
	using namespace NetCommon::Client;
	NetCommon::log(Message::Get(Message::Type::Connecting));

	status = CONNECTING;

	if (enet_initialize() != 0) {
		NetCommon::log(Message::Get(Message::Type::InitError));
    		return 1;
  	}

	client = enet_host_create(nullptr, 1, 1, 0, 0);

	if (client == nullptr) {
		NetCommon::log(Message::Get(Message::Type::ClientError));
		return 1;
	}

	enet_address_set_host(&address, cfg.host.c_str());
	address.port = cfg.port;

	host = enet_host_connect(client, &address, 1, 0);

	if (host == nullptr) {
		NetCommon::log(Message::Get(Message::Type::HostError));
		return 1;
	}

	ENetEvent event;

	if (enet_host_service(client, &event, 2000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
		std::cout <<
			"Client: Connection to " << cfg.host << ":" << cfg.port << " succeeded." <<
		std::endl;	

		using namespace NetCommon::Client::Command;
		Send(Type::Join);

		connected = true;
		status = CONNECTED;
	} else {
		std::cout <<
			"Client: Connection to " << cfg.host << ":" << cfg.port << " failed." <<
		std::endl;	
		enet_peer_reset(host);
		return 1;
	}

	return 0;
}

void Client::SkipLocalSim()
{
	skip_local_sim = true;
}

void Client::Ready()
{
	ready = true;
}

void Client::Update(f64_t t, f32_t dt)
{
	last_t = t;

	if (host == nullptr) {
		return;
	}

	if (t - last_update_t > update_interval) {

		last_update_t = t;
	}

	if (ready) {
		using namespace NetCommon::Client::Command;
		Send(Type::Ready);
		ready = false;
	}

	ENetEvent event;

  	while (enet_host_service(client, &event, 0) > 0) {
      		switch (event.type) {
		case ENET_EVENT_TYPE_RECEIVE: {
			uint8_t* data = (uint8_t*)event.packet->data;
			Receive((NetCommon::Server::Command::Type)data[0], data);
        		enet_packet_destroy(event.packet);
		} break;
		case ENET_EVENT_TYPE_DISCONNECT: {
			using namespace NetCommon::Client;
			NetCommon::log(Message::Get(Message::Type::DisconnectionPass));
			Close();

		} break;
		}
	}
}

void Client::Disconnect()
{
	if (host != nullptr) {
		enet_peer_disconnect(host, 0);
		disconnecting = true;
	}
}

void Client::Close()
{
	if (client != nullptr) {
		enet_host_destroy(client);
        	disconnected = true;
		disconnecting = false;
	}

	client = nullptr;
	host = nullptr;

	enet_deinitialize();
}
