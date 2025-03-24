#include <Arduino.h>
#include "variables.h"
//Motor Driver control-------------------------------------------------------------------------------------------------
  //Fast Ver:MD IO Write, MD_bit_XX -> MD_bit 
  void MD_IO_Write(int MD_bit){
    //If the edge sensor flag is ON, stop processing.
    if(Edge_S_flag_Val == 1){
      return; 
    }
    //
    //L_PWM:PB2//L_DIR:PB1//R_PWM:PD6//R_DIR:PD5
    switch (MD_bit) {
      case MD_bit_FF:
        //MD_bit_FF 0b00001011       //(D10,D9,D6,D5)
        PORTD |= _BV(5);  //D5  R_DIR = HIGH
        PORTD |= _BV(6);  //D6  R_PWM = HIGH
        PORTB &= ~_BV(1); //D9  L_DIR = LOW
        PORTB |= _BV(2);  //D10 L_PWM = HIGH
        break;
      case MD_bit_FB:
        //MD_bit_FB 0b00001000       //(D10,D9,D6,D5)
        PORTD &= ~_BV(5); //D5  R_DIR = LOW
        PORTD &= ~_BV(6); //D6  R_PWM = LOW
        PORTB &= ~_BV(1); //D9  L_DIR = LOW
        PORTB |= _BV(2);  //D10 L_PWM = HIGH
        break;
      case MD_bit_FR:
        //MD_bit_FR 0b00001010       //(D10,D9,D6,D5)
        PORTD &= ~_BV(5); //D5  R_DIR = LOW
        PORTD |= _BV(6);  //D6  R_PWM = HIGH
        PORTB &= ~_BV(1); //D9  L_DIR = LOW
        PORTB |= _BV(2);  //D10 L_PWM = HIGH
        break;
      case MD_bit_BF:
        //MD_bit_BF 0b00000011       //(D10,D9,D6,D5)
        PORTD |= _BV(5);  //D5  R_DIR = HIGH
        PORTD |= _BV(6);  //D6  R_PWM = HIGH
        PORTB &= ~_BV(1); //D9  L_DIR = LOW
        PORTB &= ~_BV(2); //D10 L_PWM = LOW
        break;
      case MD_bit_BB:
        //MD_bit_BB 0b00000000       //(D10,D9,D6,D5)
        PORTD &= ~(_BV(5) | _BV(6)); //L1 & L2 = LOW
        PORTB &= ~(_BV(1) | _BV(2)); //R1 & R2 = LOW
        break;
      case MD_bit_BR:
        //MD_bit_BR 0b00000010       //(D10,D9,D6,D5)
        PORTD &= ~_BV(5); //D5  R_DIR = LOW
        PORTD |= _BV(6);  //D6  R_PWM = HIGH
        PORTB &= ~_BV(1); //D9  L_DIR = LOW
        PORTB &= ~_BV(2); //D10 L_PWM = LOW
        break;
      case MD_bit_RF:
        //MD_bit_RF 0b00001111       //(D10,D9,D6,D5)
        PORTD |= _BV(5);  //D5  R_DIR = HIGH
        PORTD |= _BV(6);  //D6  R_PWM = HIGH
        PORTB |= _BV(1);  //D9  L_DIR = HIGH
        PORTB |= _BV(2);  //D10 L_PWM = HIGH
        break;
      case MD_bit_RB:
        //MD_bit_RB 0b00001100       //(D10,D9,D6,D5)
        PORTD &= ~_BV(5); //D5  R_DIR = LOW
        PORTD &= ~_BV(6); //D6  R_PWM = LOW
        PORTB |= _BV(1);  //D9  L_DIR = HIGH
        PORTB |= _BV(2);  //D10 L_PWM = HIGH
        break;
      case MD_bit_RR:
        //MD_bit_RR 0b00001110       //(D10,D9,D6,D5)
        PORTD &= ~_BV(5); //D5  R_DIR = LOW
        PORTD |= _BV(6);  //D6  R_PWM = HIGH
        PORTB |= _BV(1);  //D9  L_DIR = HIGH
        PORTB |= _BV(2);  //D10 L_PWM = HIGH
        break;
      default:
      // Executed when no case is matched
        //MD_bit_BB 0b00000000 //R2,R1,L2,L1
        //R2:PB2//R1:PB1//L2:PD6//L1:PD5
        PORTD &= ~(_BV(5) | _BV(6)); //L1 & L2 = LOW
        PORTB &= ~(_BV(1) | _BV(2)); //R1 & R2 = LOW
    }
  }

  //MOTOR PWM (PWM control of motor with Timer1 counter)
  void MD_PWM(){
    switch (MD_MODE_Val) {
      case MD_MODE_FF:
        //No brake
        if(PWM_cnt <= PWM_LF && PWM_cnt <= PWM_RF){
          MD_IO_Write(MD_bit_FF); 
        }
        //left and right brakes
        if(PWM_cnt > PWM_LF && PWM_cnt > PWM_RF){
          MD_IO_Write(MD_bit_BB); 
        }
        //left brake
        if(PWM_cnt > PWM_LF && PWM_cnt <= PWM_RF){
          MD_IO_Write(MD_bit_BF); 
        }
        //right brake
        if(PWM_cnt <= PWM_LF && PWM_cnt > PWM_RF){
          MD_IO_Write(MD_bit_FB); 
        }
        break;
      case MD_MODE_FB:
        //No brake
        if(PWM_cnt <= PWM_LF){
          MD_IO_Write(MD_bit_FB); 
        }
        //left and right brakes
        if(PWM_cnt > PWM_LF){
          MD_IO_Write(MD_bit_BB); 
        }
        break;
      case MD_MODE_FR:
        //No brake
        if(PWM_cnt <= PWM_LF && PWM_cnt <= PWM_RR){
          MD_IO_Write(MD_bit_FR); 
        }
        //left and right brakes
        if(PWM_cnt > PWM_LF && PWM_cnt > PWM_RR){
          MD_IO_Write(MD_bit_BB); 
        }
        //left brake
        if(PWM_cnt > PWM_LF && PWM_cnt <= PWM_RR){
          MD_IO_Write(MD_bit_BR); 
        }
        //right brake
        if(PWM_cnt <= PWM_LF && PWM_cnt > PWM_RR){
          MD_IO_Write(MD_bit_FB); 
        }
        break;
      case MD_MODE_BF:
        //No brake
        if(PWM_cnt <= PWM_RF){
          MD_IO_Write(MD_bit_BF); 
        }
        //left and right brakes
        if(PWM_cnt > PWM_RF){
          MD_IO_Write(MD_bit_BB); 
        }
        break;
      case MD_MODE_BB:
        //left and right brakes
        MD_IO_Write(MD_bit_BB); 
        break;
      case MD_MODE_BR:
        //No brake
        if(PWM_cnt <= PWM_RR){
          MD_IO_Write(MD_bit_BR); 
        }
        //left and right brakes
        if(PWM_cnt > PWM_RR){
          MD_IO_Write(MD_bit_BB); 
        }
        break;
      case MD_MODE_RF:
        //No brake
        if(PWM_cnt <= PWM_LR && PWM_cnt <= PWM_RF){
          MD_IO_Write(MD_bit_RF); 
        }
        //left and right brakes
        if(PWM_cnt > PWM_LR && PWM_cnt > PWM_RF){
          MD_IO_Write(MD_bit_BB); 
        }
        //left brake
        if(PWM_cnt > PWM_LR && PWM_cnt <= PWM_RF){
          MD_IO_Write(MD_bit_BF); 
        }
        //right brake
        if(PWM_cnt <= PWM_LR && PWM_cnt > PWM_RF){
          MD_IO_Write(MD_bit_RB); 
        }
        break;
      case MD_MODE_RB:
        //No brake
        if(PWM_cnt <= PWM_LR){
          MD_IO_Write(MD_bit_RB); 
        }
        //left and right brakes
        if(PWM_cnt > PWM_LR){
          MD_IO_Write(MD_bit_BB); 
        }
        break;
      case MD_MODE_RR:
        //No brake
        if(PWM_cnt <= PWM_LR && PWM_cnt <= PWM_RR){
          MD_IO_Write(MD_bit_RR); 
        }
        //left and right brakes
        if(PWM_cnt > PWM_LR && PWM_cnt > PWM_RR){
          MD_IO_Write(MD_bit_BB); 
        }
        //left brake
        if(PWM_cnt > PWM_LR && PWM_cnt <= PWM_RR){
          MD_IO_Write(MD_bit_BR); 
        }
        //right brake
        if(PWM_cnt <= PWM_LR && PWM_cnt > PWM_RR){
          MD_IO_Write(MD_bit_RB); 
        }
        break;
      default:
      // Executed when no case is matched
        MD_BB(50);     //brake
    }
  }
  //MD -> FF, msec -> MD_Time
  void MD_FF(int MD_Time){
    //If the edge sensor flag is ON, stop processing.
    if(Edge_S_flag_Val == 1){
      return; 
    }
    //MD operation judgment
    if(bitRead(MD_MODE_Val,MD_MODE_bit_Loc_FF) == HIGH){
      MD_BB(50);//brake 50ms
    }
    //MD STATUS Write
    MD_MODE_Val = MD_MODE_FF; //
    //MD_bit_Write
    //MD_IO_Write(MD_bit_FF); //FF->MD
    //MD_delay time cnt
    MD_Time_Cnt_Val = 0; //MD_Time_Cnt_Val is counted up by TIMER1B interrupt
    PWM_cnt = 0; //PWM counter clear
    while(MD_Time_Cnt_Val < MD_Time){
      //Loop for the time (msec) specified by MD_Time
      MD_PWM(); //Motor PWM processing
      if(Edge_S_flag_Val == 1){
        //If the Edge sensor responds, exit the loop
        break; 
      }
    }
  }

  //MD -> FB, msec -> MD_Time
  void MD_FB(int MD_Time){
    //If the edge sensor flag is ON, stop processing.
    if(Edge_S_flag_Val == 1){
      return; 
    }
    //MD operation judgment
    if(bitRead(MD_MODE_Val,MD_MODE_bit_Loc_FB) == HIGH){
      MD_BB(50);//brake 50ms
    }
    //MD STATUS Write
    MD_MODE_Val = MD_MODE_FB; //
    //MD_bit_Write
    MD_IO_Write(MD_bit_FB); //FB->MD
    //MD_delay time cnt
    MD_Time_Cnt_Val = 0; //MD_Time_Cnt_Val is counted up by TIMER1B interrupt
    PWM_cnt = 0; //PWM counter clear
    while(MD_Time_Cnt_Val < MD_Time){
      //Loop for the time (msec) specified by MD_Time
      //MD_PWM(); //Motor PWM processing
      if(Edge_S_flag_Val == 1){
        //If the Edge sensor responds, exit the loop
        break; 
      }
    }
  }

  //MD -> FR, msec -> MD_Time
  void MD_FR(int MD_Time){
    //If the edge sensor flag is ON, stop processing.
    if(Edge_S_flag_Val == 1){
      return; 
    }
    //MD operation judgment
    if(bitRead(MD_MODE_Val,MD_MODE_bit_Loc_FR) == HIGH){
      MD_BB(50);//brake 50ms
    }
    //MD STATUS Write
    MD_MODE_Val = MD_MODE_FR; //
    //MD_bit_Write
    MD_IO_Write(MD_bit_FR); //FR->MD
    //MD_delay time cnt
    MD_Time_Cnt_Val = 0; //MD_Time_Cnt_Val is counted up by TIMER1B interrupt
    PWM_cnt = 0; //PWM counter clear
    while(MD_Time_Cnt_Val < MD_Time){
      //Loop for the time (msec) specified by MD_Time
      //MD_PWM(); //Motor PWM processing
      if(Edge_S_flag_Val == 1){
        //If the Edge sensor responds, exit the loop
        break; 
      }
    }
  }

  //MD -> BF, msec -> MD_Time
  void MD_BF(int MD_Time){
    //If the edge sensor flag is ON, stop processing.
    if(Edge_S_flag_Val == 1){
      return; 
    }
    //MD operation judgment
    if(bitRead(MD_MODE_Val,MD_MODE_bit_Loc_BF) == HIGH){
      MD_BB(50);//brake 50ms
    }
    //MD STATUS Write
    MD_MODE_Val = MD_MODE_BF; //
    //MD_bit_Write
    MD_IO_Write(MD_bit_BF); //BF->MD
    //MD_delay time cnt
    MD_Time_Cnt_Val = 0; //MD_Time_Cnt_Val is counted up by TIMER1B interrupt
    PWM_cnt = 0; //PWM counter clear
    while(MD_Time_Cnt_Val < MD_Time){
      //Loop for the time (msec) specified by MD_Time
      //MD_PWM(); //Motor PWM processing
      if(Edge_S_flag_Val == 1){
        //If the Edge sensor responds, exit the loop
        break; 
      }
    }
  }

  //Fast output of brake command
  void MD_BB_Fst(){
    PORTD &= ~(_BV(5) | _BV(6)); //L1 & L2 = LOW
    PORTB &= ~(_BV(1) | _BV(2)); //R1 & R2 = LOW

    //MD_L1 5 //D5 PD5
    //MD_L2 6 //D6 PD6
    //MD_R1 9 //D9 PB1
    //MD_R2 10//D10 PB2
  }
  
  //MD -> BB, msec -> MD_Time
  void MD_BB(int MD_Time_BB){
    //If the edge sensor flag is ON, stop processing.
    if(Edge_S_flag_Val == 1){
      return; 
    }
    //MD STATUS Write
    MD_MODE_Val = MD_MODE_BB; //
    //MD_bit_Write
    MD_IO_Write(MD_bit_BB); //BB->MD
    //If the edge sensor flag is ON, stop processing.
    if(Edge_S_flag_Val == 1){
      return; 
    }
    delay(MD_Time_BB);
  }

  //MD -> BR, msec -> MD_Time
  void MD_BR(int MD_Time){
    //If the edge sensor flag is ON, stop processing.
    if(Edge_S_flag_Val == 1){
      return; 
    }
    //MD operation judgment
    if(bitRead(MD_MODE_Val,MD_MODE_bit_Loc_BR) == HIGH){
      MD_BB(50);//brake 50ms
    }
    //MD STATUS Write
    MD_MODE_Val = MD_MODE_BR; //
    //MD_bit_Write
    MD_IO_Write(MD_bit_BR); //BR->MD
    //MD_delay time cnt
    MD_Time_Cnt_Val = 0; //MD_Time_Cnt_Val is counted up by TIMER1B interrupt
    PWM_cnt = 0; //PWM counter clear
    while(MD_Time_Cnt_Val < MD_Time){
      //Loop for the time (msec) specified by MD_Time
      //MD_PWM(); //Motor PWM processing
      if(Edge_S_flag_Val == 1){
        //If the Edge sensor responds, exit the loop
        break; 
      }
    }
  }

  //MD -> RF, msec -> MD_Time
  void MD_RF(int MD_Time){
    //If the edge sensor flag is ON, stop processing.
    if(Edge_S_flag_Val == 1){
      return; 
    }
    //MD operation judgment
    if(bitRead(MD_MODE_Val,MD_MODE_bit_Loc_RF) == HIGH){
      MD_BB(50);//brake 50ms
    }
    //MD STATUS Write
    MD_MODE_Val = MD_MODE_RF; //
    //MD_bit_Write
    MD_IO_Write(MD_bit_RF); //RF->MD
    //MD_delay time cnt
    MD_Time_Cnt_Val = 0; //MD_Time_Cnt_Val is counted up by TIMER1B interrupt
    PWM_cnt = 0; //PWM counter clear
    while(MD_Time_Cnt_Val < MD_Time){
      //Loop for the time (msec) specified by MD_Time
      //MD_PWM(); //Motor PWM processing
      if(Edge_S_flag_Val == 1){
        //If the Edge sensor responds, exit the loop
        break; 
      }
    }
  }

  //MD -> RB, msec -> MD_Time
  void MD_RB(int MD_Time){
    //If the edge sensor flag is ON, stop processing.
    if(Edge_S_flag_Val == 1){
      return; 
    }
    //MD operation judgment
    if(bitRead(MD_MODE_Val,MD_MODE_bit_Loc_RB) == HIGH){
      MD_BB(50);//brake 50ms
    }
    //MD STATUS Write
    MD_MODE_Val = MD_MODE_RB; //
    //MD_bit_Write
    MD_IO_Write(MD_bit_RB); //RB->MD
    //MD_delay time cnt
    MD_Time_Cnt_Val = 0; //MD_Time_Cnt_Val is counted up by TIMER1B interrupt
    PWM_cnt = 0; //PWM counter clear
    while(MD_Time_Cnt_Val < MD_Time){
      //Loop for the time (msec) specified by MD_Time
      //MD_PWM(); //Motor PWM processing
      if(Edge_S_flag_Val == 1){
        //If the Edge sensor responds, exit the loop
        break; 
      }
    }
  }

  //MD -> RR, msec -> MD_Time
  void MD_RR(int MD_Time){
    //If the edge sensor flag is ON, stop processing.
    if(Edge_S_flag_Val == 1){
      return; 
    }
    //MD operation judgment
    if(bitRead(MD_MODE_Val,MD_MODE_bit_Loc_RR) == HIGH){
      MD_BB(50);//brake 50ms
    }
    //MD STATUS Write
    MD_MODE_Val = MD_MODE_RR; //
    //MD_bit_Write
    //MD_IO_Write(MD_bit_RR); //RR->MD
    //MD_delay time cnt
    MD_Time_Cnt_Val = 0; //MD_Time_Cnt_Val is counted up by TIMER1B interrupt
    PWM_cnt = 0; //PWM counter clear
    while(MD_Time_Cnt_Val < MD_Time){
      //Loop for the time (msec) specified by MD_Time
      MD_PWM(); //Motor PWM processing
      if(Edge_S_flag_Val == 1){
        //If the Edge sensor responds, exit the loop
        break; 
      }
    }
  }
