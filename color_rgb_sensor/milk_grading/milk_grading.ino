#include <OneWire.h>
#include <DallasTemperature.h>
#include "SoftwareSerial.h"

String str;

SoftwareSerial espSerial(10, 11);

// Define to which pin of the Arduino the 1-Wire bus is connected:
#define ONE_WIRE_BUS 12
#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define sensorOut 6
#define redLED 7
#define greenLED 8
#define blueLED 9
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;
int redColor = 0;
int greenColor = 0;
int blueColor = 0;
int redMin;
int redMax;
int greenMin;
int greenMax;
int blueMin;
int blueMax;
int color = 0;
float calibration_value = 21.34;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;
 // Create a new instance of the oneWire class to communicate with any OneWire device:
OneWire oneWire(ONE_WIRE_BUS);

// Pass the oneWire reference to DallasTemperature library:
DallasTemperature sensors(&oneWire);

void setup()
{
 pinMode(S0, OUTPUT);
 pinMode(S1, OUTPUT);
 pinMode(S2, OUTPUT);
 pinMode(S3, OUTPUT);
 pinMode(redLED, OUTPUT);
 pinMode(greenLED, OUTPUT);
 pinMode(blueLED, OUTPUT);
 pinMode(13, OUTPUT);
 pinMode(sensorOut, INPUT);
 digitalWrite(S0, HIGH);
 digitalWrite(S1, LOW);

 Serial.begin(9600);
 calibrate();
  sensors.begin();
}


void loop() 
{
 readColor();
 decideColor();
 printColor();
  int sensorValue = analogRead(A1);// read the input on analog pin 0:
  float voltage = sensorValue * (5.0 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  Serial.print("voltage:"); // print out the value you read:
  Serial.println(voltage);
  delay(500);
  for(int i=0;i<10;i++) 
 { 
 buffer_arr[i]=analogRead(A0);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buffer_arr[i]>buffer_arr[j])
 {
 temp=buffer_arr[i];
 buffer_arr[i]=buffer_arr[j];
 buffer_arr[j]=temp;
 }
 }
 }
 avgval=0;
 for(int i=2;i<8;i++)
 avgval+=buffer_arr[i];
 float volt=(float)avgval*5.0/1024/6;
 float ph_act = -5.70 * volt + calibration_value;
 //lcd.setCursor(0, 0);
  Serial.print("pH Val:");
// lcd.setCursor(8, 0);
  Serial.println(ph_act);
 //delay(1000);

 // Send the command for all devices on the bus to perform a temperature conversion:
  sensors.requestTemperatures();

  // Fetch the temperature in degrees Celsius for device index:
  float tempC = sensors.getTempCByIndex(0); // the index 0 refers to the first device
  // Fetch the temperature in degrees Fahrenheit for device index:
  float tempF = sensors.getTempFByIndex(0);

  // Print the temperature in Celsius in the Serial Monitor:
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.print(" \xC2\xB0"); // shows degree symbol
  Serial.print("C  |  ");

  // Print the temperature in Fahrenheit
  Serial.print(tempF);
  Serial.print(" \xC2\xB0"); // shows degree symbol
  Serial.println("F");

str=String(tempC)+String(voltage)+String(ph_act);
espSerial.println(str);

  // Wait 1 second:
  delay(5000);
}

void decideColor() 
{

 redColor = constrain(redColor, 0, 255);
 greenColor = constrain(greenColor, 0, 255);
 blueColor = constrain(blueColor, 0, 255);
 int maxVal = max(redColor, blueColor);
 maxVal = max(maxVal, greenColor);
 redColor = map(redColor, 0, maxVal, 0, 255);
 greenColor = map(greenColor, 0, maxVal, 0, 255);
 blueColor = map(blueColor, 0, maxVal, 0, 255);
 redColor = constrain(redColor, 0, 255);
 greenColor = constrain(greenColor, 0, 255);
 blueColor = constrain(blueColor, 0, 255);
 analogWrite(redLED, redColor);
 analogWrite(greenLED, greenColor);
 analogWrite(blueLED, blueColor);

 if (redColor > 250 && greenColor > 250 && blueColor > 250)
 {
   color = 1;
 }
 else if (redColor < 25 && greenColor < 25 && blueColor < 25) 
 {
   color = 2;
 }
 else if (redColor > 200 &&  greenColor > 200 && blueColor < 100)
 {
   color = 4;
 }
 else if (redColor > 200 &&  greenColor > 25 /*&& blueColor < 100*/) 
 {
   color = 3;
 }
 else if (redColor > 200 &&  greenColor < 100 && blueColor > 200) 
 {
   color = 5;
 }
 else if (redColor > 250 && greenColor < 200 && blueColor < 200)
 {
   color = 6;
 }
 else if (redColor < 200 && greenColor > 250 && blueColor < 200) 
 {
   color = 7;
 }
 else if (redColor < 200 /*&& greenColor < 200*/ && blueColor > 250) 
 {
   color = 8;
 }
 else 
 {
   color = 0;
 }
}


void calibrate() 
{
 Serial.println("Calibrating...");
 Serial.println("White");
 digitalWrite(13, HIGH);
 delay(2000);
 digitalWrite(S2, LOW);
 digitalWrite(S3, LOW);
 redMin = pulseIn(sensorOut, LOW);
 delay(100);
 digitalWrite(S2, HIGH);
 digitalWrite(S3, HIGH);
 greenMin = pulseIn(sensorOut, LOW);
 delay(100);
 digitalWrite(S2, LOW);
 digitalWrite(S3, HIGH);
 blueMin = pulseIn(sensorOut, LOW);
 delay(100);
 Serial.println("next...");
 digitalWrite(13, LOW);
 delay(2000);
 Serial.println("Black");
 digitalWrite(13, HIGH);
 delay(2000);
 digitalWrite(S2, LOW);
 digitalWrite(S3, LOW);
 redMax = pulseIn(sensorOut, LOW);
 delay(100);
 digitalWrite(S2, HIGH);
 digitalWrite(S3, HIGH);
 greenMax = pulseIn(sensorOut, LOW);
 delay(100);
 digitalWrite(S2, LOW);
 digitalWrite(S3, HIGH);
 blueMax = pulseIn(sensorOut, LOW);
 delay(100);
 Serial.println("Done calibrating.");
 digitalWrite(13, LOW);
}


void printColor() 
{
 Serial.print("R = ");
 Serial.print(redColor);
 Serial.print(" G = ");
 Serial.print(greenColor);
 Serial.print(" B = ");
 Serial.print(blueColor);
 Serial.print(" Color: ");
 switch (color) {
   case 1: Serial.println("WHITE"); break;
   case 2: Serial.println("BLACK"); break;
   case 3: Serial.println("ORANGE"); break;
   case 4: Serial.println("YELLOW"); break;
   case 5: Serial.println("PURPLE"); break;
   case 6: Serial.println("RED"); break;
   case 7: Serial.println("GREEN"); break;
   case 8: Serial.println("BLUE"); break;
   default: Serial.println("unknown"); break;
 }
}


void readColor() 
{
 digitalWrite(S2, LOW);
 digitalWrite(S3, LOW);
 redFrequency = pulseIn(sensorOut, LOW);
 redColor = map(redFrequency, redMin, redMax, 255, 0);
 delay(100);
 digitalWrite(S2, HIGH);
 digitalWrite(S3, HIGH);
 greenFrequency = pulseIn(sensorOut, LOW);
 greenColor = map(greenFrequency, greenMin, greenMax, 255, 0);
 delay(100);
 digitalWrite(S2, LOW);
 digitalWrite(S3, HIGH);
 blueFrequency = pulseIn(sensorOut, LOW);
 blueColor = map(blueFrequency, blueMin, blueMax, 255, 0);
 delay(100);
 
}

 
