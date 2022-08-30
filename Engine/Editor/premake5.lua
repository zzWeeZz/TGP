project "Editor"
	location "%{wks.location}/Editor/"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	disablewarnings { "4996", "26812", "4267" }
	targetdir ("../Dependencies/lib/")
	targetname("%{prj.name}")
	objdir ("../intermediates/%{prj.name}/%{cfg.buildcfg}")

	files {
		"**.h",
		"**.cpp",
		"**.hpp",
	}

	includedirs {
		"../Dependencies/Include/",
		"src/",
		"../ToolBox/src/",
		"../YAML/include/",
		"../ImGui",
		"../TGP_Engine/src/"
	}

	links{
		"ToolBox",
		"yaml-cpp",
		"ImGui",
		"TGP_Engine"
	}

	libdirs { "../Dependencies/lib/" }

	filter "configurations:Debug"
		defines {"_DEBUG"}
		runtime "Debug"
		symbols "on"
	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"

	filter "system:windows"
		kind "StaticLib"
		staticruntime "off"
		symbols "On"		
		systemversion "latest"
		flags { 
			"MultiProcessorCompile"
		}
		defines{
			"WIN32",
			"_LIB", 
		}