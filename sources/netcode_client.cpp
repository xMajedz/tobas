#include "api.h"
#include "game.h"

#include "netcode_common.h"
#include "netcode_client.h"

static ENetAddress address = { 0 };
static ENetHost* client = { 0 };
static ENetPeer* host = { 0 };
	
static bool connected = false;
static bool disconnected = true;
static bool disconnecting = false;

static bool skip_local_sim = false;
static bool ready = false;

static uint8_t p_id = (uint8_t)-1;

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
		} break;
		case Type::Ready: {
		} break;
		case Type::Echo: {
		} break;
	}
}

static void Receive(NetCommon::Server::Command::Type CMD, ENetPacket* p)
{
	uint8_t* data = p->data;
	auto rules = Game::GetGamerules();
	using namespace NetCommon::Server::Command;
	switch(CMD) {
		case Type::P_Connect: {
			std::cout <<
				"Client: a player connected ID: " << (int)data[1] <<
			std::endl;
		} break;
		case Type::P_Whisper: {
			char msg[p->dataLength + 1] = { 0 };
			for (int i = 0; i < p->dataLength - 3; i += 1) {
				msg[i] = (char)data[i + 3];
			}
			Console::log(msg);
		} break;
		case Type::P_Echo: {
			char msg[p->dataLength + 1] = { 0 };
			for (int i = 0; i < p->dataLength - 2; i += 1) {
				msg[i] = (char)data[i + 2];
			}
			Console::log(msg);
		} break;
		case Type::P_Disconnect: {
			std::cout <<
				"Client: a player disconnected ID: " << (int)data[1] <<
			std::endl;
		} break;
		case Type::P_Timeout: {
			std::cout <<
				"Client: a player disconnected. ID:" << (int)data[1] << " (timeout)." <<
			std::endl;
		} break;
		case Type::Accept: {
			p_id = data[1];
			std::cout <<
				"Client: received player id from server. ID: " << (int)p_id <<
			std::endl;
			Game::SetSelectedPlayer(p_id);
		} break;
		case Type::Step: {
			std::cout <<
				"Client: received step command from server." <<
			std::endl;
			if (!skip_local_sim) {
				Game::Step((int)data[1]);
			}
		} break;
	}

};

int Client::Connect(const char* host_address, int port)
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

	enet_address_set_host(&address, host_address);
	address.port = port;

	host = enet_host_connect(client, &address, 1, 0);

	if (host == nullptr) {
		NetCommon::log(Message::Get(Message::Type::HostError));
		return 1;
	}

	ENetEvent event;

	if (enet_host_service(client, &event, 2000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
		std::cout <<
			"Client: Connection to " << host_address << ":" << port << " succeeded." <<
		std::endl;	

		using namespace NetCommon::Client::Command;
		Send(Type::Join);

		connected = true;
		status = CONNECTED;
	} else {
		std::cout <<
			"Client: Connection to " << host_address << ":" << port << " failed." <<
		std::endl;	
		enet_peer_reset(host);
		return 1;
	}

	return 0;
}

int Client::Connect()
{
	return Connect(cfg.host.c_str(), cfg.port);
}

void Client::SkipLocalSim()
{
	skip_local_sim = true;
}

void Client::Join(const char* username)
{
	using namespace NetCommon::Client::Command;
	size_t b_size = 2;
	uint8_t b[b_size];
	b[0] = (uint8_t)Type::Join;
	b[1] = (uint8_t)0;
	ENetPacket* p = enet_packet_create(b, b_size, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(host, 0, p);
}

void Client::Whisper(int r_id, const char* msg)
{
	using namespace NetCommon::Client::Command;
	size_t b_size = 3 + strlen(msg);
	uint8_t b[b_size];
	b[0] = (uint8_t)Type::Whisper;
	b[1] = (uint8_t)p_id;
	b[2] = (uint8_t)r_id;
	for (int i = 0; i < 255 && i < b_size - 3; i += 1) {
		b[3 + i] = msg[i];
	}
	ENetPacket* p = enet_packet_create(b, b_size, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(host, 0, p);
}

void Client::Echo(const char* msg)
{
	using namespace NetCommon::Client::Command;
	size_t b_size = 2 + strlen(msg);
	uint8_t b[b_size];
	b[0] = (uint8_t)Type::Echo;
	b[1] = (uint8_t)p_id;
	for (int i = 0; i < 255 && i < b_size - 2; i += 1) {
		b[2 + i] = msg[i];
	}
	ENetPacket* p = enet_packet_create(b, b_size, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(host, 0, p);
}

void Client::Ready()
{
	using namespace NetCommon::Client::Command;
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

	ENetEvent event;

  	while (enet_host_service(client, &event, 0) > 0) {
      		switch (event.type) {
		case ENET_EVENT_TYPE_RECEIVE: {
			uint8_t* data = (uint8_t*)event.packet->data;
			Receive((NetCommon::Server::Command::Type)data[0], event.packet);
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
