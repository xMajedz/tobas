#include "netcode_common.h"

#include <fstream>

int write_cfg()
{
	std::ofstream server_conf("server.conf");
	server_conf << "host=127.0.0.1\n";
	server_conf << "port=1337\n";
	server_conf << "max_clients=24\n";
	server_conf.close();
	return 0;
}

Config parse_cfg()
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
		std::cout << "Server: Config file created." << std::endl;
		write_cfg();
	}
	server_conf.close();
	return cfg;
}
