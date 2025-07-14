#pragma once
#include <iostream>
#include <string>

#define ENET_IMPLEMENTATION
#include "netcode_main.h"

struct Config {
	std::string host = "127.0.0.1";
	int port = 1337;
	int max_clients = 8;
};

namespace NetCommon
{
	void Init();
	Config LoadConfig();
	void WriteConfig();
	void log (const char* message);

	namespace Server
	{
		static const char* Messages[] = {
			"Server: Starting ...",
			"Server: An error occurred while initializing ENet.",
			"Server: An error occurred while trying to create an ENet server host.",
		};

		namespace Message
		{
			enum Type
			{
				Starting = 0,
				InitError,
				ServerError,
				Started,
			};

			const char* Get(Type MessageType);
		};

		namespace Command
		{
			enum Type 
			{
				NONE = 0,
				Accept,
				Step,
				P_Connect,
				P_Disconnect,
				P_Timeout,
			};
		};
	};

	namespace Client
	{
		static const char* Messages[] = {
			"Client: Connecting ...",
			NULL,
			NULL,
			"Client: An error occurred while initializing ENet.",
			"Client: An error occurred while trying to create an ENet client host.",
			"Client: No available peers for initiating an ENet connection.",
			"Client: Disconnecting ...",
			"Client: Disconnection succeeded.",
		};

		namespace Message
		{
			enum Type
			{
				Connecting = 0,
				ConnectionPass,
				ConnectionFail,
				InitError,
				ClientError,
				HostError,
				Disconnecting,
				DisconnectionPass,
			};

			const char* Get(Type MessageType);
		};

		namespace Command
		{
			enum Type
			{
				NONE = 0,
				Join,
				Ready,
			};
		};
	};
};
