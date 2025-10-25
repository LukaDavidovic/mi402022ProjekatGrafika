# {Garage with cars}

{040/2022} - {Luka Davidovic}  
{Garage with cars in OpenGL}

This project renders a 3D scene of a garage with two cars using the
OpenGL render system within a custom engine
Implemented lighting, models with multiple textures...
Additionally, through timer events, cars disappear from the scene
after a certain amount of time.

## Controls

{W} -> {Camera movement forward}  
{A} -> {Camera movement left}  
{S} -> {Camera movement back}
{D} -> {Camera movement right}
{SPACE} -> {Move camera up}
{X} -> {Move camera down}
{L} -> {Camera rotation to the right}
{K} -> {Camera rotation to the left}
{G} -> {Triggers a sequence of events(timer: cars disappear)}
{1} -> {Increases light intensity}
{2} -> {Decreases the light intensity}
{ESC} -> {Exit the application}

## Features

### Fundamental:

[YES] Model with lighting
[YES(Point + Directional style)] Two types of lighting with customizable colors and movement through GUI or ACTIONS
[YES] {Action G} --- after 5 seconds triggers ---Triggers---> {EVENT_A(the first car disappears)} ---> after 10 seconds
---Triggers--->
{EVENT_B(the second car disappears}

### Group A:

[ ] Frame-buffers with post-processing   
[ ] Instancing  
[ ] Off-screen Anti-Aliasing  
[ ] Parallax Mapping

### Group B:

[ ] Bloom with the use of HDR  
[ ] Deferred Shading  
[ ] Point Shadows  
[ ] SSAO

### Engine improvement:

[ ] ...

## Models:

Model garage:
[ https://sketchfab.com/3d-models/parking-garage-free-download-5310b7d77b70427d936ec4253fff679c ]
Model car:
[ https://www.turbosquid.com/3d-models/free-obj-mode-car/552663 ]

## Textures

[ https://sketchfab.com/3d-models/parking-garage-free-download-5310b7d77b70427d936ec4253fff679c ]  
[ https://www.turbosquid.com/3d-models/free-obj-mode-car/552663 ]

{ https://learnopengl.com/Lighting/Basic-Lighting
https://learnopengl.com/Lighting/Multiple-lights }
