#define PIN 2

uint32_t val;

void setup() {
  Serial.begin(9600);

  pinMode(PIN, INPUT);
}

void loop() {
  val = analogRead(PIN);
  Serial.print(val);
  Serial.println();
  delay(100);
}
