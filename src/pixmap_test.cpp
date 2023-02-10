// Copyright 2021, Aline Normoyle, alinen

#include <iostream>
#include "image.h"
using namespace std;
using namespace agl;

int main(int argc, char** argv)
{
   Image image;
   if (!image.load("../images/feep.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }

   for (int i = 0; i < image.height(); i++) {
      for (int j = 0; j < image.width(); j++) {
         Pixel c = image.get(i, j);
         std::cout << "(" << (int)c.r << "," << (int)c.g << "," << (int)c.b << ") ";
      }
      std::cout << std::endl;
   }
   image.save("feep-test-save.png"); // should match original
   
   // should print 4 4
   cout << "loaded feep: " << image.width() << " " << image.height() << endl;

   // test: copy constructor
   Image copy = image; 
   copy.save("feep-test-copy.png"); // should match original and load into gimp

   // test: assignment operator
   copy = image; 
   copy.save("feep-test-assignment.png"); // should match original and load into gimp

   // should print r,g,b
   Pixel pixel = image.get(1, 1);
   cout << (int) pixel.r << " " << (int) pixel.g << " " << (int) pixel.b << endl;

   // test: setting a color
   pixel.r = 255;
   image.set(1, 1, pixel);
   image.save("feep-test-newcolor.png");

   // test a non-trivial image
   if (!image.load("../images/earth.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }

   // should print 400 400
   cout << "loaded earth: " << image.width() << " " << image.height() << endl;


   // resize
   Image resize = image.resize(200,400);
   resize.save("earth-200-300.png");
   //printf("resized\n");


   Image rotate = resize.rotate90();
   rotate.save("earth_rotated.png");
   printf("rotated\n");

   

   // grayscale
   Image grayscale = image.grayscale(); 
   grayscale.save("earth-grayscale.png");
   //printf("grayscale\n");

   // flip horizontal
   Image flip = image.flipHorizontal(); 
   flip.save("earth-flip.png");
   //printf("flipped horizontally\n");

   Image flipV = image.flipVertical();
   flipV.save("earth-flipV.png");
   //printf("flipped vertically\n");

   // sub image
   Image sub = image.subimage(200, 200, 100, 100); 
   sub.save("earth-subimage.png");
   //printf("subimage\n");

    Image lightest = image.lightest(grayscale);
   lightest.save("lightest.png");
   printf("lightest\n");

   Image darkest = image.darkest(grayscale);
   darkest.save("darkest.png");
   printf("darket\n");

   // gamma correction
   Image gamma = image.gammaCorrect(0.6f); 
   gamma.save("earth-gamma-0.6.png");
   //printf("gamma Corrected\n");

   gamma = image.gammaCorrect(2.2f);
   gamma.save("earth-gamma-2.2.png");
   //printf("gamma corrected 2\n");

   // alpha blend
   Image soup;
   soup.load("../images/soup.png");

   int y = (int) (0.5f * (image.width() - soup.width()));
   int x = (int) (0.5f * (image.height() - soup.height()));
   Image background = image.subimage(x, y, soup.width(), soup.height());
   background.save("background-test.png");
   Image blend = background.alphaBlend(soup, 0.5f);
   image.replace(blend, x, y);
   image.save("earth-blend-0.5.png");
   //printf("alpha blended\n");

   Image earth;
   earth.load("../images/earth.png");

   Image subtracted = earth.subtract(soup, x, y);
   subtracted.save("earth_subtracted.png");
   printf("subtracted\n");

   Image added = earth.add(soup, x, y);
   added.save("earth_added.png");
   printf("readded\n");

   Image blur = earth.blur();
   blur.save("blurred_earth.png");
   printf("blurred\n");

   Image inverted = earth.invert();
   inverted.save("inverted_earth.png");
   printf("inverted\n");

   Image swirled = earth.swirl(200, 200, 100, 0.7);
   swirled.save("swirled_earth.png");
   printf("swirled earth\n");

   Image jittered = earth.colorJitter(80);
   jittered.save("jittered_earth.png");
   printf("jittered\n");

   Image threshhold = soup.blurThreshold(150);
   threshhold.save("threshhold_blur.png");
   printf("threshold\n");

   Image bricks;
   bricks.load("../images/bricks.png");
   Image swap = bricks.swap(16, 0.1);
   swap.save("swap.png");
   printf("swapped\n");

   


   Pixel purple;
   purple.r = 200;
   purple.g = 5;
   purple.b = 210;
   soup.fill(purple);
   soup.save("purple.png");
   printf("fill\n");

}

