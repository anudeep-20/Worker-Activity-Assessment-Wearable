int magn_pin = A1;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (analogRead(magn_pin) > 550) {
    Serial.println("Magnetic field is HIGH");
  } else {
    Serial.println("Magnetic field is LOW");
  }
  delay(1000);
}
