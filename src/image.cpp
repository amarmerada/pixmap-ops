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
    myData = new struct Pixel[width * height];
    numChannels = 3;
    stbi = false;
}

Image::Image(const Image& orig) { 

    myHeight = orig.myHeight;
    myWidth = orig.myWidth;
    myData = new struct Pixel[myWidth * myHeight];
    memcpy(myData, orig.myData, (sizeof(struct Pixel) * myWidth * myHeight));
    stbi = false;
}

Image& Image::operator=(const Image& orig) {
  if (&orig == this) {
    return *this;
  }

  freeImage();
  myWidth = orig.myWidth;
  myHeight = orig.myHeight;
  myData = new struct Pixel[myWidth * myHeight];
  memcpy(myData, orig.myData, (sizeof(struct Pixel) * myWidth * myHeight));
  stbi = false;
  return *this;
}

Image::~Image() {
    freeImage();
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
    freeImage();
    myData = new struct Pixel[myWidth * myHeight];
    memcpy(myData, data, (sizeof(struct Pixel) * myWidth * myHeight));

}

bool Image::load(const std::string& filename, bool flip) {
    int x;
    int y;

    myData = (Pixel*) stbi_load(filename.c_str(), &x, &y, &numChannels, 3);
    if (myData != NULL) {
        myWidth = x;
        myHeight = y;
        stbi = true;
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
    Pixel toGet = myData[(row * myWidth) + col];
    return toGet;


}

void Image::set(int row, int col, const Pixel& color) {
    myData[row * myWidth + col] = color;
    return;
}


Pixel Image::get(int i) const{
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
            col2 = myWidth - i -1; 
            Pixel toSet = get(j * myWidth + i);
            result.set((j * myWidth + col2), toSet);
        }
    }
   return result;
}

Image Image::rotate90() const {
   Image result(myHeight, myWidth);
   for (int i = 0; i < myWidth; i++) {
       for (int j = 0; j < myHeight; j++) {
           Pixel toSet = get(j * myWidth + i);
           result.set((i * myHeight + (myHeight -j-1)), toSet);
       }
   }
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

Image Image::swirl(int centerx, int centery, int radius, float rotations) const {
    double pi = 3.14159265358979323846;
    Image result(myWidth, myHeight);
    float intensity = 0;
    for (int i = 0; i < myWidth; i++) {
        for (int j = 0; j < myHeight; j++) {
            float Xval = (float)j - centerx;
            float Yval = (float)i - centery;
            float distance = pow((Xval * Xval + Yval * Yval), 0.5);
            float angle = atan2(Xval, Yval);
            intensity = 1.0f - (distance / radius);
            if (intensity > 0) {
                float twistAngle = rotations * intensity * pi * 2.0;
                angle = angle + twistAngle;
                Xval = cos(angle) * distance;
                Yval = sin(angle) * distance;
            }
            Pixel toGet = get(floor((centery + Yval)) * myWidth + floor((centerx + Xval)));
            result.set(i * myWidth + j, toGet);
        }
    }
    
   return result;
}

Image Image::add(const Image& other, int startx, int starty) const {
   Image result(myWidth, myHeight);
   for (int i = 0; i < myWidth; i++) {
       for (int j = 0; j < startx; j++) {
           Pixel largerSet = get(j * myWidth + i);
           result.set(j * myWidth + i, largerSet);
       }
   }
   for (int i = 0; i < myWidth; i++) {
       for (int j = other.height() + startx; j < myHeight; j++) {
           Pixel largerSet = get(j * myWidth + i);
           result.set(j * myWidth + i, largerSet);
       }
   }
   for (int i = 0; i < starty; i++) {
       for (int j = startx; j < startx + other.height(); j++) {
           Pixel largerSet = get(j * myWidth + i);
           result.set(j * myWidth + i, largerSet);
       }
   }
   for (int i = starty + other.width(); i < myWidth; i++) {
       for (int j = startx; j < startx + other.height(); j++) {
           Pixel largerSet = get(j * myWidth + i);
           result.set(j * myWidth + i, largerSet);
       }
   }
   for (int i = starty; i < other.width() + starty; i++) {
       for (int j = startx; j < other.height() + startx; j++) {
           Pixel getFirst = get(j * myWidth + i);
           Pixel getSecond = other.get((j - startx) * other.width() + (i-starty));
           Pixel toSet;
           toSet.r = getFirst.r + getSecond.r;
           if (toSet.r > 255) {
               toSet.r = 255;
           }
           toSet.g = getFirst.g + getSecond.g;
           if (toSet.g > 255) {
               toSet.g = 255;
           }
           toSet.b = getFirst.b + getSecond.b;
           if (toSet.b > 255) {
               toSet.b = 255;
           }
           result.set((j * myWidth + i), toSet);
       }
   }
   return result;
}

Image Image::levels() const {
    Image result(myWidth, myHeight);
    Pixel* pallet = new struct Pixel[8];
    pallet[0].r = 148;
    pallet[0].g = 0;
    pallet[0].b = 74;
    pallet[1].r = 173;
    pallet[1].g = 3;
    pallet[1].b = 171;
    pallet[2].r = 212;
    pallet[2].g = 2;
    pallet[2].b = 240;
    pallet[3].r = 196;
    pallet[3].g = 25;
    pallet[3].b = 252;
    pallet[4].r = 155;
    pallet[4].g = 82;
    pallet[4].b = 250;
    pallet[5].r = 124;
    pallet[5].g = 139;
    pallet[5].b = 252;
    pallet[6].r = 147;
    pallet[6].g = 197;
    pallet[6].b = 250;
    pallet[7].r = 195;
    pallet[7].g = 243;
    pallet[7].b = 250;
    for (int i = 0; i < myWidth; i++) {
        for (int j = 0; j < myHeight; j++) {
            Pixel toSet;
            int sum = 0;
            Pixel toGet = get(j * myWidth + i);
            sum += toGet.r;
            sum += toGet.g;
            sum += toGet.b;
           
            sum = sum / 3;
            int index = floor(sum / 32);
            toSet = pallet[index];
            result.set(j * myWidth + i, toSet);
        }
    }
    return result;
}



Image Image::blur() const {
    Image result(myWidth, myHeight);
    for (int i = 0; i < myWidth; i++) {
        result.set(i, get(i));
    }
    for (int i = 0; i < myWidth; i++) {
        result.set(myWidth - 1 * myWidth + i, get(myWidth - 1 * myWidth + i));
    }
    for (int j = 1; j < myHeight-1; j++) {
        result.set(j * myWidth, get(j * myWidth));
    }
    for (int j = 1; j < myHeight-1; j++) {
        result.set(j * myWidth + myHeight-1, get(j * myWidth + myHeight -1));
    }
    for (int i = 1; i < myWidth-1; i++) {
        for (int j = 1; j < myHeight-1; j++) {
            Pixel toSet;
            int sumr = 0;
            int sumg = 0;
            int sumb = 0;
            for (int x = i - 1; x < i + 2; x++) {
                for (int y = j - 1; y < j + 2; y++) {
                    Pixel toGet = get(y * myWidth + x);
                    sumr += toGet.r;
                    sumg += toGet.g;
                    sumb += toGet.b;
                }
            }
            toSet.r = sumr / 9;
            toSet.g = sumg / 9;
            toSet.b = sumb / 9;
            result.set(j * myWidth + i, toSet);
        }
    }
    return result;
}


Image Image::subtract(const Image& other, int startx, int starty) const {
    Image result(myWidth, myHeight);
    for (int i = 0; i < myWidth; i++) {
        for (int j = 0; j < startx; j++) {
            Pixel largerSet = get(j * myWidth + i);
            result.set(j * myWidth + i, largerSet);
        }
    }
    for (int i = 0; i < myWidth; i++) {
        for (int j = other.height() + startx; j < myHeight; j++) {
            Pixel largerSet = get(j * myWidth + i);
            result.set(j * myWidth + i, largerSet);
        }
    }
    for (int i = 0; i < starty; i++) {
        for (int j = startx; j < startx + other.height(); j++) {
            Pixel largerSet = get(j * myWidth + i);
            result.set(j * myWidth + i, largerSet);
        }
    }
    for (int i = starty + other.width(); i < myWidth; i++) {
        for (int j = startx; j < startx + other.height(); j++) {
            Pixel largerSet = get(j * myWidth + i);
            result.set(j * myWidth + i, largerSet);
        }
    }
    for (int i = starty; i < (other.width() + starty); i++) {
        for (int j = startx; j < other.height() + startx; j++) {
            Pixel getFirst = get(j * myWidth + i);
            Pixel getSecond = other.get((j-startx) * other.width() + (i - starty));
            Pixel toSet;
            int red = getFirst.r - getSecond.r;
            if (red < 0) {
                red = 0;
            }
            toSet.r = red;
            int green = getFirst.g - getSecond.g;
            if (green < 0) {
                green = 0;
            }
            toSet.g = green;
            int blue = getFirst.b - getSecond.b;
            if (blue < 0) {
                blue = 0;
            }
            toSet.b = blue;
            result.set((j * myWidth + i), toSet);
        }
    }
   return result;
}

Image Image::multiply(const Image& other) const {
    Image result(myWidth, myHeight);
    float r;
    float g;
    float b;
    for (int i = 0; i < myWidth; i++) {
        for (int j = 0; j < myHeight; j++) {
            Pixel getFirst = get(j * myWidth + i);
            Pixel getSecond = other.get(j * myWidth + i);
            Pixel toSet;
            r = (((float)getFirst.r/255.0)) * (((float)getSecond.r/255.0));
            toSet.r = floor(r * 255);
            g = (((float)getFirst.g/255.0)) * (((float)getSecond.g/255.0));
            toSet.g = floor(g * 255);
            b = (((float)getFirst.b / 255.0)) * (((float)getSecond.b / 255.0));
            toSet.b = floor(b* 255);
            result.set((j * myWidth + i), toSet);
        }
    }
   return result;
}

Image Image::difference(const Image& other) const {
   Image result(0, 0);
  
   return result;
}


Image Image::sineColor(int denom, float sinConst) const {
    Image result(myWidth, myHeight);
    for (int i = 0; i < myWidth; i++) {
        for (int j = 0; j < myHeight; j++) {
            Pixel toGet = get(j * myWidth + i);
            Pixel toSet;
            float red = toGet.r * 2.5; 
            float green = toGet.g * 0.6;
            float blue = toGet.b * 1.2;
            int r = floor(green);
            r = r/denom + floor(r * abs(sin(i * j * sinConst)));
            if (r > 230) {
                toSet.r = toGet.r;
            }
            else {
                toSet.r = r;
            }
            int g = floor(blue);
            g = g/denom + floor(g *abs(sin(i * j * sinConst)));
            if (g > 230) {
                toSet.g = toGet.g;
            }
            else {
                toSet.g = g;
            }
            int b = floor(red);
            b = b/denom + floor(b* abs(sin(i * j * sinConst)));
            if (b > 230) {
                toSet.b = toGet.b;
            }
            else {
                toSet.b = b;
            }
          
            result.set(j * myWidth + i, toSet);
        }
    }
    return result;
}

Image Image::lightest(const Image& other) const {
    Image result(myWidth, myHeight);
        int sum1;
        int sum2;
        for (int i = 0; i < myWidth; i++) {
            for (int j = 0; j < myHeight; j++) {
                Pixel getFirst = get(j * myWidth + i);
                Pixel getSecond = other.get(j * myWidth + i);
                sum1 = getFirst.r + getFirst.g + getFirst.b;
                sum2 = getSecond.r + getSecond.g + getSecond.b;
                Pixel toSet;
                if (sum1 >= sum2) {
                    toSet = getFirst;
                }
                else {
                    toSet = getSecond;
                }
                result.set(j*myWidth + i, toSet);
            }
        }
        return result;
}

Image Image::darkest(const Image& other) const {
    Image result(myWidth, myHeight);
    int sum1;
    int sum2;
    for (int i = 0; i < myWidth; i++) {
        for (int j = 0; j < myHeight; j++) {
            Pixel getFirst = get(j * myWidth + i);
            Pixel getSecond = other.get(j * myWidth + i);
            sum1 = getFirst.r + getFirst.g + getFirst.b;
            sum2 = getSecond.r + getSecond.g + getSecond.b;
            Pixel toSet;
            if (sum1 < sum2) {
                toSet = getFirst;
            }
            else {
                toSet = getSecond;
            }
            result.set(j*myWidth + i, toSet);
        }
    }
  
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
   Image image(myWidth, myHeight);
   for (int i = 0; i < myWidth; i++) {
       for (int j = 0; j < myHeight; j++) {
           Pixel toSet = get(j*myWidth+i);
           toSet.r = 255 - toSet.r;
           toSet.g = 255 - toSet.g;
           toSet.b = 255 - toSet.b;
           image.set(j * myWidth + i, toSet);
       }
   }
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
    Image image(myWidth, myHeight);
    for (int i = 0; i < myWidth; i++) {
        for (int j = 0; j < myHeight; j++) {
            Pixel toSet = get(j * myWidth + i);
            toSet.r = (toSet.r - size) + (rand() % (size * 2));
            if (toSet.r > 255) {
                toSet.r = 255;
            }
            if (toSet.r < 0) {
                toSet.r = 0;
            }
            toSet.g = (toSet.g - size) + (rand() % (size * 2));
            if (toSet.g > 255) {
                toSet.g = 255;
            }
            if (toSet.g < 0) {
                toSet.g = 0;
            }
            toSet.b = (toSet.b - size) + (rand() % (size * 2));
            if (toSet.b > 255) {
                toSet.b = 255;
            }
            if (toSet.b < 0) {
                toSet.b = 0;
            }
            image.set(j * myWidth + i, toSet);
        }
    }
  
   return image;
}


void Image::fill(const Pixel& c) {
    for (int i = 0; i < myWidth * myHeight; i++) {
        set(i, c);
    }
  }

void Image::freeImage() {
    if (myData != NULL) {
        if (stbi) {
            stbi_image_free(myData);
        }
        else {
            delete[] myData;
        }
    }
}

}  // namespace agl

