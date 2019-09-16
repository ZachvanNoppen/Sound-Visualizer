#include "FastLED.h"

#define MAX_LEDS 60
#define LED_PIN 6
#define COLOR_ORDER RGB
#define BUTTON_PIN 12

int threshold;
int x;
int anPos = 0;
int state = 0;
CRGB leds[MAX_LEDS];
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  FastLED.addLeds<WS2812, LED_PIN, COLOR_ORDER>(leds, MAX_LEDS);
  FastLED.clear();
  FastLED.setBrightness(20);
  threshold = analyzeRoom();
  x= 0;
  pinMode(BUTTON_PIN,INPUT);
}

void loop(){
  //buttonTestRoutine();
  /*
  float sTime = millis();
  if(anPos == 0){
    animationOne(threshold);
  }else if(anPos == 1){
    animationTwo(threshold);
  }else if(anPos == 2){
    animationThree(threshold);
  }*/
  btTestRoutine();
  //getColour(sTime);
  //animationThree(threshold);
  //soundTestRoutine();
}
int analyzeRoom(){
  //Adjusting the colour range so nothing below a certain sound level is 
  //detected
  int inc = 50;
  int x  = 0;
  //1 a second to scan the room
  for (int y = 0; y < inc; y++){
    x  += analogRead(A0);
    delay(100);
  }
    Serial.print("Your room has been set up: ");
    Serial.println((x / 100)+10);
  //Getting the average sound level and making a buffer for it
  return (x / inc)+10;
  
}
void animationOne(int threshold){
  //Power issues keep this from working
  int x = analogRead(A0);
  if(x > threshold){
    //cue animation
    String s = "15E972";
    leds[0] = hexToRGB(s);
    
    for(int i = 1; i < MAX_LEDS; i++){
      leds[i] = leds[i-1];
      FastLED.show();
      if(digitalRead(BUTTON_PIN) == LOW){
          //Moving from animation position 0 to 1
          anPos = 1;
          delay(20);
          Serial.println(anPos);
          return 0;
      }
    } 
     FastLED.show();
  }else{
    
    //turnback to color
    for(int i = 0; i < MAX_LEDS; i++){
      leds[i] = CRGB::Black;
      if(digitalRead(BUTTON_PIN) == LOW){
          //Moving from animation position 0 to 1
          anPos = 1;
          delay(20);
          Serial.println(anPos);
          return 0;
     }
    }
    FastLED.show();
  }
}
void animationTwo(int threshold){
  //Power issues keep this from working
  int x = analogRead(A0);
  if(x > threshold){
    //cue animation
    String s = "15E972";
    leds[0] = CRGB::Yellow;
    
    for(int i = 1; i < MAX_LEDS; i++){
      leds[i] = leds[i-1];
      FastLED.show();
      if(digitalRead(BUTTON_PIN) == LOW){
          //Moving from animation position 0 to 1
          anPos = 2;
          delay(20);
          Serial.println(anPos);
          return 0;
      }
    } 
     FastLED.show();
  }else{
    
    //turnback to color
    for(int i = 0; i < MAX_LEDS; i++){
      leds[i] = CRGB::Black;
      if(digitalRead(BUTTON_PIN) == LOW){
          //Moving from animation position 0 to 1
          anPos = 2;
          delay(20);
          Serial.println(anPos);
          return 0;
     }
    }
    FastLED.show();
  }
}
void animationThree(int threshold){
    CRGB colours[] = {CRGB::Blue, CRGB::Green, CRGB::Red, CRGB::Yellow};
  
    leds[0] = colours[x];
    for(int j = 1; j < 20; j++){
      leds[j] = leds[j-1];
      FastLED.show();
      if(digitalRead(BUTTON_PIN) == LOW){
          //Moving from animation position 0 to 1
          anPos = 0;
          //Making sure it isn't pressed twice
          delay(20);
          return 0;
             //Leave
       }
    }
    
    if(analogRead(A0) > threshold){
      Serial.println(analogRead(A0));
      if(x < 3){
        x++;
      }else{
        x= 0;
      }
    }
}
CRGB hexToRGB(String hexString){
  //hexstring formatted as such:
  //hexstring = "15E972";
  long number = (long) strtol(&hexString[0], NULL, 16);

  // Split them up into r, g, b values
  long r = number >> 16;
  long g = number >> 8 & 0xFF;
  long b = number & 0xFF;
  
  return CRGB(r,g,b);
}
void ledTestRoutine(int r, int g, int b){
  for(int i = 0; i < MAX_LEDS; i++){
    leds[i] = CRGB(r,g,b);
    FastLED.show();
  }
}
void soundTestRoutine() {
  int x = analogRead(0);
  x = x/10;
  for (int count = 0; count<x; count++){
    Serial.print('|');
  }
  Serial.println();
  delay(20);
}
void btTestRoutine(){
  
   if(Serial.available() > 0){
      state = Serial.read();
   }
   if(state == '0'){ //Reading state
      Serial.println("State 0");
      ledTestRoutine(255,0,0);
   }else{
      Serial.println("State 1");
      ledTestRoutine(0,0,255);
      state = 0;
   }
}
void buttonTestRoutine(){
  int buttonState = digitalRead(BUTTON_PIN);
  if(buttonState == HIGH){
    //Moving from animation position 0 to 1
    Serial.println("HIGH");
    return 0;
    //Leave
  }else{
    Serial.println("LOW");
  }
  delay(30);
}
void getColour(int sTime){
  int cTime = millis();
  if(cTime - sTime > 500){
    sTime = cTime;
    String colourHex;
     if(Serial.available() > 0){
        colourHex = Serial.read();
        
     }
     Serial.print(colourHex);
     Serial.println(anPos);
  }
   
}
