from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.system.package_manager import Apt, Yum, PacMan, Zypper
from conan.tools.scm import Git
from conan.tools.files import copy
import os

class Engine_3dRecipe(ConanFile):
    name = "engine_3d"
    version = "1.0"
    package_type = "library"
    license = "Apache-2.0"

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    # exports_sources = "CMakeLists.txt", "src/CMakeLists.txt", "Editor/CMakeLists.txt", "src/*", "engine3d/*", "Editor/*", "Testbed/*"
    exports_sources = "CMakeLists.txt", "src/CMakeLists.txt", "Engine3D-Editor/CMakeLists.txt", "Engine3D-Editor/*.h", "Engine3D-Editor/*.cpp", "Engine3D-Editor/Panels/*.h", "Engine3D-Editor/Panels/*.cpp", "Engine3D-Editor/*", "Engine3D/Engine3DPrecompiledHeader.h", "Engine3D/*", "Engine3D/*Core/*", "Engine3D/Debug/*", "Engine3D/Event/*", "Engine3D/Imgui/*", "Engine3D/Imgui/imgui", "src/*", "Engine3D-Editor/*", "*"

    def requirements(self):
        self.requires("make/4.4.1")
        self.tool_requires("cmake/3.27.1")
        self.requires("glfw/3.4", transitive_headers=True)
        self.requires("glad/1.0")

        # These end in 1.0 because they are engine3d-customized conan packages
        # Slighly modified of the conan packages and it's CMake generators to using "Unix Makefiles" 
        self.requires("fmt/10.2.1", transitive_headers=True)
        self.requires("spdlog/1.14.1", transitive_headers=True)
        self.requires("glm/1.0.1", transitive_headers=True)
        self.requires("yaml-cpp/0.8.0", transitive_headers=True)
        self.requires("box2d/2.4.2")
        self.requires("opengl/system", transitive_headers=True)
        self.requires("imguidocking/1.0")
        self.requires("imguizmo/1.0")


    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        # If you use "MinGW Makefiles" on windows, by default looks for mingw32-make.exe instead.
        # Needed to find make.exe installed by choco
        tc = CMakeToolchain(self, generator="Unix Makefiles")
        tc.generate()

    def build(self):
        # you can again check or process additional logic before build
        if not self.conf.get("tools.system.package_manager:mode"):
            self.conf.define("tools.system.package_manager:mode", "install")
        
        cmake = CMake(self)
        cmake.verbose = True
        cmake.configure()
        cmake.build()

    def package(self):
        copy(self, "LICENSE", src=self.source_folder, dst=os.path.join(self.package_folder, "licenses"))
        copy(self, pattern="*.h", src=os.path.join(self.source_folder, "Engine3D"), dst=os.path.join(self.package_folder, "Engine3D"))
        copy(self, pattern="*.a", src=self.build_folder, dst=os.path.join(self.package_folder, "lib"), keep_path=False)
        copy(self, pattern="*.so", src=self.build_folder, dst=os.path.join(self.package_folder, "lib"), keep_path=False)
        copy(self, pattern="*.lib", src=self.build_folder, dst=os.path.join(self.package_folder, "lib"), keep_path=False)
        copy(self, pattern="*.dll", src=self.build_folder, dst=os.path.join(self.package_folder, "bin"), keep_path=False)
        copy(self, pattern="*.dylib", src=self.build_folder, dst=os.path.join(self.package_folder, "lib"), keep_path=False)
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.set_property("cmake_target_name", "Engine3D::Engine3D")
        self.cpp_info.libs = ["engine3d"]
        self.cpp_info.includedirs = ['./', './Engine_3D']  # Ordered list of include paths

    
