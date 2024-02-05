# Game Engine

### Overview
* Developing a 3D Game Engine using OpenGL/GLFW rendering APIs
* The goal for the engine is to make games, but hopefully, get to the point of being able to process audio and digital processing.
* 2D/3D game development for hoping to take into game jams.

### Engines Editor
* Currently the layout of the engine editor.
* Earlier in the development, added a profiler to monitor the function's runtime.
* The editor can add/remove entities into the scene, including adding/removing components from entities.
* Serializing/Deserializing, so users can load/save scenes using the editor.
  * `https://github.com/SpinnerX/Game_Engine/blob/main/GameEngine/include/GameEngine/Scene/SceneSerializer.cpp` - Is where the serialization/deserialization code is located for handling loading/saving different scenes.

https://github.com/SpinnerX/Game_Engine/assets/56617292/694e374d-8bfe-4e7b-9443-aaf1779295c1

### Rocket Game built with this Game Engine
* Developed a rocket game kind of similar to the one in Geometry Dash where you have a rocket that needs to avoid obstacles
* This was simply to showcase the use of this Game Engine, that I was developing
* This is the GitHub link to that repo for Rocket-Game `https://github.com/SpinnerX/Rocket-Game`

https://github.com/SpinnerX/Rocket-Game/assets/56617292/e679bf7d-ee27-47d9-a011-7670a557ce38

### Loading Tilemaps and Spritesheets
![Screenshot 2024-01-14 at 6 29 49 PM](https://github.com/SpinnerX/Game_Engine/assets/56617292/dcd2f9c1-b4e1-408b-bb18-3cb7ce9a13ef)

# Features Added
* 2D Batch Renderer
* Editor UI Layout
* Orthographic/Perspective/Editor Camera
* Profiler
* Serializer/Deserializer (YAML)
* Native Scripting
* Native File Dialog (Mac)
* Entity Component System (ECS)
* Add/Delete Entities/Components (Editor)

### Things TODO
  * 2D physics simulation
  * GPU Particle simulation
  * Multiple Rendering Targets
  * Instance Rendering
  * Rigid Body (Physics)
  * Soft Body (Physics)
