#pragma once
#include "netcode_main.h"

namespace Server
{
	void HostGame();
	void HostGameThread();
	void WaitGameThread();

	void Close();
	void CloseThread();
}
