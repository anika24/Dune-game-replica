#ifndef __POLYGON_H__
#define __POLYGON_H__

#include "list.h"
#include "vector.h"
#include "color.h"
#include <stddef.h>

/**
 * A polygon with a size (number of vertices), color (RGB), and vec_list of
 * coordinates for each vertex.
 *  This line does two things:
 * - Declares a "struct polygon" type
 * - Makes "polygon_t" an alias for "struct polygon"
 */
typedef struct polygon polygon_t;

/**
 * Allocates memory for a new polygon.
 *
 * @param v the vec_list_t of vertices of polygon
 * @param r amount of red color
 * @param g amount of green color
 * @param b amoung of blue color
 * @return a pointer to the newly allocated polygon
 */
polygon_t *polygon_init(list_t *v, float r, float g, float b,
                        double velocity_x);

/**
 * Gets velocity for polygon
 *
 * @param p polygon
 * @return a vector_t of velocity for polygon
 */
vector_t *polygon_get_velocity(polygon_t *p);

/**
 * Gets points for polygon
 *
 * @param p polygon
 * @return a list of points for allocated polygon
 */
list_t *polygon_get_points(polygon_t *p);

/**
 * Gets color for polygon
 *
 * @param p polygon
 * @return a rgb_color of points for allocated polygon
 */
rgb_color_t polygon_get_color(polygon_t *p);

/**
 * Computes the area of a polygon.
 * See https://en.wikipedia.org/wiki/Shoelace_formula#Statement.
 *
 * @param polygon the list of vertices that make up the polygon,
 * listed in a counterclockwise direction. There is an edge between
 * each pair of consecutive vertices, plus one between the first and last.
 * @return the area of the polygon
 */
double polygon_area(list_t *polygon);

/**
 * Computes the center of mass of a polygon.
 * See https://en.wikipedia.org/wiki/Centroid#Of_a_polygon.
 *
 * @param polygon the list of vertices that make up the polygon,
 * listed in a counterclockwise direction. There is an edge between
 * each pair of consecutive vertices, plus one between the first and last.
 * @return the centroid of the polygon
 */
vector_t polygon_centroid(list_t *polygon);

/**
 * Translates all vertices in a polygon by a given vector.
 * Note: mutates the original polygon.
 *
 * @param polygon the list of vertices that make up the polygon
 * @param translation the vector to add to each vertex's position
 */
void polygon_translate(list_t *polygon, vector_t translation);

/**
 * Rotates vertices in a polygon by a given angle about a given point.
 * Note: mutates the original polygon.
 *
 * @param polygon the list of vertices that make up the polygon
 * @param angle the angle to rotate the polygon, in radians.
 * A positive angle means counterclockwise.
 * @param point the point to rotate around
 */
void polygon_rotate(list_t *polygon, double angle, vector_t point);

/**
 * Releases the memory allocated for a polygon.
 * Also frees all vectors in the vec_list associated with a polygon.
 *
 * @param p a pointer to a polygon returned from polygon_init()
 */
void polygon_free(polygon_t *p);

#endif // #ifndef __POLYGON_H__
