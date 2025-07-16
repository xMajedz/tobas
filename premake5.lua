require "vendor/premake-ninja/ninja"

workspace "tobas"
	language "C++"
	cppdialect "C++20"
	configurations { "Debug", "Release" }
	location "build"
	links {
		"luaucompiler", "luauast", "luauvm",
		"raylib",
		"ode",
	}

	includedirs {
		"vendor/luau/Compiler/include",
		"vendor/luau/VM/include",

		"vendor/raylib/src",
		"vendor/raygui/src",

		"vendor/ode/include",
		"vendor/enet/include",

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
	links {"winmm", "gdi32", "opengl32"}
	libdirs { "lib/windows" }

	filter { "not system:Windows" }
	libdirs { "lib/linux" }

project "tobas_sp"
	kind "WindowedApp"
	files { "sources/tobas_sp.cpp" }
	defines { "OFFLINE" }

project "tobas"
	kind "WindowedApp"
	files {
		"sources/netcode_common.h", "sources/netcode_common.cpp",
		"sources/netcode_server.h", "sources/netcode_server.cpp",
		"sources/netcode_client.h", "sources/netcode_client.cpp",
		"sources/tobas.cpp",
	}

project "tobas_server"
	kind "ConsoleApp"
	files {
		"sources/netcode_common.h", "sources/netcode_common.cpp",
		"sources/netcode_server.h", "sources/netcode_server.cpp",
		"sources/tobas_server.cpp",
	}
