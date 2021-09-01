UE4 Procedural Mesh
===================

[![Join the chat at https://gitter.im/SRombauts/UE4ProceduralMesh](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/SRombauts/UE4ProceduralMesh?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

Note: There is now à better alternative with the [Runtime Mesh Component plugin by Koderz (forum)](https://forums.unrealengine.com/showthread.php?113432-Runtime-Mesh-Component-Rendering-high-performance-runtime-generated-meshes), available [on Github (Koderz/UE4RuntimeMeshComponent)](https://github.com/Koderz/UE4RuntimeMeshComponent) and on [the Marketplace](https://www.unrealengine.com/marketplace/runtime-mesh-component)

UE 4.7 Procedural MeshGeneration from the Epic Wiki, using C++ code and Blueprints
 
 - https://wiki.unrealengine.com/Procedural_Mesh_Generation
 
 This is a very simple demonstration on how to generate procedural meshes and spawn them in game.
 
 It is not to be taken as an example of proper programming technique,
 but only as an indication to help you generate your own meshes.

## C++ Code

- UProceduralMeshComponent, using FProceduralMeshTriangle, composed of FProceduralMeshVertex 
- AProceduralTriangleActor spwaning an simple triangle mesh with UV and a base color material applied that can be changed at runtime
- AProceduralLatheActor spwaning an example "Lathe" mesh from rotating a Polyline, with another base color applied

## Blueprints

Basic blueprints are created from the C++ Procedural Actor classes.
This help demonstrating how they can be spawned manually in the level at construction time.
Then a new Material can be applied to the generated meshes (the triangle have correct UV for applying textures)
- BP_ProceduralTriangle
- BP_ProceduralLathe

## Level Blueprint

Spawnin is done on the Level Blueprint, at the BeginPlay Event :

 ![Level Blueprint spawning a GameGeneratedActor](https://raw.githubusercontent.com/SRombauts/UE4ProceduralMesh/master/LevelBlueprint.png "Level Blueprint spawning a GameGeneratedActor")

## Result

 ![GameGeneratedActor](https://raw.githubusercontent.com/SRombauts/UE4ProceduralMesh/master/ProceduralMesh.png "GameGeneratedActor")
