# NOTE: VERY MUCH WORK IN PROGRESS AT THE MOMENT

# Virtual Reality Project

This is a simple Virtual Reality project starting from an old DOS project I wrote around 1993-94. It was written with a copy of Turbo C++ (possibly version 3.0). Although it was written in C rather than any early version of C++ that was available at the time. The style of the code at this stage was a little rough around the edges as I'd only bagan learning C around a year earlier. On revisiting the code, one thing that I was pleased about was that I had at least tried to put the code into modules and documented the code (excessively in some places).

This code was revived via a paper-based copy that I had. This was converted back to files using an OCR program called Prizmo. This was not perfect but I think the problems in the main were to do with the fact that is was code so the formatting was generally lost and numerics sometimes got confused with alphabetic characters. Most of these issues were easily resolved during the early stages of trying to compile the code. In one case, I lacked the original file entirely and had to recreate it. This was only the file containing definitions (defs.h) and it was relatively trivial to recreate this.

The original code prior to compiling is kept in a completely separate repository (see https://github.com/spjewkes/old_vr_project). Although going back to the first revision of this git repo will be effectively the same I quite liked the idea of having it on show as a separate repo. It lacks some of the later fixes although I intend to tidy it up a bit when I get the time.

The new code has been refactored to make use of SDL as its graphics base. I have stripped out the ability to link program execution to objects. This used to run a DOS specific system call to spawn a separate program. Whilst this could be re-written I not entirely sure how useful this feature is so I have left it unimplemented for the time being. I would like to revisit how to make the environment interactive at some later stage.

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

For now I would prefer to stick with a software-based renderer although at some point it would be nice to explore switching to a proper hardware accelerated solution. This is quite low in my priorities for the time being.

# Major changes

This section will list the big changes that have been made since the project was imported as an old DOS-based project. These are roughly chronological in order and some earlier changes made be superceded by items further down the list.

* Replaced old BGI graphics code with SDL (including writing some code to generate solid-shaded triangles)
* Modified all C source files into C++ equivalents
* Combined all the parsing files into one single file
* Started applying a vector3d class where appropriate (e.g. some 3d maths calculations)
* Removed edge list from file description and made polygons purely triangles
* Tried to improve sorting of objects to use C++ standard library. We now sort objects based on the furthest corner of the shape
* As each object is drawn, its triangles are sorted by a midpoint value per triangle. This is to try and help with situation where back-face culling is not enough.
