# Description ⚛
Simple 2d physics engine I wrote in C++ to start learning that language. Simulates physical interactions between particles (blobs).

# Capabilities ✅
* Simulation of movement of blobs using Verlet integration
* Simulation of collisions between blobs
* Simulation of rigid connections (sticks) between blobs
* Simulation of force attraction between blobs
* Simulation of interaction between blobs and custom shapes (containers)

# Controls 🔀
* Left Click - spawn a new blob 
* Lef Click + A - spawn an anchor blob (unaffected by gravity) 
* Left Click (after clicking C) - create a new node of a shape (container) 
* Right Click (on a blob) - inspect a blob 
* Right Click + M - move blob to a new location
* Right Click + R - set new radius 
* Right Click + U - select blobs to be connected by a stick (click on one particle then on another)
* T - freeze / unfreeze time 
* G - magnetize blobs (they will attract each other)
* C - container construction on / off (if container construction is on after every left click a new node will be created)
* Right Shift - save simstate (creates a savefile in the folder where the .exe file exists)
* Right Ctrl - load saved simstate 

# Dependencies 🔗
* To compile this code properly the SFML library is required. In my case I had to include path to the SFML library in the includePath section in cpp_properties json in VisualCode. The project uses SFML V2.5.1.
* Use this command to compile: g++ -c mainV02.cpp libraries/blob.cpp libraries/helperFunc.cpp libraries/saveload.cpp -I"YOUR-PATH-TO-SFML\SFML-2.5.1\include"
* The SFML libraries audio-2 , audio-d-2 , graphics-2 , graphics-d-2 , network-2 , network-d-2 , system-2 , system-d-2 , window-2 , window-d-2 must be included in the folder where mainV02.cpp is stored, otherwise the executable won't launch; althrough that may be an issue with my particular configuration.

# Demo 🎬
https://youtu.be/GSGCWc7b6p0



