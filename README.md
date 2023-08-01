# senior-thesis

This is the repository for all my senior thesis work, modified to be reviewed for MLH Fellowship Fall 2023.

## Thesis Motivation

The goal of my thesis is to write a program that can render photographs as stipples drawings like the [hedcuts in the Wall Street Journal](https://www.wsj.com/articles/whats-in-a-hedcut-depends-how-its-made-11576537243).These images are small, typically only three by five inches, but creating just one portrait can take an illustrator up to five hours to complete. In 2019 the WSJ had a team of only five artists who would spend all day generating these images. Although nothing can ever replace the look of a hand-drawn image, digital rendering can significantly speed up the process and make this art form more accessible.

## Process

The main paper followed for this work is the 2010 Kim et al. paper [Automated Hedcut Illustration Using Isophotes](https://link.springer.com/chapter/10.1007/978-3-642-13544-6_17). I diverged from this paper in a few ways, most notibly in the distance transform calculation for which I use the 2009 Felzenszwalb and Huttenlocher paper [Distance Transforms of Sampled Functons](https://cs.brown.edu/people/pfelzens/papers/dt-final.pdf).

## Mouse Selection Version

This is the most complete / advanced version of my tool and allows the user to select areas that they desire to have more detail. In the `Detail Selection` step the user selects regions where a greater density of stipples will be placed in the final rendering. Varying stipple density is a technique used by hedcut artists rendering images by hand. Often artists increase dot density around more complicated features such as eyes, noses, and lips. This expansion is an improvement upon other stippling techniques which assume a uniform density of stipples throughout the rendering.

To run this version:

```
> cd open_cv/render/
> mkdir build
> cd build
> cmake ..
> make render
> ./render ../demo_images/sydney_sweeny.png
```

## Code-Base Overview

This is a large code base with several components. All code in this repository was written exclusively by me. The main driver of the command line interface is contained in the `open_cv/render` folder. The `open_cv/render/render.cpp` file processes user inputs and moves through the steps for rendering. The rendering steps are the same as in the `main` branch with the addition of a mouse selection step.

Most changes to the code in this version happened in the `dots` directory. For example in `place-seeds.cpp` the areas the user has selected to have greater detail must have greater dot density, as reflected in this code:

```C++
if (details.at<uchar>(i, j) == 255) {
     // detailed sections are while
     s /= 3;
}
```

Since `1/s*s` is the probability that a dot will be placed at a given pixel decreasing `s` increases the dot density in that region.

### CLI Video

[![Watch the video](./images/thumbnail.png)](https://youtu.be/x-0Trv44cH0)
