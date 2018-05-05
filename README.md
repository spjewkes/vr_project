# NOTE: VERY MUCH WORK IN PROGRESS AT THE MOMENT

# Virtual Reality Project

A simple Virtual Reality project starting from an old DOS project I wrote

# Planned development

The order of plan for updating this will be:
* Get it running by replacing the old DOS and BGI library calls. SDL2 will be the main replacement.
* Tidy up formatting and create proper header files for each file
* Start looking at moving code to a more object-oriented design
* Improve renderer:
  * Get rid of 256 color limit
  * Improve shading of polygons
  * Add Z-buffering
* Improve collision detection
* Get rid of spawning a process for interactions (this will probably limit the scope of what can be done but I think it will be for the better)
* Replace config file with something more standard
* Improve model descriptions. Looking through the world description I have remaining it looks like you could define 4-sided shapes with 3 edges. Which is not good and is confusing too. This is something that should be fixed too.

For now I'd prefer to stick with a software-based renderer although at some point it would be nice to explore switching to a proper hardware accelerated solution. This is quite low in my priorities for the time being.

# Using program

Build by using make. Currently it is only set-up to build for the Mac OSX but as it is using SDL2, porting should not be tricky.

There is an example test script to try out. Run the program as follows:

```
./run_vr_project ./test_script.txt
```

Keys:

* Cursor key left - turn left
* Cursor key right - turn right
* Cursor key up - move forward
* Cursor key down - move back
* Cursor key left + left alt - strafe left
* Cursor key right + left alt - strafe right
* Press shift plus any above key to speed up movement

An example image of the world is as follows:

![alt text](https://github.com/spjewkes/vr_project/blob/master/res/hires_test_world.png)