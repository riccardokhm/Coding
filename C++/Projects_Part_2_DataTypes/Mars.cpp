// Computing the time necessary to communicate with a person on Mars 
// when Mars is at its closest approach to Earth.

#include <iostream>
#include <cmath>

using namespace std;

int main()
{
    const double speed_of_light_km_per_s = 299792.458; // Speed of light in km/s
    const double distance_earth_to_mars_km = 54600000; // Closest distance from Earth to Mars in km

    // Calculate the time in seconds
    double time_seconds = distance_earth_to_mars_km / speed_of_light_km_per_s;
    double time_min = time_seconds / 60.0;

    cout << "Time to communicate with Mars at closest approach: " 
         << time_seconds << " seconds." << endl;
    cout << "Time to communicate with Mars at closest approach: " 
         << time_min << " minutes." << endl;
    cout << "A bidirectional communication would take: " 
         << time_seconds * 2 << " seconds." << endl;
    return 0;

}