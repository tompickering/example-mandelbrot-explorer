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

#pragma omp parallel for private (x,y,i)
  for (y = 0; y < H; ++y) {
    Complex z;
    Complex c;
    for (x = 0; x < W; ++x) {
      z.r = 0.; z.i = 0.;
      c.r = cx + (2. * xr * (double) x / (double) W) - xr;
      c.i = cy + (2. * yr * (double) y / (double) W) - yr;

      for (i = 0; i < 100; ++i) {
	    z = z * z;
	    z = z + c;

	    if (z.r > 2. || z.i > 2. || z.r < -2. || z.i < -2.) break;
      }

      if (z.r <= 2. && z.i <= 2. && z.r > -2. && z.i > -2.) {
	    pixels[x + y * W] = (z.modulus() / rttwo) * col + (0x0000FF - col);
	    //pixels[x + y * W] = 0x0000FF;
	    //if (pixels[x + y * W] <= 0x000022) pixels[x + y * W] += 0x000022;
      } else {
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

  omp_set_num_threads(4);

  SDL_Init(SDL_INIT_EVERYTHING);
  w = SDL_SetVideoMode(W, H, 32, SDL_SWSURFACE);
  SDL_WM_SetCaption("Mandelbrot Explorer", "Mandelbrot Explorer");
  pixels = (Uint32*) w->pixels;

  while (!quit) {
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
	case SDL_MOUSEBUTTONDOWN :
	  if (e.button.button == SDL_BUTTON_LEFT || e.button.button == SDL_BUTTON_RIGHT) {
	    gen = false;
	    cx += (((double) e.button.x / (double) W) - 0.5) * 2. * xr;
	    cy += (((double) e.button.y / (double) H) - 0.5) * 2. * yr;

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

    if (!gen) {
      SDL_LockSurface(w);
      drawfract(pixels, cx, cy, xr, yr);
      SDL_UnlockSurface(w);
      SDL_Flip(w);
      gen = true;
    } else SDL_Delay(5);

    SDL_Flip(w);
  }

  SDL_Quit();

  return 0;
}
