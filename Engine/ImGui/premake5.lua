project "ImGui"
    kind "StaticLib"
    language "C++"
    location "%{wks.location}/ImGui"
    targetdir ("%{wks.location}/dependencies/lib/")
    disablewarnings { "4996", "26812" }
    objdir ("%{wks.location}/intermediates/%{cfg.architecture}")
    
        files
        {
            "%{wks.location}/ImGui/**.h",
            "%{wks.location}/ImGui/**.cpp",
            "%{wks.location}/ImGui/**.c",
        }
    
        includedirs
        {
            "%{wks.location}/ImGui"
        }
    
        filter "system:windows"
            systemversion "latest"
            cppdialect "C++17"
            staticruntime "off"
    
        filter "configurations:Debug"
            defines {"DEBUG"}
            runtime "Debug"
            symbols "on"
            
        filter "configurations:Release"
            defines "RELEASE"
            runtime "Release"
            optimize "on"