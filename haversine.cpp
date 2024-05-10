#include "haversine.h"

double to_radians(double angle)
{
    return angle*M_PI/180;
}

double haversine(double lat1, double lon1, double lat2, double lon2) {
    lat1 = to_radians(lat1);
    lon1 = to_radians(lon1);
    lat2 = to_radians(lat2);
    lon2 = to_radians(lon2);

    double delta_lat = lat2 - lat1;
    double delta_lon = lon2 - lon1;

    double aux = pow(sin(delta_lat/2),2) + cos(lat1) * cos(lat2) * pow(sin(delta_lon/2),2);
    double c = atan2(sqrt(aux), sqrt(1.0-aux));
    return 6371000 * c;
}

