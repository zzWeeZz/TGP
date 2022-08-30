workspace "TGP"
	location "."
	startproject "Launcher"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Launcher"
include "TGP_Engine"
include "ToolBox"
include "YAML"
include "ImGui"
include "Editor"

