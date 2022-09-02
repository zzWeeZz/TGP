project "Launcher"
	location "%{wks.location}/Launcher/"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++20"
	disablewarnings { "4996" , "26812"}
	targetdir ("../AssetData/")
	targetname("%{prj.name}_%{cfg.buildcfg}")
	objdir ("../Intermediates/%{prj.name}/%{cfg.buildcfg}")

	debugdir "../AssetData/"

	files {
		"**.h",
		"**.cpp",
	}

	includedirs {
		"../Dependencies/",
		"../TGP_Engine/src/",
		"../ToolBox/src/",
		"../YAML/include/",
		"../ImGui/",
		"../Editor/src/",
		"src/",
	}

	libdirs { "../Dependencies/lib/" }

	links { 
		"d3d11.lib",
		"d3dcompiler.lib",
		"TGP_Engine",
		"ToolBox",
		"yaml-cpp",
		"ImGui",
		"Editor"
	}

	linkoptions 
	{
		"/WHOLEARCHIVE:TGP_Engine"
	}

	defines {"_WINDOWS"}
	
	filter "configurations:Debug"
		defines {"_DEBUG"}
		runtime "Debug"
		symbols "on"
		links {
			"tgafbximporterd.lib",
		}
		
	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"
		links {
			"TGAFBXImporter.lib",
		}

	systemversion "latest"
	
	filter "system:windows"
		symbols "On"		
		systemversion "latest"
		flags { 
			"MultiProcessorCompile"
		}
		defines{
			"WIN32",
			"_LIB", 
		}
		