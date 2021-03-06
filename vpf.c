#include <stdlib.h>
#include <math.h>
#include "vpf.h"
#include "mtwist.h"


/////////////////////////////////////////////////////////////////////////////
// This is the main function for calculating the Void Probability          //
// Function. It does so by randomly placing spheres in a box of galaxies.  //
// The sphere locations are decided using a pseudorandom number generator  //
// (PRNG) for which I used the Mersenne Twist code from                    //
// http://www.cs.hmc.edu/~geoff/mtwist.html                                //
//                                                                         //
// The box has periodic boundary conditions imposed, making it essentially //
// a 3-torus and there is an additional function to transform the galaxy   //
// locations for this purpose. The code returns an array of probabilities  //
// for a void being found at each radius length given as argument.         //
/////////////////////////////////////////////////////////////////////////////


/* *** Transformation functions for boundary conditions. *** */
double shift_calc(double coord, double rad) {
    /* This function calculates the overlap of the sphere */
    double shift;

    if (coord + rad >= 1.0)
        shift = coord + rad - 1.0;
    else if (coord - rad < 0.0)
        shift = coord - rad;
    else
        shift = 0.0;

    return shift;
}

double which_side(double coord) {
    /* This function checks which side of our box the transformed galaxy
       has overtrodden */
    if (coord < 0.0)
        coord += 1.0;
    else
        coord -= 1.0;

    return coord;
}

double shift_dist(double x_sph, double x_gal, double xshift,
                  double y_sph, double y_gal, double yshift,
                  double z_sph, double z_gal, double zshift) {
    /* This function calculates the distance from galaxy to sphere
       centre in the case of galaxies in the overlap region*/
    double new_x = x_gal - xshift;
    double new_y = y_gal - yshift;
    double new_z = z_gal - zshift;

    new_x = which_side(new_x);
    new_y = which_side(new_y);
    new_z = which_side(new_z);

    double dx = new_x - x_sph + xshift;
    double dy = new_y - y_sph + yshift;
    double dz = new_z - z_sph + zshift;

    return sqrt(dx * dx + dy * dy + dz * dz);
}



/* *** This is the void probability density calculating function *** */
void vpf(double *prob, double *r, double *x, double *y, double *z, int N_r, int N_gal) {

    /* seed the PRNG */
    mt_goodseed();

    int gal, sph, i;
    double x_sph, y_sph, z_sph;
    int count;
    double xshift, yshift, zshift;
    double dx, dy, dz;
    double dist;

    /* *** Looping over each sphere size *** */
    for (i = 0; i < N_r; i++) {

        /* Initialise count of empty spheres*/
        count = 0;

        /* *** 50 random sphere placements *** */
        for (sph = 0; sph < 100; sph++) {
            x_sph = mt_drand();
            y_sph = mt_drand();
            z_sph = mt_drand();

            /* Default count increase for each sphere,
               lost if a galaxy is found within */
            count += 1;

            /* Make a coordinate shift to galaxies if sphere
               goes over edges of box, imposing periodic
               boundary conditions on transformation. */
            xshift = shift_calc(x_sph, r[i]);
            yshift = shift_calc(y_sph, r[i]);
            zshift = shift_calc(z_sph, r[i]);

            /* *** Checking for galaxies *** */
            for (gal = 0; gal < N_gal; gal++) {
                /* First check if any coordinate is in overlap interval */
                if (x[gal] - xshift >= 1.0 || x[gal] - xshift < 0.0) {
                    /* Send to shifted distance calculation function */
                    dist = shift_dist(x_sph, x[gal], xshift,
                                      y_sph, y[gal], yshift,
                                      z_sph, z[gal], zshift);
                }
                else if (y[gal] - yshift >= 1.0 || y[gal] - yshift < 0.0) {
                    dist = shift_dist(x_sph, x[gal], xshift,
                                      y_sph, y[gal], yshift,
                                      z_sph, z[gal], zshift);
                }
                else if (z[gal] - zshift >= 1.0 || z[gal] - zshift < 0.0) {
                    dist = shift_dist(x_sph, x[gal], xshift,
                                      y_sph, y[gal], yshift,
                                      z_sph, z[gal], zshift);
                }
                /* Final case: just calculate distance */
                else {
                    dx = x[gal] - x_sph;
                    dy = y[gal] - y_sph;
                    dz = z[gal] - z_sph;
                    dist = sqrt(dx * dx + dy * dy + dz * dz);
                }

                /* Drop count if galaxy lies within sphere's radius */
                if (dist < r[i]) {
                    count -= 1;
                    break;
                }
            }
        }

        /* Normalise the empty sphere count to obtain probability */
        prob[i] = count * 0.01;
    }
}
