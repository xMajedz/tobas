#define ENET_IMPLEMENTATION
#include "netcode_common.h"

#include <fstream>

void NetCommon::log (const char* message)
{
	std::cout << message << std::endl;
}

void NetCommon::WriteConfig()
{
	std::ofstream server_conf("server.conf");
	server_conf << "host=127.0.0.1\nport=1337\nmax_clients=24\n";
	server_conf.close();
}

Config NetCommon::LoadConfig()
{
	Config cfg;
	std::ifstream server_conf("server.conf");
	if (server_conf.good()) {
		std::string host;
		std::getline(server_conf, host);
		cfg.host = host.substr(5, host.size());

		std::string line;
		std::getline(server_conf, line);
		line = line.substr(5, line.size());
		cfg.port = std::stoi(line);

		std::getline(server_conf, line);
		line = line.substr(12, line.size());
		cfg.max_clients = std::stoi(line);
	} else {
		log("Server: Config file created.");
		WriteConfig();
	}
	server_conf.close();
	return cfg;
}

const char* NetCommon::Server::Message::Get(NetCommon::Server::Message::Type type)
{
	using namespace NetCommon::Server;
	return Messages[(Message::Type)type];
};

const char* NetCommon::Client::Message::Get(NetCommon::Client::Message::Type type)
{
	using namespace NetCommon::Client;
	return Messages[(Message::Type)type];
};

