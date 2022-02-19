set_project("software_renderer")

add_rules("mode.debug", "mode.release", "mode.profile")

add_requires("libsdl")
add_requires("stb")

add_packages("libsdl")
add_includedirs("../Hinae/include")
add_includedirs("src")

if is_mode("release") then
    add_rules("c++.unity_build")
    if is_plat("windows") then
        add_ldflags("/LTCG")
    end
end

set_languages("cxxlatest")

add_vectorexts("sse", "sse2", "sse3", "ssse3")

set_warnings("all")

if is_plat("windows") then
    add_defines("_CRT_SECURE_NO_WARNINGS")
    add_cxflags("/utf-8")
end

set_rundir("$(projectdir)")

target("object")
    set_kind("object")
    add_files("src/**.cpp")
    add_packages("stb")

target("main")
    set_default(true)
    set_kind("binary")
    add_files("examples/main.cpp")
    add_deps("object")

target("debug")
    set_kind("binary")
    add_files("examples/debug.cpp")
    add_deps("object")

target("games101_lab1")
    set_kind("binary")
    add_files("examples/games101_lab1.cpp")
    add_deps("object")

-- target("games101_lab2")
--     set_kind("binary")
--     add_files("examples/games101_lab2.cpp")
--     add_deps("object")
