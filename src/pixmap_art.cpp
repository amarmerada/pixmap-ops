#include <iostream>
#include "image.h"
using namespace std;
using namespace agl;

int main(int argc, char** argv)
{
	Image earth;
	earth.load("../images/earth.png");

	//1
	Image invert = earth.invert();
	invert.save("../images/invert_art.png");

	//2
	Image swirl = earth.swirl(100, 100, 50, 0.5);
	swirl = swirl.swirl(320, 120, 50, 1);
	swirl = swirl.swirl(90, 325, 50, 2);
	swirl = swirl.swirl(275, 275, 75, 1.5);
	swirl.save("../images/swirl_art.png");
	//3
	Image sineColor = earth.sineColor(4, 0.65);
	sineColor.save("../images/sineColor_art.png");

	Image sineColor2 = earth.sineColor(3, 0.23);
	sineColor2.save("../images/sineColor2_art.png");
	//4
	Image blur = earth.blur();
	blur = earth.blur();
	blur.save("../images/blur_art.png");
	//5
	Image multiply = earth.multiply(sineColor);
	multiply.save("../images/multiply_art.png");
	//6
	Image jitter = earth.colorJitter(60);
	jitter.save("../images/jitter_art.png");
	//7
	Image darkest = earth.darkest(invert);
	darkest.save("../images/darkest_art.png");
	//8
	Image lightest = earth.lightest(invert);
	lightest.save("../images/lightest_art.png");
	//9
	Image levels = earth.levels();
	levels.save("../images/levels_art.png");
   return 0;
}

