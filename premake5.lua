newoption {
	trigger = "location",
	description = "sets build directory.",
	default = "build",
}

workspace "tobas"
	language "C++"
	cppdialect "C++20"
	configurations { "Debug", "Release" }
	location (_OPTIONS["location"]) 

	includedirs {
		"vendor/luau/Compiler/include",
		"vendor/luau/VM/include",

		"vendor/raylib/src",
		"vendor/raygui/src",

		"vendor/ode/include",

		"sources",
	}

	files {
		"sources/luau.h", "sources/luau.cpp",

		"sources/api.h", "sources/api.cpp",
		"sources/api_net.cpp",
		"sources/api_game.cpp",
		"sources/api_raylib.cpp",
		"sources/api_raygui.cpp",
		"sources/api_raymath.cpp",

		"sources/game.h", "sources/game.cpp",
		"sources/camera.h", "sources/camera.cpp",
		"sources/replay.h", "sources/replay.cpp",
		"sources/player.h", "sources/player.cpp",
		"sources/joint.h", "sources/joint.cpp",
		"sources/body.h", "sources/body.cpp",
	}

	filter { "configurations:Debug" }
	defines { "DEBUG" }
	symbols "On"
	
	filter { "configurations:Release" }
	defines { "NDEBUG" }
	optimize "On"

	filter { "system:Windows" }
	libdirs { "lib/windows" }

	filter { "not system:Windows" }
	libdirs { "lib/linux" }

project "netcode_common"
	kind "Staticlib"
	includedirs { "sources", "vendor/enet/include" }
	files { "sources/netcode_common.h", "sources/netcode_common.cpp" }

project "netcode_client"
	kind "Staticlib"
	includedirs { "sources", "vendor/enet/include" }
	files {
		"sources/netcode_common.h", "sources/netcode_common.cpp",
		"sources/netcode_client.h", "sources/netcode_client.cpp",
	}
	links { "netcode_common" }

project "netcode_server"
	kind "Staticlib"
	includedirs { "sources", "vendor/enet/include" }
	files {
		"sources/netcode_common.h", "sources/netcode_common.cpp",
		"sources/netcode_server.h", "sources/netcode_server.cpp",
	}
	links { "netcode_common" }

project "tobas_common"
	kind "Staticlib"
	links {
		"luaucompiler", "luauast", "luauvm",
		"raylib",
		"ode",
	}

project "tobas_sp"
	kind "WindowedApp"
	files { "sources/tobas_sp.cpp" }
	links { "tobas_common" }
	links {
		"luaucompiler", "luauast", "luauvm",
		"raylib",
		"ode",
	}
	defines { "OFFLINE" }

	filter { "system:Windows" }
	links {"winmm", "gdi32", "opengl32"}
	defines{"_WIN32"}

project "tobas"
	kind "WindowedApp"
	includedirs { "sources", "vendor/enet/include" }
	files { "sources/tobas.cpp" }
	links { "tobas_common", "netcode_server", "netcode_client" }
	links {
		"luaucompiler", "luauast", "luauvm",
		"raylib",
		"ode",
	}

	filter { "system:Windows" }
	links {"winmm", "gdi32", "opengl32"}
        links { "kernel32", "ws2_32" }
	defines{"_WIN32"}

project "tobas_server"
	kind "ConsoleApp"
	includedirs { "sources", "vendor/enet/include" }
	files { "sources/tobas_server.cpp" }
	links { "tobas_common", "netcode_server" }
	links {
		"luaucompiler", "luauast", "luauvm",
		"raylib",
		"ode",
	}

	filter { "system:Windows" }
	links {"winmm", "gdi32", "opengl32"}
        links { "kernel32", "ws2_32" }
	defines{"_WIN32"}
