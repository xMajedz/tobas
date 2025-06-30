workspace "tobas"
	configurations { "Debug", "Release" }

project "tobas"
	kind "WindowedApp"
	language "C++"
	targetdir "build"

	includedirs {
		"luau/Compiler/include", "luau/VM/include",

		"raylib/src",
		"ode/include",

		"sources",
	}

	libdirs {
		"luau/build/release",

		"raylib/src",
		"ode/ode/src/.libs",
	}

	links {

		"luauast", "luaucompiler", "luauvm",

		"raylib",
		"ode",
	}

	files {
		"sources/luau.h", "sources/luau.cpp",

		"sources/api.h", "sources/api.cpp",
		"sources/api_game.h", "sources/api_game.cpp",
		"sources/api_callbacks.h", "sources/api_callbacks.cpp",
		"sources/api_raylib.h", "sources/api_raylib.cpp",
		"sources/api_raymath.h", "sources/api_raymath.cpp",

		"sources/game.h", "sources/game.cpp",
		"sources/camera.h", "sources/camera.cpp",
		"sources/player.h", "sources/player.cpp",
		"sources/joint.h", "sources/joint.cpp",
		"sources/body.h", "sources/body.cpp",

		"sources/main.cpp",
	}

	filter { "configurations:Debug" }
		defines { "DEBUG" }
		symbols "On"

	filter { "configurations:Release" }
		defines { "NDEBUG" }
		optimize "On"
