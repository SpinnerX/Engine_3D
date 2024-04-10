# Engine-3D

## Overview
* Creating Engine3D from scratch using the raw OpenGL API, for understanding further Computer Graphics, and Rendering Architecture
* The purpose for this engine, eventually is to be a Rendering-API agnostic engine that'll soon be able to support Vulkan, DirectX, and even Metal
* Allowing to build creative projects through this engine. While also giving users to use this as an API as well as having an easier setup.
* Here are the philosophies behind the codebase is to be fast, scalable, and be platform-specific.
  * Platform-specific meaning you use what you only need.

## UI Editor
* Engine3D's interactive built-in UI Editor
* Interactive UI elements for dealing with multiple scenes
* Managing entities, and assets
* Playing active scenes through the editor
* Creating a new scene, loading, and saving different scenes through serialization.

https://github.com/SpinnerX/Game_Engine/assets/56617292/694e374d-8bfe-4e7b-9443-aaf1779295c1


### Shadows, Light, Diffuse Reflections
* Will be adding in ray tracing, lighting effects
* This is ray tracing and using PCG hashing to minimize the noise we see in each frame.

![Screenshot_2024-04-08_150624](https://github.com/SpinnerX/Engine_3D/assets/56617292/2518012c-a38f-4712-b818-90260ef1a768)

## Projects using Engine3D

### Particle System
* Manually using Engine3D in its early stages to develop a particle system.
* Used for the `Rocket-Game`for giving it the propeller effects.

https://github.com/SpinnerX/Engine_3D/assets/56617292/653fe22a-8db6-4763-80eb-9404d18358f9

### Rocket-Game
* Engine3D will have support for building 2D and 3D games.
* Building a 2D game called "Rocket-Game", where mechanics represent both the flappy bird and the flying mode in geometry dash. \
* Link to that repo for Rocket-Game `https://github.com/SpinnerX/Rocket-Game`

https://github.com/SpinnerX/Rocket-Game/assets/56617292/e679bf7d-ee27-47d9-a011-7670a557ce38

### Loading Tilemaps and Spritesheets
![Screenshot 2024-01-14 at 6 29 49 PM](https://github.com/SpinnerX/Game_Engine/assets/56617292/dcd2f9c1-b4e1-408b-bb18-3cb7ce9a13ef)

### Features Added
* 2D Batch Renderer
* Editor UI Layout
* Orthographic/Perspective/Editor Camera
* Profiler
* Serializer/Deserializer (YAML)
* Native Scripting
* Native File Dialog (Mac)
* Entity Component System (ECS)
* Add/Delete Entities/Components (Editor)

### TODO List
* 2D/3D physics simulation
* GPU Particle simulation
* Animations
* Meshes
   * Mesh Fracture
* Multiple Rendering Targets
* Instance Rendering
* Ray Tracing
* Rigid Body (Physics)
* Soft Body (Physics)
* Audio Processing
   * Audio Synthesizer
   * Sound VFX
* Skeletal Animations
   * Bones Nodes
   * Joint Physics
* Terrain generator
