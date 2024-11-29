#include <iostream>
#include <ctime>
#include <cmath>

/*
 * Any variables mentioned in comments will be in [] square brackets for clarity.
 */


std::pair<double, long double> run(const double& time_increment) {
    // [time_increment] is in seconds; passed in

    // Constants:
    const double diameter = 13584.0 * 1000.0; // as given by the problem parameters, we convert km to m for calculation purposes
    const double radius = diameter / 2.0; // Radius is more useful to us for this problem, so we set a variable for it.

    // Surface gravity = 13.73N/kg = G * m1 / r^2, therefore:
    const double bigG_times_m1 = 13.73 * std::pow(radius, 2); // This variable is used to speed up calculations down the line.
    const double clock_initial = clock(); // to keep track of how long it takes to calculate everything

    double gravity; // Variable used during calculations
    long double time = 0; // in seconds, this is the output variable

    double prev_velocity = 0; // Used during calculations; allows for calculations to be based on the average velocity during an interval
    double velocity = 0; // We will assume that the probe is dropped at 0 velocity, rather than at any appreciable speed.
    // This will increase the total estimate time, as the probe will be slower at every time step compared to if it were launched.

    double distance = 2000.0 * 1000.0 + radius; // In meters. We add the radius (distance surface -> core) to the distance from the probe to the surface to find total initial distance.
    // We use this variable to keep track of progress.

    /*
     * We will assume no air friction or outside forces. This will make our estimate lower than actuality -- therefore the Vulcans should have more time than we predict.
     *
     * We step through time [time_increment] seconds per increment, and calculate for the velocity change from gravity, and the distance change from velocity.
     * Once distance is within the radius, we exit the loop with the resulting [time] variable.
     */
    while (distance > radius) {
        /* Gravity above surface:
         * g = G * m1 * m2 / (r^2) (In newtons)
         * Field force = G * m1 / (r^2) (in newtons/kg)
         * // We will assume no outside gravity sources. The effects of this aren't concrete -- they could either speed up or slow down the probe. This simply affects accuracy.
         */
        gravity = bigG_times_m1 / std::pow(distance, 2);
        /*
         * increase velocity by gravity times the increment of time we are using
         * v1 = v0 + at
         * v (m/s) += a (m/s^2) * t (s)
         */
        velocity += gravity * time_increment;
        /* Increase time by the increment:
         *  t (s) += t (s)
         */
        time += time_increment;
        /*
         * Δx = (v + v0)/2 * t
         * The distance traveled is approximated by the average velocity in some interval, multiplied by the length of that intervla.
         */
        distance -= ((velocity + prev_velocity) / 2) * time_increment;

        prev_velocity = velocity; // Set previous velocity to current end velocity
    }
    /*
     * we split the calculation into two parts: above the surface, calculated above, and below the surface.
     * We will assume gravity is linear while below the surface. This will increase out output time in relation to real time.
     * We use the same method as above to calculate for time.
     */
    while (distance > 0.0) {
        /*
         * Gravity at the surface is 13.73 N/kg (defined in the problem).
         * Gravity at the core is 0.
         * Thus, if we assume gravity to be linear, gravity must be surface gravity multiplied by the ratio distance : radius.
         */
        gravity = 13.73 * (distance / radius);
        /*
         * As above, v1 = v0 + at
         */
        velocity += gravity * time_increment;
        time += time_increment; // As above
        /*
         * As above.
         * Δx = (v + v0)/2 * t
         * The distance traveled is approximated by the average velocity in some interval, multiplied by the length of that interval.
         */
        distance -= ((velocity + prev_velocity) / 2) * time_increment;

        prev_velocity = velocity;
    }
    std::cout << "step:" << time_increment <<"s; result time: " << time << "s, or " << time / 60 << "mins" << " >>> [processing time:" << (clock() - clock_initial)/ CLOCKS_PER_SEC << "s]" << std::endl;
    return {((double) std::clock() - clock_initial) / CLOCKS_PER_SEC, time};
}


int main() {
    long long population = 7000000000; // As given by the problem definition; the total population
    float time_increment = 10000; // We start with a very wide grain, and then narrow
    double kill_time = 0.3; // In seconds. This is how much patience we (programmer) have
    std::pair<double, long double> output;
    // Loop until it takes more than [kill_time] seconds to calculate the next answer
    for (; output.first < kill_time;) {
        output = run(time_increment /= 10); // Calculate the time taken
    }
    std::cout << "end result: required average of " << population / (output.second) << " people per second, or " << population / ((output.second) / 60) << " people per minute";
    return 1;
}