# Vulkan Renderer
This project was an introduction to the modern graphics API Vulkan. We had 4 weeks to set up a software capable of displaying textured 3D models, lights and a camera.

## Features:
- Many 3D models at the same time
- Different textures at the same time
- Runtime shader compilation
- A camera to move in space

Due to lack of time, the lights have not been implemented, but can be easily done thanks to the modularity of the RHI.

## More information:
The software uses version 1.3.296.0 of [Vulkan SDK](https://vulkan.lunarg.com/sdk/home#windows) and version 3.4 of [GLFW](https://www.glfw.org/download.html).
/!\ You will also required all debug tools of the SDK that are not mandatory at the installation. /!\

It also uses [shaderc lib](https://github.com/google/shaderc) included in the Vulkan SDK for runtime shader compilation.

Documentation used:
- [Vulkan Documentation](https://docs.vulkan.org/spec/latest/index.html)
- [Vulkan Tutorial](https://vulkan-tutorial.com/)

Assets used:

- [Viking Room](https://www.turbosquid.com/fr/3d-models/3d-viking-room-1620117)
- Minecraft World

## Controls:
### Keyboard controls :

|    Controls   |     Action    |
|     :---:     |     :---:     |
|       W       |  Move Forward |
|       A       |    Move Left  |
|       S       |   Move Right  |
|       D       | Move Backward |
|       Q       |   Move Down   |
|       E       |    Move Up    |

## Screenshots:
![00](https://github.com/Auzuras/Vulkan-Introduction/blob/e0820412da789b23fdb2d74a79a9da116748e1a9/Screenshots/Screenshot_2024-11-15_160410.png)
![01](https://github.com/Auzuras/Vulkan-Introduction/blob/e0820412da789b23fdb2d74a79a9da116748e1a9/Screenshots/Screenshot_2024-11-15_173559.png)
![02](https://github.com/Auzuras/Vulkan-Introduction/blob/e0820412da789b23fdb2d74a79a9da116748e1a9/Screenshots/Screenshot_2024-11-15_173550.png)
![03](https://github.com/Auzuras/Vulkan-Introduction/blob/e0820412da789b23fdb2d74a79a9da116748e1a9/Screenshots/Screenshot_2024-11-15_173610.png)
![04](https://github.com/Auzuras/Vulkan-Introduction/blob/e0820412da789b23fdb2d74a79a9da116748e1a9/Screenshots/Screenshot_2024-11-15_173631.png)

## Author:
- FONTVERNE Antoine
