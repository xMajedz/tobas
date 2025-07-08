#pragma once
#include "enet.h"

#include <cstdio>
#include <iostream>
#include <string>

struct Config {
	std::string host = "127.0.0.1";
	int port = 1337;
	int max_clients = 24;
};

int write_cfg();
Config parse_cfg();
