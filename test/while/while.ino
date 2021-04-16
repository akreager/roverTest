#define COMMAND 0xFE
#define CLEAR   0x01
#define LINE0   0x80
#define LINE1   0xC0

int var1 = 64;
int var2 = 64;

int led = 13;

void setup() {
  Serial2.begin(9600);
  clear_lcd();

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  delay(500);
}

void loop() {
  while (var1 < 127) {
    clear_lcd();
    Serial2.write(COMMAND);
    Serial2.write(LINE0);
    Serial2.print(var1);
    delay(100);
    var1++;
  }
  clear_lcd();
  Serial2.write(COMMAND);
  Serial2.write(LINE0);
  Serial2.print(var1);
  digitalWrite(led, HIGH);
  delay(4000);
  digitalWrite(led, LOW);

  while (var1 > 64) {
    clear_lcd();
    Serial2.write(COMMAND);
    Serial2.write(LINE0);
    Serial2.print(var1);
    delay(100);
    var1--;
  }
  clear_lcd();
  Serial2.write(COMMAND);
  Serial2.write(LINE0);
  Serial2.print(var1);
  digitalWrite(led, HIGH);
  delay(4000);
  digitalWrite(led, LOW);

  while (var1 > 0) {
    clear_lcd();
    Serial2.write(COMMAND);
    Serial2.write(LINE0);
    Serial2.print(var1);
    delay(100);
    var1--;
  }
  clear_lcd();
  Serial2.write(COMMAND);
  Serial2.write(LINE0);
  Serial2.print(var1);
  digitalWrite(led, HIGH);
  delay(4000);
  digitalWrite(led, LOW);

  while (var1 < 64) {
    clear_lcd();
    Serial2.write(COMMAND);
    Serial2.write(LINE0);
    Serial2.print(var1);
    delay(100);
    var1++;
  }
  clear_lcd();
  Serial2.write(COMMAND);
  Serial2.write(LINE0);
  Serial2.print(var1);
  digitalWrite(led, HIGH);
  delay(4000);
  digitalWrite(led, LOW);
}

void clear_lcd(void)
{
  Serial2.write(COMMAND);
  Serial2.write(CLEAR);
}
