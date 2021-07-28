/*
  Character analysis operators

  Examples using the character analysis operators.
  Send any byte and the sketch will tell you about it.

  created 29 Nov 2010
  modified 2 Apr 2012
  by Tom Igoe
  modified 18 Jul 2018
  by Matthew H. McKenzie for ATTiny85

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/CharacterAnalysis
*/

#if defined (__AVR_HAVE_JMP_CALL__)
  #define VECT_0 asm("jmp 0");
#else
  #define VECT_0 asm("rjmp 0");
  #undef  LED_BUILTIN
  #define LED_BUILTIN 2 /* 0 is Tx now */
#endif

unsigned short i;

void setup() {
  // set up pins
  pinMode(LED_BUILTIN, OUTPUT);
  // Open serial communications
  Serial.begin(9600);
  preamble();
}


void loop() {

  char buffer[9];

  serialCheck();

  // get any incoming bytes:
  if (Serial.available() > 0) {
    int thisChar = Serial.read();
    /* test for Ctrl-C and reboot if sent */
    if (thisChar == 3) {
      SP    = RAMEND;
      SREG  = 0;
      MCUSR = 0;
      VECT_0
    }

    // say what was sent: flash LED_BUILTIN to show we are thinking really hard about it....
    digitalWrite(LED_BUILTIN, HIGH);

    Serial.print(F("You sent me  : \'"));
    Serial.write(thisChar);
    Serial.print(F("\'"));
    Serial.println();

    Serial.print(F("ASCII Value  : "));
    Serial.println(thisChar);

    Serial.print(F("Hex Value    : "));
    sprintf(buffer, "%x", thisChar);
    Serial.println(buffer);

    Serial.print(F("Octal Value  : "));
    sprintf(buffer, "%#o", thisChar);
    Serial.println(buffer);

    Serial.print(F("Binary Value : "));
    itoa(thisChar, buffer, 2);
    for (i = 0; i < (8 - strlen(buffer)); i++) {
      Serial.print(F("0"));
    }
    Serial.println(buffer);

    Serial.print(F("UART Binary  : "));
    strrev(buffer);
    Serial.print(buffer);
    for (i = 0; i < (8 - strlen(buffer)); i++) {
      Serial.print(F("0"));
    }
    Serial.println();
    Serial.println();

    // analyze what was sent:
    if (isAlphaNumeric(thisChar)) {
      Serial.println(F("It's alphanumeric"));
    }
    if (isAlpha(thisChar)) {
      Serial.println(F("It's alphabetic"));
    }
    if (isAscii(thisChar)) {
      Serial.println(F("It's ASCII"));
    }
    if (isWhitespace(thisChar)) {
      Serial.println(F("It's whitespace"));
    }
    if (isControl(thisChar)) {
      Serial.println(F("It's a control character"));
    }
    if (isDigit(thisChar)) {
      Serial.println(F("It's a numeric digit"));
    }
    if (isGraph(thisChar)) {
      Serial.println(F("It's a printable character that's not whitespace"));
    }
    if (isLowerCase(thisChar)) {
      Serial.println(F("It's lower case"));
    }
    if (isPrintable(thisChar)) {
      Serial.println(F("It's printable"));
    }
    if (isPunct(thisChar)) {
      Serial.println(F("It's punctuation"));
    }
    if (isSpace(thisChar)) {
      Serial.println(F("It's a space character"));
    }
    if (isUpperCase(thisChar)) {
      Serial.println(F("It's upper case"));
    }
    if (isHexadecimalDigit(thisChar)) {
      Serial.println(F("It's a valid hexadecimal digit (i.e. 0 - 9, a - f, or A - F)"));
    }
    digitalWrite(LED_BUILTIN, LOW);

    // add some space and ask for another byte:
    Serial.println();
    Serial.println(F("Give me another byte :"));
    Serial.println();
  } /* if serial available */
}


void preamble() {
  serialCheck();
  // scroll to clear screen
  for (i = 0; i < 50; i++) {
    Serial.println();
  }
  //home the cursor (VT100)
  Serial.write(27);
  Serial.print(F("[H"));

  // send an intro:
  Serial.println(F("Evil Screaming Pig Factory Character Analysis."));
  Serial.println(F("==============================================\n"));
  Serial.println(F("Send any byte and I'll tell you everything I can about it."));
  Serial.println();
}


/* 
 *  Wait for serial port, but also for incoming chars to buffer
 *  in case users press keys producing a sequence. Ideally we
 *  are honour bound to try and keep it half duplex.
 *  In an app that means ESC or "U" or menu choice to Rx.
 */
void serialCheck() {
  while (!Serial) {
    delay(10);
  }
  // ChipHead may lie, stutter, brown out.
  delay(200);
}
