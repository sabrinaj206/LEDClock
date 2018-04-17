#include <Time.h>
#include <TimeLib.h>

#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip = Adafruit_NeoPixel(144, 6, NEO_GRB + NEO_KHZ800);
int seconds;
int mins;
int hours;
int longHand;
int shortHand;
int colorLTS [3];
int colorSTL [3];
int colorSTLBlend [3];
int colorLTSBlend [3];
int distLTS;
int distSTL;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  seconds = 0;
  mins = 0;
  hours = 0;
  longHand = 0;
  shortHand = 0;
  
  colorLTS[0] = 255;
  colorLTS[1] = 0;
  colorLTS[2] = 0;
  
  colorSTL[0] = 0;
  colorSTL[1] = 0;
  colorSTL[2] = 255;

  for(int i = 0; i < 3; i++){
    colorSTLBlend[i] = (colorSTL[i] * 1 + colorLTS[i] * 1) / 2;
  }
  for(int i = 0; i < 3; i++){
    colorLTSBlend[i] = (colorLTS[i] * 1 + colorSTL[i] * 1) / 2;
  }

  distLTS = 0;
  distSTL = 0;
  
  strip.begin();
  strip.setBrightness(10);
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  // put your main code here, to run repeatedly:

  //Get hour
  int realHours = hour(); 
  if (realHours != hours) {
    hours = realHours;
  }

  //Get minute
  int realMin = minute();
  if (realMin != mins) {
    mins = realMin;
  }

  //Get second
  int realSec = second();
  if (realSec != seconds) {
    seconds = realSec;

    //During each second, update long and short hands
    longHand = seconds * 2;
    shortHand = mins * 2 + seconds / 30;

    //Updates distance between the two
    distSTL = longHand > shortHand ? longHand - shortHand : 120 - shortHand + longHand;
    distLTS = 120 - distSTL;

    //update colors
    double iterations = 0;
    for (int i = shortHand; i != longHand; i = (i + 1) % 120) {
      //figure out color
      int color [3];
      for(int i = 0; i < 3; i++){
        color[i] = (int) (colorSTL[i] - colorSTLBlend[i]) * iterations / distSTL + colorSTLBlend[i];
      }
      
      
      strip.setPixelColor(i, color[0], color[1], color[2]);
      strip.show();
      iterations++;
    }

    iterations = 0;
    for (int i = longHand; i != shortHand; i = (i + 1) % 120) {
      int color [3];
      
      for(int i = 0; i < 3; i++){
        color[i] = (int) (colorLTS[i] - colorLTSBlend[i]) * iterations / distLTS + colorLTSBlend[i];
      }
      Serial.print(iterations);
      Serial.print(" ");
      Serial.print(distLTS);
      Serial.print(" ");
      Serial.println(iterations / distLTS);
      
      strip.setPixelColor(i, color[0], color[1], color[2]);
      strip.show();
      iterations++;
      }
  }
  delay(1);
}
