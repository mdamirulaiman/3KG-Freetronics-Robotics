#include <Arduino.h>
//All "0" when playing against each other------------------------------------------------------------------------------------------------------
  //Mode OFF setting
    volatile int Edge_Sensor_Test = 0;          //[Important] 1 = ON, 0 = off The edge sensor enters kill mode after operation
    volatile int Edge_Sensor_Disable = 0;       //[Important] 1 = ON, 0 = off Edge sensor disabled
    volatile int Opt_Sensor_Disable = 0;        //[Important] 1 = ON, 0 = off Enemy sensor disabled
    volatile int Distance_Sensor_Disable = 0;   //[Important] 1=ON,0=OFF Distance sensor disabled
    volatile int Count_up_Disable = 0;          //[Important] 1=ON, 0=OFF Count up disabled
    volatile int PWM_Test_MODE = 0;             //[Important] 1=ON, 0=OFF Strategy 0 is 500mm forward, 1 is 500mm backward.
    volatile int Attack_to_STOP = 0;            //[Important] 1=ON, 0=OFF Replace advance in strategy 1 (attack mode) with stop
      //Setting debug mode
    volatile int Debug_mode_Val = 0;     //[Important] Debug flag 1 = debug (edge sensor operation OFF), 0 = normal operation
  //Forced ON mode
    volatile int SSM_Us_ON_Val = 0;     //[Important] 1 = Release the user's safety device 0 = OFF (Changed to the method of setting the serial pin to LOW)
  //MINI_dohyo mode
    volatile int MINI_dohyo_MODE = 0;     //[Important] 1 = Reduce the amount of movement so that you can move in the MINI dohyo 0 = OFF
//---------------------------------------------------------------------------------------------------------------------------
    volatile int Opt_Sensor_Disable_Sens_Val = 10; //Enemy sensor value when enemy sensor disabled ON ［7,8,9,10,11,12,13を入力すること］

//Setting the operation time limit
    volatile unsigned int END_CNT_SUB = 0;  //Counter for determining robot movement completion (count reset when enemy sensor is turned on)
    volatile unsigned int END_Val_SUB = 20000;  //Robot operation end judgment value SUB (msec)
    volatile unsigned int END_CNT_MAIN = 0; //Counter for determining robot motion completion (count during VS mode)
    volatile unsigned int END_Val_MAIN = 60000;  //Robot operation end judgment value MAIN (msec)
//Setting motor PWM output
    int PWM_cnt = 0; //Counter for PWM control of motor (counts from 0-50)
    int PWM_LF = 50; //Left: PWM output of the forward motor（MIN 0-50 MAX）
    int PWM_RF = 48; //Right: PWM output of the forward motor（MIN 0-50 MAX）
    int PWM_LR = 49; //Left: Reverse motor PWM output（MIN 0-50 MAX）
    int PWM_RR = 50; //Right: Reverse motor PWM output（MIN 0-50 MAX）
//Declaration of global variables
    volatile int Opt_Val = 0;            //BIN  L80,L45,L20,R20,R45,R80(b00111111) 1=ON,0=OFF
    volatile int Opt_Val_1 = 0;          //For sensor value storage
    volatile int Opt_Val_2 = 0;          //For sensor value storage
    volatile int Opt_Val_3 = 0;          //For sensor value storage
    volatile int Opt_Val_4 = 0;          //For sensor value storage
    volatile int Opt_Val_5 = 0;          //For sensor value storage
    volatile int Opt_Val_6 = 0;          //For sensor value storage
    volatile int Opt_Val_7 = 0;          //For sensor value storage
    volatile int Opt_Val_8 = 0;          //For sensor value storage
    volatile int Opt_Val_9 = 0;          //For sensor value storage
    volatile int Opt_Val_10 = 0;         //For sensor value storage
    volatile int Opt_Val_X = 0;          //Sensor value For operation judgment (ON information retained for 10ms) BIN  L80,L45,L20,R20,R45,R80(b00111111) 1=ON,0=OFF
    volatile int Edges_Val = 0;          //BIN  FL,FR,B,BladeSens(b00001111) 1=ON,0=OFF
    volatile int DIP_SW_Val = 0;         //BIN  SW4,SW3,SW2,SW1(b00001111) 1=ON,0=OFF
    volatile int Blade_Sens_Val = 0;     //HIGH or LOW
    volatile int Opt_R20_Val = 0;        //HIGH or LOW
    volatile int Opt_R45_Val = 0;        //HIGH or LOW
    volatile int Opt_R80_Val = 0;        //HIGH or LOW
    volatile int Opt_L20_Val = 0;        //HIGH or LOW
    volatile int Opt_L45_Val = 0;        //HIGH or LOW
    volatile int Opt_L80_Val = 0;        //HIGH or LOW
    volatile int SSM_Of_Val = 0;         //HIGH or LOW
    volatile int SSM_Us_Val = 0;         //HIGH or LOW
    volatile int DIP_SW1_Val = 0;        //0-1023
    volatile int DIP_SW2_Val = 0;        //0-1023
    volatile int DIP_SW3_Val = 0;        //HIGH or LOW
    volatile int DIP_SW4_Val = 0;        //HIGH or LOW
    volatile int Edge_FL_Val = 0;        //0-1023
    volatile int Edge_FR_Val = 0;        //0-1023
    volatile int Edge_B_Val = 0;         //0-1023
    volatile int Distance_F_Val = 0;     //0-1023
    volatile int Distance_F_Near_Val = 0;//HIGH or LOW 1=ON,0=OFF
    volatile int Distance_F_Near_CNT_Val = 0; //CNT UP TIMER1B
    volatile int Distance_F_Far_Val = 0; //HIGH or LOW 1=ON,0=OFF
    volatile int Distance_F_Far_CNT_Val = 0; //CNT UP TIMER1B
    volatile int MODE_Val = 0;           //0=Initialization,1=Standby,2=Wait,3=Versus,4=KILL,5=SSM_Check,6=Edge_Disabled
    volatile int MD_MODE_Val = 0;        //Motor driver operating mode now & MD operation permission bit
    volatile int MD_Time_Cnt_Val = 0;    //MD delay Onry
    volatile int MD_L1_Val = 0;          //HIGH or LOW
    volatile int MD_L2_Val = 0;          //HIGH or LOW
    volatile int MD_R1_Val = 0;          //HIGH or LOW
    volatile int MD_R2_Val = 0;          //HIGH or LOW
    volatile int Fst_Move_Val = 0;       //first move
    volatile int St_Direct_Val = 0;      //strategic directive
    volatile int Edge_S_flag_Val = 0;    //Edge sensor flag
    volatile int Opt_Angle_Val = 0;      //7,8,9,10,11,12,13
    volatile int Drive_MODE_Val = 0;     //Brake=0, Forward=1, Backward=2, Super Turn=3
    volatile int Creeping_F_MODE_Val = 0;    //0=BB,1=BF15,2=FB30,3=BF30
    volatile int Creeping_F_CNT_Val = 0;     //CNT UP TIMER1B
    volatile int MODE_Change_CNT_Val = 0;  //CNT UP TIMER1B
    volatile int Blade_ON_CNT_Val = 0;  //CNT UP TIMER1B
    volatile int Back_CNT_Val = 0;  //Back CNT UP. motor move
    volatile unsigned long Time_Val1 = 0;  //micros() Start
    volatile unsigned long Time_Val2 = 0;  //micros() Stop
    volatile unsigned long Time_Val3 = 0;  //micros() Stop - Start
    volatile unsigned long Time_A_Val1 = 0;  //micros() Start
    volatile unsigned long Time_A_Val2 = 0;  //micros() Stop
    volatile unsigned long Time_A_Val3 = 0;  //micros() Stop - Start
    volatile unsigned long Time_B_Val1 = 0;  //micros() Start
    volatile unsigned long Time_B_Val2 = 0;  //micros() Stop
    volatile unsigned long Time_B_Val3 = 0;  //micros() Stop - Start
    volatile unsigned long millis_cnt1 = 0;  //millis() count Val
    volatile unsigned long millis_cnt2 = 0;  //millis() count Val
    volatile unsigned long millis_cnt3 = 0;  //millis() count Val
    int PIN_RX_Val = 0; //Save pin values when using serial pins as IO
    int PIN_TX_Val = 0; //Save pin values when using serial pins as IO
    int Tone_switch_cnt = 0; //ToneUsed as a function to wait for the time when the sound is being played (so that you can exit while waiting)
//constant definition
  //Sensor ON threshold
    #define Distance_F_Thsld_Far 200    //A6 Far 0-500 Near -> 300mm
    #define Distance_F_Thsld_Near 400   //A6 Far 0-500 Near -> 100mm
    #define Edge_FL_Thsld 100           //A3 Bright 0-1023 Dark
    #define Edge_FR_Thsld 100           //A2 Bright 0-1023 Dark
    #define Edge_B_Thsld 600            //A1 Bright 0-1023 Dark(No Sensor)
  //MD operation permission bit 0=OK,1=NG ->bit(7:FF,6:FB,5:FR,4:BF,3:BR.2:RF,1:RB,0:RR) -> MD_MODE_Val
    #define MD_MODE_FF 0b00101111        //
    #define MD_MODE_FB 0b00000111        //
    #define MD_MODE_FR 0b10010111        //
    #define MD_MODE_BF 0b00101001        //
    #define MD_MODE_BB 0b00000000        //
    #define MD_MODE_BR 0b10010100        //
    #define MD_MODE_RF 0b11101001        //
    #define MD_MODE_RB 0b11100000        //
    #define MD_MODE_RR 0b11110100        //
  //Location of MD operation permission bit
    #define MD_MODE_bit_Loc_FF 7        //
    #define MD_MODE_bit_Loc_FB 6        //
    #define MD_MODE_bit_Loc_FR 5        //
    #define MD_MODE_bit_Loc_BF 4        //
    #define MD_MODE_bit_Loc_BR 3        //
    #define MD_MODE_bit_Loc_RF 2        //
    #define MD_MODE_bit_Loc_RB 1        //
    #define MD_MODE_bit_Loc_RR 0        //
  //Pololu MD ここは殆ど使われていない［MD_IO_Write］を直接編集すること
  //MD operation bit 0=LOW,1=HIGH (D10,D9,D6,D5)
    #define MD_bit_FF 0b00001011       //
    #define MD_bit_FB 0b00001000       //
    #define MD_bit_FR 0b00001010       //
    #define MD_bit_BF 0b00000011       //
    #define MD_bit_BB 0b00000000       //
    #define MD_bit_BR 0b00000010       //
    #define MD_bit_RF 0b00001111       //
    #define MD_bit_RB 0b00001100       //
    #define MD_bit_RR 0b00001110       //
    /* sumozade MD
  //MD operation bit 0=LOW,1=HIGH 
    #define MD_bit_FF 0b00000110       //
    #define MD_bit_FB 0b00000100       //
    #define MD_bit_FR 0b00000101       //
    #define MD_bit_BF 0b00000010       //
    #define MD_bit_BB 0b00000000       //
    #define MD_bit_BR 0b00000001       //
    #define MD_bit_RF 0b00001010       //
    #define MD_bit_RB 0b00001000       //
    #define MD_bit_RR 0b00001001       //
    */

  //PIN No
    #define PIN_RX 0         //D0
    #define PIN_TX 1         //D1
    #define Opt_L20 2        //D2 PD2
    #define Opt_L45 3        //D3 PD3 L45&55 & BUZZER
    #define Opt_L80 15       //A1(15)
    #define Blade_Sens 4     //D4 PD4
    #define MD_L1 5          //D5 PD5 R_DIR
    #define MD_L2 6          //D6 PD6 R_PWM
    #define Opt_R20 7        //D7 PD7
    #define Opt_R45 8        //D8 PB0 R45&55
    #define MD_R1 9          //D9 PB1 L_DIR
    #define MD_R2 10         //D10 PB2 L_PWM
    #define SSM_Of 11        //D11 PB3 START STOP MODULE OFFICIAL
    #define Opt_R80 12       //D12 PB4 
    #define LED 13           //D13 PB5 LED & BUZZER
    #define DIP_SW1 21       //A7(21) ADC7
    #define DIP_SW2 20       //A6(20) ADC6
    #define DIP_SW3 19       //A5(19) PC5
    #define DIP_SW4 18       //A4(18) PC4
    #define Edge_FL 17       //A3(17) PC3
    #define Edge_FR 16       //A2(16) PC2
    #define Edge_B 15        //A1(15) PC1
    #define Distance_F 14    //A0(14) PC0
  //BIN bit
    #define Opt_R80_B 0     //b00000001
    #define Opt_R45_B 1     //b00000010
    #define Opt_R20_B 2     //b00000100
    #define Opt_L20_B 3     //b00001000
    #define Opt_L45_B 4     //b00010000
    #define Opt_L80_B 5     //b00100000
    #define Blade_Sens_B 0  //b00000001
    #define Edge_B_B 1      //b00000010
    #define Edge_FR_B 2     //b00000100
    #define Edge_FL_B 3     //b00001000
    #define DIP_SW1_B 0     //b00000001
    #define DIP_SW2_B 1     //b00000010
    #define DIP_SW3_B 2     //b00000100
    #define DIP_SW4_B 3     //b00001000

//8S LiPo------------------------------------
  //FF&RR MD ON TIME(msec)
    #define F_R_50 18  //50mm
    #define F_R_100 31  //100mm
    #define F_R_150 44  //150mm
    #define F_R_200 57  //200mm
    #define F_R_250 70  //250mm
    #define F_R_300 83  //300mm
    #define F_R_350 96  //350mm
    #define F_R_400 109  //400mm
    #define F_R_450 122  //450mm
    #define F_R_500 135  //500mm
    #define F_R_550 148  //550mm
    #define F_R_600 161  //600mm
    #define F_R_650 174  //650mm
    #define F_R_700 188  //700mm
    #define F_R_750 201  //750mm
    #define F_R_800 214  //800mm
    #define F_R_850 227  //850mm
    #define F_R_900 240  //900mm
    #define F_R_950 253  //950mm
    #define F_R_1000 266  //1000mm
  //FB&BF&RB&BR MD ON TIME(msec)
    #define T_S_5 8  //5deg
    #define T_S_10 12  //10deg
    #define T_S_15 16  //15deg
    #define T_S_20 20  //20deg
    #define T_S_25 24  //25deg
    #define T_S_30 29  //30deg
    #define T_S_35 33  //35deg
    #define T_S_40 37  //40deg
    #define T_S_45 41  //45deg
    #define T_S_50 45  //50deg
    #define T_S_55 49  //55deg
    #define T_S_60 54  //60deg
    #define T_S_65 58  //65deg
    #define T_S_70 62  //70deg
    #define T_S_75 66  //75deg
    #define T_S_80 70  //80deg
    #define T_S_85 74  //85deg
    #define T_S_90 79  //90deg
    #define T_S_180 154  //180deg
  //FR&RF MD ON TIME(msec)
    #define T_W_5 7  //5deg
    #define T_W_10 9  //10deg
    #define T_W_15 11  //15deg
    #define T_W_20 13  //20deg
    #define T_W_25 14  //25deg
    #define T_W_30 16  //30deg
    #define T_W_35 18  //35deg
    #define T_W_40 20  //40deg
    #define T_W_45 22  //45deg
    #define T_W_50 24  //50deg
    #define T_W_55 26  //55deg
    #define T_W_60 28  //60deg
    #define T_W_65 30  //65deg
    #define T_W_70 32  //70deg
    #define T_W_75 33  //75deg
    #define T_W_80 35  //80deg
    #define T_W_85 37  //85deg
    #define T_W_90 39  //90deg
    #define T_W_180 73  //180deg
  //-----------------------------------------------
  //MD ON TIME(msec) FB
    #define FB_T_S_5 8  //5deg
    #define FB_T_S_10 12  //10deg
    #define FB_T_S_15 16  //15deg
    #define FB_T_S_20 20  //20deg
    #define FB_T_S_25 24  //25deg
    #define FB_T_S_30 29  //30deg
    #define FB_T_S_35 33  //35deg
    #define FB_T_S_40 37  //40deg
    #define FB_T_S_45 40  //45deg
    #define FB_T_S_50 44  //50deg
    #define FB_T_S_55 48  //55deg
    #define FB_T_S_60 52  //60deg
    #define FB_T_S_65 55  //65deg
    #define FB_T_S_70 59  //70deg
    #define FB_T_S_75 63  //75deg
    #define FB_T_S_80 67  //80deg
    #define FB_T_S_85 71  //85deg
    #define FB_T_S_90 74  //90deg
    #define FB_T_S_180 143  //180deg
  //MD ON TIME(msec) BF
    #define BF_T_S_5 8  //5deg
    #define BF_T_S_10 12  //10deg
    #define BF_T_S_15 16  //15deg
    #define BF_T_S_20 20  //20deg
    #define BF_T_S_25 24  //25deg
    #define BF_T_S_30 29  //30deg
    #define BF_T_S_35 33  //35deg
    #define BF_T_S_40 37  //40deg
    #define BF_T_S_45 39  //45deg
    #define BF_T_S_50 42  //50deg
    #define BF_T_S_55 46  //55deg
    #define BF_T_S_60 49  //60deg
    #define BF_T_S_65 53  //65deg
    #define BF_T_S_70 56  //70deg
    #define BF_T_S_75 59  //75deg
    #define BF_T_S_80 63  //80deg
    #define BF_T_S_85 66  //85deg
    #define BF_T_S_90 70  //90deg
    #define BF_T_S_180 132  //180deg
  //MD ON TIME(msec) RB
    #define RB_T_S_5 8  //5deg
    #define RB_T_S_10 12  //10deg
    #define RB_T_S_15 16  //15deg
    #define RB_T_S_20 20  //20deg
    #define RB_T_S_25 24  //25deg
    #define RB_T_S_30 29  //30deg
    #define RB_T_S_35 33  //35deg
    #define RB_T_S_40 37  //40deg
    #define RB_T_S_45 40  //45deg
    #define RB_T_S_50 43  //50deg
    #define RB_T_S_55 47  //55deg
    #define RB_T_S_60 50  //60deg
    #define RB_T_S_65 53  //65deg
    #define RB_T_S_70 57  //70deg
    #define RB_T_S_75 60  //75deg
    #define RB_T_S_80 64  //80deg
    #define RB_T_S_85 67  //85deg
    #define RB_T_S_90 70  //90deg
    #define RB_T_S_180 132  //180deg
  //MD ON TIME(msec) BR
    #define BR_T_S_5 8  //5deg
    #define BR_T_S_10 12  //10deg
    #define BR_T_S_15 16  //15deg
    #define BR_T_S_20 20  //20deg
    #define BR_T_S_25 24  //25deg
    #define BR_T_S_30 29  //30deg
    #define BR_T_S_35 33  //35deg
    #define BR_T_S_40 37  //40deg
    #define BR_T_S_45 41  //45deg
    #define BR_T_S_50 45  //50deg
    #define BR_T_S_55 48  //55deg
    #define BR_T_S_60 52  //60deg
    #define BR_T_S_65 55  //65deg
    #define BR_T_S_70 59  //70deg
    #define BR_T_S_75 62  //75deg
    #define BR_T_S_80 66  //80deg
    #define BR_T_S_85 69  //85deg
    #define BR_T_S_90 73  //90deg
    #define BR_T_S_180 136  //180deg
  //-------------------------------------------
  //MD ON TIME(msec) FR
    #define FR_T_W_5 7  //5deg
    #define FR_T_W_10 9  //10deg
    #define FR_T_W_15 11  //15deg
    #define FR_T_W_20 13  //20deg
    #define FR_T_W_25 14  //25deg
    #define FR_T_W_30 16  //30deg
    #define FR_T_W_35 18  //35deg
    #define FR_T_W_40 20  //40deg
    #define FR_T_W_45 22  //45deg
    #define FR_T_W_50 24  //50deg
    #define FR_T_W_55 26  //55deg
    #define FR_T_W_60 28  //60deg
    #define FR_T_W_65 30  //65deg
    #define FR_T_W_70 32  //70deg
    #define FR_T_W_75 33  //75deg
    #define FR_T_W_80 35  //80deg
    #define FR_T_W_85 37  //85deg
    #define FR_T_W_90 39  //90deg
    #define FR_T_W_180 73  //180deg
  //MD ON TIME(msec) RF
    #define RF_T_W_5 7  //5deg
    #define RF_T_W_10 9  //10deg
    #define RF_T_W_15 11  //15deg
    #define RF_T_W_20 13  //20deg
    #define RF_T_W_25 14  //25deg
    #define RF_T_W_30 16  //30deg
    #define RF_T_W_35 18  //35deg
    #define RF_T_W_40 20  //40deg
    #define RF_T_W_45 22  //45deg
    #define RF_T_W_50 24  //50deg
    #define RF_T_W_55 26  //55deg
    #define RF_T_W_60 28  //60deg
    #define RF_T_W_65 30  //65deg
    #define RF_T_W_70 32  //70deg
    #define RF_T_W_75 33  //75deg
    #define RF_T_W_80 35  //80deg
    #define RF_T_W_85 37  //85deg
    #define RF_T_W_90 39  //90deg
    #define RF_T_W_180 70  //180deg

//Tone frequency
    #define Tone_C4 262    //
    #define Tone_D4 294    //
    #define Tone_E4 330    //
    #define Tone_F4 349    //
    #define Tone_G4 392    //
    #define Tone_A4 440    //
    #define Tone_B4 494    //
    #define Tone_C5 523    //
    #define Tone_D5 587    //
    #define Tone_E5 659    //
    #define Tone_F5 698    //
    #define Tone_G5 784    //
    #define Tone_A5 880    //
    #define Tone_B5 988    //
    #define Tone_C6 1047    //
    #define Tone_D6 1175    //
    #define Tone_E6 1319    //
    #define Tone_F6 1397    //
    #define Tone_G6 1568    //
    #define Tone_A6 1760    //
    #define Tone_B6 1976    //
    #define Tone_C7 2093    //
    #define Tone_D7 2349    //
    #define Tone_E7 2637    //
    #define Tone_F7 2794    //
    #define Tone_G7 3136    //
    #define Tone_A7 3520    //
    #define Tone_B7 3951    //
    #define Tone_C8 4186    //

//IR受信用変数---------------------------------------------------------------------------------
  //Sets the threshold for receiving infrared remote control codes
  const unsigned long Limit_Time_L_Frame = 50000; //1 Frame max 43700 micro sec (1 frame limit)
  const unsigned long Limit_Time_L_upper = 4000; //Leader code is 3200 micro sec (upper limit)
  const unsigned long Limit_Time_L_lower = 2400; //Leader code is 3200 micro sec (lower limit)
  // variables will change
  int Run_Mode = 0;  // Run Mode Status (0=disable 1=ready 2=start 3=stop)
  int IR_Recording_Mode = 0;  // IR Recording Mode Status (0=disable 1=ready 2=start 3=stop)
  int IR_Recording_Data = 255;  // IR Recording data value
  unsigned long Limit_Time_L2_upper = 2000; // Leader code Toff upper limit
  unsigned long Limit_Time_L2_lower = 1200; // Leader code Toff lower limit
  unsigned long Limit_Time_L_Frame_now1 = 0; // Limit Time Count only
  unsigned long Limit_Time_L_Frame_now2 = 0; // Limit Time Count only
  unsigned long Base_1T = 400; //Base time 600us 
  unsigned long Limit_T_D0_upper = 500; //micro sec Data "0" code 600us
  unsigned long Limit_T_D1_upper = 1500; //micro sec Data "1" code 1200us
  unsigned long time_IR_T_off = 0; // IR Time Count only
  unsigned long time_IR_T_on = 0; // IR Time Count only
  unsigned long time_IR_Frame = 0; //  Count value when IR pin turns off
  unsigned long time_IR_Frame_Now1 = 0; // Limit Time Count only
  unsigned long time_IR_Frame_Now2 = 0; // Limit Time Count only
  unsigned long time_IR_Leader = 0; //  Count value when IR pin turns off
  unsigned long time_IR_Leader_0 = 0; //  Count value when IR pin turns on
  unsigned long time_IR_Data_Now = 0; // IR Time Count only
  unsigned long time_Now1 = 0; // Limit Time Count only
  unsigned long time_Now2 = 0; // Limit Time Count only
  int time_IR_Data0 = 0; //  IR Received data 0
  int time_IR_Data1 = 0; //  IR Received data 1
  int time_IR_Data2 = 0; //  IR Received data 2
  int time_IR_Data3 = 0; //  IR Received data 3
  int time_IR_Data4 = 0; //  IR Received data 4
  int time_IR_Data5 = 0; //  IR Received data 5
  int time_IR_Data6 = 0; //  IR Received data 6
  int time_IR_Data7 = 0; //  IR Received data 7
  int IR_Data_Val = 256;  // IR Received data value
  int IR_Data_Read_bit = 0;  // IR Received data bit Status
  int IR_Data_H = 256;  // IR Received H data value
  int IR_Data_L = 256;  // IR Received L data value
  int codeLow = 256;  // IR Received data lowByte
  int inChar = 0; //Serial DATA IN
  int inChar1 = 255; //Serial DATA IN
  int inChar2 = 255; //Serial DATA IN
//---------------------------------------------------------------------------------------------