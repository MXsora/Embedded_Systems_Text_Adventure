#include "Adafruit_ILI9341.h"
#include "Adafruit_GFX.h"
#include "SdFat.h"
#include "Adafruit_ImageReader.h"


#define SD_CS   8
#define TFT_DC 9
#define TFT_CS 10
#define TFT_MOSI 11
#define TFT_SCK 13
#define LEFTpin 7
#define RIGHTpin 5
#define OKpin 3
#define CANCELpin 2



SdFat SD;
Adafruit_ImageReader reader(SD);

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

File textFile;

enum states {start, intro, hub1, cave, altar, door, fog, lose, win};
states currentState;

bool smashSkull;
bool altarSkull;
bool hasKey;
bool hasSkull;

bool okPressed;
bool cancelPressed;
bool leftPressed;
bool rightPressed;

int okPrev;
int cancelPrev;
int leftPrev;
int rightPrev;

int okCurr;
int cancelCurr;
int leftCurr;
int rightCurr;

int choice;

void updateButtons() {
  okPressed = false;
  cancelPressed = false;
  leftPressed = false;
  rightPressed = false;
  okCurr = digitalRead(OKpin);
  cancelCurr = digitalRead(CANCELpin);
  leftCurr = digitalRead(LEFTpin);
  rightCurr = digitalRead(RIGHTpin);
  
  if (okCurr == LOW && okPrev == HIGH) {
    okPressed = true;
  }
  if (cancelCurr == LOW && cancelPrev == HIGH) {
    cancelPressed = true;
  }
  if (leftCurr == LOW && leftPrev == HIGH) {
    leftPressed = true;
  }
  if (rightCurr == LOW && rightPrev == HIGH) {
    rightPressed = true;
  }
    okPrev = okCurr;
    cancelPrev = cancelCurr;
    leftPrev = leftCurr;
    rightPrev = rightCurr;
}


String findLine(unsigned int lineNumber){
  textFile.seek(0);
  char cr;
  String returnString;  
  for(unsigned int i = 0; i < (lineNumber -1);){
    cr = textFile.read();
    if(cr == '\n'){
      i++;
    }
  }
  while(true){
    cr = textFile.read();
    returnString.concat(cr);
    if(cr == '\n'){
      break;
    }
  }
  return returnString;
}

void displayTextLine(String textString) {
  tft.setCursor(10, 145);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println(textString);
}

void displayChoices(String choices) {
  String token;
  int nextPos = choices.indexOf('/');
  int currPos = 0;
  int x = 0;
  for(int i = 0; i <= 2; i++) {
    token = choices.substring(currPos, nextPos);
    tft.setCursor(60 + x, 190);
    tft.println(token);
    x += 80;
    currPos = nextPos + 1;
    nextPos = choices.indexOf('/', currPos);
  }
}

int makeChoice() {
  int x = 0;
  int x1 = 84;
  int x2 = 92;
  int x3 = 88;
  int y1 = 180;
  int y2 = 188;
  tft.setCursor(80, 190);
  tft.fillTriangle(x1, y1, x2, y1, x3, y2, ILI9341_WHITE);
  while(true) {
    updateButtons();
    if(leftPressed == true) {
      x--;
      if(x <= -1) {
        x = 2;
      }
      tft.fillRect(0, 179, 320, 10, ILI9341_BLACK);
      if((x1 -= 80) < 20){x1 = 244;}
      if((x2 -= 80) < 20){x2 = 252;}
      if((x3 -= 80) < 20){x3 = 248;}
      tft.fillTriangle(x1, y1, x2, y1, x3, y2, ILI9341_WHITE);
    }
    if(rightPressed == true) {
      x++;
      x = x%3;
      tft.fillRect(0, 179, 320, 10, ILI9341_BLACK);
      if((x1 += 80) > 320){x1 = 84;}
      if((x2 += 80) > 320){x2 = 92;}
      if((x3 += 80) > 320){x3 = 88;}
      tft.fillTriangle(x1, y1, x2, y1, x3, y2, ILI9341_WHITE);
    }
    if(okPressed == true) {
      break;
    }
    if(cancelPressed == true) {
      x = -1;
      break;
    }
  }
  return x;
}

void clearTextBox() {
  tft.fillRect(0, 136, 320, 104, ILI9341_BLACK);
}

void waitForOK() {
  while(true) {
    updateButtons();
    if(okPressed) {
      break;
    }
  }
}





void setup() {
  
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  
 
  
  pinMode(SD_CS, OUTPUT);
  pinMode(OKpin, INPUT_PULLUP);
  pinMode(CANCELpin, INPUT_PULLUP);
  pinMode(LEFTpin, INPUT_PULLUP);
  pinMode(RIGHTpin, INPUT_PULLUP);

  SD.begin(SD_CS);

  textFile = SD.open("text.txt", FILE_READ);

  smashSkull = false;
  altarSkull = false;
  hasKey = false;
  hasSkull = false;

  okPrev = HIGH;
  cancelPrev = HIGH;
  leftPrev = HIGH;
  rightPrev = HIGH;

  currentState = start;
  
}




void loop() {

switch(currentState) {
    
      case start:
        tft.fillScreen(ILI9341_BLACK);
        //teach the player how to play
        tft.setCursor(10, 20);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(2);
        tft.println(findLine(44));
        waitForOK();
        //now actually begin the game
        tft.fillScreen(ILI9341_BLACK);
        reader.drawBMP("/start.bmp", tft, 0, 0);
        waitForOK();
        currentState = intro;
        break;
          
      case intro:
        tft.fillScreen(ILI9341_BLACK);
        reader.drawBMP("/txtbx.bmp", tft, 0, 123);
        reader.drawBMP("/intro1.bmp", tft, 0, 0);
        clearTextBox();
        displayTextLine(findLine(1));
        waitForOK();
        reader.drawBMP("/intro2.bmp", tft, 0, 0);
        clearTextBox();
        displayTextLine(findLine(2));
        waitForOK();
        clearTextBox();
        displayTextLine(findLine(3));
        waitForOK();
        currentState = hub1;
        break;
        
      case hub1:
        clearTextBox();
        reader.drawBMP("/hub1.bmp", tft, 0, 0);
        displayTextLine(findLine(5));
        waitForOK();
        clearTextBox();
        displayChoices(findLine(6));
        choice = makeChoice();
        if(choice == 0) {
          currentState = cave;
        }
        if(choice == 1) {
          currentState = altar;
        }
        if(choice == 2) {
          currentState = door;
        }
        break;
        
      case cave:
        clearTextBox();
        if(hasSkull == true || smashSkull == true) {
          reader.drawBMP("/cave2.bmp", tft, 0, 0);
        } else {
          reader.drawBMP("/cave1.bmp", tft, 0, 0);
        }
        displayTextLine(findLine(8));
        waitForOK();
        clearTextBox();
        displayChoices(findLine(9));
        choice = makeChoice();
        if(choice == 0) {
          clearTextBox();
          if(smashSkull == true || hasSkull == true) {
            displayTextLine(findLine(11));
            waitForOK();
          } else {
            displayTextLine(findLine(10));
            waitForOK();
          }
          
        }
        if(choice == 1) {
          clearTextBox();
          if(smashSkull == true || hasSkull == true) {
            displayTextLine(findLine(13));
            waitForOK();
          } else {
            reader.drawBMP("/cave2.bmp", tft, 0, 0);
            displayTextLine(findLine(12));
            smashSkull = true;
            hasKey = true;
            waitForOK();
          }
        }
        if(choice == 2) {
          clearTextBox();
          if(smashSkull == true || hasSkull == true) {
            displayTextLine(findLine(15));
            waitForOK();
          } else {
            reader.drawBMP("/cave2.bmp", tft, 0, 0);
            displayTextLine(findLine(14));
            hasSkull = true;
            waitForOK();
          }
        }
        if(choice == -1) {
          currentState = hub1;
        }
        break;
        
      case altar:
        clearTextBox();
        if(altarSkull == true) {
          reader.drawBMP("/altar2.bmp", tft, 0, 0);
        } else {
          reader.drawBMP("/altar1.bmp", tft, 0, 0);
        }
        displayTextLine(findLine(17));
        waitForOK();
        clearTextBox();
        displayChoices(findLine(18));
        choice = makeChoice();
        if(choice == 0) {
          clearTextBox();
          displayTextLine(findLine(19));
          waitForOK();
        }
        if(choice == 1) {
          clearTextBox();
          if(altarSkull == false) {
            displayTextLine(findLine(20));
            waitForOK();
            currentState = lose;
          }
          if(altarSkull == true && hasKey == false) {
            reader.drawBMP("/altar1.bmp", tft, 0, 0);
            displayTextLine(findLine(21));
            hasKey = true;
            altarSkull = false;
            smashSkull = true;
            waitForOK();
          }
        }
        if(choice == 2) {
          clearTextBox();
          if(hasSkull == false) {
            displayTextLine(findLine(22));
            waitForOK();
          }
          if(hasSkull == true) {
            reader.drawBMP("/altar2.bmp", tft, 0, 0);
            displayTextLine(findLine(23));
            altarSkull = true;
            hasSkull = false;
            waitForOK();
          }
        }
        if(choice == -1) {
          currentState = hub1;
        }
        break;
        
      case door:
        clearTextBox();
        reader.drawBMP("/door1.bmp", tft, 0, 0);
        displayTextLine(findLine(25));
        waitForOK();
        clearTextBox();
        displayChoices(findLine(26));
        choice = makeChoice();
        if(choice == 0) {
          clearTextBox();
          reader.drawBMP("/door2.bmp", tft, 0, 0);
          displayTextLine(findLine(27));
          waitForOK();
        }
        if(choice == 1) {
          clearTextBox();
          displayTextLine(findLine(28));
          waitForOK();
        }
        if(choice == 2) {
          clearTextBox();
          if(hasKey == false) {
            displayTextLine(findLine(29));
            waitForOK();
          }
          if(hasKey == true) {
            displayTextLine(findLine(30));
            waitForOK();
            clearTextBox();
            displayTextLine(findLine(31));
            waitForOK();
            clearTextBox();
            displayTextLine(findLine(32));
            waitForOK();
            currentState = fog;
          }
          
        }
        if(choice == -1) {
          currentState = hub1;
        }
        break;
        
      case fog:
        clearTextBox();
        reader.drawBMP("/fog.bmp", tft, 0, 0);
        displayTextLine(findLine(34));
        waitForOK();
        clearTextBox();
        displayChoices(findLine(35));
        choice = makeChoice();
        if(choice == 0) {
          clearTextBox();
          displayTextLine(findLine(36));
          waitForOK();
        }
        if(choice == 1) {
          clearTextBox();
          displayTextLine(findLine(37));
          waitForOK();
          currentState = lose;
        }
        if(choice == 2) {
          clearTextBox();
          displayTextLine(findLine(38));
          waitForOK();
          currentState = win;
        }
        break;
        
      case win:
        clearTextBox();
        reader.drawBMP("/win.bmp", tft, 0, 0);
        displayTextLine(findLine(40));
        waitForOK();
        clearTextBox();
        tft.fillRect(0, 0, 320, 122, ILI9341_BLACK);
        displayTextLine(findLine(41));
        waitForOK();
        clearTextBox();
        reader.drawBMP("/end.bmp", tft, 0, 0);
        displayTextLine(findLine(42));
        waitForOK();
        currentState = start;
        break;
        
      case lose:
        clearTextBox();
        reader.drawBMP("/lose.bmp", tft, 0, 0);
        displayTextLine("you have failed, embrace death");
        waitForOK();
        currentState = start;
        break;
  }
}
