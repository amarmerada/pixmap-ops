// Copyright 2021, Aline Normoyle, alinen

#include "image.h"

#include <cassert>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace agl {


Image::Image() {
    myWidth = 0;
    myHeight= 0;
}

Image::Image(int width, int height)  { 
    myWidth = width;
    myHeight = height;
    myData = new Pixel[width * height];
    numChannels = 3;
}

Image::Image(const Image& orig) { 
    int newHeight = orig.myHeight;
    int newWidth = orig.myWidth;
    Pixel* newData = orig.myData;
    myHeight = newHeight;
    myWidth = newWidth;
    myData = newData;
}

Image& Image::operator=(const Image& orig) {
  if (&orig == this) {
    return *this;
  }
  myData = orig.myData;
  myWidth = orig.myWidth;
  myHeight = orig.myHeight;
  numChannels = orig.numChannels;
  
  return *this;
}

Image::~Image() {
}

int Image::width() const { 
   return myWidth;
}

int Image::height() const {
   return myHeight;
}

char* Image::data() const {
   return (char*)myData;
}

void Image::set(int width, int height, unsigned char* data) {
    myWidth = width;
    myHeight = height;
    myData = (Pixel*)data;

}

bool Image::load(const std::string& filename, bool flip) {
    int x;
    int y;

    myData = (Pixel*) stbi_load(filename.c_str(), &x, &y, &numChannels, 3);
    if (myData != NULL) {
        myWidth = x;
        myHeight = y;
        return true;
    }
  return false;
}


bool Image::save(const std::string& filename, bool flip) const { 
    if (myData != NULL) {
        Pixel* savedImage = (Pixel*)(stbi_write_png(filename.c_str(), myWidth, myHeight, numChannels, myData, sizeof(struct Pixel)*myWidth));
        return true;
    }    
  return false;
}

Pixel Image::get(int row, int col) const {
    Pixel toGet = myData[(row * myHeight) + col];
    return toGet;


}

void Image::set(int row, int col, const Pixel& color) {
    myData[row * myWidth + col] = color;
    return;
}


Pixel Image::get(int i) const
{
    Pixel toGet = myData[i];
    return toGet;
}

void Image::set(int i, const Pixel& c)
{
    myData[i] = c;
}

Image Image::resize(int w, int h) const {
   int oldArray;
   float row1;
   float col1;
   Image result(w, h);
   for (int i = 0; i < w; i++) {
       for (int j = 0; j < h; j++) {
           row1 = (((float)myHeight - 1.0) * (float)j) / (float)(h - 1.0); 
           col1 = (((float)myWidth - 1.0) * (float)i) / (float)(w - 1.0); 
           oldArray = floor(row1) * myWidth + floor(col1); 
           Pixel toSet = get(oldArray);
           result.set((j*w + i), toSet); 
       }
   }
   return result;
}

Image Image::flipHorizontal() const {
   Image result(myWidth, myHeight);
   int row2;
   int col2;
   int oldArray;
   for (int i = 0; i < myWidth; i++) {
       for (int j = 0; j < myHeight; j++) { 
           row2 = myHeight - (j + 1);
           Pixel toSet = get(j*myWidth + i);
           result.set((row2 * myWidth + i), toSet);
       }
   }
   return result;

}

Image Image::flipVertical() const {
    Image result(myWidth, myHeight);
    int row2;
    int col2;
    int oldArray;
    for (int i = 0; i < myWidth; i++) { 
        for (int j = 0; j < myHeight; j++) { 
            col2 = myWidth - i; 
            Pixel toSet = get(j * myWidth + i);
            result.set((j * myWidth + col2), toSet);
        }
    }
    return result;
   return result;
}

Image Image::rotate90(char key) const {

   Image result(myHeight, myWidth);
   return result;
}

Image Image::subimage(int startx, int starty, int w, int h) const {
    Image sub(w, h);
    for (int i = starty; i < starty+ w; i++) {
        for (int j = startx; j < startx+h; j++) {
            Pixel toSet = get(j * myWidth + i);
            sub.set(((j - startx) * w + (i - starty)), toSet);
        }
    }
    return sub;
}

void Image::replace(const Image& image, int startx, int starty) {
    for (int i = starty; i < (starty + image.width()); i++) { 
        for (int j = startx; j < (startx + image.height()); j++) { 
            Pixel toSet = image.get((j - startx) * image.width() + (i - starty));
            set((j * myWidth + i), toSet);
        }
  }
}

Image Image::swirl() const {
   Image result(0, 0);
   return result;
}

Image Image::add(const Image& other) const {
   Image result(0, 0);
  
   return result;
}

Image Image::subtract(const Image& other) const {
   Image result(0, 0);
   
   return result;
}

Image Image::multiply(const Image& other) const {
   Image result(0, 0);
   
   return result;
}

Image Image::difference(const Image& other) const {
   Image result(0, 0);
  
   return result;
}

Image Image::lightest(const Image& other) const {
   Image result(0, 0);
  
   return result;
}

Image Image::darkest(const Image& other) const {
   Image result(0, 0);
  
   return result;
}

Image Image::gammaCorrect(float gamma) const {
    Image result(myWidth, myHeight);
    for (int i = 0; i < myWidth; i++) {
        for (int j = 0; j < myHeight; j++) {
            Pixel toSet = get((j * myWidth + i));
            float red = pow(((float)toSet.r / 255), (1 / gamma)); 
            float blue = pow(((float)toSet.b / 255), (1 / gamma));
            float green = pow(((float)toSet.g / 255), (1 / gamma));
            toSet.r = red * 255;
            toSet.b = blue * 255;
            toSet.g = green * 255;
            result.set((j * myWidth + i), toSet);
        }
    }
 
   return result;
}

Image Image::alphaBlend(const Image& other, float alpha) const {
   Image result(myWidth, myHeight); 
   for (int i = 0; i < myWidth; i++) { 
       for (int j = 0; j < myHeight; j++) { 
           Pixel thisImage = get(j * myWidth + i); 
           Pixel otherImage = other.get(j * myWidth + i);
           Pixel toSet; 
           toSet.r = thisImage.r * alpha + otherImage.r * (1.0 - alpha); 
           toSet.g = thisImage.g * alpha + otherImage.g * (1.0 - alpha);
           toSet.b = thisImage.b * alpha + otherImage.b * (1.0 - alpha);
           result.set((j * myWidth + i), toSet);
       }
   }


   return result;
}

Image Image::invert() const {
   Image image(0, 0);
   
   return image;
}

Image Image::grayscale() const {
    Image result(myWidth, myHeight);
    int avg;
    for (int i = 0; i < myWidth; i++) {
        for (int j = 0; j < myHeight; j++) {
            Pixel toSet = get(j * myWidth + i);
            avg = toSet.r + toSet.b + toSet.g;
            avg = avg / 3;
            toSet.r = avg;
            toSet.b = avg;
            toSet.g = avg;
            result.set((j * myWidth + i), toSet);
        }
    }
   return result;
}

Image Image::colorJitter(int size) const {
   Image image(0, 0);
  
   return image;
}

Image Image::bitmap(int size) const {
   Image image(0, 0);
   
   return image;
}

void Image::fill(const Pixel& c) {
  }

}  // namespace agl

