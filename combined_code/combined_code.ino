#include <Wire.h>

long int time_now;
long int time_temp;
int pulse = 3;
int bpm = 0;
long int time_diff;
bool isT = true;
bool isone;
int tempPin = A0;
int magnPin = A1;
float magnInt = 0.0;
float tempRead = 0.0;
float gyroX, gyroY, gyroZ;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4)
  Wire.write(0x00000000); //Setting the gyro to full scale +/- 250deg./s
  Wire.endTransmission();
  time_temp = millis();
  magnInt = analogRead(magnPin);
}

void loop()
{
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000, 6); //Request Gyro Registers (43 - 48)
  while (Wire.available() < 6);
  gyroX = Wire.read() << 8 | Wire.read(); //Store first two bytes into accelX
  gyroY = Wire.read() << 8 | Wire.read(); //Store middle two bytes into accelY
  gyroZ = Wire.read() << 8 | Wire.read(); //Store last two bytes into accelZ
  gyroX = gyroX / 131.0;
  gyroY = gyroY / 131.0;
  gyroZ = gyroZ / 131.0;

  if (digitalRead(pulse) == 1 and isT) {
    time_now = millis();
    isT = false;
    isone = true;
  }

  if (digitalRead(pulse) == 1 and isone and !isT) {
    bpm++;
    isone = false;
  }

  if (digitalRead(pulse) == 0 and !isone) {
    isone = true;
  }

  if ((millis() - time_now >= 60000) and !isT) {
    bpm = 0;
    isT = true;
  }

  if (millis() - time_temp >= 1000) {
    tempRead = (analogRead(tempPin) / 1024.0) * 500;
    magnInt = analogRead(magnPin);
    time_temp = millis();
  }
  Serial.print(gyroX);
  Serial.print(":");
  Serial.print(gyroY);
  Serial.print(":");
  Serial.print(gyroZ);
  Serial.print(";");
  if(bpm > 50){ 
    Serial.print(bpm);
  } else { 
    Serial.print(0);
  }
  Serial.print(";");
  Serial.print(tempRead);
  Serial.print(";");
  Serial.print(magnInt);
  Serial.println(";");
}
