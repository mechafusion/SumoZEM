#include <Wire.h>
#include <VL53L0X.h>

//-Implementat functie de detectare a inamicului
//-Implementat functie de evitare a liniei albe 

#define XSHUT1 7
#define XSHUT2 8  // Pinul senzorului problematic
#define XSHUT3 9
#define XSHUT4 10

//pin-urile digitale pentru motor stanga
#define L1 3
#define L2 4
#define LPWM 5

//pin-urile digitale pentru motor dreapta
#define R1 11
#define R2 12
#define RPWM 6


VL53L0X sensorLeft;
VL53L0X sensorLeftFront;
VL53L0X sensorRight;
VL53L0X sensorRightFront;

void resetI2CBus() {
  Serial.println("Resetare bus I2C...");
  Wire.end();
  pinMode(SDA, INPUT_PULLUP);
  pinMode(SCL, INPUT_PULLUP);
  delay(250);
  Wire.begin();
  Wire.setClock(100000);
  Serial.println("Reset I2C complet");
}

void setPowerR(float speed)
{
        speed = constrain(speed, -1.0, 1.0); // Limităm viteza la intervalul [-1, 1]
        if (speed > 0)
        {                                    // Merge înainte
            int pwmValue = abs(speed) * 255; // Convertim viteza la PWM (0 - 255)
            digitalWrite(R1, LOW);
            digitalWrite(R2, HIGH);
            analogWrite(RPWM, pwmValue);
        }
        else if (speed < 0)
        {                                    // Merge înapoi
            int pwmValue = abs(speed) * 255; // Convertim viteza la PWM (0 - 255)
            digitalWrite(R1, HIGH);
            digitalWrite(R2, LOW);
            analogWrite(RPWM, pwmValue);
        }
        else
        { // Oprire
            digitalWrite(R1, LOW);
            digitalWrite(R2, LOW);
            analogWrite(RPWM, 0);
        }
}


void setPowerL(float speed) // viteza pins:in1 in2 pwm
{
        speed = constrain(speed, -1.0, 1.0); // Limităm viteza la intervalul [-1, 1]
        if (speed > 0)
        {                                    // Merge înainte
            int pwmValue = abs(speed) * 255; // Convertim viteza la PWM (0 - 255)
            digitalWrite(L1, LOW);
            digitalWrite(L2, HIGH);
            analogWrite(LPWM, pwmValue);
        }
        else if (speed < 0)
        {                                    // Merge înapoi
            int pwmValue = abs(speed) * 255; // Convertim viteza la PWM (0 - 255)
            digitalWrite(L1, HIGH);
            digitalWrite(L2, LOW);
            analogWrite(LPWM, pwmValue);
        }
        else
        { // Oprire
            digitalWrite(L1, LOW);
            digitalWrite(L2, LOW);
            analogWrite(LPWM, 0);
        }
}


void setup() {
  Serial.begin(9600);
  while(!Serial);

  //Configurare pini motoare
  //Dreapta
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(RPWM, OUTPUT);
  
  //Stanga
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(LPWM, OUTPUT);
  
  // Inițializare pini XSHUT
  pinMode(XSHUT1, OUTPUT); digitalWrite(XSHUT1, LOW);
  pinMode(XSHUT2, OUTPUT); digitalWrite(XSHUT2, LOW);
  pinMode(XSHUT3, OUTPUT); digitalWrite(XSHUT3, LOW);
  pinMode(XSHUT4, OUTPUT); digitalWrite(XSHUT4, LOW);
  delay(300);

  // Resetare bus I2C
  resetI2CBus();

  // Inițializare senzori normali
  digitalWrite(XSHUT1, HIGH);
  if(sensorLeft.init()) {
    sensorLeft.setAddress(0x30);
    sensorLeft.startContinuous();
    Serial.println("Left OK");
  }
  delay(200);

  digitalWrite(XSHUT2, HIGH);
  if(sensorLeftFront.init()) {
    sensorLeftFront.setAddress(0x31);
    sensorLeftFront.startContinuous();
    Serial.println("LeftFront OK"); 
  }
  delay(200);

  // Ceilalți senzori
  digitalWrite(XSHUT3, HIGH);
  if(sensorRight.init()) {
    sensorRight.setAddress(0x32);
    sensorRight.startContinuous();
    Serial.println("Right OK");
  }
  delay(200);

  digitalWrite(XSHUT4, HIGH);
  if(sensorRightFront.init()) {
    sensorRightFront.setAddress(0x33);
    sensorRightFront.startContinuous();
    Serial.println("RightFront OK");
  }

  Serial.println("\nConfigurare finalizată");
}

void loop() {
  // Afișare date doar de la senzorii operaționali
  Serial.print("Left: ");
  Serial.print(sensorLeft.readRangeContinuousMillimeters());
  
  Serial.print(" | Right: ");
  Serial.print(sensorRight.readRangeContinuousMillimeters());
  Serial.print(" mm | RightFront: ");
  Serial.print(sensorRightFront.readRangeContinuousMillimeters());
  Serial.println(" mm");
  
  //Pornit motoare
  setPowerL(0.3);
  setPowerR(0.3);

  delay(500);
}