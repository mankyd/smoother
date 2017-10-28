// This #include statement was automatically added by the Particle IDE.
//#include <LiquidCrystal_I2C_Spark.h>


//Compatible with the Arduino IDE 1.0
//Library version:1.1
//#include <Wire.h>
#include "LiquidCrystal_I2C_Spark.h"
#include "Encoder.h"
#include "Stepper.h"

#define BUTTON_PIN D2
#define ROTARY_CLK D3
#define ROTARY_DT D4

#define STEPPER_A A0
#define STEPPER_B A1
#define STEPPER_C A2
#define STEPPER_D A3
#define STEPS_PER_REVOLUTION 2048
#define MAX_SPEED 8

int num_pushed = 0;
int lastButtonState = 0;
long encoderValue = 0;

LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display

Encoder encoder(ROTARY_CLK, ROTARY_DT);

Stepper stepper(STEPS_PER_REVOLUTION, STEPPER_A, STEPPER_B, STEPPER_C, STEPPER_D);

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Hello, world!");
  lcd.setCursor(2,1);
  lcd.print("awesome!");

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  stepper.setSpeed(MAX_SPEED);

}

void loop() {
  handleButton();
  handleEncoder();
}

void handleButton() {
  int buttonState = digitalRead(BUTTON_PIN) == LOW;

  if (buttonState != lastButtonState) {
    lcd.clear();
    if (buttonState == 0) {
      lcd.setCursor(2,1);
      char *output = NULL;
      size_t size = 0;
      output = asnprintf(output, &size, "released %d", num_pushed);
      if (output == NULL) {
        lcd.print("NULL");
      } else {
        lcd.print(output);
      }
      free(output);
    } else {
        lcd.setCursor(2,1);
        lcd.print("pushed!");
        stepper.step(STEPS_PER_REVOLUTION / 4);
        num_pushed++;
    }
    delay(100);
  }

  lastButtonState = buttonState;
}

void handleEncoder() {
  long value = encoder.read();
  if (value != 0) {
    stepper.step(value * 100);
    encoderValue += value;
    lcd.clear();
    lcd.setCursor(2,1);
    char *output = NULL;
    size_t size = 0;
    output = asnprintf(output, &size, "rotated %d", encoderValue);
    if (output == NULL) {
      lcd.print("NULL");
    } else {
      lcd.print(output);
    }
    free(output);
    encoder.write(0);
  }
}
