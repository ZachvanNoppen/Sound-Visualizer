/* 
 * Sound Visualizer
 * Authour: Zachary van Noppen
 * Purpose: To create a project that visualizes sound and allows users to control
 * colour values via serial communication. 
 * Date: April 8, 2019
 * 
 */

//Importing LED Library
#include "FastLED.h"

//Defining pins and LED Presets
#define MAX_LEDS 60
#define LED_PIN 6
#define COLOR_ORDER RGB
#define BUTTON_PIN 12

//Global Variables
int threshold;
int x;
int anPos = 0;
int state = 0;
String cProfile[6];
CRGB leds[MAX_LEDS];

//Setting up LEDS, pins and ambient room noise level
void setup() {
  Serial.begin(9600);
  //              Strip type, PIN, RGB or BGR,   name of array and size
  FastLED.addLeds<WS2812, LED_PIN, COLOR_ORDER>(leds, MAX_LEDS);
  FastLED.clear();
  //Getting the ambient noise level in the room
  threshold = analyzeRoom();
  x = 0;
  pinMode(BUTTON_PIN,INPUT);
}

void loop(){
  //Getting current time
  float sTime = millis();
  //Checking to see if there are colours to use from the processing sketched
  getColourProfile();
  //Determining what animation to play based on button presses
  if(anPos == 0){ 
    //Each animation takes the threshold and colours to be used as a parametre
    animationOne(threshold, cProfile);  
  }else if(anPos == 1){
    animationTwo(threshold, cProfile);
  }else if(anPos == 2){
    animationFive(threshold,cProfile);
  }
}
/* TEST ROUTINES
 *  
 *  ..
 *  
 *  
 *  ..
 */
void serialComsTestRoutine(){
  //Resetting the input value
  String input = "0";
  //Checking if there is data to be read
  if(Serial.available() > 0){
      input = Serial.readString();
      //Printing back the data read to make sure it is interpretted correctly
      Serial.println(input);
   } 
}

void ledTestRoutine(int r, int g, int b){
  //Takes a RGB value and lights up the leds
  for(int i = 0; i < MAX_LEDS; i++){
    leds[i] = CRGB(r,g,b);
    FastLED.show();
  }
}
void soundTestRoutine() {
  //Getting the input noise
  int x = analogRead(0);
  //Cutting it down to a more managable interval
  x = x/10;
  //Printing out a corresponding number of lines to sound
  for (int count = 0; count<x; count++){
    Serial.print('|');
  }
  Serial.println();
  delay(20);
}

void buttonTestRoutine(){
  //Checking if the button has been presses
  int buttonState = digitalRead(BUTTON_PIN);
  if(buttonState == HIGH){
    Serial.println("HIGH");
    return 0;
    //Leave
  }else{
    Serial.println("LOW");
  }
  delay(30);
}

/* 
 *  
 *  Project Functions
 *  
 *  ..
 */

int analyzeRoom(){
  //Adjusting the colour range so nothing below a certain sound level is detected
  int inc = 5;
  int x  = 0;
  //5 second to scan the room
  for (int y = 0; y < inc; y++){
    x  += analogRead(A0);
    delay(1000);
  }
  //Getting the average sound level and making a buffer for it
  return (x / inc)+10;
}
CRGB hexToRGB(String hexString){
  //hexstring formatted as such:
  //hexstring = "15E972";
  //Taking the string received over the serial port
  long number = (long) strtol(&hexString[0], NULL, 16);

  // Split them up into r, g, b values
  long r = number >> 16;
  long g = number >> 8 & 0xFF;
  long b = number & 0xFF;

  //This is formatted grb since the rgb colour space is only used by the LED strip
  return CRGB(g,r,b);
}
void getColourProfile(){
  //Resetting the input value
  String input = "0";
  //Checking if there is data to be read
  if(Serial.available() > 0){
      input = Serial.readString();
      //Split the data into two variables since we will recieve two Hex Codes from the COMPORT
      //Depending on the animation that is active, the colours will apply to the corresponding range
      if(anPos == 0){
        cProfile[0] = input.substring(0,6);      
        cProfile[1] = input.substring(7,13);
      }else if(anPos == 1){
        cProfile[2] = input.substring(0,6);      
        cProfile[3] = input.substring(7,13);
      }else{
        cProfile[4] = input.substring(0,6);      
        cProfile[5] = input.substring(7,13);
      }
   }
}
void sendPosition(){
    //Sending the current animation being altered.
    Serial.println(anPos);
}
/* Animations
 *  
 *  ..
 *  ..
 */
void animationOne(int threshold, String cProfile[6]){

  //Reading the input
  int x = analogRead(A0);
  //If sound is made
  if(x > threshold){
    //Cue animation
    //Getting the first colour
    leds[0] = hexToRGB(cProfile[0]);
    
    for(int i = 1; i < MAX_LEDS; i++){
      leds[i] = leds[i-1];
      FastLED.show();
      //Checking to see if the button has been pressed.
      //Note that the checks are not put in a function so that leaving the function can be done
      //efficiently
      if(digitalRead(BUTTON_PIN) == LOW){
          //Moving from animation position 0 to 1
          anPos = 1;
          //Telling the processing sketch that the animation has changed
          sendPosition();
          //Delaying so the button is not detected being pressed more than once
          delay(150);
          return 0;
      }
    } 
     FastLED.show();
  }else{ 
    //Turn off
    for(int i = 0; i < MAX_LEDS; i++){
      leds[i] = CRGB::Black;
      if(digitalRead(BUTTON_PIN) == LOW){
          //Moving from animation position 0 to 1
          anPos = 1;
          sendPosition();
          delay(150);
          return 0;
     }
    }
    FastLED.show();
  }
}
void animationTwo(int threshold, String cProfile[6]){
  //Reading the input
  int x = analogRead(A0);
  //If sound is made
  if(x > threshold){
    //Cue animation
    //Getting the first colour
    leds[0] = hexToRGB(cProfile[2]);
    
    for(int i = 1; i < MAX_LEDS; i++){
      leds[i] = leds[i-1];
      FastLED.show();
      //Checking to see if the button has been pressed.
      //Note that the checks are not put in a function so that leaving the function can be done
      //efficiently
      if(digitalRead(BUTTON_PIN) == LOW){
          //Moving from animation position 0 to 1
          anPos = 2;
          //Telling the processing sketch that the animation has changed
          sendPosition();
          //Delaying so the button is not detected being pressed more than once
          delay(150);
          return 0;
      }
    } 
     FastLED.show();
  }else{ 
    //Turn off
    for(int i = 0; i < MAX_LEDS; i++){
      leds[i] = CRGB::Black;
      if(digitalRead(BUTTON_PIN) == LOW){
          //Moving from animation position 0 to 1
          anPos = 2;
          sendPosition();
          delay(150);
          return 0;
     }
    }
    FastLED.show();
  }
}
void animationThree(int threshold, String cProfile[6]){
    //Getting the colours in the range
    CRGB startC = hexToRGB(cProfile[4]);
    CRGB endC = hexToRGB(cProfile[5]);
    //Making a gradient between those colours
    fill_gradient_RGB(leds, 0, startC, 59, endC);
    
    FastLED.show();
    if(digitalRead(BUTTON_PIN) == LOW){
          //Moving from animation position 0 to 1
          anPos = 0;
          sendPosition();
          delay(150);
          return 0;
    } 
}
