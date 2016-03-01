#include <Encoder.h>
#include "HID-Project.h"
Encoder myEnc(3,2);
const int pinLed = LED_BUILTIN;
const int pinButton = 12;
int bulbPin = 10;
int oldPosition  = -999;
boolean muted = 0;
int safePosition = 0;
int volume = 0;
int oldVolume = 0;
int actualVolume = 0;

int intervale = 1000;
bool clicked = false;
int userDelay=250; //delay for debouncing

bool debug = false; //set to true to view messages on serial monitor
void setup() {

  pinMode(pinLed, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(pinButton, INPUT_PULLUP);

  Serial.begin(9600);
  delay(1000);

  Consumer.begin();

  for(int i = 50; i > 0; i--){
    Consumer.write(MEDIA_VOLUME_DOWN);
    delay(1);
  }
}

void loop() {

  //Encoder rotation
  int newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    safePosition = newPosition;
    if(newPosition < 0){
      safePosition = 0;
      myEnc.write(safePosition);
    }
    if(newPosition > 400){
      safePosition = 400;
      myEnc.write(safePosition);
    }
    oldPosition = safePosition;
    volume = safePosition / 4;
    volume = volume / 2;
    noInterrupts();

    if (debug){
      Serial.print("calc volume: ");
      Serial.print(volume);
      Serial.print(" act volume: ");
      Serial.print(actualVolume);
      Serial.println("");
      Serial.println(map(volume, 0, 50, 0, 255));
    }
    
    changeVolume();
    interrupts();

  }

  //long && short click support
  unsigned long nextTime = millis();
  while(digitalRead(7)){
    clicked = true;
  }
  if(clicked){
    //The button has been clicked
    if(nextTime > millis() + intervale){
      //long click

    }else{
      //short click
      Consumer.write(MEDIA_VOLUME_MUTE);
      muted = !muted;
      delay(300);
    }
    delay(userDelay);
  }
  clicked=false;

  //Change led bright
  if(muted){
    analogWrite(bulbPin, 0);
  }
  else{
    analogWrite(bulbPin, map(volume, 0, 50, 0, 255));
  }

//  if (digitalRead(7)) {

//    Keyboard.press(KEY_LEFT_GUI);
 //   Keyboard.press('d');
 //   Keyboard.releaseAll();
 //   delay(300);
//  }
 //if (!digitalRead(8)) {
 // Keyboard.println("Thanks,");
  //Keyboard.println("");
  //Keyboard.println("David Watts");
   // Keyboard.println("Resident Nerd and Tinkerer");
  //  delay(300);
 // }

}

void changeVolume(){
  if (volume != oldVolume) {
      if(volume > oldVolume){
        //delay(100);
        Consumer.write(MEDIA_VOLUME_UP);
        actualVolume = actualVolume + 2;
        oldVolume = volume;
      }
      else{
        //delay(100);
        Consumer.write(MEDIA_VOLUME_DOWN);
        actualVolume = actualVolume - 2;
        oldVolume = volume;
      }
    }
}
