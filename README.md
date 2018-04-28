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
