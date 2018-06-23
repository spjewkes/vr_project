#ifndef __PCOMNDS_HPP__
#define __PCOMNDS_HPP__

#include <string>
#include "defs.hpp"
#include "vector3d.hpp"

int process_location(Vector3d &loc);
int process_direction(Vector3d &ang);
int process_radius(float *radius);
int process_angle(Vector3d &ang);
int process_scale(Vector3d &scl);
int process_colour(int *colour);
int process_specularity(float *specularity);
bool process_outcome(std::string &outcome);
int process_style(RenderStyle &style);
Status process_verts(master &mast, int no_vertices);
Status process_edges(master &mast, int no_edges);
Status process_polys(master &mast, int no_polygons);
int process_sky(int *colour);
int process_ground(int *colour);

#endif // __PCOMNDS_HPP__
