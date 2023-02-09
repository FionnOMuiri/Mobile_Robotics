/*
 Start at 0, start going left

 *01 - if CC turn left at pA, stop at 1
      if C turn 180, turn left at pA, stop at 1
 *02 - if CC NO turn, stop at 2
      if C turn 180, NO turn, stop at 2
 03 - do 02, stop at 3
 
 *04 - if CC turn 180, stop at 4
      if C stop at 4
 *10 - if CC turn 180, turn right at A, stop at 0
      if C turn right at A, stop at 0
 *12 - if CC turn 180, turn left at A, stop at 2
      if C turn left at A, stop at 2
 13 - do 12, stop at 3

 14 - do 10, stop at 4

 *20 - if CC turn 180, NO turn at pA, stop at 0
      if C NO turn at pA, stop at 0
 *21 - if CC turn 180, turn right at pA, stop at 1
      if C turn right at pA, stop at 1
 *23 - if CC stop at 3
      if C turn 180, stop at 3
 24 - do 20, stop at 4

 30 - do 32, do 20
 
 31 - do 32, do 21

 *32 - if CC do 180, stop at 2
      if C stop at 2
 34 - do 32, do 20, stop at 4

 *40 - if CC stop at 0
      if C do 180, stop at 0
 41 - do 40, do 01

 42 - do 40, do 02

 43 - do 40, do 02, stop at 3
      
 */

void getRoute(){
  if(position == 0){
    if(dest == 1){route = 1;} 
    if(dest == 2){route = 2;}
    if(dest == 3){route = 3;}
    if(dest == 4){route = 4;} 
  } 
  if(position == 1){
    if(dest == 0){route = 5;} 
    if(dest == 2){route = 6;}
    if(dest == 3){route = 7;}
    if(dest == 4){route = 8;} 
  }
  if(position == 2){
    if(dest == 0){route = 9;} 
    if(dest == 1){route = 10;}
    if(dest == 3){route = 11;}
    if(dest == 4){route = 12;} 
  }
  if(position == 3){
    if(dest == 0){route = 13;} 
    if(dest == 1){route = 14;}
    if(dest == 2){route = 15;}
    if(dest == 4){route = 16;} 
  }
  if(position == 4){
    if(dest == 0){route = 17;} 
    if(dest == 1){route = 18;}
    if(dest == 2){route = 19;}
    if(dest == 3){route = 20;} 
  } 
  
}

void getDir(){
  if(orientation == 0){
    switch(route){
      Serial.println("Getting Directions");
      //01
      case 1:
        instructionsInt= 121;
        break;  
      //02
      case 2:
        instructionsInt= 11;
        break;  
      //03
      case 3:
        instructionsInt= 111;
        break;  
      //04
      case 4:
        instructionsInt= 14;
        break;  
      //10
      case 5:
        instructionsInt= 1314;
        break;  
      //12
      case 6:
        instructionsInt= 1214;
        break;  
      //13
      case 7:
        instructionsInt= 11214;
        break;  
      //14
      case 8:
        instructionsInt= 11314;
        break;  
      //20
      case 9:
        instructionsInt= 114;
        break;  
      //21
      case 10:
        instructionsInt= 1314;
        break;  
      //23
      case 11:
        instructionsInt= 1;
        break;  
      //24
      case 12:
        instructionsInt= 1114;
        break;  
      //30
      case 13:
        instructionsInt= 1114;
        break;  
      //31
      case 14:
        instructionsInt= 13114;
        break;  
      //32
      case 15:
        instructionsInt= 14;
        break;  
      //34
      case 16:
        instructionsInt= 11114;
        break;  
      //40
      case 17:
        instructionsInt= 1;
        break;  
      //41
      case 18:
        instructionsInt= 1211;
        break;  
      //42
      case 19:
        instructionsInt= 111;
        break;  
      //43
      case 20:
        instructionsInt= 1111;
        break;  
    }
  }  
  
  else{
    switch(route){
      //01
      case 1:
        instructionsInt= 1214;
        break;  
      //02
      case 2:
        instructionsInt= 114;
        break;  
      //03
      case 3:
        instructionsInt= 1114;
        break;  
      //04
      case 4:
        instructionsInt= 1;
        break;  
      //10
      case 5:
        instructionsInt= 131;
        break;  
      //12
      case 6:
        instructionsInt= 121;
        break;  
      //13
      case 7:
        instructionsInt= 1121;
        break;  
      //14
      case 8:
        instructionsInt= 1131;
        break;  
      //20
      case 9:
        instructionsInt= 11;
        break;  
      //21
      case 10:
        instructionsInt= 131;
        break;  
      //23
      case 11:
        instructionsInt= 14;
        break;  
      //24
      case 12:
        instructionsInt= 111;
        break;  
      //30
      case 13:
        instructionsInt= 111;
        break;  
      //31
      case 14:
        instructionsInt= 1311;
        break;  
      //32
      case 15:
        instructionsInt= 1;
        break;  
      //34
      case 16:
        instructionsInt= 1111;
        break;  
      //40
      case 17:
        instructionsInt= 14;
        break;  
      //41
      case 18:
        instructionsInt= 12114;
        break;  
      //42
      case 19:
        instructionsInt= 1114;
        break;  
      //43
      case 20:
        instructionsInt= 11114;
        break;  
    }
  }
  for(int i = 0; i<8; i++){
    instructionsArr[i] = 0;  
  }
  
  int ct = 0;
  while(instructionsInt != 0){
    instructionsArr[ct] = instructionsInt % 10;
    instructionsInt /= 10;
    ct++;
  }  
}