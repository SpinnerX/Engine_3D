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

### Rocket Game built with this Game Engine
* Developed a rocket game kind of similar to the one in Geometry Dash where you have a rocket that needs to avoid obstacles
* This was simply to showcase the use of this Game Engine, that I was developing
* This is the GitHub link to that repo for Rocket-Game `https://github.com/SpinnerX/Rocket-Game`

https://github.com/SpinnerX/Rocket-Game/assets/56617292/e679bf7d-ee27-47d9-a011-7670a557ce38

### Loading Tile maps and Sprites
![Screenshot 2024-01-14 at 6 29 49 PM](https://github.com/SpinnerX/Game_Engine/assets/56617292/dcd2f9c1-b4e1-408b-bb18-3cb7ce9a13ef)


### DockSpaces and Viewports
* Showcasing viewports working by resizing the different docked windows

https://github.com/SpinnerX/Game_Engine/assets/56617292/12aeda2f-b223-48fc-9c80-0708b189abc0

https://github.com/SpinnerX/Game_Engine/assets/56617292/1db92e9c-6a9b-4188-a1f8-25052414fde0



## Features Added/TODO

### Edit - Things Added and Improvements made.
* Things currently added to the Game Engine at this stage are the following:
* Just added some improvements towards the 2D renderer
* Instead of rendering textures, or shaders as each draw call, implemented batch rendering
* Improved 2D renderer by adding SubTexture2D to handle loading individual sprites in a sprite sheet, and allow for tile mapping
* Added Docking spaces to forward in integrating a UI for easier user use of development
* Implemented Frame buffers, this way we can render multiple scenes either directly to the screen or based on the targets set for the frame buffers.

### List Features Added
   *  Basic 2D renderer (where each object we draw is as a quad instead of as in batches)
   *  Profiling to time stamp the functions being called
   *  Batch Rendering
   *  Tile Mapping and Loading SpriteSheets (with Textures)
   *  Docking Spaces
   * Frame Buffers

### TODO
  * Scene Viewport
  * 2D physics simulation
  * Particle simulation
  * ECS (Entity Component System)
  * Better Camera System
  * Camera Scenes
