
#include <Adafruit_NeoPixel.h> // NeoPixel library https://www.arduino.cc/en/guide/libraries
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define BUTTON_PIN 4 // ATTiny85 pin 4 located left side 3rd from top
#define LED_PIN 1 // ATTiny85 pin 1 located right side 3rd from top
#define LED_COUNT 38 // Number of LEDs in the strip
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800); // Declare the strip

/*----------
Colour values with named colours for ease of use
when making your own you can change these colours as you like or make new ones.
----------*/
const auto red    =   Adafruit_NeoPixel::Color(255, 0, 0);
const auto orange =   Adafruit_NeoPixel::Color(235,45,0);
const auto yellow =   Adafruit_NeoPixel::Color(180,100,0);
const auto green  =   Adafruit_NeoPixel::Color(0,255,20);
const auto blue   =   Adafruit_NeoPixel::Color(0,40,255);
const auto purple =   Adafruit_NeoPixel::Color(180,30,180);
const auto white  =   Adafruit_NeoPixel::Color(50,50,50);
const auto black  =   Adafruit_NeoPixel::Color(0,0,0);

/*----------
Pride rainbow flag colours (from top to bottom)
red, orange, yellow, green, blue, purple
----------*/
int colourCycle = 0; // click through available colours
int cycleMax = 6; // maximum number of colours
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
      setStripColour(white);
      break;
      case 1:
      setStripColour(red);
      break;
      case 2:
      setStripColour(orange);
      break;
      case 3:
      setStripColour(yellow);
      break;
      case 4:
      setStripColour(green);
      break;
      case 5:
      setStripColour(blue);
      break;
      case 6:
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
