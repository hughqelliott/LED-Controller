#include <Adafruit_NeoPixel.h> // NeoPixel library https://www.arduino.cc/en/guide/libraries
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define BUTTON_PIN 4 // ATTiny85 pin 4 located left side 3rd from top
#define LED_PIN 1 // ATTiny85 pin 1 located right side 3rd from top
#define LED_COUNT 60 // Number of LEDs in the strip
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800); // Declare the strip

/*----------
Colour values with named colours for ease of use
when making your own you can change these colours as you like or make new ones.
----------*/
static uint32_t red    =   Adafruit_NeoPixel::Color(255, 0, 0);
static uint32_t orange =   Adafruit_NeoPixel::Color(235,45,0);
static uint32_t yellow =   Adafruit_NeoPixel::Color(180,100,0);
static uint32_t green  =   Adafruit_NeoPixel::Color(0,255,20);
static uint32_t blue   =   Adafruit_NeoPixel::Color(0,40,255);
static uint32_t purple =   Adafruit_NeoPixel::Color(100,0,255);
static uint32_t white  =   Adafruit_NeoPixel::Color(50,50,50);
static uint32_t black  =   Adafruit_NeoPixel::Color(0,0,0);

/*----------
Pride rainbow flag colours (from top to bottom)
red, orange, yellow, green, blue, purple
----------*/
int upsideDown = 1; // refers to which 'direction' the rainbow will be. 
                    // 1 had red closest to the handle, 0 purple is closest to the handle
int colourCycle = 0; // click through available colours
int cycleMax = 7; // maximum number of colours
unsigned long buttonWaitAmnt = 0; // time in milliseconds to wait to check the loop

unsigned long buttonAmnt = 100; // delay in milliseconds to add to the check

void setup() {
  delay(1000); // give the ATTiny85 time to power up
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1); //the ATTiny85 clock is 16mHz
#endif
  delay(1000); // one more second wait
  setButtonPause(); //initialize the check
  pinMode(BUTTON_PIN,INPUT);  // set the tactile switch pin to input. 
                              // This pin has a 10K pulldown resistor connected to ground. 
                              // The switch is connected to +5V between the resistor and pin.
                              // Switch pulls the pin HIGH when clicked.
  strip.begin(); // initialize the NeoPixel strip
  setRainbow(); // divide the strip and set the pixel colours
  setStripColour(white); // solid colour to entire strip with a short delay
  delay(100);
  setStripColour(red);
  delay(100);
  setStripColour(orange);
  delay(100);
  setStripColour(yellow);
  delay(100);
  setStripColour(green);
  delay(100);
  setStripColour(blue);
  delay(100);
  setStripColour(purple);
  delay(100);
  setStripColour(black); // turn off the strip
}

int divisible = 0; // number of pixels divided by 6. Evenly divide for the rainbow
/*----------
setRainbow function - no arguments passed
  evenly divide the strip by 6
  check if the rainbow is "upside down" or not
  set the 6 colours
  turn on the strip
----------*/
void setRainbow(){
  divisible = LED_COUNT/6;
    int i=0;
    if(upsideDown == 0){
    for(i=0; i<divisible; i++) {
      strip.setPixelColor(i,purple);
    }
    for(i=divisible; i<divisible*2; i++) {
      strip.setPixelColor(i,blue);
    }
    for(i=divisible*2; i<divisible*3; i++) {
      strip.setPixelColor(i,green);
    }
    for(i=divisible*3; i<divisible*4; i++) {
      strip.setPixelColor(i,yellow);
    }
    for(i=divisible*4; i<divisible*5; i++) {
      strip.setPixelColor(i,orange);
    }
    for(i=divisible*5; i<divisible*6; i++) {
     strip.setPixelColor(i,red);
    }
    }else{
    for(i=0; i<divisible; i++) {
      strip.setPixelColor(i,red);
    }
    for(i=divisible; i<divisible*2; i++) {
      strip.setPixelColor(i,orange);
    }
    for(i=divisible*2; i<divisible*3; i++) {
      strip.setPixelColor(i,yellow);
    }
    for(i=divisible*3; i<divisible*4; i++) {
      strip.setPixelColor(i,green);
    }
    for(i=divisible*4; i<divisible*5; i++) {
      strip.setPixelColor(i,blue);
    }
    for(i=divisible*5; i<divisible*6; i++) {
     strip.setPixelColor(i,purple);
    }
    }
    strip.show();
}
/*----------
setButtonPause function - no arguments passed
  reset the wait time buttonWaitAmnt, in milliseconds millis()
  by adding the delay buttonAmnt
----------*/
void setButtonPause(){
    buttonWaitAmnt = millis() + buttonAmnt;
}
/*----------
setStripColour function - colour argument in plain text as long as declared above
  sets the colour for the entire strip
  otherwise can call with a different colour using strip.color(r,g,b); 
  r,g,b values between 0-255
----------*/
void setStripColour(uint32_t colour){
    for(int i=0; i<LED_COUNT; i++) {
    strip.setPixelColor(i, colour);
    }
    strip.show();
}
boolean checkState = false; // switch on off if off set to false if on set to true
void loop() {
    if(millis() > buttonWaitAmnt){ // check wait
  int buttonState = digitalRead(BUTTON_PIN); // get switch state
  if(buttonState == HIGH && !checkState){ // if switch pressed and we didn't know it was pressed
    checkState = true; // we know the switch is pressed. avoids double calls
    switch(colourCycle){
      case 0:
      setRainbow(); // set the rainbow, turn on strip
      break;
      case 1:
      setStripColour(white);
      break;
      case 2:
      setStripColour(red);
      break;
      case 3:
      setStripColour(orange);
      break;
      case 4:
      setStripColour(yellow);
      break;
      case 5:
      setStripColour(green);
      break;
      case 6:
      setStripColour(blue);
      break;
      case 7:
      setStripColour(purple);
      break;
    }
    if(colourCycle<cycleMax){
      colourCycle++;
    }else{
      colourCycle = 0;
    }
  }else if(buttonState == LOW && checkState){ // if switch released and we didn't know
    checkState = false; // we know the switch is released. avoids double calls
  setStripColour(black); // turn off strip
  }
  setButtonPause(); // reset the timer
    }
}
