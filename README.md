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
* tba



