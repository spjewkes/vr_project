#ifndef __PCOMNDS_H__
#define __PCOMNDS_H__

int process_location(float *locx, float *locy, float *locz);
int process_direction(float *angx, float *angy, float *angz);
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

#endif // __PCOMNDS_H__
