//Setup
bool Connected = false;
int routeARR[5];

//Motors

int motorLPWM = 39; 
int motorLPhase = 40; 
int motorRPWM = 37;
int motorRPhase = 38;
int motorMode = 1;

int L = 230;
int R = 230;
int LPrev;
int RPrev;

/*
 * 0 --> Line Follow
 * 1 --> Calibrate
 * 2 --> Checkpoint
 */

//Sensors

int sensors[5] = {0,0,0,0,0};
int sensorPins[5] = {A8, A13, A11, A9, A14};

int centreMin = 1000;
int LMin;
int RMin;
int turbo;

int sensorBin;

int pos = 0;
int checkTimer = 10000000;

int calibTimer = 0;

//Wifi
#include <WiFi.h>
#define BUFSIZE 512

char server[] = "54.78.246.30";
int port = 8081;

char ssid[] = "NETGEAR63";
char password[] = "littlecello367";
WiFiClient client;

int position = 0;
int destination;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~SETUP~~~~~~~~~~~~~~~~~~~~~~~~

void setup() {
  Serial.begin(115200);
  }

//~~~~~~~~~~~~~~~~~~~~~~~~MAIN LOOP~~~~~~~~~~~~~~~~~~~~~~~~

void loop() {
  if(Connected){
    if(millis()>checkTimer){
      motorMode = 2;  
    }
    
    if(motorMode == 0){
      sensorUpdate();
      speedCalc2();
      motorUpdate(L, R);
    }
    else if(motorMode == 1){
      sensorUpdate();
      calibrate();  
    }
    else if(motorMode == 2){
      checkpoint();    
    }
  }
} 

//~~~~~~~~~~~~~~~~~~~~~~MOTOR UPDATE~~~~~~~~~~~~~~~~~~~~~~~

void motorUpdate(int LS, int RS) {
  //Serial.println(LS);
  analogWrite(motorLPhase, 255);  //forward
  analogWrite(motorLPWM, LS);  // set speed of motor
  analogWrite(motorRPhase, 255);  //forward
  analogWrite(motorRPWM, RS);  // set speed of motor
}

//~~~~~~~~~~~~~~~~~~~~~~SENSOR UPDATE~~~~~~~~~~~~~~~~~~~~~~

void sensorUpdate(){
  sensorBin = 0;
  for(int i = 0; i < 5; i++){
    sensors[i] = analogRead(sensorPins[i]);
    //Serial.print(sensors[i]);
    //Serial.print("   ");
    if(sensors[i]<500){
      sensorBin += pow(2,i);       
    }
  }
  //Serial.print(sensorBin,BIN);
  //Serial.print("   ");
  //Serial.println();
}

//~~~~~~~~~~~~~~~~~~~~~~SPEED CALCULATION~~~~~~~~~~~~~~~~~~~~

//Version 1: Sensor Elevated

void speedCalculation(){

  L = 238;
  R = 238;
  
  if(sensors[1]<sensors[0]-20 || sensors[3]<sensors[4]-20){
    if(sensors[1] < sensors[3]){
      L = 238-((sensors[2] - centreMin) * 0.1);  
    }
    else{
      R = 238-((sensors[2] - centreMin) * 0.1); 
    }
  }
  else{
    if(sensors[0]<sensors[4]){
      L = 180-((sensors[1]-LMin) * 0.3);
      R = 220;
    }
    else{
      R = 180-((sensors[3]-RMin) * 0.3); 
      L = 220;
    }
  }

  /*if(sensors[2]<turbo){
    L = 250;
    R = 250;  
  }*/

  if(sensors[0] < 850 && sensors[4] < 850){
    checkTimer = millis()+200;
  }
}

//Version 2: Binary (Sensor Low)
void speedCalc2(){
  R = 240;
  L = 240;
  switch(sensorBin){
      //Hard Right
      case 1:
        R = 140;
        break;
      case 3:
        R = 160;
      //Soft Right
      case 2:
        R = 210;
        break;
      case 6:
        R = 230;
      //Straight
      case 4:
        break;
      case 12:
        L = 230;
      //Soft Left
      case 8:
        L = 210;
        break;
      case 24:
        L = 160;
      //Hard Right
      case 16:
        L = 140;
        break;
      //Checkpoint
      case 31:
        checkTimer = millis()+200;
  }
  //Serial.print(L);
  //Serial.print("   ");
  //Serial.println(R);
}


//~~~~~~~~~~~~~~~~~~~~~CALIBRATE~~~~~~~~~~~~~~~~~~~~~~~~~~~

void calibrate(){
  if(millis() - calibTimer > 2000 && millis() - calibTimer < 5000){
    if(sensors[2] < centreMin){
      centreMin = sensors[2];
    }  
    //Serial.println(centreMin);
  }
  else if(millis() - calibTimer > 3000){
    turbo = centreMin + 10;
    LMin = centreMin + 20;
    RMin = centreMin + 20;
    
    motorMode = 0;  
  }
}

//~~~~~~~~~~~~~~~~~~~~CHECKPOINT~~~~~~~~~~~~~~~~~~~~~~~~~~

void checkpoint(){
  pos++;
  tone(36,440,100);
  delay(100);
  motorUpdate(128, 128);
  delay(1000);
  if(pos == 3 || pos == 5){
    turn(90);  
  }
  delay(1000);
  motorUpdate(200, 200);
  delay(200);
  motorMode = 0;

  checkTimer = 10000000;
}

//~~~~~~~~~~~~~~~~~~~~~TURN~~~~~~~~~~~~~~~~~~~~~~~~~

void turn(int angle){
  //left
  if(angle == 90){
    motorUpdate(58,198);
    delay(670);
  } else if(angle == 180){
    motorUpdate(58,198);
    delay(1280);
  } else if(angle == -90){
    motorUpdate(198,58);
    delay(670); 
  }
  motorUpdate(128,128);
}