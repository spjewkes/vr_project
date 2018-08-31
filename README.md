# NOTE: VERY MUCH WORK IN PROGRESS AT THE MOMENT

# Virtual Reality Project

A simple Virtual Reality project starting from an old DOS project I wrote around 1993-94. It was written with a copy of Turbo C++ (possibly version 3.0). Although it was written in C rather than any early version of C++ that was available at the time. The style of the code at that stage was a little rough around the edges as I'd only started learning C around a year earlier. One thing that I was pleased about was that I had at least tried to group the functions into files depending on what they were doing. There's also a fair amount of documentation. In some cases this was a little over-the-top but it has helped in my understanding of what I was trying to do when looking at it 25 years later.

This code was originally retrieve via a paper-based copy that I had. This required some cleaning up and even then there was a missing header file (defs.h) that needed some recreation. The original code in this state I have kept in a completely separate repository (see https://github.com/spjewkes/old_vr_project). Although going back to the first revision of this git repo will be effectively the same I quite liked the idea of having it on show as a separate repo.

The new code has been refactored to make use of SDL as its graphics base. I've stripped out the ability to link programs to objects. This used to run a DOS specific system call to spawn a separate program. Whilst this could be re-written I not entirely sure how useful this feature is so I have left it unimplemented for the time being.

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

# Initial planned development

The initial planned development listed the following changes:

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

# Major changes

This section will list the big changes that have been made since the project was imported as an old DOS-based project. These are roughly chronological in order and some earlier changes made be superceded by items further down the list.

* Replaced old BGI graphics code with SDL (including writing some code to generate solid-shaded triangles)
* Modified all C source files into C++ equivalents
* Combined all the parsing files into one single file
* Started applying a vector3d class where appropriate (e.g. some 3d maths calculations)
* Removed edge list from file description and made polygons purely triangles
* Tried to improve sorting of objects to use C++ standard library. We now sort objects based on the furthest corner of the shape
* As each object is drawn, its triangles are sorted by a midpoint value per triangle. This is to try and help with situation where back-face culling is not enough.
