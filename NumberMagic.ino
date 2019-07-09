// Input

#include <Keypad.h>

const byte rows = 4; //four rows
const byte cols = 4; //three columns
char keys[rows][cols] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};
byte rowPins[rows] = {9,8,7,6}; //connect to the row pinouts of the keypad
byte colPins[cols] = {5,4,3,2}; //connect to the column pinouts of the keypad
Keypad k = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

// Ouput

#include <LiquidCrystal.h>

#define LCD_RS 18
#define LCD_ENABLE 19
#define LCD_D4 14
#define LCD_D5 15
#define LCD_D6 16
#define LCD_D7 17

LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

#include <LedControl.h>

#define MAX_DATA 14
#define MAX_CLOCK 12
#define MAX_CS 13

LedControl lc(MAX_DATA, MAX_CLOCK, MAX_CS);

void setup() {
  int snake[][2] = {{0,4},{1,4},{2,4},{3,4},
                    {3,3},{3,2},
                    {3,1},{2,1},{1,1},{0,1},
                    {0,6},{0,5}};
  int snakeidx = 0;

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Appuie sur une");
  lcd.setCursor(0, 1);
  lcd.print("touche et joue");

  //wake up the MAX72XX from power-saving mode
  lc.shutdown(0,false);
  //set a medium brightness for the Leds
  lc.setIntensity(0,8);

  while (k.getKey() == NO_KEY) {
    lcd.setCursor(0, 1);
    // lcd.print(String("I") + snakeidx + " C" + snake[snakeidx][0] + " R" + snake[snakeidx][1] + "    ");
    lc.setLed(0, snake[snakeidx][0], snake[snakeidx][1], true);
    delay(200);
    lc.setLed(0, snake[snakeidx][0], snake[snakeidx][1], false);
    if (++snakeidx >= sizeof snake / sizeof snake[0])
      snakeidx = 0;
  }

  randomSeed(millis());
  lc.clearDisplay(0);

}

int a, b, tries, instart, inlen;
int st = 0;
char answer[16];
char ch;
int score = 0;

void show(int score, int tries) {
          lc.setDigit(0, 0, (score / 1000) % 10, false);
          lc.setDigit(0, 1, (score /  100) % 10, tries >= 3);
          lc.setDigit(0, 2, (score /   10) % 10, tries >= 2);
          lc.setDigit(0, 3, (score       ) % 10, tries >= 1);
}

void loop() {
  if (st == 0) {
    a = random(2, 9);
    b = random(2, 9);
    tries = 3;
    st = 1;
    show(score, tries);
  }
  if (st == 1) {
    lcd.clear();
    String problem = String("") + a + " x " + b + " = ";
    instart = problem.length();

    inlen = 0;
    lcd.setCursor(0, 1);
    lcd.print(problem + "          ");
    lcd.setCursor(instart, 1);
    lcd.cursor();
    st = 2;
  }

  if (st == 2 && (ch = k.getKey ()) != NO_KEY) {
    switch (ch) {
      case '#': {
        int result = 0, factor = 1;

        while (inlen) {
          result += (answer[--inlen] - '0') * factor;
          factor *= 10;
        }
        lcd.noCursor();

        if (result == a*b) {
          lcd.setCursor(0, 1);
          lcd.print("** Bravo! **   ");
          switch (tries) {
            case 3: score += 10; break;
            case 2: score += 5;  break;
            case 1: score += 2;  break;
          }
          st = 0;
        } else if (--tries > 0) {
          lcd.home();
          lcd.print("Essaie encore...");
          lcd.setCursor(0, 1);
          lcd.print(String("reste ") + tries + " essai" + (tries > 1 ? "s" : ""));
          st = 1;
        } else {
          lcd.home();
          lcd.print("La bonne reponse");
          lcd.setCursor(0, 1);
          lcd.print(String("etait ") + a*b + "          ");
          st = 0;
        }
        show(score, tries);
        delay(2000);
        break;
      }
      case 'D':
        if (inlen > 0) {
          inlen--;
          lcd.setCursor(instart + inlen, 1);
          lcd.print(' ');
          lcd.setCursor(instart + inlen, 1);
        }
        break;

      default:
        if (instart + inlen < 15) {
          answer[inlen++] = ch;
          lcd.print(ch);
        }
        break;
    }
  }
}
