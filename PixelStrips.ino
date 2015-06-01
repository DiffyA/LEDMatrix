// PixelStrips.ino

#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN 4 // Data wire.

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(20, PIN);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

 randomSeed(analogRead(0)); // Betters the randomness of random calls.
 strip.begin();
 strip.show(); // Initialize all pixels to 'off'
  
}

void loop() {
 // Some example procedures showing how to display to the pixels:
 // colorWipe(strip.Color(255, 0, 0), 50); // Red
 // colorWipe(strip.Color(0, 255, 0), 50); // Green
 // colorWipe(strip.Color(0, 0, 255), 50); // Blue
 // Send a theater pixel chase in...
 //  theaterChase(strip.Color(127, 127, 127), 50); // White
 //  theaterChase(strip.Color(127,   0,   0), 50); // Red
 //  theaterChase(strip.Color(  0,   0, 127), 50); // Blue
  
 // BUILT-IN ANIMATIONS

 //  rainbow(20);
 //  rainbowCycle(5);
 //  theaterChaseRainbow(75);

 // HOME-MADE ANIMATIONS
 // colorWipeRandom(200);
 // colorWipeRandomBrightness (100,15);
 // turnOnRandomColor(200, 10);
 // pulseEffect();
 // pulseEffectMultiple(7);
 }

// HOME-MADE ANIMATIONS


// Fill the pixels one after the other with a specified color.
// PARAMETERS
// color: Unsigned 32 bit integer, sets color that will be used to display on the pixels. Can be specified using strip.Color(x,y,z).
// wait: Unsigned 16 bit integer, sets the delay time taken in order to update the following pixel.
void colorWipe(uint32_t color, uint16_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }
}

// Fill the pixels one after the other with a random color.
// PARAMETERS
// wait: Unsigned 16 bit integer, sets the delay time taken in order to update the following pixel.
void colorWipeRandom (uint16_t wait) {
  for (uint16_t ii = 0; ii < strip.numPixels(); ii++) {
    byte colorR = random(0,150);
    byte colorG = random(0,150);
    byte colorB = random(0,150);
    strip.setPixelColor(ii, colorR, colorG, colorB);
    strip.show();
    delay(wait);
  }
}
// @OVERLOADED
// Fill the pixels one after the other with a random color and a maximum brightness value.
// PARAMETERS
// wait: Unsigned 16 bit integer, sets the delay time taken in order to update the following pixel.
// brightness: byte (0-255) value, sets the maximum value a single color can take.
void colorWipeRandom (uint16_t wait, byte brightness) {
  for (uint16_t ii = 0; ii < strip.numPixels(); ii++) {
    uint32_t colorR = random(0,brightness);
    uint32_t colorG = random(0,brightness);
    uint32_t colorB = random(0,brightness);
    strip.setPixelColor(ii, colorR, colorG, colorB);
    strip.show();
    delay(wait);
  }
}

// Fill the pixels all at once, same color, leaving a random amount of pixels that are not turned on.
// PARAMETERS
// wait: Unsigned 16 bit integer, sets the delay time taken when refreshing the strip.
// color: Unsigned 32 bit integer, sets color that will be used to display on the pixels. Can be specified using strip.Color(x,y,z).
void turnOnRandom (uint16_t wait, uint32_t color){
  for (uint16_t ii = 0; ii <  strip.numPixels(); ii++) {
   byte rand = random(0,2); // Maximum is exclusive.
   if (rand == 1) { // Random pixel picking process.
      strip.setPixelColor(ii, color);
      strip.show(); 
    }
  }
  for (uint16_t ii = 0; ii <  strip.numPixels(); ii++) { // Sets them all to 0 in order to start from scratch again.
      strip.setPixelColor(ii, 0);
    }
  delay(wait);
}

// Fill the pixels all at once, random color, leaving a random amount of pixels that are not turned on.
// PARAMETERS
// wait: Unsigned 16 bit integer, sets the delay time taken when refreshing the strip.
void turnOnRandomColor (uint16_t wait){
  for (uint16_t ii = 0; ii <  strip.numPixels(); ii++) {
   byte colorR = random(0,255);
   byte colorG = random(0,255);
   byte colorB = random(0,255);
   byte rand = random(0,2); // Maximum is exclusive.
   if (rand == 1) { // Random pixel picking process.
      strip.setPixelColor(ii, colorR, colorG, colorB);
      strip.show(); 
    }
  }
  for (uint16_t ii = 0; ii <  strip.numPixels(); ii++) { // Sets them all to 0 in order to start from scratch again.
    strip.setPixelColor(ii, 0);
  }
  delay(wait);
}

// @OVERLOADED
// Fill the pixels all at once, random color, with a maximum brightness value, leaving a random amount of pixels that are not turned on.
// PARAMETERS
// wait: Unsigned 16 bit integer, sets the delay time taken when refreshing the strip.
// brightness: byte (0-255) value used to set the maximum value that each individual color can take.
void turnOnRandomColor (uint16_t wait, byte brightness){
 for (uint16_t ii = 0; ii <  strip.numPixels(); ii++) {
   byte colorR = random(0,brightness);
   byte colorG = random(0,brightness);
   byte colorB = random(0,brightness);
   byte rand = random(0,2); //Maximum is exclusive
   if (rand == 1) {
    strip.setPixelColor(ii, colorR, colorG, colorB);
   strip.show(); 
   }
 }
 for (uint16_t ii = 0; ii <  strip.numPixels(); ii++) { // Sets them all to 0 in order to start from scratch again.
   strip.setPixelColor(ii, 0);
 }
 delay(wait);
}

// Pulsing effect on one random pixel, with a random color.
// PARAMETERS
// wait: Unsigned 16 bit integer, sets the delay time taken when refreshing the strip.
// hold: Unsigned 16 bit integer, sets the amount of time the pixel holds until it begins to fade.
void pulseEffect(uint16_t hold, uint16_t wait){
  uint16_t pos = random (0, strip.numPixels()); // Choose random pixel.
  byte colorR = random(0,255);
  byte colorG = random(0,255);
  byte colorB = random(0,255);
  strip.setPixelColor(pos, colorR, colorG, colorB);
  strip.show();
  delay(hold);
  while (strip.getPixelColor(pos) != 0){
    if (colorR > 0){
    colorR--;
    }
    if (colorG > 0){
    colorG--;
    }
    if (colorB > 0){
    colorB--;
    }
    strip.setPixelColor(pos, colorR, colorG, colorB);
    strip.show();
    delay(3);
  }
  delay(wait);
}

// Pulsing effect on a given amount of pixels, with random colors.
// PARAMETERS
// amount: Unsigned 32 bit integer, the number of pixels we want to light up.
// wait: Unsigned 16 bit integer, sets the delay time taken when refreshing the strip.
// hold: Unsigned 16 bit integer, sets the amount of time the pixel holds until it begins to fade.
void pulseEffectMultiple(uint32_t amount, uint16_t hold, uint16_t wait){
  uint32_t counter = 0; // Counter in order to know how many pixels we've already set.
  uint32_t positions [amount]; // Array that will store the positions of the pixels we have to set.
  uint32_t index = 0; // Array index.
  uint32_t ii = 0; // Index that goes through all the positions of our strip.
  strip.setBrightness(255); // Resets the brightness, because at the end of this method we set the brightness to 0. Brightness doesn't automatically set itself back, we must do it manually.
 
  while (counter < amount){ // Do this while we still have not reached the amount of pixels specified.
    boolean checker = true; // Lets us know if we should add the position to the array. Repeated positions are not allowed.
    int rand = random(0,200); // Max is exclusive.
    for (uint32_t jj = 0; jj < amount; jj++){ //Checks if the pixel being checked is already in the array or not.
      if (positions[jj] == ii){
        checker = false;
      }
    } 
  if (rand == 1 && checker){ // Random pixel picker. Only goes in if this pixel is not in the array.
    positions[index] = ii; // Adds this pixel to the array.
    index++; // Moves the array index down by one.
    counter++; // Update the amount of set pixels
    }
  ii++; // Moves the strip index down by one.
  if ( ii == strip.numPixels()){ // Checks if we reached the end of the string before allocating the amount of pixels specified, go back and start again (this is because we might have stopped at a pixel that is already set, but not set it, thus the counter didn't go up).
    ii = 0;
    }
  }

  for (uint32_t ii = 0; ii <= amount; ii++){ // Once all pixel positions are in our array, we give each one of them a random color.
    strip.setPixelColor(positions[ii], random(0,255), random(0,255), random(0,255));
  }
  strip.show();
  delay(hold);
  byte bright = strip.getBrightness();
  while (bright > 0){ // Lowers the strip brightness until 0.
    bright--;
    strip.setBrightness(bright);
    strip.show();
    delay(3);
  }
  delay(wait);
}

// BUILT-IN ANIMATIONS

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
