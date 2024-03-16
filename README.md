# Engine-3D

## Overview
* Game Engine called Engine3D.
* Rendering-API agnostic engine that'll soon be able to support Vulkan, DirectX, and even Metal
* Platform-agnostic engine that'll support both Windows, Linux, and Mac.

## Engines Editor
* Editor's current design and layout
* User can add/remove components from entities.
* User can attach different components to entities. (Static, Dynamic)
* Playing/Stopping scenes
* Native File Dialogs
* Serializer/Deserializer for loading/saving current scenes.
* Is where the serialization/deserialization code is located for handling loading/saving different scenes, `https://github.com/SpinnerX/Game_Engine/blob/main/GameEngine/include/GameEngine/Scene/SceneSerializer.cpp`.

## UI Editor Usage
https://github.com/SpinnerX/Game_Engine/assets/56617292/694e374d-8bfe-4e7b-9443-aaf1779295c1

## Projects Built using Engine3D

### Rocket-Game
* The game is like the mini rocket mode in Geometry Dash.
* Using the 2D renderer built in Engine3D showcases the capabilities of the engine.
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
