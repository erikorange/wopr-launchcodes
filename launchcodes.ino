#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_AlphaNum4 alpha1 = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 alpha2 = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 alpha3 = Adafruit_AlphaNum4();

int NUM_LAUNCH_CODE_CHARS = 10;
int NUM_RANDOM_CHARS = 100;
int DELAY_BETWEEN_RANDOM_CHARS = 60;

struct launchCodeChar {
  uint8_t c;
  int pos;
  bool isSet;
};

struct launchCodeChar launchSequence[] = {
  {'4', 7, false},
  {'P', 1, false},
  {'1', 4, false},
  {'0', 6, false},
  {'S', 11, false},
  {'E', 2, false},
  {'7', 5, false},
  {'C', 0, false},
  {'K', 10, false},
  {'T', 9, false}
};

char msg1[] = "             GREETINGS PROFESSOR FALKEN                        ";
char msg2[] = "             A STRANGE GAME.  THE ONLY WINNING MOVE IS NOT TO PLAY.                        ";
char msg3[] = "             HOW ABOUT A NICE GAME OF CHESS?                        ";

void setup() {
  randomSeed(analogRead(0));
  initializeDisplay();
  clearDisplay();
}

void loop() {
    int i, j, k;

    for (i=0; i<NUM_LAUNCH_CODE_CHARS; i++) {             // outer loop to sequence through the eventual setting of all launch code characters

      for (j=0; j<NUM_RANDOM_CHARS; j++) {                // controls # of random characters to be displayed before setting the next launch code char

        for (k=0; k<NUM_LAUNCH_CODE_CHARS; k++) {         // display random characters - or not - at each launch code character position
          if (!launchSequence[k].isSet) {
            displayRandomCharacter(launchSequence[k].pos);
          }
        }
        delay(DELAY_BETWEEN_RANDOM_CHARS);
      }                 
      
      displayChar(launchSequence[i].pos, launchSequence[i].c);
      launchSequence[i].isSet = true;
    }

    // code has been cracked!
    delay(1000);
    blinkDisplay(true);
    delay(2500);
    blinkDisplay(false);
    delay(3000);
    clearDisplay();

    // show WOPR messages
    scrollMessage(msg1);
    scrollMessage(msg2);
    scrollMessage(msg3);

    // clear & start over
    resetLaunchCodeChars();
    delay(3000);
}

void scrollMessage(char msg[])
{
  int i,j;

  for (i=0; i<strlen(msg)-12; i++)
    {
      for (j=0; j<12; j++) {
        displayChar(j, msg[i+j]);
      }
      delay(100);
    }
}

void resetLaunchCodeChars()
{
  int i;

  for (i=0; i<NUM_LAUNCH_CODE_CHARS; i++) {
    launchSequence[i].isSet = false;    
  } 
}

void displayRandomCharacter(int pos)
{
  if ((pos>=3) && (pos<=6)) {
    displayChar(pos, random(48,57));      // display random numbers
  }
  else {
    displayChar(pos, random(65,90));      // display random characters
  }
}


void initializeDisplay()
{
  alpha1.begin(0x70);
  alpha2.begin(0x72);
  alpha3.begin(0x74);
}

void blinkDisplay(bool flag)
{
  if (flag) {
    alpha1.blinkRate(HT16K33_BLINK_2HZ);
    alpha2.blinkRate(HT16K33_BLINK_2HZ);
    alpha3.blinkRate(HT16K33_BLINK_2HZ);
  }
  else {
    alpha1.blinkRate(HT16K33_BLINK_OFF);
    alpha2.blinkRate(HT16K33_BLINK_OFF);
    alpha3.blinkRate(HT16K33_BLINK_OFF);
  }
}

void clearDisplay()
{
  alpha1.clear();
  alpha1.writeDisplay();
  alpha2.clear();
  alpha2.writeDisplay();
  alpha3.clear();
  alpha3.writeDisplay();
}

void displayChar(int pos, uint8_t c)
{
  if ((pos >= 0) && (pos < 4)) {
    alpha1.writeDigitAscii(pos, c);
    alpha1.writeDisplay();
  }
  else if ((pos >= 4) && (pos < 8)) {
    alpha2.writeDigitAscii(pos-4, c);
    alpha2.writeDisplay();
  }
  else if ((pos >= 8) && (pos < 12)) {
    alpha3.writeDigitAscii(pos-8, c);
    alpha3.writeDisplay();
  }
}