#Simple Mandelbrot Explorer

This is a small program to explore the [Mandelbrot set](https://en.wikipedia.org/wiki/Mandelbrot_set). Clicking with the left or right mouse button zooms in and out respectively, whilst changing the point in the Mandelbrot co-ordinate space which is rendered at the centre of the image.

The program only crudely approximates the Mandelbrot set by terminating after an arbitrary number of 'z^2 + c' iterations and defining an arbitrary boundary to decide whether each pixel should or should not be shaded. There are almost certainly better ways of doing this, however the program serves to illustrate the 'meaning' of this wonderful fractal and demonstrates a few interesting concepts such as C++ operator overloading and simple OpenMP parallelisation.

Note that resolution and accuracy diminishes after a certain level of zoom, since the calculations begin to require levels of precision which this program cannot handle. 

##Prerequisites
* [SDL](https://www.libsdl.org/) 1.2
* An [OpenMP](http://www.openmp.org)-compliant C++ compiler, such as g++ (part of the [GCC](https://gcc.gnu.org/) suite).

##Note on 'example-' repositories
None of my repositories beginning with 'example-' are intended to be useful pieces of software - rather, they exist to illustrate fun and interesting principles in a complete context. The purpose is simply to show how these sorts of things can be acheived, such that others might be able to deepen their understanding of certain algorithms or the utilities and techniques used to implement them. As such, the files are very thoroughly commented, ocasionally with tangential information such as justifications for design decisions etc. By the same token, I wrote these small programs to develop my own understanding in the first place, so if you can offer any constructuve criticism of my code, or if there's anywhere that I've done anything horifically inefficiently and there is a much simpler and clearer way of writing something then please let me know or throw a pull request my way - it's always greatly appreciated!
