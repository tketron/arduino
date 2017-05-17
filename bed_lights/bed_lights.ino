/*****************************************************************************
Random Eyes sketch for WS2801 pixels
W. Earl 10/16/11
For Adafruit Industries

Creates randomized pairs of WS2801 led pixels that look like eyes peering 
from the darkness.

Blinking is implemented as an array of state machines so that multiple pairs
of eyes can be active concurrently, but in different phases of a blink.
*****************************************************************************/
#include "SPI.h"
#include "Adafruit_WS2801.h"


int dataPin = 2;      
int clockPin = 3;  

const int numPixels = 25;  // Change this if using more than one strand

const int numBreathingLights = 12; //Total number of "breathing" lights
const int numConstantLights = 6;

const int constantLightsPos[] = {1, 4, 9, 15, 21, 23};
const int breathingLightsPos[] = {0, 2, 3, 5, 10, 12, 13, 14, 19, 20, 22, 24};
const int lightsPos[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};

long lastStep;
const int stepInterval = 50;

Adafruit_WS2801 strip = Adafruit_WS2801(numPixels, dataPin, clockPin);

class light
{
  public:

  int m_pos; //position of light

  int m_red; //RBG components
  int m_green;
  int m_blue;

  int m_increment;
  int m_intensity;
  int m_minIntensity;
  int m_maxIntensity;

  public:
  light()
  {
        
  }

  public:
  light(int pos, int red, int green, int blue, int increment, int minIntensity, int maxIntensity)
  {
    m_pos = pos;
    m_red = red;
    m_green = green;
    m_blue = blue;
    m_increment = increment;
    m_intensity = 100;
    m_minIntensity = minIntensity;
    m_maxIntensity = maxIntensity;
  }

  // Create a 24 bit color value from R,G,B
  uint32_t Color(byte r, byte g, byte b)
  {
    uint32_t c;
    c = r;
    c <<= 8;
    c |= g;
    c <<= 8;
    c |= b;
    return c;
  }

  void StartLight() {
    UpdateLightColor();
  }

  void UpdateLightColor() {
    // Generate the color at the current intensity level
    int r =  map(m_red, 0, 255, 0, m_intensity);
    int g =  map(m_green, 0, 255, 0, m_intensity);
    int b =  map(m_blue, 0, 255, 0, m_intensity);
    uint32_t color = Color(r, g, b);
     
    // Write to light
    strip.setPixelColor(m_pos, color);  
  }

  void step()
  {
    m_intensity += m_increment;
    if (m_intensity >= m_maxIntensity)
    {
      m_increment = -m_increment;
      m_intensity += m_increment;
    }
    if (m_intensity <= m_minIntensity)
    {
      m_increment = -m_increment;
      m_intensity += m_increment;
    }
    UpdateLightColor();
  }
};

light breathingLights[numPixels];

void setup() 
{
  Serial.begin(9600);
  // initialize the strip
  strip.begin();
//  for (int i=0; i<numConstantLights; i++) {
//     strip.setPixelColor(constantLightsPos[i], random(200,255), random(0,10), random(0,10));
//  }
  for (int j=0; j<numPixels; j++) {
     light l = light(lightsPos[j], random(200,255), random(0,10), random(0, 10), random(1,5), random(10,80), random(150,200));
     breathingLights[j] = l;
     breathingLights[j].StartLight();
  }
  strip.show();
}


void loop()
{
  if (millis() - lastStep > stepInterval) 
  {
    lastStep = millis();
     for (int j=0; j<numPixels; j++) {
       breathingLights[j].step();
     }
    strip.show();
    //delay(10);
  }
}



