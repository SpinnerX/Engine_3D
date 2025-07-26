from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.scm import Git
from conan.tools.files import copy
import os
import shutil

class Engine_3DRecipe(ConanFile):
    name = "engine-3d"
    version = "1.0"
    package_type = "library"
    license = "Apache-2.0"
    homepage = "https://github.com/SpinnerX/Engine_3D"
    description = "Multipurpose 3D engine"
    topics = ("Engine", "Graphics", "Animations", "Simulations")
    
    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"

    # Specifying our build_type is only Debug and Release
    options = {"shared": [True, False], "fPIC": [True, False], "enable_tests_only": [True, False], "enable_shaderc": [True, False]}
    default_options = {"shared": False, "fPIC": True, "enable_tests_only": True, "enable_shaderc": False}


    def build_requirements(self):
        self.tool_requires("make/4.4.1")
        self.tool_requires("cmake/3.27.1")
        self.tool_requires("engine3d-cmake-utils/4.0")

    def requirements(self):
        self.requires("box2d/2.4.2")
        self.requires("glad/0.1.36")
        self.requires("imguidocking/2.0")
        self.requires("imguizmo/1.0")
        self.requires("entt/3.15.0")
        self.requires("glfw/3.4", transitive_headers=True)
        self.requires("opengl/system", transitive_headers=True)
        self.requires("spdlog/1.15.1")
        self.requires("glm/1.0.1", transitive_headers=True)

        # Vulkan-related headers and includes packages
        self.requires("vulkan-headers/1.3.290.0", transitive_headers=True)
        self.requires("tinyobjloader/2.0.0-rc10")
        self.requires("tinygltf/2.9.0")
        self.requires("stb/cci.20230920")
        self.requires("yaml-cpp/0.8.0")
        self.requires("nfd/1.0")
    
    def export_sources(self):
        copy(self,"CMakeLists.txt", self.recipe_folder, self.export_sources_folder)
        copy(self,"src/CMakeLists.txt", self.recipe_folder, self.export_sources_folder)
        copy(self,"*.hpp", self.recipe_folder, self.export_sources_folder)
        copy(self,"*.h", self.recipe_folder, self.export_sources_folder)
        copy(self,"*.h", self.recipe_folder, self.export_sources_folder)
        copy(self,"*.cpp", self.recipe_folder, self.export_sources_folder)
        copy(self, "shader_ubo_tutorial", self.recipe_folder, self.export_sources_folder)

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        print(f"Current build_type = {self.settings.build_type}")
        if not self.settings.get_safe("build_type"):
            self.settings.build_type = "Debug"
        
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        # TODO: Remove this once fixing shaderc issue in the CI is resolved
        # These are options that can be enabled/disabled with the `-o` parameter when compiling with `conan` command
        tc.variables["USE_SHADERC"] = self.options.enable_shaderc
        tc.variables["ENABLE_TESTS_ONLY"] = self.options.enable_tests_only
        tc.generate()

    def build(self):
        # Removing the .gcda files for specifically tests
        # Due to some arc-tag
        # gcda_tests_dir = os.path.join(self.build_folder, "CMakeFiles/unit_test.dir/tests")
        # tests_dir = os.path.join(self.source_folder, "tests");
        # tests_files = os.path.join(tests_dir, "*.test.cpp")

        # gcda_files = os.path.join(gcda_tests_dir, "*.gcda")

        # if os.path.exists(gcda_tests_dir):
        #     for file in glob.glob(gcda_files):
        #         os.remove(file)
        unit_test_dir = os.path.join(self.build_folder, "CMakeFiles\engine3d_unit_test.dir")
        print(f"unit test dir === {unit_test_dir}")

        if os.path.exists(unit_test_dir):
            print("Cleaning up unit_test directory cache")
            shutil.rmtree(unit_test_dir)

        cmake = CMake(self)
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
        self.cpp_info.libs = ["Engine3D"]
        self.cpp_info.includedirs = ['./', './Engine3D']  # Ordered list of include paths
