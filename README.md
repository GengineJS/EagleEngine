![Eagle Logo](./icons/eagle_logo.jpg)
# EagleEngine 

Eagle Engine is a mobile and web-first rendering engine, Graphics API uses Vulkan and WebGL/WebGPU. Developed with C++14.

## Architecture diagram

We organize and submit the model data of the scene through the Render Graph.

```          
  FRAME LAYER                RENDER PIPELINE                             GRAPHICS
┌-------------┐             ┌---------------┐                          ┌----------┐
| SceneGraph  | ------------|               ├------------------------> |          |
| Components  |             |  SceneCulling |                          |          |
| Models      |             |               |                          |  GFX API |
| SubModels   |             |  RenderGraph  |                          |          |
| ......      | <---------- |               ├<------------------------ |          |
└-------------┘             └---------------┘                          └----------┘
    |             
    |               APPLICATION              
    |     ┌----------------------------┐
    └---->|  EagleApplication/Renderer |
          └----------------------------┘
```
## Cloning

This repository contains submodules for external dependencies, so when doing a fresh clone you need to clone recursively:

```
git clone git@github.com:GengineJS/EagleEngine.git --recurse-submodules
```

Existing repositories can be updated manually:

```
git submodule init
git submodule update
```
## Assets

Many examples require assets from the asset pack that is not part of this repository due to file size. A python script is included to download the asset pack that. Run

```
python download_assets.py
```
from the root of the repository after cloning or see [this](data/README.md) for manual download.

## Building

Currently our supported platforms include <img src="./icons/windowslogo.png" alt="" height="22px" valign="bottom"> windows, <img src="./icons/macos.png" alt="" height="22px" valign="bottom"> macos, <img src="./icons/applelogo.png" alt="" height="22px" valign="bottom"> ios and <img src="./icons/androidlogo.png" alt="" height="22px" valign="bottom"> android.

### <img src="./icons/windowslogo.png" alt="" height="15px"> Windows 

Use the provided CMakeLists.txt with [CMake](https://cmake.org) to generate a build configuration for your favorite IDE or compiler, e.g.:

```
cmake -G "Visual Studio 16 2019" -A x64
```

### <img src="./icons/androidlogo.png" alt="" height="15px"> [Android](android/)

Building on Android is done using the [Gradle Build Tool](https://gradle.org/):

```
cd android
./gradlew assembleDebug
```
This will download gradle locally, build all samples and output the apks to ```android\examples\bin```.

On Windows execute `gradlew.bat assembleDebug`.

If you want to build and install on a connected device or emulator image, run ```gradle installDebug``` instead.

### <img src="./icons/applelogo.png" alt="" height="20px"> [iOS and macOS](xcode/)

Building for *iOS* and *macOS* is done using the [examples](xcode/examples.xcodeproj) *Xcode* project found in the [xcode](xcode) directory. These examples use the [**MoltenVK**](https://moltengl.com/moltenvk) Vulkan driver to provide Vulkan support on *iOS* and *macOS*, and require an *iOS* or *macOS* device that supports *Metal*. Please see the [MoltenVK Examples readme](xcode/README_MoltenVK_Examples.md) for more info on acquiring **MoltenVK** and building and deploying the examples on *iOS* and *macOS*.

### <img src="./icons/macos.png" alt="" height="15px"> MacOS
Use the provided CMakeLists.txt with [CMake](https://cmake.org) to generate a build configuration for your favorite IDE or compiler, e.g.:
```
cmake -G "Xcode"
```