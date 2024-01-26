### Game Engine

## Overview
- Currently developing a 3D game engine in C++ using OpenGL rendering APIs.
The goal is to allow to create interfaces and other internal interfaces allowing users to use what they pay for.
- Essentially, this will contain interfaces, and users can implement them as they see fit.
- Implementations can be platform-specific depending on the OS the user is developing on

* Designing and currently developing my very own Game Engine using C++ and the OpenGL Rendering API's
* The goal is to eventually have this game engine do 3D stuff, including animation scripting and other things.
* Currently this Game Engine only renders in 2D, as my focus is making sure I am at a good point in making 2D games
    * This is to allow the ability to create both 2D and 3D games

https://github.com/SpinnerX/Game_Engine/assets/56617292/5639f948-3ad6-41c1-ac68-9ba48e29aa4b

### Rocket Game built with this Game Engine
* Developed a rocket game kind of similar to the one in Geometry Dash where you have a rocket that needs to avoid obstacles
* This was simply to showcase the use of this Game Engine, that I was developing
* This is the GitHub link to that repo for Rocket-Game `https://github.com/SpinnerX/Rocket-Game`

https://github.com/SpinnerX/Rocket-Game/assets/56617292/e679bf7d-ee27-47d9-a011-7670a557ce38

### Loading Tile maps and Sprites
![Screenshot 2024-01-14 at 6 29 49 PM](https://github.com/SpinnerX/Game_Engine/assets/56617292/dcd2f9c1-b4e1-408b-bb18-3cb7ce9a13ef)

## Features Added/TODO

### Edit - Things Added and Improvements made.
* Things currently added to the Game Engine at this stage are the following:
* Just added some improvements towards the 2D renderer
* Instead of rendering textures, or shaders as each draw call, implemented batch rendering
* Improved 2D renderer by adding SubTexture2D to handle loading individual sprites in a sprite sheet, and allow for tile mapping
* Added Docking spaces to forward in integrating a UI for easier user use of development
* Implemented Frame buffers, this way we can render multiple scenes either directly to the screen or based on the targets set for the frame buffers.

### List Features Added
* 2D Batch Renderer
* Profiler
* Serializer/Deserializer
* Native File Dialog (Mac)
* Editor Camera
* UI Components/ECS

### TODO
  * 2D physics simulation
  * GPU Particle simulation
  * Multiple Rendering Targets
  * Instance Rendering
