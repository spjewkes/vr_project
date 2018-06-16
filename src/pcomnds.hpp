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
int process_verts(struct master *masterptr, int no_vertices, int object_no);
int process_edges(struct master *masterptr, int no_edges, int object_no);
int process_polys(struct master *masterptr, int no_polygons, int object_no);
int process_sky(int *colour);
int process_ground(int *colour);

#endif // __PCOMNDS_HPP__
