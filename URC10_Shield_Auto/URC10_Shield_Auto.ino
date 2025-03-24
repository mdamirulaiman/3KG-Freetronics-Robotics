int LEdge  = A5 ;
int REdge  = A4 ;
int DS1    = A0 ;
int DS2    = A1 ;
int DS3    = A2 ;
int DS4    = A3 ;
//int Buzzer = A5 ;
//int SPD    = A7 ;
//int TRN    = A6 ;

int Start  = 9 ;
int RSens  = 12 ;
int RFSens = 11 ;
int MSens  = 8 ;
int LFSens = 3 ;
int LSens  = 2 ;
int LED    = 13 ;
int LDir   = 4 ; //IN2
int LPwm   = 5 ; //AN2
int RDir   = 7 ; //IN1
int RPwm   = 6 ; //AN1

#include <EEPROM.h>

// EEPROM Address.
#define EEADD_EDGE_L  0
#define EEADD_EDGE_R  (EEADD_EDGE_L + sizeof(int))

// Default value for edge sensors threshold if not calibrated.
#define DEFAULT_EDGE_THRESHOLD  650
int EDGE_L_THRESHOLD, EDGE_R_THRESHOLD;
int EDGE_L = 300, EDGE_R = 300;
// Variables
#define LEFT    0
#define RIGHT   1
int duration = 5 ; //5 SECOND delay
// Global variables.
uint8_t searchDir = LEFT;
// Speed values
int IdleSpeed = 65; // Idle Speed while not detect opponent.
int MaxSpeed = 100; // Max Speed when sensor detect opponent
int TurnSpeed = 75; // Left and Right Forward Turning Speed
uint16_t EdgeTurn = 300; // Turning Time variable when minisumo sees white line in milliseconds

/*******************************************************************************
	Start Routine
	This function should be called once only when the game start.
*******************************************************************************/
void startRoutine() {
  delay(1500);
  while (!digitalRead(Start)) {
    sensordebug();
    if (analogRead(LEdge) < EDGE_L_THRESHOLD || analogRead(REdge) < EDGE_R_THRESHOLD ) {
      digitalWrite(LED, HIGH);
    } else {
      digitalWrite(LED, LOW);
    } delay(20);
  }
  if (digitalRead(Start)) {
    Serial.println("Start Sumo");

    /* for (int i = 0; i <= duration ; i++) { //set start delay time
      noTone(Buzzer);
      delay(700);
      tone(Buzzer, 523, 300);
      delay(300);
    } */

    uint32_t startTimestamp = millis();
    if (digitalRead(DS1) == 1 && digitalRead(DS2) == 1 && digitalRead(DS3) == 1 ) { //Forward attack (000)
      Serial.print("Forward");
      Set_Motor(100, 100, 100);
      while (digitalRead(RFSens) || digitalRead(LFSens) ) {
        // Quit if opponent is not found after timeout.
        if (millis() - startTimestamp > 200) {
          break;
        }
      }
    } else if (digitalRead(DS1) == 0 && digitalRead(DS2) == 1 && digitalRead(DS3) == 1) { //Left side attack (100)
      Serial.print("Left 90");
      kiri_90();
      while (digitalRead(MSens) ) {
        // Quit if opponent is not found after timeout.
        if (millis() - startTimestamp > 450) {
          break;
        }
      }
    } else if (digitalRead(DS1) == 1 && digitalRead(DS2) == 0 && digitalRead(DS3) == 1) { //Middle side attack (010)
      Serial.print("Forward attack");
      Set_Motor(100, 100, 10);
      while (digitalRead(MSens) ) {
        // Quit if opponent is not found after timeout.
        if (millis() - startTimestamp > 220) {
          break;
        }
      }
    } else if (digitalRead(DS1) == 1 && digitalRead(DS2) == 1 && digitalRead(DS3) == 0) { //Right attack (001)
      Serial.print("Right attack");
      kanan_90();
      while (digitalRead(MSens) ) {
        // Quit if opponent is not found after timeout.
        if (millis() - startTimestamp > 450) {
          break;
        }
      }
    } else if (digitalRead(DS1) == 0 && digitalRead(DS2) == 0 && digitalRead(DS3) == 1) { //Left round attack (110)
      Serial.print("Left moon");
      kiri_45();
      while (digitalRead(MSens) ) {
        // Quit if opponent is not found after timeout.
        if (millis() - startTimestamp > 200) {
          break;
        }
      }
    } else if (digitalRead(DS1) == 1 && digitalRead(DS2) == 0 && digitalRead(DS3) == 0) { //Right round attack (011)
      Serial.print("Right moon");
      kanan_45();
      while (digitalRead(MSens) ) {
        // Quit if opponent is not found after timeout.
        if (millis() - startTimestamp > 200) {
          break;
        }
      }
    } else if (digitalRead(DS1) == 0 && digitalRead(DS2) == 0 && digitalRead(DS3) == 0) { //Front wait attack (111)
      Serial.println("Forward wait");
      Set_Motor(100, 100, 50);
      delay(100);
      Set_Motor(10, 10, 20);
      Set_Motor(0, 0, 20);
      while (digitalRead(LSens) || digitalRead(LFSens) || digitalRead(MSens) || digitalRead(RFSens) || digitalRead(RSens)) {

        if (!digitalRead(LSens) || !digitalRead(LFSens) || !digitalRead(MSens) || !digitalRead(RFSens) || !digitalRead(RSens)) {
          break;
        }
        // Quit if opponent is not found after timeout.
        if (millis() - startTimestamp > 5000) {
          break;
        }
      }
    }  else if (digitalRead(DS1) == 0 && digitalRead(DS2) == 1 && digitalRead(DS3) == 0) { //Backward wait attack (101)
      Serial.println("Reverse wait");
      belakang();
      delay(100);
      Set_Motor(0, 0, 20);
      while (digitalRead(LSens) || digitalRead(LFSens) || digitalRead(MSens) || digitalRead(RFSens) || digitalRead(RSens)) {

        if (!digitalRead(LSens) || !digitalRead(LFSens) || !digitalRead(MSens) || !digitalRead(RFSens) || !digitalRead(RSens)) {
          break;
        }
        // Quit if opponent is not found after timeout.
        if (millis() - startTimestamp > 5000) {
          break;
        }
      }
    }
  }
}

/*******************************************************************************
	Back Off
	This function should be called when the ring edge is detected.
*******************************************************************************/
void backoff(uint8_t dir) {

  // Reverse.
  Set_Motor(-70, -70, 300);

  // Stop the motors.
  Set_Motor(0, 0, 10);

  // Rotate..
  if (dir == LEFT) {
    Set_Motor(-TurnSpeed, TurnSpeed, 10);
    //delay(EdgeTurn);
  } else {
    Set_Motor(TurnSpeed, -TurnSpeed, 10);
    //delay(EdgeTurn);
  }

  // Start looking for opponent.
  // Timeout after a short period.
  uint32_t uTurnTimestamp = millis();
  while (millis() - uTurnTimestamp < EdgeTurn) {
    // Opponent is detected if either one of the opponent sensor is triggered.
    if ( !digitalRead(MSens) || !digitalRead(LFSens) || !digitalRead(RFSens) || !digitalRead(LSens) || !digitalRead(RSens) )
    {
      // Stop the motors.
      Set_Motor(0, 0, 10);

      // Return to the main loop and run the attach program.
      return;
    }
  }

  // If opponent is not found, move forward and continue searching in the main loop..
  Set_Motor(IdleSpeed, IdleSpeed, 50);

}


/*******************************************************************************
	Search
*******************************************************************************/
void search() {
  // Move in circular motion.
  if (searchDir == LEFT) {
    Set_Motor(IdleSpeed, IdleSpeed, 30);
  } else {
    Set_Motor(IdleSpeed, IdleSpeed, 30);
  }
}


/*******************************************************************************
	Attack
	Track and attack the opponent in full speed.
	Do nothing if opponent is not found.
*******************************************************************************/
void attack() {
  uint32_t attackTimestamp = millis();

  // Opponent in front center.
  // Go straight in full speed.
  if (!digitalRead(MSens) ) {
    Set_Motor(MaxSpeed, MaxSpeed, 30);
    //Set_Motor(0,0,30);
  }

  // Opponent in front left.
  // Turn left.
  else if (!digitalRead(LFSens) ) {
    Set_Motor(0, MaxSpeed, 30);
  }

  else if (!digitalRead(LFSens) && !digitalRead(MSens) ) {
    Set_Motor(0, MaxSpeed, 30);
  }

  // Opponent in front right.
  // Turn right.
  else if (!digitalRead(RFSens) ) {
    Set_Motor(MaxSpeed, 0, 30);
  }

  // Front right and Middle sensor detect opponent
  else if (!digitalRead(RFSens) && !digitalRead(MSens) ) {
    Set_Motor(MaxSpeed, 0, 30);
  }

  // Both Front Left & Front Right sensor detect opponent
  else if (!digitalRead(LFSens) && !digitalRead(RFSens) ) {
    Set_Motor(MaxSpeed, MaxSpeed, 20);
  }

  // All Front Sensor detect opponent
  else if (!digitalRead(LFSens) && !digitalRead(RFSens) && !digitalRead(MSens) ) {
    Set_Motor(MaxSpeed, MaxSpeed, 20);
  }

  // Opponent in left side.
  // Rotate left until opponent is in front.
  else if (!digitalRead(LSens) ) {
    //Set_Motor(-MaxSpeed,MaxSpeed,30);
    Set_Motor(-MaxSpeed, MaxSpeed, 40);
    while (digitalRead(MSens) == 1) {
      // Quit if opponent is not found after timeout.
      if (millis() - attackTimestamp > 350) {
        break;
      }
    }
  }

  else if (!digitalRead(LSens) && !digitalRead(LFSens) ) {
    //Set_Motor(-MaxSpeed,MaxSpeed,30);
    Set_Motor(-MaxSpeed, MaxSpeed, 40);
    while (digitalRead(MSens) == 1) {
      // Quit if opponent is not found after timeout.
      if (millis() - attackTimestamp > 350) {
        break;
      }
    }
  }

  // Opponent in right side.
  // Rotate right until opponent is in front.
  else if (!digitalRead(RSens) ) {
    //Set_Motor(MaxSpeed,-MaxSpeed,30);
    Set_Motor(MaxSpeed, -MaxSpeed, 30);
    while (digitalRead(MSens) == 1 ) {
      // Quit if opponent is not found after timeout.
      if (millis() - attackTimestamp > 350) {
        break;
      }
    }
  }

  else if (!digitalRead(RSens) && !digitalRead(RFSens) ) {
    //Set_Motor(MaxSpeed,-MaxSpeed,30);
    Set_Motor(MaxSpeed, -MaxSpeed, 30);
    while (digitalRead(MSens) == 1 ) {
      // Quit if opponent is not found after timeout.
      if (millis() - attackTimestamp > 350) {
        break;
      }
    }
  }

}

void read_line() {
  int left_raw = analogRead(LEdge);
  Serial.print("Left : ");
  Serial.print(left_raw);
  Serial.print(" | ");
  int right_raw = analogRead(REdge);
  Serial.print("Right : ");
  Serial.println(right_raw);
}

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  //Line Sensor Connection
  pinMode(LEdge, INPUT);
  pinMode(REdge, INPUT);
  //Board Connection
  pinMode(DS1, INPUT_PULLUP);
  pinMode(DS2, INPUT_PULLUP);
  pinMode(DS3, INPUT_PULLUP);
  pinMode(DS4, INPUT_PULLUP);
  pinMode(Start, INPUT);
  pinMode(LED, OUTPUT);
  //Opponent Sensor Connection
  pinMode(RSens, INPUT_PULLUP);
  pinMode(RFSens, INPUT_PULLUP);
  pinMode(MSens, INPUT_PULLUP);
  pinMode(LFSens, INPUT_PULLUP);
  pinMode(LSens, INPUT_PULLUP);
  //Motor Connection
  pinMode(RPwm, OUTPUT);
  pinMode(RDir, OUTPUT);
  pinMode(LPwm, OUTPUT);
  pinMode(LDir, OUTPUT);

  // Stop the motors.
  Set_Motor(0, 0, 10);

  while (millis() < 200 ) {
    digitalWrite(LED, HIGH);
    Serial.println("Press Start for Calibration");
    if (digitalRead(Start)) { // if button is triggered/pressed while powering on. Goes to line calibration mode
      Serial.println("Calibrating starts in 3");
      digitalWrite(LED, HIGH);
      delay(500);
      digitalWrite(LED, LOW);
      delay(500);
      Serial.println("Calibrating starts in 2");
      digitalWrite(LED, HIGH);
      delay(500);
      digitalWrite(LED, LOW);
      delay(500);
      Serial.println("Calibrating starts in 1");
      digitalWrite(LED, HIGH);
      delay(500);
      digitalWrite(LED, LOW);
      calibrateEdgeSensor();
    }
  }

  digitalWrite(LED, LOW);
  EDGE_L_THRESHOLD = sensorThreshold(LEFT);
  EDGE_R_THRESHOLD = sensorThreshold(RIGHT);
  // Start routine..
  startRoutine();
}

void Set_Motor (float Lval, float Rval, int timex) {
  Lval = Lval * 2.55;
  Rval = Rval * 2.55;
  if (Lval >= 0) {
    analogWrite(LPwm, Lval);
    digitalWrite(LDir, HIGH);
  } else {
    Lval = abs(Lval);
    analogWrite(LPwm, Lval);
    digitalWrite(LDir, LOW);
  }
  if (Rval >= 0) {
    analogWrite(RPwm, Rval);
    digitalWrite(RDir, LOW);
  } else {
    Rval = abs(Rval);
    analogWrite(RPwm, Rval);
    digitalWrite(RDir, HIGH);
  }
  // Serial.print(Rval); Serial.print("-"); Serial.println(Lval);
  delay(timex);
}

void kanan_90() {
  Set_Motor(100, -100, 100);
}
void kanan_45() {
  Set_Motor(100, -100, 160);
}
void kiri_90() {
  Set_Motor(-100, 100, 100);
}
void kiri_45() {
  Set_Motor(-100, 100, 160);
}
void belakang() {
  Set_Motor(-100, -100, 200);
}
void keluar_kiri() {
  Set_Motor(-100, 100, 150);
  Set_Motor(100, 50, 400);
  Set_Motor(100, -100, 250);
}
void keluar_kanan() {
  Set_Motor(100, -100, 150);
  Set_Motor(50, 100, 400);
  Set_Motor(-100, 100, 250);
}

/*******************************************************************************
	Main program loop.
*******************************************************************************/
void loop() {
  //IdleSpeed = map(analogRead(SPD), 0, 1023, 0, 100);
  //TurnSpeed = map(analogRead(TRN), 0, 1023, 0, 100);
  // Edge is detected on the left.
  if (analogRead(LEdge) < EDGE_L_THRESHOLD && analogRead(REdge) > EDGE_R_THRESHOLD) {
    delay(20);
    if (analogRead(LEdge) < EDGE_L_THRESHOLD && analogRead(REdge) > EDGE_R_THRESHOLD) {

      //(analogRead(LEdge) <10 && analogRead(REdge) > 999) {
      // Back off and make a U-Turn to the right.
      backoff(RIGHT);

      // Toggle the search direction.
      searchDir ^= 1;
    }
  }

  // Edge is detected on the right.
  else if (analogRead(REdge) < EDGE_R_THRESHOLD && analogRead(LEdge) > EDGE_L_THRESHOLD) {
    delay(20);
    if (analogRead(REdge) < EDGE_R_THRESHOLD && analogRead(LEdge) > EDGE_L_THRESHOLD) {


      //(analogRead(REdge) < 10 && analogRead(LEdge) > 999) {
      // Back off and make a U-Turn to the right.
      backoff(LEFT);

      // Toggle the search direction.
      searchDir ^= 1;
    }
  }

  // Edge is not detected.
  else {
    // Keep searching if opponent is not detected.
    if ( digitalRead(MSens) && digitalRead(LFSens) && digitalRead(RFSens) && digitalRead(LSens) && digitalRead(RSens) )
    {
      search();
    }

    // Attack if opponent is in view.
    else {
      attack();
    }
  }

  // Stop the robot if the button/remote is pressed/triggered.
  if (!digitalRead(Start)) {
    // Stop the motors.
    Set_Motor(0, 0, 10);
    // Loop forever here.
    while (1);
  }

}

void calibrateEdgeSensor() {
  int minL = 1024 ;
  int minR = 1024 ;
  int maxL = 0 ;
  int maxR = 0 ;
  uint32_t timestamp = millis();

  do {
    int tempL = analogRead(LEdge);
    int tempR = analogRead(REdge);

    if (minL > tempL) minL = tempL;
    if (minR > tempR) minR = tempR;

    if (maxL < tempL) maxL = tempL;
    if (maxR < tempR) maxR = tempR;

    if (millis() - timestamp >= 200) {
      timestamp += 100 ;
      Serial.println("Calibrating...");
      digitalWrite(LED, !digitalRead(LED)) ;
    }

  } while (digitalRead(Start));
  if (!digitalRead(Start));
  Serial.println("DONE CALIBRATE wait for values");
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);

  if (maxL > minL) {
    int threshold = ((maxL - minL) * 3 / 5) + minL ;
    EEPROM.put(EEADD_EDGE_L, threshold);
  }
  if (maxR > minR) {
    int threshold = ((maxR - minR) * 3 / 5) + minR ;
    EEPROM.put(EEADD_EDGE_R, threshold);
  }
  Serial.println("NEW CALIBRATED VALUE");
  Serial.print("Left: "); Serial.print(sensorThreshold(LEFT)); Serial.print(" | ");
  Serial.print("Right: "); Serial.println(sensorThreshold(RIGHT));
}

int sensorThreshold(int side) {
  int eepromAddress;
  if (side == LEFT) {
    eepromAddress = EEADD_EDGE_L;
  }
  else if (side == RIGHT) {
    eepromAddress = EEADD_EDGE_R;
  }
  else {
    return 0;
  }
  int threshold;
  EEPROM.get(eepromAddress, threshold);

  if ((threshold <= 0) || (threshold >= 1023)) {
    threshold = DEFAULT_EDGE_THRESHOLD;
  }

  return threshold;
}

void sensordebug() {
  Serial.print("Start Button : "); Serial.print(digitalRead(Start)); Serial.print("   ");
  Serial.print("Dipswitch : "); Serial.print(digitalRead(DS1)); Serial.print(digitalRead(DS2)); Serial.print(digitalRead(DS3)); Serial.print(digitalRead(DS4)); Serial.print("\t");
  //Serial.print("Trimpot : "); Serial.print(speed); Serial.print(" | "); Serial.print(turn); Serial.print("\t");
  Serial.print("Line Sensor : "); Serial.print(analogRead(LEdge));  Serial.print(" | "); Serial.print(analogRead(REdge)); Serial.print("\t");
  Serial.print("Opponent Sensor : ") ; Serial.print(digitalRead(LSens)); Serial.print(digitalRead(LFSens)); Serial.print(digitalRead(MSens)); Serial.print(digitalRead(RFSens)); Serial.println(digitalRead(RSens));
}
