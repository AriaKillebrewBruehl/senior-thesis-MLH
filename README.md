# senior-thesis

This is the repository for all my senior thesis work.

## Thesis Motivation

The goal of my thesis is to write a program that can render photographs as stipples drawings like the [hedcuts in the Wall Street Journal](https://www.wsj.com/articles/whats-in-a-hedcut-depends-how-its-made-11576537243). These images are small, typically only three by five inches, but creating just one portrait can take an illustrator up to five hours to complete. In 2019 the WSJ had a team of only five artists who would spend all day generating these images. Although nothing can ever replace the look of a hand-drawn image, digital rendering can significantly speed up the process and make this art form more accessible.

## Process

The main paper I will be following for this work is the 2010 Kim et al. paper [Automated Hedcut Illustration Using Isophotes](https://link.springer.com/chapter/10.1007/978-3-642-13544-6_17). I have diverged from this paper in a few ways, most notibly in the edge detection process and the distance transform calculation for which I use the 2009 Felzenszwalb and Huttenlocher paper [Distance Transforms of Sampled Functons](https://cs.brown.edu/people/pfelzens/papers/dt-final.pdf).

## MLH Fellowship Code Example Version

This version of my command line interface is the most basic version of the program. I completed this version around February of 2023 and in the following two and a half months made several additions to the CLI to improve renderings. I am showcasing this version for the sake of simplicity. To see the most complete and final version switch to the `mouse-selection` branch.

Thi version is fully automated. The only user input is the input image. All parameters to edge detection, isophote detection, offset map generation, etc. are hardcoded using the parameters specified in the papers I followed.

To run the program:

    > cd open_cv/kitten-caboodle/
    > mkdir build
    > cd build
    > cmake ..
    > make caboodle
    > ./caboodle ../demo_images/sydney_sweeny.png

The program will run and print feedback in the terminal. Please note that the dot adjusting process (after initial dot placement) can be quite slow for large images.

This program can be run on any `.png` and it is preferred that the image is large (large than `750 px x 750 px`). Final renderings look best if the background is removed. This can be easily done using Apple's copy subject tool. Additionally it helps if the subject is well lit, has neat hair, is face on, and if there is contrast in the image.

## Code-Base Overview

All code for this project was written exclusively by me. There are six basic steps to this process:

1. Detect edges in input image.
2. Detect isophotes in input image.
3. Generate a distance map (tonal) and offset map (with lines) for input image to indicate a pixel's distance from the nearest edge / isophote.
4. Generate initial dot placement.
5. Adjust dot placement to align with edges and isophotes.
6. Size dots based on tone in input image.
