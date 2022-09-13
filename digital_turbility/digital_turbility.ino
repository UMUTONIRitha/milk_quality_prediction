int ledPin = 3;               
int sensor_in = 2;                 // Turbidity sensor on Digital Pin 2
 
void setup(){
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);      // Set ledPin as output mode
  pinMode(sensor_in, INPUT);       //Set Turbidity sensor pin to input mode
}
 
void loop(){
   int sensorValue = digitalRead(sensor_in);
   Serial.println(sensorValue);
   if(sensorValue==HIGH){       //Read sensor signal 
        digitalWrite(ledPin, HIGH);   // if sensor is LOW, then turn on
     }else{
        digitalWrite(ledPin, LOW);    // if sensor is HIGH, then turn off the led
     }
    delay(500);
}
