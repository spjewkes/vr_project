#ifndef __PCOMNDS_HPP__
#define __PCOMNDS_HPP__

#include "vector3d.hpp"

int process_location(Vector3d &loc);
int process_direction(Vector3d &ang);
int process_radius(float *radius);
int process_angle(float *angx, float *angy, float *angz);
int process_scale(float *sclx, float *scly, float *sclz);
int process_colour(int *colour);
int process_specularity(float *specularity);
char *process_outcome(void);
int process_style(int *style);
int process_verts(struct master *masterptr, int no_vertices, int object_no);
int process_edges(struct master *masterptr, int no_edges, int object_no);
int process_polys(struct master *masterptr, int no_polygons, int object_no);
int process_sky(int *colour);
int process_ground(int *colour);

#endif // __PCOMNDS_HPP__
