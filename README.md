# Phosphor - An open source game engine

Phosphor is a game engine written in C++ and OpenGL. It is designed to be a simple and easy to use game engine for beginners and experienced developers alike. The engine is still in development and is not yet ready for use.

## Features

- 2D and 3D rendering (OpenGL, Vulkan and Direct3D support planned, as well as Metal)
- Audio support planned (OpenAL, FMOD)
- Input handling (SDL2, SDL3 planned)
- Scripting planned (Lua, Python, ???)
- Physics planned (Bullet, Box2D)
More to come soon

## External Libraries

- [VCPkg](https://github.com/microsoft/vcpkg) - Dependency management
- [SDL2](https://www.libsdl.org/) - Windowing and input handling
- [GLM](https://github.com/g-truc/glm) - Math library
- [ImGui](https://github.com/ocornut/imgui) - GUI library
- [spdlog](https://github.com/gabime/spdlog) - Logging library

## Running

Phosphor uses CMake as its build system. To build the engine, you will need to have CMake installed on your system. You will also need to have VCPkg installed and set up on your system.

### From Source

1. Clone the repository and submodules
```bash
git clone https://github.com/Animus-Surge/phosphor.git
cd phosphor
git submodule update --init --recursive
```

2. Set up VCPkg
```bash
cd external/vcpkg
./bootstrap-vcpkg.sh -disableMetrics
cd ../..
```
(Note: the `-disableMetrics` flag is optional, I included it because I personally don't like having metrics enabled, but it is entirely up to you)
(Note: If you are on Windows, you will need to run the bootstrap-vcpkg.bat file instead of the shell script)
(Note: After the script gets run, ensure the generated `vcpkg` executable is in your PATH)

3. Install dependencies
```bash
vcpkg install
```

4. Build the engine
```bash
cmake --profile=default
cmake --build build
```

Exectuables and libraries will be located in the `build` directory.

Now simply launch the editor (when it exists), run any of the examples, or link against the shared library in your own project!

## Contributing

Contributions are welcome! If you would like to contribute to the project, please fork the repository and submit a pull request. Please ensure that your code is well documented and that it follows the existing code style.

## License

Phosphor is licensed under the MIT License. See the [LICENSE](LICENSE) file for more information.
