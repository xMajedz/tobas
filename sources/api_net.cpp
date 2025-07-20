#include "api.h"
#include "luau.h"
#ifndef OFFLINE
/*#include "netcode_client.h"*/
/*#include "netcode_server.h"*/
#endif

static const luaL_Reg api_net[] {
#ifndef OFFLINE
#endif
	{NULL, NULL},
};

int luaopen_api_net(lua_State* L)
{
#ifndef OFFLINE
	luaL_register(L, "Net", api_net);
#endif
	return 1;

}
