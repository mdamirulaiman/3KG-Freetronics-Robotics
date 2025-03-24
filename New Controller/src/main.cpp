#include <Arduino.h>
#include "variables.h"
#include "motor_control.h"
#include "ir_remote.h"

//check SSM_Of OFF & SSM_Us OFF-------------------------------------------------------------------------------------------------
void SSM_CHECK(){   
    MODE_Val = 5 ;            //0=Initialization,1=Standby,2=Wait,3=Versus,4=KILL,5=SSM_Check,6=Edge_Disabled
    //Comment out serial output when not debugging
    Sensor_Val_to_Serial(); //Output sensor value by serial communication

    pinMode(Opt_R45, OUTPUT);          //BUZZER ON
    SSM_Of_Val = digitalRead(SSM_Of);
    if(SSM_Of_Val == 0 && SSM_Us_Val == 0){
      //START OK
    //play mario coin tone
      tone(8,Tone_B5,100);               //D8 When the board arrives, change from D8 to D13
      delay(100);
      digitalWrite(LED, LOW); // LED OFF
      tone(8,Tone_E6,250);               //D8 When the board arrives, change from D8 to D13
      delay(300);
    } else {
      while(SSM_Of_Val > 0){
        //NOT START
        //Comment out serial output when not debugging
        Sensor_Val_to_Serial(); //Output sensor value by serial communication
        SSM_Of_Val = digitalRead(SSM_Of);
        tone(8,Tone_B4, 75);               //D8 When the board arrives, change from D8 to D13
        delay(150); 
        tone(8,Tone_F5, 75);               //D8 When the board arrives, change from D8 to D13
        delay(225); 
        tone(8,Tone_F5, 75);               //D8 When the board arrives, change from D8 to D13
        delay(150); 
        tone(8,Tone_F5, 75);               //D8 When the board arrives, change from D8 to D13
        delay(150); 
        tone(8,Tone_E5, 75);               //D8 When the board arrives, change from D8 to D13
        delay(225); 
        tone(8,Tone_D5, 75);               //D8 When the board arrives, change from D8 to D13
        delay(150); 
        tone(8,Tone_C5, 75);               //D8 When the board arrives, change from D8 to D13
        delay(1000); 
      }
    }
    delay(1000);
    //マリオのテーマ
    tone(8,Tone_E5,75);               //D8 When the board arrives, change from D8 to D13
    delay(150);
    tone(8,Tone_E5,75);               //D8 When the board arrives, change from D8 to D13
    delay(300);
    tone(8,Tone_E5,75);               //D8 When the board arrives, change from D8 to D13
    delay(300);
    tone(8,Tone_C5,75);               //D8 When the board arrives, change from D8 to D13
    delay(150);
    tone(8,Tone_E5,75);               //D8 When the board arrives, change from D8 to D13
    delay(300);
    tone(8,Tone_G5,75);               //D8 When the board arrives, change from D8 to D13
    delay(600);
    tone(8,Tone_G4,75);               //D8 When the board arrives, change from D8 to D13
    delay(150);
    delay(500);
    noTone(8);                        //BUZZER END
    pinMode(Opt_R45, INPUT);            //BUZZER OFF

    return ;
}

//Sound the buzzer according to the reaction of the sensor-------------------------------------------------------------------------------------------------
void Sensor_Val_to_Tone(){ 
    Tone_switch_cnt++;
    if(DIP_SW_Val == 0b00001111){
      //Edges DIP_SW 111 ONLY
      switch (Tone_switch_cnt) {
        case 1:
          if(Edge_FL_Val <= Edge_FL_Thsld){
            tone(8,Tone_F5, 50); //
          }
          Tone_delay(100);
          break;
        case 2:
          if(Edge_FR_Val <= Edge_FR_Thsld){
            tone(8,Tone_G5, 50); //
          }
          Tone_delay(100);
          break;
        case 3:
          if(Blade_Sens_Val == 0){
            tone(8,Tone_B5, 50); //
          }
          Tone_delay(100); 
          break;
        default:
          Tone_switch_cnt = 0;
      }
           
    } else {
    //Opt_Sens
      switch (Tone_switch_cnt) {
        case 1:
          if(Opt_L80_Val == 1){
            tone(8,Tone_G4, 50); //
          }
          Tone_delay(100);
          break;
        case 2:
          if(Opt_L45_Val == 1){
            tone(8,Tone_A4, 50); //
          }
          Tone_delay(100);
          break;
        case 3:
          if(Opt_L20_Val == 1){
            tone(8,Tone_B4, 50); //
          }
          Tone_delay(100);
          break;
        case 4:
          if(Distance_F_Val >= Distance_F_Thsld_Far){
            tone(8,Tone_C5, 50); //
          }
          Tone_delay(100);
          break;
        case 5:
          if(Opt_R20_Val == 1){
            tone(8,Tone_D5, 50); //
          }
          Tone_delay(100);
          break;
        case 6:
          if(Opt_R45_Val == 1){
            tone(8,Tone_E5, 50); //
          }
          Tone_delay(100);
          break;
        case 7:
          if(Opt_R80_Val == 1){
            tone(8,Tone_F5, 50); //
          }
          Tone_delay(100);
          break;
        default:
          Tone_switch_cnt = 0;
      }
    }
    return ;
}

//Delay for Tone: There is a process to exit the process when serial communication is received.
void Tone_delay(int Tone_delay_time){
    unsigned long time_Now = 0; // Limit Time Count only
    unsigned long time_Now1 = 0; // Limit Time Count only
    unsigned long time_Now2 = 0; // Limit Time Count only
    time_Now1 = millis();
    while(time_Now < Tone_delay_time){
      if(digitalRead(PIN_TX) == LOW) {
        //Exit the loop if an IR signal is received
        break; 
      }
      time_Now2 = millis();
      time_Now = time_Now2 - time_Now1;
    }
}

//Output sensor value by serial communication-------------------------------------------------------------------------------------------------
void Sensor_Val_to_Serial(){ 
//Serial output---------------------------------------------------------------------------
  //Serial output only in debug mode
  if(Debug_mode_Val == 1){
    Serial.print("FL:");
    Serial.println(Edge_FL_Val);
    Serial.print("FR:");
    Serial.println(Edge_FR_Val);
    Serial.print("Dist_F:");
    Serial.println(Distance_F_Val);
    Serial.print("Dist_F_Far:");
    Serial.println(Distance_F_Far_Val);
    Serial.print("Dist_F_Near:");
    Serial.println(Distance_F_Near_Val);
    Serial.print("Time_Get_Sens:");
    Serial.println(Time_Val3);
    Serial.print("Time_A:");
    Serial.println(Time_A_Val3);
    Serial.print("Time_B:");
    Serial.println(Time_B_Val3);
    Serial.print("Edges:");
    Serial.println(Edges_Val,BIN);
    Serial.print("Opt:");
    Serial.println(Opt_Val_X,BIN);
    Serial.print("DIP_SW:");
    Serial.println(DIP_SW_Val,BIN);
    Serial.print("SSM_Of:");
    Serial.println(SSM_Of_Val,BIN);
    Serial.print("SSM_Us:");
    Serial.println(SSM_Us_Val,BIN);
    Serial.print("MODE:");
    Serial.println(MODE_Val);
    Serial.print("MD_MODE:");
    Serial.println(MD_MODE_Val,BIN);
    Opt_Angle_Val = Opt_Angle(); //Optセンサーの偏りを取得 L 8,9,10,11,12 R // 10=Centre
    Serial.print("Opt_Angle:");
    Serial.println(Opt_Angle_Val);
    delay(10);
  }
  return ;
}

//Brake the motor and wait for the power to be turned off-------------------------------------------------------------------------------------------------
void KILL_MODE(){ 
    MODE_Val = 4 ;            //0=Initialization,1=Standby,2=Wait,3=Versus,4=KILL,5=SSM_Check,6=Edge_Disabled
    //MD_L & R Brake
    digitalWrite(MD_L1, LOW); // L Side Brake
    digitalWrite(MD_L2, LOW); // L Side Brake
    digitalWrite(MD_R1, LOW); // R Side Brake
    digitalWrite(MD_R2, LOW); // R Side Brake
    delay(100); 
    //Serial communication ON
    if(Debug_mode_Val == 1){
      pinMode(PIN_RX, INPUT); 
      pinMode(PIN_TX, OUTPUT); 
      Serial.begin(115200); 
    }
    //Infinite loop: LED blink & buzzer
    while(1){
      Sensor_Val_to_Serial(); //Output sensor value by serial communication
      OCR1A = 31000; //100.0msec TIMER1 INTERRUPT A <--- Cycle
      digitalWrite(LED, HIGH); // LED ON
      //Mario STAR
      tone(8,Tone_C5,100);               //D8 When the board arrives, change from D8 to D13
      delay(200);
      digitalWrite(LED, LOW); // LED OFF
      tone(8,Tone_C5,100);               //D8 When the board arrives, change from D8 to D13
      delay(200);
      tone(8,Tone_C5,100);               //D8 When the board arrives, change from D8 to D13
      delay(200);
      tone(8,Tone_D4,50);               //D8 When the board arrives, change from D8 to D13
      delay(100);
      tone(8,Tone_C5,50);               //D8 When the board arrives, change from D8 to D13
      delay(100);
      tone(8,Tone_C5,50);               //D8 When the board arrives, change from D8 to D13
      delay(100);
      tone(8,Tone_C5,100);               //D8 When the board arrives, change from D8 to D13
      delay(200);
      Sensor_Val_to_Serial(); //Output sensor value by serial communication
      tone(8,Tone_D4,50);               //D8 When the board arrives, change from D8 to D13
      delay(100);
      tone(8,Tone_C5,100);               //D8 When the board arrives, change from D8 to D13
      delay(200);
      tone(8,Tone_C5,100);               //D8 When the board arrives, change from D8 to D13
      delay(200);
      digitalWrite(LED, HIGH); // LED ON
      tone(8,Tone_B4,100);               //D8 When the board arrives, change from D8 to D13
      delay(200);
      digitalWrite(LED, LOW); // LED OFF
      tone(8,Tone_B4,100);               //D8 When the board arrives, change from D8 to D13
      delay(200);
      tone(8,Tone_B4,100);               //D8 When the board arrives, change from D8 to D13
      delay(200);
      Sensor_Val_to_Serial(); //Output sensor value by serial communication
      tone(8,Tone_C4,50);               //D8 When the board arrives, change from D8 to D13
      delay(100);
      tone(8,Tone_B4,50);               //D8 When the board arrives, change from D8 to D13
      delay(100);
      tone(8,Tone_B4,50);               //D8 When the board arrives, change from D8 to D13
      delay(100);
      tone(8,Tone_B4,100);               //D8 When the board arrives, change from D8 to D13
      delay(200);
      tone(8,Tone_C4,50);               //D8 When the board arrives, change from D8 to D13
      delay(100);
      tone(8,Tone_B4,100);               //D8 When the board arrives, change from D8 to D13
      delay(200);
      tone(8,Tone_B4,100);               //D8 When the board arrives, change from D8 to D13
      delay(200);
    }
}

//Opt_Angle OUTPUT
//Returns the bias of the six Opt sensor values as a value of 7-13. L 7,8,9,10,11,12,13 R 10=Centre
int Opt_Angle() {

  int result;
  result = 10;

  if(Opt_Sensor_Disable == 1){
    result = Opt_Sensor_Disable_Sens_Val;
    return result;
  }

  //If you are in attack mode and the distance sensor short range is ON, the sensor value will be forced to the center.
  if(Distance_F_Near_Val == 1 && St_Direct_Val == 1){
    result = 10;
  }else{
    //Reflect the Opt sensor value in result (L80,L45,L20 = -1~-3 // R20,R45,L80 = +1~+3)
    if(bitRead(Opt_Val_X,Opt_R80_B) == HIGH){
      result = result + 3;
    } else{
      if(bitRead(Opt_Val_X,Opt_R45_B) == HIGH){
        result = result + 2;
      } else{
        if(bitRead(Opt_Val_X,Opt_R20_B) == HIGH){
          result = result + 1;
        }
      }
    }
    if(bitRead(Opt_Val_X,Opt_L80_B) == HIGH){
      result = result - 3;
    } else{
      if(bitRead(Opt_Val_X,Opt_L45_B) == HIGH){
        result = result - 2;
      } else{
        if(bitRead(Opt_Val_X,Opt_L20_B) == HIGH){
          result = result - 1;
        }
      }
    }
  }
  return result;
}

//Creeping forward --------------------------------------------------------------------------------------
void Creeping_F4(){ 
        switch (Creeping_F_MODE_Val) {
          // Branch according to the value of Creeping_F_MODE_Val 0=BB,1=BF15,2=FB30,3=BF30
          case 0:
            MD_BB(10); //Brake 10ms
            if(Creeping_F_CNT_Val > 1000){
              Creeping_F_CNT_Val = 0;  //clear
              Creeping_F_MODE_Val = 1; //Mode change
            }
            break;
          case 1:
            if(Creeping_F_CNT_Val > 250){
              MD_BF(BF_T_S_15); //Front left 15deg
              MD_BB(10); //Brake 10ms
              Creeping_F_CNT_Val = 0;  //clear
              Creeping_F_MODE_Val = 2; //Mode change
            }
            break;
          case 2:
            if(Creeping_F_CNT_Val > 250){
              MD_FB(FB_T_S_30); //Front right 30deg
              MD_BB(10); //Brake 10ms
              Creeping_F_CNT_Val = 0;  //clear
              Creeping_F_MODE_Val = 3; //Mode change
            }
            break;
          case 3:
            if(Creeping_F_CNT_Val > 250){
              MD_BF(BF_T_S_30); //Front left 30deg
              MD_BB(10); //Brake 10ms
              Creeping_F_CNT_Val = 0;  //clear
              Creeping_F_MODE_Val = 2; //Mode change
            }
            break;
          default:
            // Executed when no case is matched
            Creeping_F_MODE_Val = 0; //clear
            Creeping_F_CNT_Val = 0;  //clear
        }     
}

//Creeping forward (fifth: straight forward mode) --------------------------------------------------------------------------------------
void Creeping_F5(){ 
        switch (Creeping_F_MODE_Val) {
          // Branch according to the value of Creeping_F_MODE_Val 0=BB,1=BF15,2=FB30,3=BF30
          case 0:
            MD_BB(10); //Brake 10ms
            if(Creeping_F_CNT_Val > 1000){
              Creeping_F_CNT_Val = 0;  //clear
              Creeping_F_MODE_Val = 1; //Mode change
            }
            break;
          case 1:
            if(Creeping_F_CNT_Val > 250){
              MD_FF(F_R_50); //Forward 50mm
              MD_BB(10); //Brake 10ms
              Creeping_F_CNT_Val = 0;  //clear
              Creeping_F_MODE_Val = 2; //Mode change
            }
            break;
          case 2:
            if(Creeping_F_CNT_Val > 250){
              MD_BB(10); //Brake 10ms
              Creeping_F_CNT_Val = 0;  //clear
              Creeping_F_MODE_Val = 1; //Mode change
            }
            break;
          default:
            // Executed when no case is matched
            Creeping_F_MODE_Val = 0; //clear
            Creeping_F_CNT_Val = 0;  //clear
        }     
}

//Edge Sensor move void -------------------------------------------------------------------------------------------------
  //Edge Sensor move normal Ver-------------------------------------------------------------------------------- 
    void Edge_S_Move(){ 
      Edge_S_flag_Val = 0;  //clear flag
      if(bitRead(Edges_Val,Blade_Sens_B) == HIGH){
        if(Drive_MODE_Val == 2 || Drive_MODE_Val == 0){
          //If MD operation state is retreat=2 OR brake=0
          MD_RR(F_R_100); //retreat
        } else{
          MD_BB(10); //brake
          MD_RR(F_R_100); //retreat
        }
        //Blade ON avoidance movement
          Opt_Angle_Val = Opt_Angle(); //Get Opt sensor bias L 7,8,9,10,11,12,13 R // 10=Centre
          switch (Opt_Angle_Val) {
            case 7:
              //In the case of Opt sensor value is left -3
              MD_RR(F_R_150); //retreat
              MD_RB(RB_T_S_25); //left circle 25deg
              break;
            case 8:
              //In the case of Opt sensor value is left -2
              MD_RR(F_R_150); //retreat
              MD_RB(RB_T_S_10); //left circle 10deg
              break;
            case 9:
              //In the case of Opt sensor value is left -1
              MD_RR(F_R_150); //retreat
              MD_BR(BR_T_S_30); //right side back 30deg
              MD_RB(RB_T_S_30); //left circle 30deg
              break;
            case 10:
              //If the Opt sensor value is in the middle
              MD_BR(BR_T_S_30); //right side back 30deg
              MD_RR(F_R_150); //retreat
              MD_RB(RB_T_S_15); //left circle 15deg
              break;
            case 11:
              //In the case of Opt sensor value is right +1
              MD_RR(F_R_150); //retreat
              MD_RB(RB_T_S_30); //left circle 30deg
              MD_BR(BR_T_S_30); //right side back 30deg
              break;
            case 12:
              //In the case of Opt sensor value is right +2
              MD_RR(F_R_150); //retreat
              MD_BR(BR_T_S_10); //right side back 10deg
              break;
            case 13:
              //In the case of Opt sensor value is right +3
              MD_RR(F_R_150); //retreat
              MD_BR(BR_T_S_25); //right side back 25deg
              break;
            default:
            // Executed when no case is matched
              MD_BR(BR_T_S_30); //right side back 30deg
              MD_RR(F_R_150); //retreat
              MD_RB(RB_T_S_15); //left circle 15deg
          }
          MD_BB(30); //brake
          St_Direct_Val = 1; //Change to attack mode
      } else{
          //Left and right edge sensor operation
          if(bitRead(Edges_Val,Edge_FL_B) == HIGH && bitRead(Edges_Val,Edge_FR_B) == HIGH){
            //左For right ON
            MD_BB(10); //brake
            MD_RR(F_R_100); //retreat 100mm
            MD_BB(50); //brake
            MD_RR(F_R_100); //retreat 100mm
            MD_BB(50); //brake
            MD_RF(RF_T_W_180); //Left super turn 180deg
            MD_BB(30); //brake
          }
          if(bitRead(Edges_Val,Edge_FL_B) == HIGH && bitRead(Edges_Val,Edge_FR_B) == LOW){
            //左ONの場合
            MD_BB(10); //brake
            MD_RR(F_R_100); //retreat 100mm
            MD_BB(50); //brake
            MD_RR(F_R_100); //retreat 100mm
            MD_BB(50); //brake
            MD_FR(FR_T_W_90); //Right super turn 90deg
            if(Opt_Val_X == 0){
              //If the Opt sensor is not responding
              MD_FR(FR_T_W_60); //Right super turn 60deg
            }
            MD_BB(30); //brake
          }
          if(bitRead(Edges_Val,Edge_FL_B) == LOW && bitRead(Edges_Val,Edge_FR_B) == HIGH){
            //For right ON
            MD_BB(10); //brake
            MD_RR(F_R_100); //retreat 100mm
            MD_BB(50); //brake
            MD_RR(F_R_100); //retreat 100mm
            MD_BB(50); //brake
            MD_RF(RF_T_W_90); //Left super turn 90deg
            if(Opt_Val_X == 0){
              //If the Opt sensor is not responding
              MD_RF(RF_T_W_60); //Left super turn 60deg
            }
            MD_BB(30); //brake
          }
      }
      //If edge sensor test is ON, set to KILL_MODE
      if(Edge_Sensor_Test == 1){
        KILL_MODE();
      }
    }
  //Edge Sensor move For attack mode----------------------------------------------------------------------------- 
    void Edge_S_Move_Attack(){ 
      Edge_S_flag_Val = 0;  //clear flag
      MD_BB(30); //brake
      MD_RR(F_R_100); //retreat
    //Blade ON avoidance movement
      Opt_Angle_Val = Opt_Angle(); //Get Opt sensor bias L 7,8,9,10,11,12,13 R // 10=Centre
      switch (Opt_Angle_Val) {
        case 7:
          //In the case of Opt sensor value is left -3
          MD_RR(F_R_150); //retreat
          MD_RB(RB_T_S_25); //left circle 25deg
          break;
        case 8:
          //In the case of Opt sensor value is left -2
          MD_RR(F_R_150); //retreat
          MD_RB(RB_T_S_10); //left circle 10deg
          break;
        case 9:
          //In the case of Opt sensor value is left -1
          MD_RR(F_R_150); //retreat
          MD_BR(BR_T_S_30); //right side back 30deg
          MD_RB(RB_T_S_30); //left circle 30deg
          break;
        case 10:
          //If the Opt sensor value is in the middle
          MD_BR(BR_T_S_30); //right side back 30deg
          MD_RR(F_R_150); //retreat
          MD_RB(RB_T_S_15); //left circle 15deg
          break;
        case 11:
          //In the case of Opt sensor value is right +1
          MD_RR(F_R_150); //retreat
          MD_RB(RB_T_S_30); //left circle 30deg
          MD_BR(BR_T_S_30); //right side back 30deg
          break;
        case 12:
          //In the case of Opt sensor value is right +2
          MD_RR(F_R_150); //retreat
          MD_BR(BR_T_S_10); //right side back 10deg
          break;
        case 13:
          //In the case of Opt sensor value is right +3
          MD_RR(F_R_150); //retreat
          MD_BR(BR_T_S_25); //right side back 25deg
          break;
        default:
        // Executed when no case is matched
          MD_BR(BR_T_S_30); //right side back 30deg
          MD_RR(F_R_150); //retreat
          MD_RB(RB_T_S_15); //left circle 15deg
      }
      MD_BB(30); //brake

    //If edge sensor test is ON, set to KILL_MODE
      if(Edge_Sensor_Test == 1){
        KILL_MODE();
      }
    }

//first move void -------------------------------------------------------------------------------------------------
  //first move 0 --------------------------------------------------------------------------------------
  void Fst_Move_0(){ 
    //Normally, I do nothing.
  }
  //first move 1 --------------------------------------------------------------------------------------
  void Fst_Move_1(){ 
    //Right side turn
    if(MINI_dohyo_MODE == 1){
      MD_FB(FB_T_S_45); //front right side 45deg
      MD_FF(F_R_200); //forward
      MD_BF(BF_T_S_45); //front left side 45deg
      MD_FF(F_R_100); //forward
      MD_BF(BF_T_S_90); //front left side 90deg
      MD_FF(F_R_50); //forward
    } else{
      MD_FB(FB_T_S_45); //front right side 45deg
      MD_FF(F_R_500); //forward
      MD_BF(BF_T_S_45); //front left side 45deg
      MD_FF(F_R_200); //forward
      MD_BF(BF_T_S_90); //front left side 90deg
      MD_FF(F_R_50); //forward
    }
  }
  //first move 2 --------------------------------------------------------------------------------------
  void Fst_Move_2(){ 
    //Move 60 degrees to the right
    if(MINI_dohyo_MODE == 1){
      MD_FB(FB_T_S_60); //front right side 60deg
      MD_FF(F_R_200); //forward
      MD_BF(BF_T_S_60); //front left side 60deg
      MD_BF(BF_T_S_60); //front left side 60deg
      MD_FF(F_R_50); //forward
    } else{
      MD_FB(FB_T_S_60); //front right side 60deg
      MD_FF(F_R_400); //forward
      MD_BF(BF_T_S_60); //front left side 60deg
      MD_BF(BF_T_S_60); //front left side 60deg
      MD_FF(F_R_50); //forward
    }
  }
  //first move 3 --------------------------------------------------------------------------------------
  void Fst_Move_3(){ 
    //400mm forward and 600mm retreat
    if(MINI_dohyo_MODE == 1){
      MD_FF(F_R_200); //forward
      MD_BB(50); //brake
      MD_RR(F_R_300); //retreat
      MD_BB(50); //brake
    } else{
      MD_FF(F_R_400); //forward
      MD_BB(50); //brake
      MD_RR(F_R_600); //retreat
      MD_BB(50); //brake
    }
  }
  //first move 4 --------------------------------------------------------------------------------------
  void Fst_Move_4(){ 
    //diagonal right back
    if(MINI_dohyo_MODE == 1){
      MD_RB(RB_T_S_60); //right side back 60deg
      MD_RR(F_R_200); //retreat
      MD_BB(50); //brake 50ms
    } else{
      MD_RB(RB_T_S_60); //right side back 60deg
      MD_RR(F_R_400); //retreat
      MD_BB(50); //brake 50ms
    }
  }
  //first move 5 --------------------------------------------------------------------------------------
  void Fst_Move_5(){ 
    //Retreat in random orientation depending on Opt sensor value
    Opt_Angle_Val = Opt_Angle(); //Get Opt sensor bias L 7,8,9,10,11,12,13 R // 10=Centre
    if(Opt_Angle_Val == 7){
      //In the case of Opt sensor value is left -3
      MD_RB(RB_T_S_80); //left circle 80deg
    }
    if(Opt_Angle_Val == 8){
      //In the case of Opt sensor value is left -2
      MD_RB(RB_T_S_45); //left circle 45deg
    }
    if(Opt_Angle_Val == 9){
      //In the case of Opt sensor value is left -1
      MD_RB(RB_T_S_20); //left circle 20deg
    }
    if(Opt_Angle_Val == 10){
      //If the Opt sensor value is in the middle
      //do nothing
    }
    if(Opt_Angle_Val == 11){
      //In the case of Opt sensor value is right +1
      MD_BR(BR_T_S_20); //right side back 20deg
    }
    if(Opt_Angle_Val == 12){
      //In the case of Opt sensor value is right +2
      MD_BR(BR_T_S_45); //right side back 45deg
    }
    if(Opt_Angle_Val == 13){
      //In the case of Opt sensor value is right +3
      MD_BR(BR_T_S_80); //right side back 80deg
    }
    if(MINI_dohyo_MODE == 1){
      MD_RR(F_R_200); //retreat
    } else{
      MD_RR(F_R_400); //retreat
    }
    MD_BB(50); //brake 50ms
  }
  //first move 6 --------------------------------------------------------------------------------------
  void Fst_Move_6(){ 
    //diagonal left back
    if(MINI_dohyo_MODE == 1){
      MD_BR(BR_T_S_60); //left circle 60deg
      MD_RR(F_R_200); //retreat
      MD_BB(50); //brake 50ms
    } else{
      MD_BR(BR_T_S_60); //left circle 60deg
      MD_RR(F_R_400); //retreat
      MD_BB(50); //brake 50ms
    }
  }
  //first move 7 --------------------------------------------------------------------------------------
  void Fst_Move_7(){ 
    //180 degree turn
    if(MINI_dohyo_MODE == 1){
      MD_RF(RF_T_W_180); //front left side 180deg
      MD_BB(50); //brake 50ms
    } else{
      MD_RF(RF_T_W_180); //front left side 180deg
      MD_BB(50); //brake 50ms
    }
  }
  //first move 8 --------------------------------------------------------------------------------------
  void Fst_Move_8(){ 
    //Move to the left 60 degrees
    if(MINI_dohyo_MODE == 1){
      MD_BF(BF_T_S_60); //front left side 60deg
      MD_FF(F_R_200); //forward 400m
      MD_FB(FB_T_S_60); //front right side 60deg
      MD_FB(FB_T_S_60); //front right side 60deg
      MD_FF(F_R_50); //forward 50mm
    } else{
      MD_BF(BF_T_S_60); //front left side 60deg
      MD_FF(F_R_400); //forward 400m
      MD_FB(FB_T_S_60); //front right side 60deg
      MD_FB(FB_T_S_60); //front right side 60deg
      MD_FF(F_R_50); //forward 50mm
    }
  }
  //first move 9 --------------------------------------------------------------------------------------
  void Fst_Move_9(){ 
    //left side turn
    if(MINI_dohyo_MODE == 1){
      MD_BF(BF_T_S_45); //front left side 45deg
      MD_FF(F_R_200); //forward 500m
      MD_FB(FB_T_S_45); //front right side 45deg
      MD_FF(F_R_100); //forward 200m
      MD_FB(FB_T_S_90); //front right side 90deg
      MD_FF(F_R_50); //forward 50mm
    } else{
      MD_BF(BF_T_S_45); //front left side 45deg
      MD_FF(F_R_500); //forward 500m
      MD_FB(FB_T_S_45); //front right side 45deg
      MD_FF(F_R_200); //forward 200m
      MD_FB(FB_T_S_90); //front right side 90deg
      MD_FF(F_R_50); //forward 50mm
    }
  }
  //first move default --------------------------------------------------------------------------------------
  void Fst_Move_default(){ 
    digitalWrite(LED, HIGH); // LED ON
    digitalWrite(LED, LOW); // LED OFF
  }

//strategic directive void -------------------------------------------------------------------------------------------------
  //strategic directive 0 --------------------------------------------------------------------------------------
  //Creeping forward mode
  void St_Direct_0(){ 
    Opt_Angle_Val = Opt_Angle(); //Get Opt sensor bias L 7,8,9,10,11,12,13 R // 10=Centre
    //If the distance is close, change to attack mode
    if(Distance_F_Near_CNT_Val > 50){
      St_Direct_Val = 1; //Change to attack mode and exit the function
      Distance_F_Near_CNT_Val = 0; //counter reset
      return;
    }
    //If the distance sensor (long distance) remains ON, attack.
    if(Distance_F_Far_CNT_Val > 2000){
      //If the distance sensor (long distance) is ON for more than 2 seconds
      St_Direct_Val = 1; //Change to attack mode and exit the function
      Distance_F_Far_CNT_Val = 0; //counter reset
      return;
    }
    //If the edge sensor flag is ON, stop processing.
    if(Edge_S_flag_Val == 1){
      return; 
    }
    switch (Opt_Angle_Val) {
      case 7:
        MD_BF(BF_T_S_80); //front left 80deg
        break;
      case 8:
        MD_BF(BF_T_S_45); //front left 45deg
        break;
      case 9:
        MD_BF(BF_T_S_20); //front left 20deg
        break;
      case 10:
        //Creeping forward, moving forward one wheel at a time
        Creeping_F4(); //   
        break;
      case 11:
        MD_FB(FB_T_S_20); //front right20deg
        break;
      case 12:
        MD_FB(FB_T_S_45); //front right45deg
        break;
      case 13:
        MD_FB(FB_T_S_80); //front right80deg
        break;
    }
  }
  //strategic directive 1 --------------------------------------------------------------------------------------
  //attack mode
  void St_Direct_1(){ 
    Opt_Angle_Val = Opt_Angle(); //Get Opt sensor bias L 7,8,9,10,11,12,13 R // 10=Centre
    
    //If the distance remains close, retry
    if(Distance_F_Near_CNT_Val > 1500){
      //If short distance ON exceeds 1.5 seconds
      MD_BB(50); //brake 50ms
      MD_RR(F_R_200); //retreat
      MD_BB(50); //brake 50ms
      Distance_F_Near_CNT_Val = 0; //counter reset
    }
    //If the blade sensor remains ON, retry.
    if(Blade_ON_CNT_Val > 200){
      //If the blade sensor ON exceeds 0.2 seconds
      Edge_S_Move_Attack(); //Blade sensor operation for attack mode
      Blade_ON_CNT_Val = 0; //counter reset
    }
    //Mode change by operating time
    if(MODE_Change_CNT_Val > 5000){
      Creeping_F_CNT_Val = 0;  //count clear
      MODE_Change_CNT_Val = 0;  //count clear
      St_Direct_Val = 0;   //Change to Creeping forward mode and exit the function
      return;
    }
    //If the edge sensor flag is ON, stop processing.
    if(Edge_S_flag_Val == 1){
      return; 
    }
    //MD_IO_Write(MD_bit_FF); //FF->MD
    switch (Opt_Angle_Val) {
      case 7:
        MD_BF(BF_T_S_90); //Front left 90deg
        MD_BF(BF_T_S_30); //front left 30deg
        break;
      case 8:
        MD_BF(BF_T_S_60); //front left 60deg
        break;
      case 9:
        MD_BF(BF_T_S_30); //front left 30deg
        break;
      case 10:
        if(Attack_to_STOP == 1){
          MD_BB(50); //brake
        }else{
          MD_FF(F_R_50); //forward
        }
        break;
      case 11:
        MD_FB(FB_T_S_30); //front right30deg
        break;
      case 12:
        MD_FB(FB_T_S_60); //front right60deg
        break;
      case 13:
        MD_FB(FB_T_S_90); //Front right 90deg
        MD_FB(FB_T_S_30); //front right30deg
        break;
    }
  }
  //strategic directive 2 --------------------------------------------------------------------------------------
  //Mixed mode
  void St_Direct_2(){ 
    Opt_Angle_Val = Opt_Angle(); //Get Opt sensor bias L 7,8,9,10,11,12,13 R // 10=Centre
    //If the distance is close, change to attack mode
    if(Distance_F_Near_CNT_Val > 50){
      St_Direct_Val = 1; //Change to attack mode and exit the function
      Distance_F_Near_CNT_Val = 0; //counter reset
      return;
    }
    //Mode change by operating time
    if(MODE_Change_CNT_Val > 5000){
      Creeping_F_CNT_Val = 0;  //count clear
      MODE_Change_CNT_Val = 0;  //count clear
      St_Direct_Val = 0;   //Change to Creeping forward mode and exit the function
      return;
    }
    //If the edge sensor flag is ON, stop processing.
    if(Edge_S_flag_Val == 1){
      return; 
    }
    switch (Opt_Angle_Val) {
      case 7:
        MD_RF(RF_T_W_45); //左W45deg
        MD_BB(50); //brake 50ms
        break;
      case 8:
        MD_RF(RF_T_W_30); //左W30deg
        MD_BB(50); //brake 50ms
        break;
      case 9:
        MD_RF(RF_T_W_20); //左W20deg
        MD_BB(50); //brake 50ms
        break;
      case 10:
        //do nothing
        break;
      case 11:
        MD_FR(FR_T_W_20); //右W20deg
        MD_BB(50); //brake 50ms
        break;
      case 12:
        MD_FR(FR_T_W_30); //右W30deg
        MD_BB(50); //brake 50ms
        break;
      case 13:
        MD_FR(FR_T_W_45); //右W45deg
        MD_BB(50); //brake 50ms
        break;
    }
  }
  //strategic directive 3 --------------------------------------------------------------------------------------
  //Sensor ignore mode
  void St_Direct_3(){ 
    //Mode change by operating time
    if(MODE_Change_CNT_Val > 3000){
      Creeping_F_CNT_Val = 0;  //count clear
      MODE_Change_CNT_Val = 0;  //count clear
      St_Direct_Val = 0;   //Change to Creeping forward mode and exit the function
      return;
    }
    //If the blade sensor remains ON, retry.
    if(Blade_ON_CNT_Val > 200){
      //If the blade sensor ON exceeds 0.2 seconds
      Edge_S_Move_Attack(); //Blade sensor operation for attack mode
      Blade_ON_CNT_Val = 0; //counter reset
    }
    //If the edge sensor flag is ON, stop processing.
    if(Edge_S_flag_Val == 1){
      return; 
    }
    MD_FF(F_R_50); //forward
  }
  //strategic directive 4 --------------------------------------------------------------------------------------
  //Creeping forward 5th mode (crawling straight) Super solid turning Ver.
  void St_Direct_4(){ 
    Opt_Angle_Val = Opt_Angle(); //Get Opt sensor bias L 7,8,9,10,11,12,13 R // 10=Centre
    //If the distance is close, change to attack mode
    if(Distance_F_Near_CNT_Val > 50){
      St_Direct_Val = 1; //Change to attack mode and exit the function
      Distance_F_Near_CNT_Val = 0; //counter reset
      return;
    }
    //If the edge sensor flag is ON, stop processing.
    if(Edge_S_flag_Val == 1){
      return; 
    }
    switch (Opt_Angle_Val) {
      case 7:
        MD_RF(RF_T_W_45); //左W45deg
        MD_BB(50); //brake 50ms
        break;
      case 8:
        MD_RF(RF_T_W_30); //左W30deg
        MD_BB(50); //brake 50ms
        break;
      case 9:
        MD_RF(RF_T_W_20); //左W20deg
        MD_BB(50); //brake 50ms
        break;
      case 10:
        Creeping_F5();//匍匐forward第五(まっすぐ50mm)
        break;
      case 11:
        MD_FR(FR_T_W_20); //右W20deg
        MD_BB(50); //brake 50ms
        break;
      case 12:
        MD_FR(FR_T_W_30); //右W30deg
        MD_BB(50); //brake 50ms
        break;
      case 13:
        MD_FR(FR_T_W_45); //右W45deg
        MD_BB(50); //brake 50ms
        break;
    }
  }
  //strategic directive 5 --------------------------------------------------------------------------------------
  //Creeping forward 5th mode (crawling straight) Single wheel forward turning Ver.
  void St_Direct_5(){ 
    Opt_Angle_Val = Opt_Angle(); //Get Opt sensor bias L 7,8,9,10,11,12,13 R // 10=Centre
    //If the distance is close, change to attack mode
    if(Distance_F_Near_CNT_Val > 50){
      St_Direct_Val = 1; //Change to attack mode and exit the function
      Distance_F_Near_CNT_Val = 0; //counter reset
      return;
    }
    //If the edge sensor flag is ON, stop processing.
    if(Edge_S_flag_Val == 1){
      return; 
    }
    switch (Opt_Angle_Val) {
      case 7:
        MD_BF(BF_T_S_80); //front left 80deg
        break;
      case 8:
        MD_BF(BF_T_S_45); //front left 45deg
        break;
      case 9:
        MD_BF(BF_T_S_20); //front left 20deg
        break;
      case 10:
        Creeping_F5();//匍匐forward第五(まっすぐ50mm)
        //MD_BB(50); //brake 50ms
        break;
      case 11:
        MD_FB(FB_T_S_20); //front right20deg
        break;
      case 12:
        MD_FB(FB_T_S_45); //front right45deg
        break;
      case 13:
        MD_FB(FB_T_S_80); //front right80deg
        break;
    }
  }
  //strategic directive 6 --------------------------------------------------------------------------------------
  //Creeping forward 5th mode (crawling straight) Single wheel rear turning Ver.
  void St_Direct_6(){ 
    Opt_Angle_Val = Opt_Angle(); //Get Opt sensor bias L 7,8,9,10,11,12,13 R // 10=Centre
    //If the distance is close, change to attack mode
    if(Distance_F_Near_CNT_Val > 50){
      St_Direct_Val = 1; //Change to attack mode and exit the function
      Distance_F_Near_CNT_Val = 0; //counter reset
      return;
    }
    //If the edge sensor flag is ON, stop processing.
    if(Edge_S_flag_Val == 1){
      return; 
    }
    //バック回数制限（規定回数以上でMode change）
    if(Back_CNT_Val > 9){
      St_Direct_Val = 5; //Creeping forward 5th mode (straight creeping) Change to single wheel forward turning Ver and end the function
      Back_CNT_Val = 0; //counter reset
      return;
    }
    switch (Opt_Angle_Val) {
      case 7:
        MD_RB(RB_T_S_80); //左後80deg
        Back_CNT_Val ++;
        break;
      case 8:
        MD_RB(RB_T_S_45); //左後45deg
        Back_CNT_Val ++;
        break;
      case 9:
        MD_RB(RB_T_S_20); //左後20deg
        Back_CNT_Val ++;
        break;
      case 10:
        Creeping_F5();//Creeping forward fifth (straight 50mm)
        //MD_BB(50); //brake 50ms
        break;
      case 11:
        MD_BR(RB_T_S_20); //右後20deg
        Back_CNT_Val ++;
        break;
      case 12:
        MD_BR(RB_T_S_45); //右後45deg
        Back_CNT_Val ++;
        break;
      case 13:
        MD_BR(RB_T_S_80); //右後80deg
        Back_CNT_Val ++;
        break;
    }
  }
  //strategic directive 7 --------------------------------------------------------------------------------------
  //Attack mode (for robots with strong blades)
  void St_Direct_7(){ 
    Opt_Angle_Val = Opt_Angle(); //Get Opt sensor bias L 7,8,9,10,11,12,13 R // 10=Centre
    
    //If the distance remains close, retry
    if(Distance_F_Near_CNT_Val > 1500){
      //If short distance ON exceeds 1.5 seconds
      MD_BB(50); //brake 50ms
      MD_RR(F_R_200); //retreat
      MD_BB(50); //brake 50ms
      Distance_F_Near_CNT_Val = 0; //counter reset
    }
    //If the blade sensor remains ON, retry.
    if(Blade_ON_CNT_Val > 100){
      //If the blade sensor ON exceeds 0.1 seconds
      Edge_S_Move_Attack(); //Blade sensor operation for attack mode
      Blade_ON_CNT_Val = 0; //counter reset
    }
    //Mode change by operating time
    if(MODE_Change_CNT_Val > 5000){
      Creeping_F_CNT_Val = 0;  //count clear
      MODE_Change_CNT_Val = 0;  //count clear
      St_Direct_Val = 0;   //Change to Creeping forward mode and exit the function
      return;
    }
    //If the edge sensor flag is ON, stop processing.
    if(Edge_S_flag_Val == 1){
      return; 
    }
    //MD_IO_Write(MD_bit_FF); //FF->MD
    switch (Opt_Angle_Val) {
      case 7:
        MD_BF(BF_T_S_90); //Front left 90deg
        MD_BF(BF_T_S_30); //front left 30deg
        break;
      case 8:
        MD_BF(BF_T_S_60); //front left 60deg
        break;
      case 9:
        MD_BF(BF_T_S_30); //front left 30deg
        break;
      case 10:
        if(Attack_to_STOP == 1){
          MD_BB(50); //brake
        }else{
          MD_FF(F_R_50); //forward
        }
        break;
      case 11:
        MD_FB(FB_T_S_30); //front right30deg
        break;
      case 12:
        MD_FB(FB_T_S_60); //front right60deg
        break;
      case 13:
        MD_FB(FB_T_S_90); //Front right 90deg
        MD_FB(FB_T_S_30); //front right30deg
        break;
    }
  }
  //strategic directive 8 --------------------------------------------------------------------------------------
  //
  void St_Direct_8(){ 
    St_Direct_Val = 0;      //clear
  }
  //strategic directive 9 --------------------------------------------------------------------------------------
  //
  void St_Direct_9(){ 
    St_Direct_Val = 0;      //clear
  }
  //strategic directive 10 --------------------------------------------------------------------------------------
  //
  void St_Direct_10(){ 
    St_Direct_Val = 0;      //clear
  }
  //strategic directive 11 --------------------------------------------------------------------------------------
  //
  void St_Direct_11(){ 
    St_Direct_Val = 0;      //clear
  }
  //strategic directive 12 --------------------------------------------------------------------------------------
  //
  void St_Direct_12(){ 
    St_Direct_Val = 0;      //clear
  }
  //strategic directive 13 --------------------------------------------------------------------------------------
  //
  void St_Direct_13(){ 
    St_Direct_Val = 0;      //clear
  }
  //strategic directive 14 --------------------------------------------------------------------------------------
  //
  void St_Direct_14(){ 
    St_Direct_Val = 0;      //clear
  }
  //strategic directive 15 --------------------------------------------------------------------------------------
  //
  void St_Direct_15(){ 
    St_Direct_Val = 0;      //clear
  }
  //strategic directive Debag --------------------------------------------------------------------------------------
  //デバッグモード
  void St_Direct_D(){ 
    digitalWrite(LED, HIGH); // LED ON
    MD_BB(500); //brake
    digitalWrite(LED, LOW); // LED OFF
    MD_BB(500); //brake
  }
  //strategic directive default --------------------------------------------------------------------------------------
  void St_Direct_default(){ 
    MD_BB(50); //brake 50ms
    digitalWrite(LED, HIGH); // LED ON
    St_Direct_Val = 0;      //clear
  }

//Initialization-------------------------------------------------------------------------------------------------
void setup(){   
    pinMode(PIN_RX, INPUT_PULLUP);
    pinMode(PIN_TX, INPUT_PULLUP);
    pinMode(Opt_L20, INPUT);          //D2
    pinMode(Opt_L45, INPUT);          //D3 L45&55
    pinMode(Blade_Sens, INPUT_PULLUP);//D4
    pinMode(MD_L1, OUTPUT);           //D5
    pinMode(MD_L2, OUTPUT);           //D6
    pinMode(Opt_R20, INPUT);          //D7
    pinMode(Opt_R45, OUTPUT);         //D8 R45&55 AND BUZZER, Switched by TIMER1 interrupt.
    pinMode(MD_R1, OUTPUT);           //D9
    pinMode(MD_R2, OUTPUT);           //D10
    pinMode(SSM_Of, INPUT);           //D11
    pinMode(Opt_R80, INPUT);          //D12
    //    pinMode(SSM_Us, INPUT);           //D12
    pinMode(LED, OUTPUT);             //D13 LED AND BUZZER
    pinMode(DIP_SW1, INPUT);          //A7 external pull up // AnalogRead ONLY & INT PULLUP NONE 
    pinMode(DIP_SW2, INPUT);          //A6 external pull up // AnalogRead ONLY & INT PULLUP NONE
    pinMode(DIP_SW3, INPUT);          //A5 external pull up //
    //    pinMode(Opt_R80, INPUT);          //A4 external pull up //
    pinMode(DIP_SW4, INPUT);          //A4 external pull up //
    pinMode(Edge_FL, INPUT);          //A3
    pinMode(Edge_FR, INPUT);          //A2
    pinMode(Distance_F, INPUT);       //A0
    pinMode(Opt_L80, INPUT);          //A1
    //MD_L & R Brake
    digitalWrite(MD_L1, LOW); // L Side Brake
    digitalWrite(MD_L2, LOW); // L Side Brake
    digitalWrite(MD_R1, LOW); // R Side Brake
    digitalWrite(MD_R2, LOW); // R Side Brake
    digitalWrite(LED, LOW); // LED OFF
    delay(100); //
    //If the serial port is LOW, set the safety device to forced OFF mode.
    if(digitalRead(PIN_RX) == LOW && digitalRead(PIN_TX) == LOW){
      SSM_Us_ON_Val = 1;     //[Important] 1 = Release user safety device 0 = OFF
      END_Val_MAIN = 30000;  //Change robot operation end judgment value MAIN (msec)
    } else{
      //pinMode(PIN_RX, INPUT);           //D0
      //pinMode(PIN_TX, INPUT);           //D1
    }
    delay(900); //Wait 1 second after powering on
    //Serial output
    if(Debug_mode_Val == 1){
      Serial.begin(115200); 
      pinMode(PIN_RX, INPUT);           //D0
      pinMode(PIN_TX, OUTPUT);           //D1
    }
    //Change the frequency division value of analogRead() 111=128,110=64,101=32,100=16,011=8,010=4,001=2,000=1
    bitWrite(ADCSRA, 0, 1); //bit0
    bitWrite(ADCSRA, 1, 0); //bit1
    bitWrite(ADCSRA, 2, 1); //bit2
    SSM_CHECK(); 
    //TIMER1 INTERRUPT SETUP
    TCCR1A = 0; // Init Timer1A
    TCCR1B = 0; // Init Timer1B
    OCR1A = 31000; //500.0msec TIMER1 INTERRUPT A <--- Cycle
    OCR1B = 50; //0.8msec TIMER1 INTERRUPT B <--- Duty
    TCCR1B |= (1 << CS12) | (1 << WGM12); // CS12 -> 1(prescaler -> 256)   CTC mode on
    //OCIEA -> 1 (enable OCR1A Interrupt)   OCIEB -> 1 (enable OCR1B Interript) 
    TIMSK1 |= (1 << OCIE1A) | (1 << OCIE1B); 
}

//TIMER1 INTERRUPT A <Processing must be completed within 0.6msec>
//Get sensor value
ISR(TIMER1_COMPA_vect){
  //Measure time to acquire sensor value START
    Time_Val1 = micros(); //watch Start
  //Measure the execution time of timer 1 interrupt A
    Time_A_Val2 = Time_A_Val1; //calcu
    Time_A_Val1 = micros(); //watch Start
    Time_A_Val3 = Time_A_Val1 - Time_A_Val2; //calcu
  //Measure the execution time of timer 1 interrupt B
    Time_B_Val2 = micros();  //watch Stop
    Time_B_Val3 = Time_B_Val2 - Time_B_Val1; //calcu
  //analogRead in Val
    if(Edge_Sensor_Disable == 0){
      Edge_FL_Val = analogRead(Edge_FL);
      Edge_FR_Val = analogRead(Edge_FR);
      //Edge_B_Val = analogRead(Edge_B);
    } else{
      Edge_FL_Val = 1023;
      Edge_FR_Val = 1023;
      //Edge_B_Val = 1023;
    }
    if(Distance_Sensor_Disable == 0){
      Distance_F_Val = analogRead(Distance_F);
    } else{
      Distance_F_Val = 0;
    }
  //D8 PIN MODE INPUT & Tone OFF
    noTone(8);                        //
    pinMode(Opt_R45, INPUT);          //D8 R45&55&80 AND BUZZER
  //digitalRead in Val
    if(Edge_Sensor_Disable == 0){
      Blade_Sens_Val = digitalRead(Blade_Sens);
    } else{
      Blade_Sens_Val = 1;
    }
    if(Opt_Sensor_Disable == 0){
      Opt_R20_Val = digitalRead(Opt_R20);
      Opt_R45_Val = digitalRead(Opt_R45);
      Opt_R80_Val = digitalRead(Opt_R80);
      Opt_L20_Val = digitalRead(Opt_L20);
      Opt_L45_Val = digitalRead(Opt_L45);
      Opt_L80_Val = digitalRead(Opt_L80);
    } else{
      Opt_R20_Val = 0;
      Opt_R45_Val = 0;
      Opt_R80_Val = 0;
      Opt_L20_Val = 0;
      Opt_L45_Val = 0;
      Opt_L80_Val = 0;
    }
    SSM_Of_Val = digitalRead(SSM_Of);
    
    //SSM_Us_Val = digitalRead(SSM_Us);//Commented out because the input was changed from IO to serial communication

    
    //Safety device forced OFF mode
    if(SSM_Us_ON_Val == 1){
      if(MODE_Val == 2||MODE_Val == 3||MODE_Val == 6){
        //SSM_Us_Val=0 if either RX/TX pin is HIGH
        if(digitalRead(PIN_RX) == HIGH || digitalRead(PIN_TX) == HIGH){
          SSM_Us_Val = 0;
        }
      }
    }
    DIP_SW1_Val = analogRead(DIP_SW1);
    DIP_SW2_Val = analogRead(DIP_SW2);
    DIP_SW3_Val = digitalRead(DIP_SW3);
    DIP_SW4_Val = digitalRead(DIP_SW4);
  //Val in Bit
    if(Blade_Sens_Val == 1){
      bitWrite(Edges_Val,Blade_Sens_B,0);
    } else{
      bitWrite(Edges_Val,Blade_Sens_B,1);
    }
    if(Edge_FL_Val > Edge_FL_Thsld){
      bitWrite(Edges_Val,Edge_FL_B,0);
    } else{
      bitWrite(Edges_Val,Edge_FL_B,1);
    }
    if(Edge_FR_Val > Edge_FR_Thsld){
      bitWrite(Edges_Val,Edge_FR_B,0);
    } else {
      bitWrite(Edges_Val,Edge_FR_B,1);
    }
    //Input the operating state to Drive_MODE_Val based on the value of MD_MODE (used to determine whether the edge sensor is disabled)
    switch (MD_MODE_Val) {
      case MD_MODE_FF:
        Drive_MODE_Val = 1;     //Brake=0, Forward=1, Backward=2, Super Turn=3
        break;
      case MD_MODE_FB:
        Drive_MODE_Val = 1;     //Brake=0, Forward=1, Backward=2, Super Turn=3
        break;
      case MD_MODE_FR:
        Drive_MODE_Val = 3;     //Brake=0, Forward=1, Backward=2, Super Turn=3
        break;
      case MD_MODE_BF:
        Drive_MODE_Val = 1;     //Brake=0, Forward=1, Backward=2, Super Turn=3
        break;
      case MD_MODE_BB:
        Drive_MODE_Val = 0;     //Brake=0, Forward=1, Backward=2, Super Turn=3
        break;
      case MD_MODE_BR:
        Drive_MODE_Val = 2;     //Brake=0, Forward=1, Backward=2, Super Turn=3
        break;
      case MD_MODE_RF:
        Drive_MODE_Val = 3;     //Brake=0, Forward=1, Backward=2, Super Turn=3
        break;
      case MD_MODE_RB:
        Drive_MODE_Val = 2;     //Brake=0, Forward=1, Backward=2, Super Turn=3
        break;
      case MD_MODE_RR:
        Drive_MODE_Val = 2;     //Brake=0, Forward=1, Backward=2, Super Turn=3
        break;
      default:
      // Executed when no case is matched
        Drive_MODE_Val = 0;     //Brake=0, Forward=1, Backward=2, Super Turn=3
    }
    //Edge sensor operation judgment (flag ON) ---> Edge_S_flag_Val
    if(MODE_Val == 3){
      //Edge sensor flag can be turned on only in VS mode
      if(bitRead(Edges_Val,Blade_Sens_B) == HIGH){
        //Ignore blade sensors when in attack mode or sensor ignore mode
        if(St_Direct_Val != 1 && St_Direct_Val != 3 && St_Direct_Val != 7){
          Edge_S_flag_Val = 1;    //Flag ON
        }
      } else{
          //Front sensor judgment
          if(Drive_MODE_Val == 0 || Drive_MODE_Val == 1){
            //If MD operation status is Brake = 0 or Forward = 1, perform Flag ON judgment
            if(bitRead(Edges_Val,Edge_FL_B) == HIGH || bitRead(Edges_Val,Edge_FR_B) == HIGH){
              //If the edge sensor left or right is ON
              MD_BB_Fst(); //Brake ON BB->MD
              Edge_S_flag_Val = 1;    //Flag ON
            } 
          }
      }
    }
    bitWrite(Opt_Val,Opt_R20_B,Opt_R20_Val);
    bitWrite(Opt_Val,Opt_R45_B,Opt_R45_Val);
    bitWrite(Opt_Val,Opt_R80_B,Opt_R80_Val);
    bitWrite(Opt_Val,Opt_L20_B,Opt_L20_Val);
    bitWrite(Opt_Val,Opt_L45_B,Opt_L45_Val);
    bitWrite(Opt_Val,Opt_L80_B,Opt_L80_Val);
    //DIP SW
    if(DIP_SW1_Val >= 512){
      bitWrite(DIP_SW_Val,DIP_SW1_B,0);
    } else{
      bitWrite(DIP_SW_Val,DIP_SW1_B,1);
    }
    if(DIP_SW2_Val >= 512){
      bitWrite(DIP_SW_Val,DIP_SW2_B,0);
    } else{
      bitWrite(DIP_SW_Val,DIP_SW2_B,1);
    }
    if(DIP_SW3_Val == 1){
      bitWrite(DIP_SW_Val,DIP_SW3_B,0);
    } else{
      bitWrite(DIP_SW_Val,DIP_SW3_B,1);
    }
    if(DIP_SW4_Val == 1){
      bitWrite(DIP_SW_Val,DIP_SW4_B,0);
    } else{
      bitWrite(DIP_SW_Val,DIP_SW4_B,1);
    }
    //Distance_F Far & Near
    if(Distance_F_Val > Distance_F_Thsld_Far){
      Distance_F_Far_Val = 1;
    } else{
      Distance_F_Far_Val = 0;
    }
    if(Distance_F_Val > Distance_F_Thsld_Near){
      Distance_F_Near_Val = 1;
    } else{
      Distance_F_Near_Val = 0;
    }
  //D8 PIN MODE OUTPUT(Processing for sharing buzzer and sensor input)
    switch (MODE_Val) {
      case 2:
        // MODE_Val is executed when is 1
        pinMode(Opt_R45, INPUT);          //D8 R45&55 AND BUZZER
        break;
      case 3:
        // MODE_Val is executed when is 2
        pinMode(Opt_R45, INPUT);          //D8 R45&55 AND BUZZER
        break;
      default:
        // Executed when no case is matched
        pinMode(Opt_R45, OUTPUT);          //D8 R45&55 AND BUZZER
    }
  //Measure time to acquire sensor value END
    Time_Val2 = micros();  //watch Stop
    Time_Val3 = Time_Val2 - Time_Val1; //calcu
}

//TIMER1 INTERRUPT B <Processing must be completed within 0.2msec>
//Operation mode change judgment & IR Receive
ISR(TIMER1_COMPB_vect){
    Time_B_Val1 = micros(); //watch Start
    if(Count_up_Disable == 0){
      //Operation mode change judgment counter
        MODE_Change_CNT_Val ++; //+1
      //Close range counter for retry
        if(Distance_F_Near_Val == 1){
          //If close range ON, count +1
          Distance_F_Near_CNT_Val ++;
        } else{
          //Clear count if close range OFF
          Distance_F_Near_CNT_Val = 0;
        }
      //Long range counter for attack detection
        if(Distance_F_Far_Val == 1){
          //If long distance ON, count +1
          Distance_F_Far_CNT_Val ++;
        } else{
          //Clear count if long distance OFF
          Distance_F_Far_CNT_Val = 0;
        }
      //Blade sensor ON time count
        if(bitRead(Edges_Val,Blade_Sens_B) == HIGH){
          //If blade sensor is ON, count +1
          Blade_ON_CNT_Val ++;
        } else{
          //If the blade sensor is OFF, clear the count
          Blade_ON_CNT_Val = 0;
        }
    }
  //Counter MAIN for determining operation stop (counts up only in VS mode and edge sensor disabled mode)
    if(MODE_Val != 3 && MODE_Val != 6){
      END_CNT_MAIN = 0; //clear count
    } else{
      END_CNT_MAIN ++; //count ＋１
    }
  //Counter SUB for determining operation stop (counts up only in VS mode and edge sensor disabled mode)
    if(MODE_Val != 3 && MODE_Val != 6){
      END_CNT_SUB = 0; //clear count
    } else if(Opt_Val == 0){
      END_CNT_SUB ++; //count ＋１
    } else{
      END_CNT_SUB = 0; //clear count
    }
  //Counter for motor PWM
    if(PWM_cnt >= 50){
      PWM_cnt = 0; //clear count
    } else{
      PWM_cnt ++;  //count ＋１
    }
  //Counter for creeping forward
    Creeping_F_CNT_Val ++; //+1
  //Delay counter for motor operation control      
    MD_Time_Cnt_Val ++; //+1
  //Opt_Sens_OR_12345678910 Retains enemy sensor ON information for the past 10ms
    Opt_Val_10 = Opt_Val_9;       //Storage sensor value shift
    Opt_Val_9 = Opt_Val_8;       //Storage sensor value shift
    Opt_Val_8 = Opt_Val_7;       //Storage sensor value shift
    Opt_Val_7 = Opt_Val_6;       //Storage sensor value shift
    Opt_Val_6 = Opt_Val_5;       //Storage sensor value shift
    Opt_Val_5 = Opt_Val_4;       //Storage sensor value shift
    Opt_Val_4 = Opt_Val_3;       //Storage sensor value shift
    Opt_Val_3 = Opt_Val_2;       //Storage sensor value shift
    Opt_Val_2 = Opt_Val_1;       //Storage sensor value shift
    Opt_Val_1 = Opt_Val;         //Storage sensor value shift
    //OR
    Opt_Val = Opt_Val | Opt_Val_2;    //Storage sensor value OR
    Opt_Val = Opt_Val | Opt_Val_3;    //Storage sensor value OR
    Opt_Val = Opt_Val | Opt_Val_4;    //Storage sensor value OR
    Opt_Val = Opt_Val | Opt_Val_5;    //Storage sensor value OR
    Opt_Val = Opt_Val | Opt_Val_6;    //Storage sensor value OR
    Opt_Val = Opt_Val | Opt_Val_7;    //Storage sensor value OR
    Opt_Val = Opt_Val | Opt_Val_8;    //Storage sensor value OR
    Opt_Val = Opt_Val | Opt_Val_9;    //Storage sensor value OR
    Opt_Val = Opt_Val | Opt_Val_10;    //Storage sensor value OR
    //Reflects processed sensor values
    Opt_Val_X = Opt_Val;         //

  //Input variables for strategic actions ---> St_Direct_Val
    if(MODE_Val != 3 && MODE_Val != 6){
      //Reflect DIP_SW input in strategy movement variables
      St_Direct_Val = DIP_SW_Val;
    }    
}


//Main routine--------------------------------------------------------------------------------------------------
void loop(){
//Standby mode ---------------------------------------------------------------------------
  //Waiting to receive player remote control
  while(SSM_Us_Val == 0){
    MODE_Val = 1 ;            //0=Initialization,1=Standby,2=Wait,3=Versus,4=KILL,5=SSM_Check,6=Edge_Disabled

    IR_reception_data_processing();
    Sensor_Val_to_Tone(); //Sound the buzzer according to the reaction of the sensor
    Sensor_Val_to_Serial(); //Output sensor value by serial communication
    if(Run_Mode == 1){
      Fst_Move_Val = codeLow; //Initial operation number input
      SSM_Us_Val = 1; //Safety device release
    }
    //Fail safe
    if(SSM_Of_Val == 1){
      SSM_CHECK(); //If SSM_Of turns ON, wait until it turns OFF.
    }
    //Safety device forced OFF mode
    if(SSM_Us_ON_Val == 1){
      //DIP SW
      if(DIP_SW1_Val >= 512){
        bitWrite(DIP_SW_Val,DIP_SW1_B,0);
      } else{
        bitWrite(DIP_SW_Val,DIP_SW1_B,1);
      }
      if(DIP_SW2_Val >= 512){
        bitWrite(DIP_SW_Val,DIP_SW2_B,0);
      } else{
        bitWrite(DIP_SW_Val,DIP_SW2_B,1);
      }
      if(DIP_SW3_Val == 1){
        bitWrite(DIP_SW_Val,DIP_SW3_B,0);
      } else{
        bitWrite(DIP_SW_Val,DIP_SW3_B,1);
      }
      if(DIP_SW4_Val == 1){
        bitWrite(DIP_SW_Val,DIP_SW4_B,0);
      } else{
        bitWrite(DIP_SW_Val,DIP_SW4_B,1);
      }
      Fst_Move_Val = DIP_SW_Val; //Initial operation number input
      SSM_Us_Val = 1; //Safety device release
    }
  }
//Wait mode ---------------------------------------------------------------------------
    //Serial communication OFF
    Serial.end();
    pinMode(PIN_RX, INPUT_PULLUP);
    pinMode(PIN_TX, INPUT_PULLUP);
  //Mario 1UP
    tone(8,Tone_E5,100);               //D8 When the board arrives, change from D8 to D13
    delay(100);
    tone(8,Tone_G5,100);               //D8 When the board arrives, change from D8 to D13
    delay(100);
    tone(8,Tone_E6,100);               //D8 When the board arrives, change from D8 to D13
    delay(100);
    tone(8,Tone_C6,100);               //D8 When the board arrives, change from D8 to D13
    delay(100);
    tone(8,Tone_D6,100);               //D8 When the board arrives, change from D8 to D13
    delay(100);
    tone(8,Tone_G6,100);               //D8 When the board arrives, change from D8 to D13
    delay(100);
    tone(8,Tone_G7,10);               //D8 When the board arrives, change from D8 to D13
    delay(500);
  //MODE change & serial data transmission
    MODE_Val = 2 ;            //0=Initialization,1=Standby,2=Wait,3=Versus,4=KILL,5=SSM_Check,6=Edge_Disabled
    Sensor_Val_to_Serial(); //Output sensor value by serial communication
  //TIMER1 interrupt frequency changed to 1kHz
    OCR1A = 62; //1.0msec TIMER1 INTERRUPT A <--- Cycle
    TIMSK1 |= (1 << OCIE1A) | (1 << OCIE1B); 
  //Wait for the official start module to turn ON.
    while(SSM_Of_Val == 0){
      MODE_Val = 2 ;            //0=Initialization,1=Standby,2=Wait,3=Versus,4=KILL,5=SSM_Check,6=Edge_Disabled
      //D13 LED an blink & BUZZER ON
      digitalWrite(LED, bitRead(millis(), 7));
      //Fail safe
      if(SSM_Us_Val == 0){
        KILL_MODE();//Brake the motor and wait for the power to be turned off
      }
    }
//Versus mode ---------------------------------------------------------------------------
  MODE_Val = 6 ;            //0=Initialization,1=Standby,2=Wait,3=Versus,4=KILL,5=SSM_Check,6=Edge_Disabled
  digitalWrite(LED, LOW); // LED OFF

//Initial behavior (branch with switch statement)-----------------------------------------------------
  switch (Fst_Move_Val) {
    case 0:
      Fst_Move_0();
      break;
    case 1:
      Fst_Move_1();
      break;
    case 2:
      Fst_Move_2();
      break;
    case 3:
      Fst_Move_3();
      break;
    case 4:
      Fst_Move_4();
      break;
    case 5:
      Fst_Move_5();
      break;
    case 6:
      Fst_Move_6();
      break;
    case 7:
      Fst_Move_7();
      break;
    case 8:
      Fst_Move_8();
      break;
    case 9:
      Fst_Move_9();
      break;
    default:
    // Executed when no case is matched
      Fst_Move_default();
  }

  Creeping_F_CNT_Val = 0;  //count clear
  MODE_Change_CNT_Val = 0;  //count clear

//Battle pattern loop----------------------------------------------------------------
  //If both player module and official module are ON, loop continues
  while(SSM_Of_Val == 1 && SSM_Us_Val == 1){
    //Stop the robot 60 seconds after starting VS mode
      if(END_CNT_MAIN > END_Val_MAIN){
        SSM_Us_Val = 0;
      }
    //Stops robot in 20 seconds without enemy sensor response
      if(END_CNT_SUB > END_Val_SUB){
        SSM_Us_Val = 0;
      }
    //robot motion branch
    if(Debug_mode_Val == 1){
    //When debug mode is ON
      St_Direct_D();
    } else{
      //Execute edge sensor reaction operation (variables are changed by TIMER1B interrupt)
      if(Edge_S_flag_Val == 1){
        MODE_Val = 6 ;            //0=Initialization,1=Standby,2=Wait,3=Versus,4=KILL,5=SSM_Check,6=Edge_Disabled
        //If Flag ON, edge sensor works
        Edge_S_Move(); //Normal Ver
        Edge_S_flag_Val = 0;  //clear flag
      }else {
          MODE_Val = 3 ;            //0=Initialization,1=Standby,2=Wait,3=Versus,4=KILL,5=SSM_Check,6=Edge_Disabled
          //Battle pattern branch (variables are changed by TIMER1B interrupt)
          switch (St_Direct_Val) {
            case 0:
              if(PWM_Test_MODE == 1){
                MD_FF(F_R_500); //Forward 500mm
                MD_BB(50); //Brake 10ms
                KILL_MODE();
              }
              St_Direct_0();
              break;
            case 1:
              if(PWM_Test_MODE == 1){
                MD_RR(F_R_500); //Reverse 500mm
                MD_BB(50); //Brake 10ms
                KILL_MODE();
              }
              St_Direct_1();
              break;
            case 2:
              if(PWM_Test_MODE == 1){
                MD_RF(RF_T_W_90); //Left 90W
                MD_BB(50); //Brake 10ms
                KILL_MODE();
              }
              St_Direct_2();
              break;
            case 3:
              if(PWM_Test_MODE == 1){
                MD_FR(FR_T_W_90); //Right 90W
                MD_BB(50); //Brake 10ms
                KILL_MODE();
              }
              St_Direct_3();
              break;
            case 4:
              if(PWM_Test_MODE == 1){
                MD_BF(BF_T_S_90); //Front left 90deg
                MD_BB(50); //Brake 10ms
                KILL_MODE();
              }
              St_Direct_4();
              break;
            case 5:
              if(PWM_Test_MODE == 1){
                MD_FB(FB_T_S_90); //Front right 90deg
                MD_BB(50); //Brake 10ms
                KILL_MODE();
              }
              St_Direct_5();
              break;
            case 6:
              if(PWM_Test_MODE == 1){
                MD_BR(BR_T_S_90); //Rear right 90deg
                MD_BB(50); //Brake 10ms
                KILL_MODE();
              }
              St_Direct_6();
              break;
            case 7:
              if(PWM_Test_MODE == 1){
                MD_RB(RB_T_S_90); //Rear left 90deg
                MD_BB(50); //Brake 10ms
                KILL_MODE();
              }
              St_Direct_7();
              break;
            case 8:
              St_Direct_8();
              break;
            case 9:
              St_Direct_9();
              break;
            case 10:
              St_Direct_10();
              break;
            case 11:
              St_Direct_11();
              break;
            case 12:
              St_Direct_12();
              break;
            case 13:
              St_Direct_13();
              break;
            case 14:
              St_Direct_14();
              break;
            case 15:
              St_Direct_15();
              break;
            default:
            // Executed when no case is matched
              St_Direct_default();
          }
      }
    }
  }
  KILL_MODE();//Brake the motor and wait for the power to be turn}

}