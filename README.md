# **Nihil**
3D game engine built in C++ with minimal dependencies.

## ⚙️ Setup
1. Setup and run [vcpkg](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-powershell)
2. Clone the repository:
   ```bash
   git clone https://github.com/lukasstarinsky/Nihil.git
   cd Nihil
   ```

3. Configure and build the project using CMake Presets:
   ```bash
   cmake --list-presets
   

   cmake --preset *-Debug
   cmake --build --preset *-Debug
   # OR
   cmake --preset *-Release
   cmake --build --preset *-Release
   ```

4. Run the executable:
   ```bash
   ./Binaries/Debug/NihilSandbox.exe
   # OR
   ./Binaries/Release/NihilSandbox.exe
   ```

## 📦 Dependencies

| **Name** | **Usage** |
|-----------|------------|
| [shaderc](https://github.com/google/shaderc) | Compilation of shaders to spv |
| [spirv-cross](https://github.com/KhronosGroup/SPIRV-Cross) | Cross compilation of spv to other APIs |
| [gtest](https://github.com/google/googletest) | Unit tests |
| [assimp](https://github.com/assimp/assimp) | Importing mesh files |
| [zstd](https://github.com/facebook/zstd) | Asset Compression/Decompression |
| [stb](https://github.com/nothings/stb) | Importing of images, sound, fonts |