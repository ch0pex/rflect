from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.files import copy

required_conan_version = ">=2.0.15"


class RflectConan(ConanFile):
    name = "rflect"
    version = "0.1"
    package_type = "header-library"

    # Optional metadata
    license = "MIT"
    author = "Álvaro Cabrera Barrio (alvcabre@pa.uc3m.es)"
    url = "https://github.com/ch0pex/rflect"
    topics = ("static", "reflection", "meta-programing")

    settings = "os", "arch", "compiler", "build_type"

    # Sources are located in the same place as this recipe
    exports_sources = "src/include/*"
    no_copy_source = True

    def layout(self):
        self.folders.root = '..'
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.user_presets_path = 'conan/ConanPresets.json'
        tc.generate()

    def build_requirements(self):
        # Test requires are not forwarded as rflect dependencies
        self.test_requires("doctest/2.4.11")
        self.test_requires("benchmark/1.9.4")

    def package(self):
        copy(self, "*.hpp", self.source_folder, self.package_folder)

    def package_info(self):
        # For header-only packages, libdirs and bindirs are not used
        # so it's necessary to set those as empty.
        self.cpp_info.bindirs = []
        self.cpp_info.libdirs = []
