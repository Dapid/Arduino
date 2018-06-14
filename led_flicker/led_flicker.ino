#include <math.h>


// These must be PMW leds:
int led9 = 9;
int led10 = 10;

void setup() {
  // declare pins 9 and 10 to be an output:
  pinMode(led9, OUTPUT);
  pinMode(led10, OUTPUT);

  // Debug info:
  #ifdef DEBUG
  Serial.begin(9600);
  #endif
}

void led_9(float x){
  float y;
  int brightness;
  y = 0.5 * (sin(x / 10) + sin(x / 2.4856));
  y = y * y;
  y = min(y, 1);

  brightness = round(255 * y);
  analogWrite(led9, brightness);

  #ifdef DEBUG
  Serial.print(y);
  Serial.print(F("\n"));
  #endif
}

void led_10(float x){
  float y;
  int brightness;
  y = 0.5 * (sin(-x / 10) + sin(-x / 2.4856));
  y = y * y;
  y = min(y, 1);

  brightness = round(255 * y);
  analogWrite(led10, brightness);
}

void loop() {
  float x;

  float t_step = millis();
  x = t_step / 100;

  // Set the flickering LEDs
  led_9(x);  
  led_10(x);

  // wait for 30 milliseconds
  delay(30);

}
