#include <iostream>
#include <cmath>

#include "SDL/SDL.h"

#include "Complex.hpp"

#include <omp.h>

using std::cout;
using std::endl;

const int W = 400;
const int H = 400;
const double INIT_X_RAN = 2.;
const double INIT_Y_RAN = 2.;
const double ZOOM_FACTOR = 2.;

Uint32 col = 0x0000DD;


void drawfract(Uint32* pixels, double cx, double cy, double xr, double yr) {
    int x, y, i;
    double rttwo = sqrt(2.);

/* This section of code is 'embarassingly parallel', meaning that
 * there is no dependency between loop iterations and as such, the
 * workload can be very easily divided between multiple computation
 * units. This loop is by far the most intensive component of the
 * program, so parallelising it yields huge speed advantages.
 *
 * OpenMP makes it very easy to instruct a compliant compiler to
 * generate an executable which will divide a workload amongst
 * multiple cores, although we have to let it know which variables
 * each processing unit will need its own copy of.
 *
 * It would be possible to write this loop to be executable on a GPU
 * by using a language called OpenCL which could potentially make this
 * step even faster.
 */
#pragma omp parallel for private (x,y,i)
    /* For each (x,y) co-ordinate, determine (within error
     * margins) whether it's in the Mandelbrot set, and set
     * the pixel colour accordingly.
     */
    for (y = 0; y < H; ++y) {
        Complex z;
        Complex c;
        for (x = 0; x < W; ++x) {
            z.r = 0.; z.i = 0.;
            c.r = cx + (2. * xr * (double) x / (double) W) - xr;
            c.i = cy + (2. * yr * (double) y / (double) W) - yr;

            /* This can be inaccurate, so we have to stop
             * computation arbitrarily, at which point we should
             * have a good idea of whether or not the point is
             * likely to be in the Mandelbrot set for most cases.
             */
            for (i = 0; i < 100; ++i) {
                z = z * z;
                z = z + c;

                /* The point has stayed close to the centre, and
                 * is likely to be in the Mandelbrot set.
                 */
                if (z.r > 2. || z.i > 2. || z.r < -2. || z.i < -2.) break;
            }

            if (z.r <= 2. && z.i <= 2. && z.r > -2. && z.i > -2.) {
                // In set - set pixel to a colour.
                pixels[x + y * W] = 0x0000FF;

                /* Makng the colour intensity a function of z.modulus()
                 * was an experiment which both had an interesting effect
                 * on the image and also made it look prettier :) .
                 */
                //pixels[x + y * W] = (z.modulus() / rttwo) * col + (0x0000FF - col);
            } else {
                // Not in set - set pixel to black.
                pixels[x + y * W] = 0;
            }
        }
    }
}


int main(int argc, char** argv) {
    bool quit = false;
    double cx = 0.;
    double cy = 0.;
    double xr = INIT_X_RAN;
    double yr = INIT_Y_RAN;
    bool gen = false;
    double rttwo = sqrt(2.);
    SDL_Surface* w;
    SDL_Event e;
    Uint32* pixels;

    /* The number of OpenMP threads to be used
     * can be set explicitly, however it will
     * automatically detect the best number of
     * threads to use on a given system.
     */
    //omp_set_num_threads(4);

    // Initialise SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    w = SDL_SetVideoMode(W, H, 32, SDL_SWSURFACE);
    SDL_WM_SetCaption("Mandelbrot Explorer", "Mandelbrot Explorer");

    // Obtain a reference to the raw pixel data
    pixels = (Uint32*) w->pixels;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_MOUSEBUTTONDOWN :
                    if (e.button.button == SDL_BUTTON_LEFT || e.button.button == SDL_BUTTON_RIGHT) {

                        /* Since the mouse was clicked, a new image needs to be generated.
                         * Use the 'gen' flag to indicate this,
                         */
                        gen = false;

                        /* Set the centre of focus, such that wherever the mouse
                         * was clicked will be at the centre of the next image.
                         */
                        cx += (((double) e.button.x / (double) W) - 0.5) * 2. * xr;
                        cy += (((double) e.button.y / (double) H) - 0.5) * 2. * yr;

                        /* Zoom in or out by adjusting the range of x and y
                         * which will be rendered in the next image.
                         */
                        if (e.button.button == SDL_BUTTON_LEFT) {
                            xr /= ZOOM_FACTOR;
                            yr /= ZOOM_FACTOR;
                        } else {
                            xr *= ZOOM_FACTOR;
                            yr *= ZOOM_FACTOR;
                        }
                    }
                    break;

                case SDL_KEYDOWN :
                case SDL_QUIT    :
                    quit = true;
                    break;
            }
        }

        /* If the user hasn't clicked anywhere, then
         * re-rendering the fractal is a waste of
         * computational power since the result will
         * be the same.
         */
        if (!gen) {

            // Draw the fractal
            SDL_LockSurface(w);
            drawfract(pixels, cx, cy, xr, yr);
            SDL_UnlockSurface(w);
            SDL_Flip(w);

            /* Reset the 'gen' flag so that
             * we don't re-render until after
             * the next click.
             */
            gen = true;
        } else SDL_Delay(5);

        SDL_Flip(w);
    }

    SDL_Quit();

    return 0;
}
