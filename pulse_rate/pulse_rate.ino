long int time_now;
int pulse = 3;
int bpm = 0;
long int time_diff;
bool isT = true;
bool isone;

void setup()
{
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(pulse) == 1 and isT) {
    time_now = millis();
    isT = false;
    isone = true;
    Serial.println("Wait for a minute");
  }

  if (digitalRead(pulse) == 1 and isone and !isT) {
    Serial.print(".");
    bpm++;
    isone = false;
  }  
  
  if (digitalRead(pulse) == 0 and !isone) {
    isone = true;
  }

  if ((millis() - time_now >= 60000) and !isT) {
    Serial.print("\nYour Heart Rate = ");
    Serial.println(bpm);
    bpm = 0;
    isT = true;
  }
}

