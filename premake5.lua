newaction {
	trigger = "dist",
	description = "distribute a release",
	onStart = function() 
		os.mkdir "dist"
	end,
	onProject = function(prj)
		os.execute("zip dist/"
			.. prj.name  .. "-" .. (_OPTIONS["release"] or "git")
			.. ".zip mods scripts replays build/" .. prj.name
		)
	end,
	onEnd = function() 
		os.execute "ls dist"
	end,
}

newaction {
	trigger = "dist-clean",
	description = "clean dist",
	onStart = function() 
		os.rmdir "dist"
	end,
}

newaction {
	trigger = "run",
	description = "build and run source",
	onStart = function() 
		os.execute "make -j8 -C build"
	end,
	onEnd = function()
		os.execute "build/tobas_sp"
	end,
}

newaction {
	trigger = "build",
	description = "build source",
	onStart = function() 
		os.execute "make -j8 -C build"
	end,
}

newaction {
	trigger = "clean",
	description = "clean",
	onStart = function() 
		os.rmdir "build"
	end,
}

workspace "tobas"
	language "C++"
	cppdialect "C++20"
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
		--"ode/ode/src/.libs",
		"ode/lib/Release",
	}

	links {

		"luaucompiler", "luauast", "luauvm",

		"raylib",
		"ode",
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

	filter { "system:windows" }
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
	defines { "OFFLINE" }

project "tobas"
	kind "WindowedApp"
	objdir "build"
	basedir "build"
	targetdir "build"
	files {
		"sources/netcode_common.h", "sources/netcode_common.cpp",
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
		"sources/netcode_common.h", "sources/netcode_common.cpp",
		"sources/netcode_server.h", "sources/netcode_server.cpp",
		"sources/tobas_server.cpp",
	}




