project "ToolBox"
	location "%{wks.location}/ToolBox/"
	kind "StaticLib"	
	language "C++"
	cppdialect "C++20"
	disablewarnings { "4996" }
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