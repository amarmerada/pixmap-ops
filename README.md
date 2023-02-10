# pixmap-ops

Image manipulation demos based on the PPM image format.
![levels_art](https://user-images.githubusercontent.com/67133103/218204013-c326e0cb-87c8-4c8f-985e-0d1def1edd20.png)



## How to build

*Windows*

Open git bash to the directory containing this repository.

```
pixmap-ops $ mkdir build
pixmap-ops $ cd build
pixmap-ops/build $ cmake -G "Visual Studio 17 2022" ..
pixmap-ops/build $ start pixmap-ops.sln
```

Your solution file should contain two projects: `pixmap_art` and `pixmap_test`.
To run from the git bash command shell, 

```
pixmap-ops/build $ ../bin/Debug/pixmap_test
pixmap-ops/build $ ../bin/Debug/pixmap_art
```

*macOS*

Open terminal to the directory containing this repository.

```
pixmap-ops $ mkdir build
pixmap-ops $ cd build
pixmap-ops/build $ cmake ..
pixmap-ops/build $ make
```

To run each program from build, you would type

```
pixmap-ops/build $ ../bin/pixmap_test
pixmap-ops/build $ ../bin/pixmap_art
```

## Image operators

TODO: Document the features of your PPM image class here. Include example images.
I implemented a function that inverts a given image, a function that creates a swirl of given radius and rotations at given x and y coordinates, a function that puts an image through a box blur,  a function that distorts an image's color using a sine function, a function that splits an image into 8 different colors by brightness, functions that return the darkest and lightest pixels from two images, a function that jitters colors, and function that multiplies the colors of two images. 

## Results
![blur_art](https://user-images.githubusercontent.com/67133103/218204652-f490ecc8-7725-4cb9-9341-ef263521291b.png)
![darkest_art](https://user-images.githubusercontent.com/67133103/218204656-87d131a5-caca-4019-aff0-ac85a8c96eb4.png)
![invert_art](https://user-images.githubusercontent.com/67133103/218204707-f4ed8bdf-aa01-4301-8904-1cd5b756528c.png)
![jitter_art](https://user-images.githubusercontent.com/67133103/218204712-99266bad-6e12-4b5d-825b-5a9c12b270ae.png)
![levels_art](https://user-images.githubusercontent.com/67133103/218204724-204a6df8-fe26-44f4-84fe-26714fd87c73.png)
![swirl_art](https://user-images.githubusercontent.com/67133103/218204717-d347aa5f-1b18-4fa2-8e93-2b17481e60fe.png)
![sineColor2_art](https://user-images.githubusercontent.com/67133103/218204750-42825754-a573-4d1b-9b8a-48b203bfea7b.png)
![multiply_art](https://user-images.githubusercontent.com/67133103/218204767-38fad444-8277-4782-b2cf-9f0f495d9d13.png)
![lightest_art](https://user-images.githubusercontent.com/67133103/218204782-c1d9f1d5-c513-4138-aef4-1220cd15683c.png)
