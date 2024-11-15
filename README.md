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
![00](https://gitlabstudents.isartintra.com/projets/2024_gp_2028_gp_2028_api_graphique_moderne-gp2_fontverne_a/uploads/2c23a4dd53579d39fdc0e406ee3847c6/Screenshot_2024-11-15_160410.png)
![01](https://gitlabstudents.isartintra.com/projets/2024_gp_2028_gp_2028_api_graphique_moderne-gp2_fontverne_a/uploads/e8985b82f176489d96cc8066b8b951a8/Screenshot_2024-11-15_173559.png)
![02](https://gitlabstudents.isartintra.com/projets/2024_gp_2028_gp_2028_api_graphique_moderne-gp2_fontverne_a/uploads/638b0340e28ed7d3816560c2a567dd7a/Screenshot_2024-11-15_173550.png)
![03](https://gitlabstudents.isartintra.com/projets/2024_gp_2028_gp_2028_api_graphique_moderne-gp2_fontverne_a/uploads/e5ee3bef7b8903592eced50b0f687f76/Screenshot_2024-11-15_173610.png)
![04](https://gitlabstudents.isartintra.com/projets/2024_gp_2028_gp_2028_api_graphique_moderne-gp2_fontverne_a/uploads/b235f0c3dc023a1c7f78d17126c800fa/Screenshot_2024-11-15_173631.png)

## Author:
- FONTVERNE Antoine