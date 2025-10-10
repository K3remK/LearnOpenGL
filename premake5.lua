workspace "LearnOpenGL"
    architecture "x86_64"
    startproject "LearnOpenGL"

    configurations
    {
        "Debug",
        "Release"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    include "LearnOpenGL/vendor/GLFW"
    include "LearnOpenGL/vendor/GLAD"
    include "LearnOpenGL/vendor/imgui"
    include "LearnOpenGL"
    