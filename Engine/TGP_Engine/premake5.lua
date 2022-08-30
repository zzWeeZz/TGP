project "TGP_Engine"
	location "%{wks.location}/TGP_Engine/"
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
		"**.hlsl",
		"**.hlsli",
	}

	includedirs {
		"../Dependencies/Include/",
		"src/",
		"../ToolBox/src/",
		"../YAML/include/",
		"../ImGui"
	}

	links{
		"ToolBox",
		"yaml-cpp",
		"ImGui"
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
		
	local shader_dir = "../AssetData/Shaders/"
    os.mkdir(shader_dir)

    filter("files:**.hlsl")
        flags("ExcludeFromBuild")
        shaderobjectfileoutput(shader_dir.."%{file.basename}"..".cso")
        shadermodel "5.0"

    filter("files:**_ps.hlsl")
        removeflags("ExcludeFromBuild")
        shadertype("Pixel")
        shadermodel "5.0"

    filter("files:**_vs.hlsl")
        removeflags("ExcludeFromBuild")
        shadertype("Vertex")
        shadermodel "5.0"

    filter("files:**_gs.hlsl")
        removeflags("ExcludeFromBuild")
        shadertype("Geometry")
        shadermodel "5.0"

    filter("files:**_cs.hlsl")
        removeflags("ExcludeFromBuild")
        shadertype("Compute")
        shadermodel "5.0"
        filter ("files:**.c")