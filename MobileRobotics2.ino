//Setup
bool Connected = false;
int routeARR[6] = {9,9,9,9,9,9};

//Motors

int motorLPWM = 39; 
int motorLPhase = 40; 
int motorRPWM = 37;
int motorRPhase = 38;
int motorMode = 1;

int L = 230;
int R = 230;

/*
 * 0 --> Line Follow
 * 1 --> Calibrate
 * 2 --> Checkpoint
 * 3 --> Checkpoint 5
 */

//Sensors

int sensors[5] = {0,0,0,0,0};
int sensorPins[5] = {A8, A13, A11, A9, A14};

int centreMin = 1000;
int LMin;
int RMin;

int sensorBin;
int checkTimer = 10000000;
int calibTimer = 0;

int distanceSensorPin = A6;
int distanceSensor;

//Wifi
#include <WiFi.h>
#define BUFSIZE 512

char server[] = "54.78.246.30";
int port = 8081;

char ssid[] = "NETGEAR63";
char password[] = "littlecello367";
WiFiClient client;


//Mapping
int position = 0;
int dest = 0;
int orientation = 0; // 0 = CC    1 = C
int instructionsInt;
int instructionsArr[8] = {0,0,0,0,0,0,0,0};
int route = 0;
int instructionsCt = 0;
int routeCt = 0;

bool c5 = false;

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
    else if(motorMode == 3){
      Serial.println("C5 approaching");
      motorUpdate(255,250);
      while(distanceSensor < 500){//720
        distanceUpdate();
        Serial.println(distanceSensor);
      }  
      motorUpdate(128,128);
      Serial.println("C5 reached");
      SEND();
      delay(200);
      Serial.println(readResponse());
      delay(5000);
      exit(0);
    }
  }
  //distanceUpdate();
  //Serial.println(distanceSensor);
  //delay(10);
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
    if(sensors[i]<650){
      sensorBin += pow(2,i);       
    }
  }
  //Serial.print(sensorBin,BIN);
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
        checkTimer = millis()+150;
  }
  //Serial.print(L);
  //Serial.print("   ");
  //Serial.println(R);
}


//~~~~~~~~~~~~~~~~~~~~~CALIBRATE~~~~~~~~~~~~~~~~~~~~~~~~~~~

void calibrate(){
  /*if(millis() - calibTimer > 2000 && millis() - calibTimer < 5000){
    if(sensors[2] < centreMin){
      centreMin = sensors[2];
    }  
    //Serial.println(centreMin);
  }
  else if(millis() - calibTimer > 3000){
    LMin = centreMin + 20;
    RMin = centreMin + 20;
    
    motorMode = 0;  
  }
  */
  motorMode = 0;
}

//~~~~~~~~~~~~~~~~~~~~CHECKPOINT~~~~~~~~~~~~~~~~~~~~~~~~~~

void checkpoint(){
  tone(36,440,100);
  motorUpdate(128, 128);
  
  if(instructionsArr[instructionsCt] == 0){  
    Serial.println("Destination reached");
    delay(300);
  
    newRoute();

    SEND();
    Serial.println(readResponse());
    
    Serial.print("Position: ");
    Serial.print(position);
    Serial.print("  dest: ");
    Serial.println(dest);
    instructionsCt = 0;
  }
  else if(instructionsArr[instructionsCt] == 1){
    motorMode = 0;
    Serial.println("Straight");
    instructionsCt++;
  }
  else if(instructionsArr[instructionsCt] == 2){
    turn(90);
    Serial.println("Turn Left");
    instructionsCt++;
  }
  else if(instructionsArr[instructionsCt] == 3){
    turn(-90);
    Serial.println("Turn Right");
    instructionsCt++;
  }
  else if(instructionsArr[instructionsCt] == 4){
    turn(180);
    Serial.println("Turn 180");
    instructionsCt++;
  }
  
  checkTimer = 10000000;
}

//~~~~~~~~~~~~~~~~~~~~~TURN~~~~~~~~~~~~~~~~~~~~~~~~~

void turn(int angle){
  //left
  if(angle == 90){
    motorUpdate(70,186);
    delay(300);
    sensorUpdate(); 
    while(sensorBin != 4){// && sensorBin != 12 && sensorBin != 6){
      sensorUpdate(); 
    }
    if(position == 1){
      orientation = 0;  
    }
  } 
  
  //180
  else if(angle == 180){
    if(orientation == 0){
      motorUpdate(70,186);
      delay(300);
      sensorUpdate(); 
      while(sensorBin != 4){// && sensorBin != 12 && sensorBin != 6){
        sensorUpdate();  
      }
      delay(300);
      sensorUpdate(); 
      while(sensorBin != 4){// && sensorBin != 12 && sensorBin != 6){
        sensorUpdate();  
      }
      orientation = 1;  
    } else{
      motorUpdate(186,70);
      delay(300);
      sensorUpdate(); 
      while(sensorBin != 4){// && sensorBin != 12 && sensorBin != 6){
        sensorUpdate();  
      }
      delay(300);
      sensorUpdate(); 
      while(sensorBin != 4){// && sensorBin != 12 && sensorBin != 6){
        sensorUpdate();  
      }
      
      orientation = 0;  
    }
  } 

  //right
  else if(angle == -90){
    motorUpdate(186,70);
    delay(300);
    sensorUpdate(); 
    while(sensorBin != 4){// && sensorBin != 12 && sensorBin != 6){
      sensorUpdate();  
    }
    if(position == 1){
      orientation = 1;  
    }
  }
  
  motorUpdate(128,128);
}

//~~~~~~~~~~~~~~~~~NEW ROUTE~~~~~~~~~~~~~~~~~~~~~~~~~

void newRoute(){
  if(routeARR[routeCt + 1] < 9){
    position = routeARR[routeCt];
    dest = routeARR[routeCt + 1];
    
    getRoute();
    getDir();
    Serial.print("Directions: ");
    for(int i = 0; i < 8; i++){
      Serial.print(instructionsArr[i]);
    }
    Serial.println();
    Serial.println();
  }
  else if(c5){
    motorMode = 3;
    Serial.println("Full Speed Ahead!");
    position = 5;  
  }
  else{
    SEND();
    delay(200);
    Serial.println(readResponse());
    delay(5000);
    exit(0);
  }
  routeCt++;
}

//~~~~~~~~~~~~~~~~DISTANCE SENSOR~~~~~~~~~~~~~~~~~~~

void distanceUpdate(){
  int input;
  int distanceSensorSmooth;

  input = analogRead(distanceSensorPin);

  distanceSensorSmooth = (input * 0.4) + (distanceSensor * 0.6);
  distanceSensor = distanceSensorSmooth;
}