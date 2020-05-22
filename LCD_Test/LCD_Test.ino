#define COMMAND 0xFE
#define CLEAR   0x01
#define LINE0   0x80
#define LINE1   0xC0

void setup() {
  Serial2.begin(9600);              // Setup serial LCD
  clear_lcd();                   // Clear the LCD
}

void loop() {
  Serial2.write(COMMAND);          // Move LCD cursor to first character
  Serial2.write(LINE0);            // Move LCD cursor to the first line
  Serial2.print("Hey Ashley,");    // Print message
  delay(2000);                  // Wait
  clear_lcd();                  // Clear LCD
  delay(200);
  Serial2.write(COMMAND);          // Move LCD cursor to first character
  Serial2.write(LINE0);            // Move LCD cursor to the second line
  Serial2.print("Guess What?");    // Print message
  delay(2000);                  // Wait
  clear_lcd();                  // Clear LCD
  delay(200);
  Serial2.write(COMMAND);          // Move LCD cursor to first character
  Serial2.write(LINE0);            // Move LCD cursor to the first line
  Serial2.print("I love you!!");   // Print message
  delay(2000);                  // Wait
  clear_lcd();                  // Clear LCD
  delay(200);
}

void clear_lcd(void)
{
  Serial2.write(COMMAND);
  Serial2.write(CLEAR);
}
