workspace "tobas"
	language "C++"
	configurations { "Debug", "Release" }
	basedir "build"

	includedirs {
		"luau/Compiler/include", "luau/VM/include",

		"raylib/src", "raygui/src",

		"ode/include",
		"enet/include",

		"sources",
	}

	libdirs {
		"luau/build/release",

		"raylib/src",
		"ode/ode/src/.libs",
	}

	links {

		"luaucompiler", "luauast", "luauvm",

		"raylib",
		"ode",
	}

	files {
		"sources/luau.h", "sources/luau.cpp",

		"sources/api.h", "sources/api.cpp",
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

	filter { "platforms:Win*" }
		links {"winmm", "gdi32", "opengl32"}

	filter { "configurations:Debug" }
		defines { "DEBUG" }
		symbols "On"

	filter { "configurations:Release" }
		defines { "NDEBUG" }
		optimize "On"

project "tobas_sp"
	kind "WindowedApp"
	objdir "build"
	basedir "build"
	targetdir "build"
	files { "sources/tobas_sp.cpp" }

project "tobas"
	kind "WindowedApp"
	objdir "build"
	basedir "build"
	targetdir "build"
	files {
		"sources/netcode_server.h", "sources/netcode_server.cpp",
		"sources/netcode_client.h", "sources/netcode_client.cpp",
		"sources/tobas.cpp",
	}

project "tobas_server"
	kind "ConsoleApp"
	objdir "build"
	basedir "build"
	targetdir "build"
	files {
		"sources/netcode_server.h", "sources/netcode_server.cpp",
		"sources/tobas_server.cpp",
	}




