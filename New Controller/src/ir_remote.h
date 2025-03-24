#include <Arduino.h>
#include "variables.h"
//IR reception---------------------------------------------------------
  //If 8bit data can be received, save the value in IR_Data_Val
  void bit_data_SAVE() {
        if(IR_Data_Read_bit == 8){
          IR_Data_Val = 0;
          //DATA0 -> IR_Data_Val
          if(time_IR_Data0 > Limit_T_D0_upper){
            bitWrite(IR_Data_Val,0,1);
          } else{
            bitWrite(IR_Data_Val,0,0);
          }
          //DATA1 -> IR_Data_Val
          if(time_IR_Data1 > Limit_T_D0_upper){
            bitWrite(IR_Data_Val,1,1);
          } else{
            bitWrite(IR_Data_Val,1,0);
          }
          //DATA2 -> IR_Data_Val
          if(time_IR_Data2 > Limit_T_D0_upper){
            bitWrite(IR_Data_Val,2,1);
          } else{
            bitWrite(IR_Data_Val,2,0);
          }
          //DATA3 -> IR_Data_Val
          if(time_IR_Data3 > Limit_T_D0_upper){
            bitWrite(IR_Data_Val,3,1);
          } else{
            bitWrite(IR_Data_Val,3,0);
          }
          //DATA4 -> IR_Data_Val
          if(time_IR_Data4 > Limit_T_D0_upper){
            bitWrite(IR_Data_Val,4,1);
          } else{
            bitWrite(IR_Data_Val,4,0);
          }
          //DATA5 -> IR_Data_Val
          if(time_IR_Data5 > Limit_T_D0_upper){
            bitWrite(IR_Data_Val,5,1);
          } else{
            bitWrite(IR_Data_Val,5,0);
          }
          //DATA6 -> IR_Data_Val
          if(time_IR_Data6 > Limit_T_D0_upper){
            bitWrite(IR_Data_Val,6,1);
          } else{
            bitWrite(IR_Data_Val,6,0);
          }
          //DATA7 -> IR_Data_Val
          if(time_IR_Data7 > Limit_T_D0_upper){
            bitWrite(IR_Data_Val,7,1);
          } else{
            bitWrite(IR_Data_Val,7,0);
          }
        }
  }
  // Loop until 8bit data is received
  void bit_data_receive() {
    IR_Data_Read_bit = 0;
    while(IR_Data_Read_bit <= 7){
      if (digitalRead(PIN_TX) == HIGH) {
        time_IR_T_off = micros();
        time_IR_Frame = time_IR_T_off;
        time_IR_Leader_0 =  time_IR_T_off - time_IR_T_on;
        while(digitalRead(PIN_TX) == HIGH){
          time_Now2 = micros();
          time_Now1 = time_Now2 - time_IR_T_off;
          if(time_Now1 > Limit_T_D1_upper){
            IR_Data_Read_bit = 256;
            break;
          }
        }
        time_IR_T_on = micros();
        time_IR_Data_Now = time_IR_T_on - time_IR_T_off;
        switch (IR_Data_Read_bit) {
          case 7:
            time_IR_Data0 = time_IR_Data_Now;
            break;
          case 6:
            time_IR_Data1 = time_IR_Data_Now;
            break;
          case 5:
            time_IR_Data2 = time_IR_Data_Now;
            break;
          case 4:
            time_IR_Data3 = time_IR_Data_Now;
            break;
          case 3:
            time_IR_Data4 = time_IR_Data_Now;
            break;
          case 2:
            time_IR_Data5 = time_IR_Data_Now;
            break;
          case 1:
            time_IR_Data6 = time_IR_Data_Now;
            break;
          case 0:
            time_IR_Data7 = time_IR_Data_Now;
            break;
        }
        IR_Data_Read_bit ++;
      }
      time_IR_Frame_Now2 = micros();
      time_IR_Frame_Now1 = time_IR_Frame_Now2 - time_IR_Frame;
      if(time_IR_Frame_Now1 > Limit_Time_L_Frame){
        IR_Data_Read_bit = 256;
        break;
      }
    }
  }
  // Infrared remote control reception data processing
  void IR_reception_data_processing() {
    if (IR_Data_Read_bit == 8) {

      Run_Mode = 1;


      //  Serial.println(codeLow, DEC);

      IR_Data_Val = 256;
      IR_Data_Read_bit = 0;
      delay(400);
    } else {
      //digitalWrite(LED, LOW);
      // When the IR pin becomes LOW
      if (digitalRead(PIN_TX) == LOW) {
        //digitalWrite(LED, HIGH);
        time_IR_T_off = micros();
        time_IR_Frame = time_IR_T_off;
        // Wait for IR pin to go HIGH
        while(digitalRead(PIN_TX) == LOW){
          time_Now2 = micros();
          time_Now1 = time_Now2 - time_IR_T_off;
          if(time_Now1 > Limit_Time_L_upper){
            break;
          }
        }
        //Calculate and save LOW time
        time_IR_T_on = micros();
        time_IR_Leader = time_IR_T_on - time_IR_T_off;
        //If the LOW time (reader code ON time) is within the range
        if(time_IR_Leader < Limit_Time_L_upper && time_IR_Leader > Limit_Time_L_lower) {
          //Reflect in standard time
          Base_1T = time_IR_Leader / 8;
          Limit_T_D0_upper = Base_1T * 1.25;
          Limit_T_D1_upper = Base_1T * 3.75;
          Limit_Time_L2_upper = Base_1T * 5;
          Limit_Time_L2_lower = Base_1T * 3;

          // Wait for IR pin to go LOW
          while(digitalRead(PIN_TX) == HIGH){
            //If the HIGH time exceeds the limit, stop processing
            if(time_IR_Leader_0 > Limit_Time_L2_upper){
              IR_Data_Read_bit = 256;
              break;
            }
            time_IR_T_off = micros();
            time_IR_Frame = time_IR_T_off;
            time_IR_Leader_0 =  time_IR_T_off - time_IR_T_on;
          }
          //Calculate and save HIGH time (reader code OFF time)
          time_IR_T_off = micros();
          time_IR_Leader = time_IR_T_off - time_IR_T_on;
          //If the HIGH time (reader code OFF time) is within the range
          if(time_IR_Leader < Limit_Time_L2_upper && time_IR_Leader > Limit_Time_L2_lower) {
            // Wait for IR pin to go HIGH (Leader code 2nd time)
            while(digitalRead(PIN_TX) == LOW){
              time_Now2 = micros();
              time_Now1 = time_Now2 - time_IR_T_off;
              if(time_Now1 > Limit_Time_L_upper){
                break;
              }
            }
            //Calculate and save LOW time
            time_IR_T_on = micros();
            time_IR_Leader = time_IR_T_on - time_IR_T_off;
            //If the LOW time (reader code ON time) is within the range
            if(time_IR_Leader < Limit_Time_L_upper && time_IR_Leader > Limit_Time_L_lower) {
              // Wait for IR pin to go LOW
              while(digitalRead(PIN_TX) == HIGH){
                //If the HIGH time exceeds the limit, stop processing
                if(time_IR_Leader_0 > Limit_Time_L2_upper){
                  IR_Data_Read_bit = 256;
                  break;
                }
                time_IR_T_off = micros();
                time_IR_Frame = time_IR_T_off;
                time_IR_Leader_0 =  time_IR_T_off - time_IR_T_on;
              }
              //Calculate and save HIGH time (reader code OFF time)
              time_IR_T_off = micros();
              time_IR_Leader = time_IR_T_off - time_IR_T_on;
              if(time_IR_Leader < Limit_Time_L2_upper && time_IR_Leader > Limit_Time_L2_lower) {
                bit_data_receive();
                bit_data_SAVE();
                IR_Data_H = IR_Data_Val;
                bit_data_receive();
                bit_data_SAVE();
                IR_Data_L = IR_Data_Val;
                IR_Data_Val &= IR_Data_H;
                if(IR_Data_Val == 0){
                  codeLow = IR_Data_H & 0b00001111;
                  //codeLow = IR_Data_H & 0b11110000;
                  //codeLow = codeLow >> 4;
                }
              }
            }
          }
        }
        //clear data
        time_IR_Frame = 0;
        time_IR_Frame_Now1 = 0;
        time_IR_Frame_Now2 = 0;
        time_IR_T_off = 0;
        time_IR_T_on = 0;
        time_IR_Leader = 0;
        time_IR_Data0 = 0;
        time_IR_Data1 = 0;
        time_IR_Data2 = 0;
        time_IR_Data3 = 0;
        time_IR_Data4 = 0;
        time_IR_Data5 = 0;
        time_IR_Data6 = 0;
        time_Now1 = 0;
        time_Now2 = 0;
      }
    }
  }
//---------------------------------------------------------------