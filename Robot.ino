#include <Wire.h> // We use it to allow us the comunication between the different components of the robot
#include <Adafruit_PWMServoDriver.h> //For the servomotors

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); // An object to control the servos

#define SERVOMIN  280
#define SERVOMAX  400
#define SERVOSTOP 340 // Pulse for the servo
#define SERVOMAXI 280 // Max speed for the left servo
#define SERVOMAXD 400 // Max speed for the right servo
#define SERVOMINI 310 // Another speed parameter
#define SERVOMIND 370 // Another speed parameter
#define TRI_Pin 4 // Pin where TRI signal of the ultrasounds sensor is connected to
#define ECH_Pin 5 // Pin where ECH signal of the ultrasounds sensor is connected to
#define IR_left 2 // We define the pin where the LEFT IR sensor is connected to
#define IR_right 3 // We define the pin where the RIGHT IR sensor is connected to
#define Light_left A0 // We define the pin where the LEFT LIGHT sensor is connected to
#define Light_right A1 // We define the pin where the RIGHT LIGHT sensor is connected to
#define Buzzpin 6 // The pin where the zumbador is connected to

/***   Global variables   ***/
int servo_left=0; 
int servo_right=1; 
int miniservo=2; 
const int WHITE=1;
#define SERVO_90deg 420 // Servo 90º

const float c = 261.63; // Do (Octava 0)
const float d = 293.66; // Re (Octava 0)
const float e = 329.63; // Mi (Octava 0)
const float f = 349.23; // Fa (Octava 0)
const float g = 392.00; // Sol (Octava 0)
const float gS = 415.30;  // Sol# (Octava 0)
const float a = 440.00; // La (Octava 0)
const float b = 493.98; // Si (Octava 0)
const float aS = 466.16; // La# (Octava 0)
const float cH = 523.25;   // Do (Octava 1)
const float cSH = 554.37;  // Do# (Octava 1)
const float dH = 587.33; // Re (Octava 1)
const float dSH = 622.25; // Re# (Octava 1)
const float eH = 659.26; // Mi (Octava 1)
const float fH = 698.46; // Fa (Octava 1)
const float fS = 369.99; // Fa# (Octava 0)
const float gH = 783.99;  // Sol (Octava 1)
const float gSH = 830.61; // Sol# (Octava 1)
const float aH = 880.00; // La (Octava 1)
const long PAU=30000; // Pause

int d1=650, d2=500, d3=350, d4=150; // We define the time for each 
float melody[] ={d,b,fS,d,b,fS,e,cH,g,e,cH,g,d,b,fS,d,b,fS,e,d,f,g,g,gS,a,a,aS,b,cH,PAU}; // Array with all notes of melody 1
int timeNote[] = {d4,d4,d4,d4,d4,d4,d4,d4,d4,d4,d4,d4,d4,d4,d4,d4,d4,d4,d4,d4,d4,d4,d4,d4,d4,d4,d4,d4,d4,d4}; // Array with the time each note takes in melody 1
float melody2[] ={c,f,f,f,e,d,e,PAU}; // Array with all notes of melody 2
int timeNote2[] = {d4,d4,d4,d4,d4,d4,d4,d4}; // Array with the time each note takes in melody 2
float melody3[] = {d,d,d,d,PAU}; // Array with all notes of melody 3
int timeNote3[] = {d4,d4,d4,d4,d4};


/***********METHODS********/
void playMelody(){ // Function to play melody 1
  for (int note = 0; note<30; note++) { 
    tone(Buzzpin, melody[note]); 
    delay(timeNote[note]);
    noTone(Buzzpin); 
  }
}

void playMelody2(){ // Function to play melody 2
  for (int note = 0; note<8; note++) { 
    tone(Buzzpin, melody2[note]); 
    delay(timeNote2[note]); 
    noTone(Buzzpin); 
  }
}

void playMelody3(){ // Function to play melody 3
  for (int note = 0; note<5; note++) { 
    tone(Buzzpin, melody3[note]); 
    delay(timeNote3[note]); 
    noTone(Buzzpin); 
  }
}

void turnRight(){ // 
  pwm.setPWM(servo_right, 0, SERVOMINI);
  pwm.setPWM(servo_left, 0, SERVOMAXI);
  delayMicroseconds(5);
}

void turnLeft(){ // 
  pwm.setPWM(servo_right, 0, SERVOMAXD);
  pwm.setPWM(servo_left, 0, SERVOMIND);
  delayMicroseconds(5);
}

void Continue(){ // 
  pwm.setPWM(servo_right, 0, SERVOMAXD);
  pwm.setPWM(servo_left, 0, SERVOMAXI);
  delayMicroseconds(5);
}

void Stop(){ // 
  pwm.setPWM(servo_right, 0, SERVOSTOP);
  pwm.setPWM(servo_left, 0, SERVOSTOP);
  delayMicroseconds(5);
}

void Back(){
  pwm.setPWM(servo_left, 0, SERVOMAX);
  pwm.setPWM(servo_right, 0, SERVOMIN);
  delayMicroseconds(5);
}






void setup() {
  Serial.begin(9600);
  pwm.begin(); // It allows servos to run
  pwm.setPWMFreq(60);  // Servos will run with 60Hz
  pwm.setPWM(miniservo, 0, SERVO_90deg);
  //THE FUNCTION pinMode ALLOWS US TO ATTACH EACH COMPONENT AS OUTPUT OR INPUT
  pinMode(IR_left , INPUT); 
  pinMode(IR_right , INPUT); 
  pinMode(Light_left , INPUT); 
  pinMode(Light_right , INPUT); 
  pinMode(TRI_Pin, OUTPUT); 
  pinMode(ECH_Pin, INPUT); 
  pinMode(Buzzpin, OUTPUT); 
  playMelody(); // We will reproduce melody one when the robot is on
}

void loop() {
  long distance, Time;
  int lightD=analogRead(Light_left); // Variable to store the amount of light by the left sensor
  int lightI=analogRead(Light_right); //Variable to store the amount of light by the right sensor
  int go_right=digitalRead(IR_right); //Variable to check if the surface detected is BLACK(0) or WHITE(1)
  int go_left=digitalRead(IR_left); //Variable to check if the surface detected is BLACK(0) or WHITE(1)
  digitalWrite(TRI_Pin, LOW);
  delayMicroseconds(10);
  digitalWrite(TRI_Pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRI_Pin, LOW);
   Time = pulseIn(ECH_Pin, HIGH); // We calculate the time pulse takes to return
   distance = Time/58; // Distance is the result of the division of the time between 58

  Serial.print("Distancia "); // Imprime el texto "Distancia: "
  Serial.print(distance); // Imprime el valor de la distancia
  Serial.println(" cm");

if(lightD > 50 && lightI > 50){
  if(distance>5){
    if(go_right == WHITE && go_left == WHITE){
      Continue();
    }else{
      Stop();
      tone(Buzzpin, c);
      delay(3000);
      noTone(Buzzpin);
      Back();
      delay(1500);
      turnRight();
      delay(700);
    }
  }else{
    Stop();
    delay(500);
    playMelody3();
    delay(100);
    Back();
    delay(1000);
    turnRight();
    delay(700);
  }
}else{
  Stop();
  playMelody2();
}
} //END OF LOOP
