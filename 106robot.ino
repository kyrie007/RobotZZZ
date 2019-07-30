#include <Servo.h>
#include <Wire.h>
#include <LSM303.h>

Servo myservo;
LSM303 compass;
const int piston=2; //port 2
const int reedswitch=4; //port 4
const int servo=3; //port 3
float heading;

float angleServo=85; // starting state of servo motor
int pistonState=LOW;
float h = 138; // heading of the direction of the path
unsigned long Time =0;
unsigned long preTime=0;
float timeInterval1=100; // extending time
float timeInterval2=300; // extracting time
float timeInterval=200;
//***Timging part parameters***//
float waitTime=15000; // 15s
float startTurnCount=10; // when should it turn
float countTurnLeft = startTurnCount + 50; // shoot for extra credit
float leftTurningCount = countTurnLeft + 4; // lest turning distance
float countStop=leftTurningCount+5; // distance to keep direction straight
int switchState;
int preSwitchState;
int count = 0;

void setup() {
Serial.begin(9600);
Wire.begin();
myservo.attach(servo);
pinMode(piston,OUTPUT);
pinMode(reedswitch,INPUT_PULLUP);
compass.init();
compass.enableDefault();
compass.m_min = (LSM303::vector<int16_t>){-1803, -1877, +1247};
compass.m_max = (LSM303::vector<int16_t>){+1235, +954, +1977};
}
void loop()
{
//*************************************************//
//***********Reed Switch Control*******************//
//*************************************************//
switchState = digitalRead(reedswitch);
if (switchState == 0 && preSwitchState == 1)
{
count++;

}
preSwitchState = switchState;

//*************************************************//
//***********Piston Control************************//
//*************************************************//
Time=millis();
if (Time>=waitTime) {
if (count>=0 && count<=leftTurningCount)
{
if (Time-preTime>=timeInterval)
{
preTime=Time;
if (pistonState == LOW)
{
pistonState = HIGH;
timeInterval = timeInterval1;
}
else
{
pistonState = LOW;
timeInterval = timeInterval2;
}
digitalWrite(piston,pistonState);
}
}
}

//*************************************************//
//***********Servo Control*************************//
//*************************************************//
compass.read();
heading = compass.heading();
if (Time>=waitTime && count<startTurnCount) {
angleServo=85;
myservo.write(angleServo);
}
else if (count>=startTurnCount && count<=countTurnLeft)

{
if (heading<= (h-2) && heading>= (h+2))
{
angleServo=85;
myservo.write(angleServo);
}
else if (heading< (h-2) && heading>= (h-10))
{
angleServo=90;
myservo.write(angleServo);
}
else if (heading< (h-10) && heading>= (h-20))
{
angleServo=95;
myservo.write(angleServo);
}
else if ( (heading< (h-20) && heading>= 0) || (heading<=360 && heading > 320) )
{
angleServo=100;
myservo.write(angleServo);
}
else if (heading>(h+2) && heading<= (h+10))
{
angleServo=80;
myservo.write(angleServo);
}
else if (heading>(h+10) && heading<= (h+20))
{
angleServo=75;
myservo.write(angleServo);
}
else if (heading> (h+20) && heading<=320) {
angleServo=70;
myservo.write(angleServo);
}
}
//**************Turning Left************************************//
else if (count>countTurnLeft && count<=leftTurningCount) {
angleServo=75;
myservo.write(angleServo);

}
//*************************Keep Straignt***********************//
else if(Time>leftTurningCount && count<=countStop) {
angleServo=85;
myservo.write(angleServo);
}

//***********//
//***Print out data on serial monitor***//
//***********//
Serial.print("Time:");
Serial.print(Time);
Serial.print(" reedswitch counts:");
Serial.print(count);
Serial.print(" Piston:");
Serial.print(pistonState);
Serial.print(" Heading:");
Serial.print(heading);
Serial.print(" ServoAngle:");
Serial.println(angleServo);
}
