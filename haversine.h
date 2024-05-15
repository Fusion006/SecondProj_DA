#ifndef SECOND_PROJ_DA_HAVERSINE_H
#define SECOND_PROJ_DA_HAVERSINE_H

#include <cmath>
/**
 * Converts the parameter angle (in degrees) into radians.
 * @param angle double variable that represents the angle in degrees.
 * @return double variable that represents the angle in radians.
 */
double to_radians(double angle);

/**
 * Function calculates the accurate distance between two points on earth's surface.
 * Uses the haversine method make the result more accurate.
 * It calls the function into_radians to keep track of the earth's curvature.
 * @param lat1 latitude of the first point.
 * @param lon1 longitude of the first point.
 * @param lat2 latitude of the second point.
 * @param lon2 longitude of the second point.
 * @return value between to points on the earth's surface.
 */
double haversine(double lat1, double lon1, double lat2, double lon2);
#endif //SECOND_PROJ_DA_HAVERSINE_H
