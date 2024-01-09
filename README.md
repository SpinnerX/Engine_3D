### Game Engine

## Overview
- Currently developing a 3D game engine in C++ using OpenGL rendering API's.
- Goal is to allow to create interfaces and other internal interfaces allowing users to use what they pay for.
- Essentially this will be containing interfaces, and users can implement these interfaces as they see fit.
- Implementations are able to be platform specific depending on the OS the user is developing on

* Designing and currently developing my very own Game Engine using C++ and the OpenGL Rendering API's
* Goal is to eventually have this game engine do do 3D stuff, including animations scripting and other things.
* Currently this Game Engine only renders in 2D, as my focus is making sure I am at a good point in making 2D games
    * This is to allow the ability to create both 2D and 3D games

### Rocket Game built with this Game Engine
* Developed a rocket game kind of similar to the one in Geometry Dash where you have a rocket that needs to avoid obstacles
* This was simply to showcase the use of this Game Engine, that I was developing
* This is the github link to that repo for Rocket-Game `https://github.com/SpinnerX/Rocket-Game`

https://github.com/SpinnerX/Rocket-Game/assets/56617292/e679bf7d-ee27-47d9-a011-7670a557ce38

### Features TODO
* Things currently added to the Game Engine at this stage is the following:
    * Basic 2D renderer (where each object we draw are as a quad instead of as batch)
    * Profiling to time stamp the functions being called
    * Reading in shaders
    * Able to rendering textures
    * Applying different tinting colors and rotations to quads
* To be implemented are the following:
  * Batch Renderer
  * 2D physics simulation
  * Particle simulation
  * frame buffers
  * Reading in sprite sheets (specifically also handling TileMaps)
