#include <avr/pgmspace.h>
#include <extras/ar4.h>

String inData;
String function;

const int debugg = 0;

const int Input32 = 32;
const int Input33 = 33;
const int Input34 = 34;
const int Input35 = 35;
const int Input36 = 36;

const int Output37 = 37;
const int Output38 = 38;
const int Output39 = 39;
const int Output40 = 40;
const int Output41 = 41;

AR4 robot;

void setup()
{
  // run once:
  Serial.begin(9600);
  pinMode(Input32, INPUT_PULLUP);
  pinMode(Input33, INPUT_PULLUP);
  pinMode(Input34, INPUT_PULLUP);
  pinMode(Input35, INPUT_PULLUP);
  pinMode(Input36, INPUT_PULLUP);

  pinMode(Output37, OUTPUT);
  pinMode(Output38, OUTPUT);
  pinMode(Output39, OUTPUT);
  pinMode(Output40, OUTPUT);
  pinMode(Output41, OUTPUT);
}

void loop()
{

  // start loop
  WayPtDel = 0;
  while (Serial.available())
  {
    char recieved = Serial.read();
    inData += recieved;
    // Process message when new line character is recieved
    if (recieved == '\n')
    {
      inData.trim();
      String function = inData.substring(0, 2);
      inData = inData.substring(2);
      KinematicError = 0;

      //-----COMMAND TO CLOSE---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "CL")
      {
        delay(5);
        Serial.end();
      }

      //-----COMMAND TEST LIMIT SWITCHES---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "TL")
      {

        String J1calTest = "0";
        String J2calTest = "0";
        String J3calTest = "0";
        String J4calTest = "0";
        String J5calTest = "0";
        String J6calTest = "0";

        if (digitalRead(J1calPin) == HIGH)
        {
          J1calTest = "1";
        }
        if (digitalRead(J2calPin) == HIGH)
        {
          J2calTest = "1";
        }
        if (digitalRead(J3calPin) == HIGH)
        {
          J3calTest = "1";
        }
        if (digitalRead(J4calPin) == HIGH)
        {
          J4calTest = "1";
        }
        if (digitalRead(J5calPin) == HIGH)
        {
          J5calTest = "1";
        }
        if (digitalRead(J6calPin) == HIGH)
        {
          J6calTest = "1";
        }
        String TestLim = " J1 = " + J1calTest + "   J2 = " + J2calTest + "   J3 = " + J3calTest + "   J4 = " + J4calTest + "   J5 = " + J5calTest + "   J6 = " + J6calTest;
        delay(5);
        Serial.println(TestLim);
      }

      //-----COMMAND SET ENCODERS TO 1000---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "SE")
      {
        J1encPos.write(1000);
        J2encPos.write(1000);
        J3encPos.write(1000);
        J4encPos.write(1000);
        J5encPos.write(1000);
        J6encPos.write(1000);
        delay(5);
        Serial.print("Done");
      }

      //-----COMMAND READ ENCODERS---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "RE")
      {
        J1EncSteps = J1encPos.read();
        J2EncSteps = J2encPos.read();
        J3EncSteps = J3encPos.read();
        J4EncSteps = J4encPos.read();
        J5EncSteps = J5encPos.read();
        J6EncSteps = J6encPos.read();
        String Read = " J1 = " + String(J1EncSteps) + "   J2 = " + String(J2EncSteps) + "   J3 = " + String(J3EncSteps) + "   J4 = " + String(J4EncSteps) + "   J5 = " + String(J5EncSteps) + "   J6 = " + String(J6EncSteps);
        delay(5);
        Serial.println(Read);
      }

      //-----COMMAND REQUEST POSITION---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "RP")
      {
        sendRobotPos();
      }

      //-----COMMAND HOME POSITION---------------------------------------------------
      //-----------------------------------------------------------------------

      // For debugging
      if (function == "HM")
      {

        int J1dir;
        int J2dir;
        int J3dir;
        int J4dir;
        int J5dir;
        int J6dir;
        int TRdir;

        String SpeedType = "p";
        float SpeedVal = 25.0;
        float ACCspd = 10.0;
        float DCCspd = 10.0;
        float ACCramp = 20.0;

        JangleIn[0] = 0.00;
        JangleIn[1] = 0.00;
        JangleIn[2] = 0.00;
        JangleIn[3] = 0.00;
        JangleIn[4] = 0.00;
        JangleIn[5] = 0.00;

        // calc destination motor steps
        int J1futStepM = J1axisLimNeg * J1StepDeg;
        int J2futStepM = J2axisLimNeg * J2StepDeg;
        int J3futStepM = J3axisLimNeg * J3StepDeg;
        int J4futStepM = J4axisLimNeg * J4StepDeg;
        int J5futStepM = J5axisLimNeg * J5StepDeg;
        int J6futStepM = J6axisLimNeg * J6StepDeg;

        // calc delta from current to destination
        int J1stepDif = J1StepM - J1futStepM;
        int J2stepDif = J2StepM - J2futStepM;
        int J3stepDif = J3StepM - J3futStepM;
        int J4stepDif = J4StepM - J4futStepM;
        int J5stepDif = J5StepM - J5futStepM;
        int J6stepDif = J6StepM - J6futStepM;
        int TRstepDif = 0;

        // determine motor directions
        if (J1stepDif <= 0)
        {
          J1dir = 1;
        }
        else
        {
          J1dir = 0;
        }

        if (J2stepDif <= 0)
        {
          J2dir = 1;
        }
        else
        {
          J2dir = 0;
        }

        if (J3stepDif <= 0)
        {
          J3dir = 1;
        }
        else
        {
          J3dir = 0;
        }

        if (J4stepDif <= 0)
        {
          J4dir = 1;
        }
        else
        {
          J4dir = 0;
        }

        if (J5stepDif <= 0)
        {
          J5dir = 1;
        }
        else
        {
          J5dir = 0;
        }

        if (J6stepDif <= 0)
        {
          J6dir = 1;
        }
        else
        {
          J6dir = 0;
        }

        TRdir = 0;

        resetEncoders();
        driveMotorsJ(abs(J1stepDif), abs(J2stepDif), abs(J3stepDif), abs(J4stepDif), abs(J5stepDif), abs(J6stepDif), abs(TRstepDif), J1dir, J2dir, J3dir, J4dir, J5dir, J6dir, TRdir, SpeedType, SpeedVal, ACCspd, DCCspd, ACCramp);
        checkEncoders();
        sendRobotPos();
        delay(5);
        Serial.println("Done");
      }

      //-----COMMAND CORRECT POSITION---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "CP")
      {
        correctRobotPos();
      }

      //-----COMMAND SET TOOL FRAME---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "TF")
      {
        int TFxStart = inData.indexOf('A');
        int TFyStart = inData.indexOf('B');
        int TFzStart = inData.indexOf('C');
        int TFrzStart = inData.indexOf('D');
        int TFryStart = inData.indexOf('E');
        int TFrxStart = inData.indexOf('F');
        Robot_Kin_Tool[0] = inData.substring(TFxStart + 1, TFyStart).toFloat();
        Robot_Kin_Tool[1] = inData.substring(TFyStart + 1, TFzStart).toFloat();
        Robot_Kin_Tool[2] = inData.substring(TFzStart + 1, TFrzStart).toFloat();
        Robot_Kin_Tool[3] = inData.substring(TFrzStart + 1, TFryStart).toFloat() * M_PI / 180;
        Robot_Kin_Tool[4] = inData.substring(TFryStart + 1, TFrxStart).toFloat() * M_PI / 180;
        Robot_Kin_Tool[5] = inData.substring(TFrxStart + 1).toFloat() * M_PI / 180;
        delay(5);
        Serial.println("Done");
      }

      //-----COMMAND CALIBRATE TRACK---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "CT")
      {
        int axis7lengthStart = inData.indexOf('A');
        int axis7rotStart = inData.indexOf('B');
        int axis7stepsStart = inData.indexOf('C');
        axis7length = inData.substring(axis7lengthStart + 1, axis7rotStart).toFloat();
        axis7rot = inData.substring(axis7rotStart + 1, axis7stepsStart).toFloat();
        axis7steps = inData.substring(axis7stepsStart + 1).toFloat();
        TRaxisLimNeg = 0;
        TRaxisLimPos = axis7length;
        TRaxisLim = TRaxisLimPos + TRaxisLimNeg;
        TRStepDeg = axis7steps / axis7rot;
        TRStepLim = TRaxisLim * TRStepDeg;
        delay(5);
        Serial.print("Done");
      }

      //-----COMMAND ZERO TRACK---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "ZT")
      {
        TRStepM = 0;
        sendRobotPos();
      }

      //-----COMMAND TO WAIT TIME---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "WT")
      {
        int WTstart = inData.indexOf('S');
        float WaitTime = inData.substring(WTstart + 1).toFloat();
        int WaitTimeMS = WaitTime * 1000;
        delay(WaitTimeMS);
        Serial.println("Done");
      }

      //-----COMMAND IF INPUT THEN JUMP---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "JF")
      {
        int IJstart = inData.indexOf('X');
        int IJTabstart = inData.indexOf('T');
        int IJInputNum = inData.substring(IJstart + 1, IJTabstart).toInt();
        if (digitalRead(IJInputNum) == HIGH)
        {
          delay(5);
          Serial.println("T");
        }
        if (digitalRead(IJInputNum) == LOW)
        {
          delay(5);
          Serial.println("F");
        }
      }
      //-----COMMAND SET OUTPUT ON---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "ON")
      {
        int ONstart = inData.indexOf('X');
        int outputNum = inData.substring(ONstart + 1).toInt();
        digitalWrite(outputNum, HIGH);
        delay(5);
        Serial.println("Done");
      }
      //-----COMMAND SET OUTPUT OFF---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "OF")
      {
        int ONstart = inData.indexOf('X');
        int outputNum = inData.substring(ONstart + 1).toInt();
        digitalWrite(outputNum, LOW);
        delay(5);
        Serial.println("Done");
      }
      //-----COMMAND TO WAIT INPUT ON---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "WI")
      {
        int WIstart = inData.indexOf('N');
        int InputNum = inData.substring(WIstart + 1).toInt();
        while (digitalRead(InputNum) == LOW)
        {
          delay(100);
        }
        delay(5);
        Serial.println("Done");
      }
      //-----COMMAND TO WAIT INPUT OFF---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "WO")
      {
        int WIstart = inData.indexOf('N');
        int InputNum = inData.substring(WIstart + 1).toInt();
        while (digitalRead(InputNum) == HIGH)
        {
          delay(100);
        }
        delay(5);
        Serial.println("Done");
      }

      //-----COMMAND SEND POSITION---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "SP")
      {
        int J1angStart = inData.indexOf('A');
        int J2angStart = inData.indexOf('B');
        int J3angStart = inData.indexOf('C');
        int J4angStart = inData.indexOf('D');
        int J5angStart = inData.indexOf('E');
        int J6angStart = inData.indexOf('F');
        int TRstepStart = inData.indexOf('G');
        J1StepM = ((inData.substring(J1angStart + 1, J2angStart).toFloat()) + J1axisLimNeg) * J1StepDeg;
        J2StepM = ((inData.substring(J2angStart + 1, J3angStart).toFloat()) + J2axisLimNeg) * J2StepDeg;
        J3StepM = ((inData.substring(J3angStart + 1, J4angStart).toFloat()) + J3axisLimNeg) * J3StepDeg;
        J4StepM = ((inData.substring(J4angStart + 1, J5angStart).toFloat()) + J4axisLimNeg) * J4StepDeg;
        J5StepM = ((inData.substring(J5angStart + 1, J6angStart).toFloat()) + J5axisLimNeg) * J5StepDeg;
        J6StepM = ((inData.substring(J6angStart + 1, TRstepStart).toFloat()) + J6axisLimNeg) * J6StepDeg;
        TRStepM = inData.substring(TRstepStart + 1).toFloat();
        delay(5);
        Serial.println("Done");
      }

      //-----COMMAND ECHO TEST MESSAGE---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "TM")
      {
        int J1start = inData.indexOf('A');
        int J2start = inData.indexOf('B');
        int J3start = inData.indexOf('C');
        int J4start = inData.indexOf('D');
        int J5start = inData.indexOf('E');
        int J6start = inData.indexOf('F');
        int WristConStart = inData.indexOf('W');
        JangleIn[0] = inData.substring(J1start + 1, J2start).toFloat();
        JangleIn[1] = inData.substring(J2start + 1, J3start).toFloat();
        JangleIn[2] = inData.substring(J3start + 1, J4start).toFloat();
        JangleIn[3] = inData.substring(J4start + 1, J5start).toFloat();
        JangleIn[4] = inData.substring(J5start + 1, J6start).toFloat();
        JangleIn[5] = inData.substring(J6start + 1, WristConStart).toFloat();
        WristCon = inData.substring(WristConStart + 1);
        WristCon.trim();

        SolveInverseKinematic();

        String echo = "";
        delay(5);
        Serial.println(inData);
      }
      //-----COMMAND TO CALIBRATE---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "LL")
      {
        int J1start = inData.indexOf('A');
        int J2start = inData.indexOf('B');
        int J3start = inData.indexOf('C');
        int J4start = inData.indexOf('D');
        int J5start = inData.indexOf('E');
        int J6start = inData.indexOf('F');
        int J1calstart = inData.indexOf('G');
        int J2calstart = inData.indexOf('H');
        int J3calstart = inData.indexOf('I');
        int J4calstart = inData.indexOf('J');
        int J5calstart = inData.indexOf('K');
        int J6calstart = inData.indexOf('L');
        ///
        int J1req = inData.substring(J1start + 1, J2start).toInt();
        int J2req = inData.substring(J2start + 1, J3start).toInt();
        int J3req = inData.substring(J3start + 1, J4start).toInt();
        int J4req = inData.substring(J4start + 1, J5start).toInt();
        int J5req = inData.substring(J5start + 1, J6start).toInt();
        int J6req = inData.substring(J6start + 1, J1calstart).toInt();
        float J1calOff = inData.substring(J1calstart + 1, J2calstart).toFloat();
        float J2calOff = inData.substring(J2calstart + 1, J3calstart).toFloat();
        float J3calOff = inData.substring(J3calstart + 1, J4calstart).toFloat();
        float J4calOff = inData.substring(J4calstart + 1, J5calstart).toFloat();
        float J5calOff = inData.substring(J5calstart + 1, J6calstart).toFloat();
        float J6calOff = inData.substring(J6calstart + 1).toFloat();
        ///
        float SpeedIn;
        ///
        int J1Step = 0;
        int J2Step = 0;
        int J3Step = 0;
        int J4Step = 0;
        int J5Step = 0;
        int J6Step = 0;
        ///
        int J1stepCen = 0;
        int J2stepCen = 0;
        int J3stepCen = 0;
        int J4stepCen = 0;
        int J5stepCen = 0;
        int J6stepCen = 0;
        Alarm = "0";

        //--IF JOINT IS CALLED FOR CALIBRATION PASS ITS STEP LIMIT OTHERWISE PASS 0---
        if (J1req == 1)
        {
          J1Step = J1StepLim;
        }
        if (J2req == 1)
        {
          J2Step = J2StepLim;
        }
        if (J3req == 1)
        {
          J3Step = J3StepLim;
        }
        if (J4req == 1)
        {
          J4Step = J4StepLim;
        }
        if (J5req == 1)
        {
          J5Step = J5StepLim;
        }
        if (J6req == 1)
        {
          J6Step = J6StepLim;
        }

        //--CALL FUNCT TO DRIVE TO LIMITS--
        SpeedIn = 80;
        driveLimit(J1Step, J2Step, J3Step, J4Step, J5Step, J6Step, SpeedIn);
        delay(500);

        // BACKOFF
        digitalWrite(J1dirPin, LOW);
        digitalWrite(J2dirPin, LOW);
        digitalWrite(J3dirPin, HIGH);
        digitalWrite(J4dirPin, HIGH);
        digitalWrite(J5dirPin, LOW);
        digitalWrite(J6dirPin, HIGH);

        int BacOff = 0;
        while (BacOff <= 250)
        {
          if (J1req == 1)
          {
            digitalWrite(J1stepPin, LOW);
            delayMicroseconds(5);
            digitalWrite(J1stepPin, HIGH);
            delayMicroseconds(5);
          }
          if (J2req == 1)
          {
            digitalWrite(J2stepPin, LOW);
            delayMicroseconds(5);
            digitalWrite(J2stepPin, HIGH);
            delayMicroseconds(5);
          }
          if (J3req == 1)
          {
            digitalWrite(J3stepPin, LOW);
            delayMicroseconds(5);
            digitalWrite(J3stepPin, HIGH);
            delayMicroseconds(5);
          }
          if (J4req == 1)
          {
            digitalWrite(J4stepPin, LOW);
            delayMicroseconds(5);
            digitalWrite(J4stepPin, HIGH);
            delayMicroseconds(5);
          }
          if (J5req == 1)
          {
            digitalWrite(J5stepPin, LOW);
            delayMicroseconds(5);
            digitalWrite(J5stepPin, HIGH);
            delayMicroseconds(5);
          }
          if (J6req == 1)
          {
            digitalWrite(J6stepPin, LOW);
            delayMicroseconds(5);
            digitalWrite(J6stepPin, HIGH);
            delayMicroseconds(5);
          }
          BacOff = ++BacOff;
          delayMicroseconds(4000);
        }

        //--CALL FUNCT TO DRIVE BACK TO LIMITS SLOWLY--
        SpeedIn = .02;
        driveLimit(J1Step, J2Step, J3Step, J4Step, J5Step, J6Step, SpeedIn);

        // OVERDRIVE - MAKE SURE LIMIT SWITCH STAYS MADE
        digitalWrite(J1dirPin, HIGH);
        digitalWrite(J2dirPin, HIGH);
        digitalWrite(J3dirPin, LOW);
        digitalWrite(J4dirPin, LOW);
        digitalWrite(J5dirPin, HIGH);
        digitalWrite(J6dirPin, LOW);

        int OvrDrv = 0;
        while (OvrDrv <= 30)
        {
          if (J1req == 1)
          {
            digitalWrite(J1stepPin, LOW);
            delayMicroseconds(5);
            digitalWrite(J1stepPin, HIGH);
            delayMicroseconds(5);
          }
          if (J2req == 1)
          {
            digitalWrite(J2stepPin, LOW);
            delayMicroseconds(5);
            digitalWrite(J2stepPin, HIGH);
            delayMicroseconds(5);
          }
          if (J3req == 1)
          {
            digitalWrite(J3stepPin, LOW);
            delayMicroseconds(5);
            digitalWrite(J3stepPin, HIGH);
            delayMicroseconds(5);
          }
          if (J4req == 1)
          {
            digitalWrite(J4stepPin, LOW);
            delayMicroseconds(5);
            digitalWrite(J4stepPin, HIGH);
            delayMicroseconds(5);
          }
          if (J5req == 1)
          {
            digitalWrite(J5stepPin, LOW);
            delayMicroseconds(5);
            digitalWrite(J5stepPin, HIGH);
            delayMicroseconds(5);
          }
          if (J6req == 1)
          {
            digitalWrite(J6stepPin, LOW);
            delayMicroseconds(5);
            digitalWrite(J6stepPin, HIGH);
            delayMicroseconds(5);
          }
          OvrDrv = ++OvrDrv;
          delayMicroseconds(3000);
        }

        // SEE IF ANY SWITCHES NOT MADE
        delay(500);
        ///
        if (J1req == 1)
        {
          if (digitalRead(J1calPin) == LOW)
          {
            Alarm = "1";
          }
        }
        if (J2req == 1)
        {
          if (digitalRead(J2calPin) == LOW)
          {
            Alarm = "2";
          }
        }
        if (J3req == 1)
        {
          if (digitalRead(J3calPin) == LOW)
          {
            Alarm = "3";
          }
        }
        if (J4req == 1)
        {
          if (digitalRead(J4calPin) == LOW)
          {
            Alarm = "4";
          }
        }
        if (J5req == 1)
        {
          if (digitalRead(J5calPin) == LOW)
          {
            Alarm = "5";
          }
        }
        if (J6req == 1)
        {
          if (digitalRead(J6calPin) == LOW)
          {
            Alarm = "6";
          }
        }
        ///
        if (Alarm == "0")
        {

          // set master steps and center step
          if (J1req == 1)
          {
            J1StepM = ((J1axisLim) + J1calBaseOff + J1calOff) * J1StepDeg;
            J1stepCen = ((J1axisLimPos) + J1calBaseOff + J1calOff) * J1StepDeg;
          }
          if (J2req == 1)
          {
            J2StepM = (0 + J2calBaseOff + J2calOff) * J2StepDeg;
            J2stepCen = ((J2axisLimNeg)-J2calBaseOff - J2calOff) * J2StepDeg;
          }
          if (J3req == 1)
          {
            J3StepM = ((J3axisLim) + J3calBaseOff + J3calOff) * J3StepDeg;
            J3stepCen = ((J3axisLimPos) + J3calBaseOff + J3calOff) * J3StepDeg;
          }
          if (J4req == 1)
          {
            J4StepM = (0 + J4calBaseOff + J4calOff) * J4StepDeg;
            J4stepCen = ((J4axisLimNeg)-J4calBaseOff - J4calOff) * J4StepDeg;
          }
          if (J5req == 1)
          {
            J5StepM = (0 + J5calBaseOff + J5calOff) * J5StepDeg;
            J5stepCen = ((J5axisLimNeg)-J5calBaseOff - J5calOff) * J5StepDeg;
          }
          if (J6req == 1)
          {
            J6StepM = ((J6axisLim) + J6calBaseOff + J6calOff) * J6StepDeg;
            J6stepCen = ((J6axisLimNeg) + J6calBaseOff + J6calOff) * J6StepDeg;
          }
          // move to center
          int J1dir = 0;
          int J2dir = 1;
          int J3dir = 0;
          int J4dir = 1;
          int J5dir = 1;
          int J6dir = 0;
          int TRdir = 0;
          int TRstep = 0;
          float ACCspd = 10;
          float DCCspd = 10;
          String SpeedType = "p";
          float SpeedVal = 80;
          float ACCramp = 50;

          driveMotorsJ(J1stepCen, J2stepCen, J3stepCen, J4stepCen, J5stepCen, J6stepCen, TRstep, J1dir, J2dir, J3dir, J4dir, J5dir, J6dir, TRdir, SpeedType, SpeedVal, ACCspd, DCCspd, ACCramp);
          sendRobotPos();
        }
        else
        {
          delay(5);
          Serial.println(Alarm);
        }

        inData = ""; // Clear recieved buffer
      }

      //----- MOVE J ---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "MJ")
      {
        int J1dir;
        int J2dir;
        int J3dir;
        int J4dir;
        int J5dir;
        int J6dir;
        int TRdir;

        int J1axisFault = 0;
        int J2axisFault = 0;
        int J3axisFault = 0;
        int J4axisFault = 0;
        int J5axisFault = 0;
        int J6axisFault = 0;
        int TRaxisFault = 0;
        int TotalAxisFault = 0;

        Alarm = "0";

        int xStart = inData.indexOf("X");
        int yStart = inData.indexOf("Y");
        int zStart = inData.indexOf("Z");
        int rzStart = inData.indexOf("Rz");
        int ryStart = inData.indexOf("Ry");
        int rxStart = inData.indexOf("Rx");
        int tStart = inData.indexOf("Tr");
        int SPstart = inData.indexOf("S");
        int AcStart = inData.indexOf("Ac");
        int DcStart = inData.indexOf("Dc");
        int RmStart = inData.indexOf("Rm");
        int WristConStart = inData.indexOf("W");

        xyzuvw_In[0] = inData.substring(xStart + 1, yStart).toFloat();
        xyzuvw_In[1] = inData.substring(yStart + 1, zStart).toFloat();
        xyzuvw_In[2] = inData.substring(zStart + 1, rzStart).toFloat();
        xyzuvw_In[3] = inData.substring(rzStart + 2, ryStart).toFloat();
        xyzuvw_In[4] = inData.substring(ryStart + 2, rxStart).toFloat();
        xyzuvw_In[5] = inData.substring(rxStart + 2, tStart).toFloat();
        xyzuvw_In[6] = inData.substring(tStart + 2, SPstart).toFloat();

        String SpeedType = inData.substring(SPstart + 1, SPstart + 2);
        float SpeedVal = inData.substring(SPstart + 2, AcStart).toFloat();
        float ACCspd = inData.substring(AcStart + 2, DcStart).toFloat();
        float DCCspd = inData.substring(DcStart + 2, RmStart).toFloat();
        float ACCramp = inData.substring(RmStart + 2, WristConStart).toFloat();

        WristCon = inData.substring(WristConStart + 1);
        WristCon.trim();

        SolveInverseKinematic();

        // calc destination motor steps
        int J1futStepM = (JangleOut[0] + J1axisLimNeg) * J1StepDeg;
        int J2futStepM = (JangleOut[1] + J2axisLimNeg) * J2StepDeg;
        int J3futStepM = (JangleOut[2] + J3axisLimNeg) * J3StepDeg;
        int J4futStepM = (JangleOut[3] + J4axisLimNeg) * J4StepDeg;
        int J5futStepM = (JangleOut[4] + J5axisLimNeg) * J5StepDeg;
        int J6futStepM = (JangleOut[5] + J6axisLimNeg) * J6StepDeg;
        int TRfutStepM = (xyzuvw_In[6] + TRaxisLimNeg) * TRStepDeg;

        // calc delta from current to destination
        int J1stepDif = J1StepM - J1futStepM;
        int J2stepDif = J2StepM - J2futStepM;
        int J3stepDif = J3StepM - J3futStepM;
        int J4stepDif = J4StepM - J4futStepM;
        int J5stepDif = J5StepM - J5futStepM;
        int J6stepDif = J6StepM - J6futStepM;
        int TRstepDif = TRStepM - TRfutStepM;

        // determine motor directions
        if (J1stepDif <= 0)
        {
          J1dir = 1;
        }
        else
        {
          J1dir = 0;
        }

        if (J2stepDif <= 0)
        {
          J2dir = 1;
        }
        else
        {
          J2dir = 0;
        }

        if (J3stepDif <= 0)
        {
          J3dir = 1;
        }
        else
        {
          J3dir = 0;
        }

        if (J4stepDif <= 0)
        {
          J4dir = 1;
        }
        else
        {
          J4dir = 0;
        }

        if (J5stepDif <= 0)
        {
          J5dir = 1;
        }
        else
        {
          J5dir = 0;
        }

        if (J6stepDif <= 0)
        {
          J6dir = 1;
        }
        else
        {
          J6dir = 0;
        }

        if (TRstepDif <= 0)
        {
          TRdir = 1;
        }
        else
        {
          TRdir = 0;
        }

        // determine if requested position is within axis limits
        if ((J1dir == 1 and (J1StepM + J1stepDif > J1StepLim)) or (J1dir == 0 and (J1StepM - J1stepDif < 0)))
        {
          J1axisFault = 1;
        }
        if ((J2dir == 1 and (J2StepM + J2stepDif > J2StepLim)) or (J2dir == 0 and (J2StepM - J2stepDif < 0)))
        {
          J2axisFault = 1;
        }
        if ((J3dir == 1 and (J3StepM + J3stepDif > J3StepLim)) or (J3dir == 0 and (J3StepM - J3stepDif < 0)))
        {
          J3axisFault = 1;
        }
        if ((J4dir == 1 and (J4StepM + J4stepDif > J4StepLim)) or (J4dir == 0 and (J4StepM - J4stepDif < 0)))
        {
          J4axisFault = 1;
        }
        if ((J5dir == 1 and (J5StepM + J5stepDif > J5StepLim)) or (J5dir == 0 and (J5StepM - J5stepDif < 0)))
        {
          J5axisFault = 1;
        }
        if ((J6dir == 1 and (J6StepM + J6stepDif > J6StepLim)) or (J6dir == 0 and (J6StepM - J6stepDif < 0)))
        {
          J6axisFault = 1;
        }
        if ((TRdir == 1 and (TRStepM + TRstepDif > TRStepLim)) or (TRdir == 0 and (TRStepM - TRstepDif < 0)))
        {
          TRaxisFault = 1;
        }
        TotalAxisFault = J1axisFault + J2axisFault + J3axisFault + J4axisFault + J5axisFault + J6axisFault + TRaxisFault;

        // send move command if no axis limit error
        if (TotalAxisFault == 0 && KinematicError == 0)
        {
          resetEncoders();
          driveMotorsJ(abs(J1stepDif), abs(J2stepDif), abs(J3stepDif), abs(J4stepDif), abs(J5stepDif), abs(J6stepDif), abs(TRstepDif), J1dir, J2dir, J3dir, J4dir, J5dir, J6dir, TRdir, SpeedType, SpeedVal, ACCspd, DCCspd, ACCramp);
          checkEncoders();
          sendRobotPos();
        }
        else if (KinematicError == 1)
        {
          Alarm = "ER";
          delay(5);
          Serial.println(Alarm);
        }
        else
        {
          Alarm = "EL" + String(J1axisFault) + String(J2axisFault) + String(J3axisFault) + String(J4axisFault) + String(J5axisFault) + String(J6axisFault) + String(TRaxisFault);
          delay(5);
          Serial.println(Alarm);
        }

        inData = ""; // Clear recieved buffer
        ////////MOVE COMPLETE///////////
      }

      //----- LIVE CARTESIAN JOG  ---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "LC")
      {
        delay(5);
        Serial.println();

        updatePos();

        int J1dir;
        int J2dir;
        int J3dir;
        int J4dir;
        int J5dir;
        int J6dir;
        int TRdir;

        int J1axisFault = 0;
        int J2axisFault = 0;
        int J3axisFault = 0;
        int J4axisFault = 0;
        int J5axisFault = 0;
        int J6axisFault = 0;
        int TRaxisFault = 0;
        int TotalAxisFault = 0;

        bool JogInPoc = true;
        Alarm = "0";

        int VStart = inData.indexOf("V");
        int SPstart = inData.indexOf("S");
        int AcStart = inData.indexOf("Ac");
        int DcStart = inData.indexOf("Dc");
        int RmStart = inData.indexOf("Rm");
        int WristConStart = inData.indexOf("W");

        float Vector = inData.substring(VStart + 1, SPstart).toFloat();
        String SpeedType = inData.substring(SPstart + 1, SPstart + 2);
        float SpeedVal = inData.substring(SPstart + 2, AcStart).toFloat();
        float ACCspd = 100;
        float DCCspd = 100;
        float ACCramp = 100;

        WristCon = inData.substring(WristConStart + 1);
        WristCon.trim();

        inData = ""; // Clear recieved buffer

        while (JogInPoc = true)
        {

          xyzuvw_In[0] = xyzuvw_Out[0];
          xyzuvw_In[1] = xyzuvw_Out[1];
          xyzuvw_In[2] = xyzuvw_Out[2];
          xyzuvw_In[3] = xyzuvw_Out[3];
          xyzuvw_In[4] = xyzuvw_Out[4];
          xyzuvw_In[5] = xyzuvw_Out[5];

          if (Vector == 10)
          {
            xyzuvw_In[0] = xyzuvw_Out[0] - 1;
          }
          if (Vector == 11)
          {
            xyzuvw_In[0] = xyzuvw_Out[0] + 1;
          }

          if (Vector == 20)
          {
            xyzuvw_In[1] = xyzuvw_Out[1] - 1;
          }
          if (Vector == 21)
          {
            xyzuvw_In[1] = xyzuvw_Out[1] + 1;
          }

          if (Vector == 30)
          {
            xyzuvw_In[2] = xyzuvw_Out[2] - 1;
          }
          if (Vector == 31)
          {
            xyzuvw_In[2] = xyzuvw_Out[2] + 1;
          }

          if (Vector == 40)
          {
            xyzuvw_In[3] = xyzuvw_Out[3] - 1;
          }
          if (Vector == 41)
          {
            xyzuvw_In[3] = xyzuvw_Out[3] + 1;
          }

          if (Vector == 50)
          {
            xyzuvw_In[4] = xyzuvw_Out[4] - 1;
          }
          if (Vector == 51)
          {
            xyzuvw_In[4] = xyzuvw_Out[4] + 1;
          }

          if (Vector == 60)
          {
            xyzuvw_In[5] = xyzuvw_Out[5] - 1;
          }
          if (Vector == 61)
          {
            xyzuvw_In[5] = xyzuvw_Out[5] + 1;
          }

          SolveInverseKinematic();

          // calc destination motor steps
          int J1futStepM = (JangleOut[0] + J1axisLimNeg) * J1StepDeg;
          int J2futStepM = (JangleOut[1] + J2axisLimNeg) * J2StepDeg;
          int J3futStepM = (JangleOut[2] + J3axisLimNeg) * J3StepDeg;
          int J4futStepM = (JangleOut[3] + J4axisLimNeg) * J4StepDeg;
          int J5futStepM = (JangleOut[4] + J5axisLimNeg) * J5StepDeg;
          int J6futStepM = (JangleOut[5] + J6axisLimNeg) * J6StepDeg;

          // calc delta from current to destination
          int J1stepDif = J1StepM - J1futStepM;
          int J2stepDif = J2StepM - J2futStepM;
          int J3stepDif = J3StepM - J3futStepM;
          int J4stepDif = J4StepM - J4futStepM;
          int J5stepDif = J5StepM - J5futStepM;
          int J6stepDif = J6StepM - J6futStepM;
          int TRstepDif = 0;

          // determine motor directions
          if (J1stepDif <= 0)
          {
            J1dir = 1;
          }
          else
          {
            J1dir = 0;
          }

          if (J2stepDif <= 0)
          {
            J2dir = 1;
          }
          else
          {
            J2dir = 0;
          }

          if (J3stepDif <= 0)
          {
            J3dir = 1;
          }
          else
          {
            J3dir = 0;
          }

          if (J4stepDif <= 0)
          {
            J4dir = 1;
          }
          else
          {
            J4dir = 0;
          }

          if (J5stepDif <= 0)
          {
            J5dir = 1;
          }
          else
          {
            J5dir = 0;
          }

          if (J6stepDif <= 0)
          {
            J6dir = 1;
          }
          else
          {
            J6dir = 0;
          }

          if (TRstepDif <= 0)
          {
            TRdir = 1;
          }
          else
          {
            TRdir = 0;
          }

          // determine if requested position is within axis limits
          if ((J1dir == 1 and (J1StepM + J1stepDif > J1StepLim)) or (J1dir == 0 and (J1StepM - J1stepDif < 0)))
          {
            J1axisFault = 1;
          }
          if ((J2dir == 1 and (J2StepM + J2stepDif > J2StepLim)) or (J2dir == 0 and (J2StepM - J2stepDif < 0)))
          {
            J2axisFault = 1;
          }
          if ((J3dir == 1 and (J3StepM + J3stepDif > J3StepLim)) or (J3dir == 0 and (J3StepM - J3stepDif < 0)))
          {
            J3axisFault = 1;
          }
          if ((J4dir == 1 and (J4StepM + J4stepDif > J4StepLim)) or (J4dir == 0 and (J4StepM - J4stepDif < 0)))
          {
            J4axisFault = 1;
          }
          if ((J5dir == 1 and (J5StepM + J5stepDif > J5StepLim)) or (J5dir == 0 and (J5StepM - J5stepDif < 0)))
          {
            J5axisFault = 1;
          }
          if ((J6dir == 1 and (J6StepM + J6stepDif > J6StepLim)) or (J6dir == 0 and (J6StepM - J6stepDif < 0)))
          {
            J6axisFault = 1;
          }
          if ((TRdir == 1 and (TRStepM + TRstepDif > TRStepLim)) or (TRdir == 0 and (TRStepM - TRstepDif < 0)))
          {
            TRaxisFault = 1;
          }
          TotalAxisFault = J1axisFault + J2axisFault + J3axisFault + J4axisFault + J5axisFault + J6axisFault + TRaxisFault;

          // send move command if no axis limit error
          if (TotalAxisFault == 0 && KinematicError == 0)
          {
            resetEncoders();
            driveMotorsJ(abs(J1stepDif), abs(J2stepDif), abs(J3stepDif), abs(J4stepDif), abs(J5stepDif), abs(J6stepDif), abs(TRstepDif), J1dir, J2dir, J3dir, J4dir, J5dir, J6dir, TRdir, SpeedType, SpeedVal, ACCspd, DCCspd, ACCramp);
            checkEncoders();
            updatePos();
          }

          // stop loop if any serial command is recieved - but the expected command is "S" to stop the loop.

          char recieved = Serial.read();
          inData += recieved;
          if (recieved == '\n')
          {
            break;
          }

          // end loop
        }

        // send move command if no axis limit error
        if (TotalAxisFault == 0 && KinematicError == 0)
        {
          sendRobotPos();
        }
        else if (KinematicError == 1)
        {
          Alarm = "ER";
          delay(5);
          Serial.println(Alarm);
        }
        else
        {
          Alarm = "EL" + String(J1axisFault) + String(J2axisFault) + String(J3axisFault) + String(J4axisFault) + String(J5axisFault) + String(J6axisFault) + String(TRaxisFault);
          delay(5);
          Serial.println(Alarm);
        }

        inData = ""; // Clear recieved buffer
        ////////MOVE COMPLETE///////////
      }

      //----- LIVE JOINT JOG  ---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "LJ")
      {
        delay(5);
        Serial.println();
        updatePos();

        int J1dir;
        int J2dir;
        int J3dir;
        int J4dir;
        int J5dir;
        int J6dir;
        int TRdir;

        int J1axisFault = 0;
        int J2axisFault = 0;
        int J3axisFault = 0;
        int J4axisFault = 0;
        int J5axisFault = 0;
        int J6axisFault = 0;
        int TRaxisFault = 0;
        int TotalAxisFault = 0;

        bool JogInPoc = true;
        Alarm = "0";

        int VStart = inData.indexOf("V");
        int SPstart = inData.indexOf("S");
        int AcStart = inData.indexOf("Ac");
        int DcStart = inData.indexOf("Dc");
        int RmStart = inData.indexOf("Rm");
        int WristConStart = inData.indexOf("W");

        float Vector = inData.substring(VStart + 1, SPstart).toFloat();
        String SpeedType = inData.substring(SPstart + 1, SPstart + 2);
        float SpeedVal = inData.substring(SPstart + 2, AcStart).toFloat();
        float ACCspd = 100;
        float DCCspd = 100;
        float ACCramp = 100;

        WristCon = inData.substring(WristConStart + 1);
        WristCon.trim();

        inData = ""; // Clear recieved buffer

        while (JogInPoc = true)
        {

          float J1Angle = JangleIn[0];
          float J2Angle = JangleIn[1];
          float J3Angle = JangleIn[2];
          float J4Angle = JangleIn[3];
          float J5Angle = JangleIn[4];
          float J6Angle = JangleIn[5];
          float xyzuvw_In[6];

          if (Vector == 10)
          {
            J1Angle = JangleIn[0] - .5;
          }
          if (Vector == 11)
          {
            J1Angle = JangleIn[0] + .5;
          }

          if (Vector == 20)
          {
            J2Angle = JangleIn[1] - .5;
          }
          if (Vector == 21)
          {
            J2Angle = JangleIn[1] + .5;
          }

          if (Vector == 30)
          {
            J3Angle = JangleIn[2] - .5;
          }
          if (Vector == 31)
          {
            J3Angle = JangleIn[2] + .5;
          }

          if (Vector == 40)
          {
            J4Angle = JangleIn[3] - .5;
          }
          if (Vector == 41)
          {
            J4Angle = JangleIn[3] + .5;
          }

          if (Vector == 50)
          {
            J5Angle = JangleIn[4] - .5;
          }
          if (Vector == 51)
          {
            J5Angle = JangleIn[4] + .5;
          }

          if (Vector == 60)
          {
            J6Angle = JangleIn[5] - .5;
          }
          if (Vector == 61)
          {
            J6Angle = JangleIn[5] + .5;
          }

          // calc destination motor steps
          int J1futStepM = (J1Angle + J1axisLimNeg) * J1StepDeg;
          int J2futStepM = (J2Angle + J2axisLimNeg) * J2StepDeg;
          int J3futStepM = (J3Angle + J3axisLimNeg) * J3StepDeg;
          int J4futStepM = (J4Angle + J4axisLimNeg) * J4StepDeg;
          int J5futStepM = (J5Angle + J5axisLimNeg) * J5StepDeg;
          int J6futStepM = (J6Angle + J6axisLimNeg) * J6StepDeg;

          // calc delta from current to destination
          int J1stepDif = J1StepM - J1futStepM;
          int J2stepDif = J2StepM - J2futStepM;
          int J3stepDif = J3StepM - J3futStepM;
          int J4stepDif = J4StepM - J4futStepM;
          int J5stepDif = J5StepM - J5futStepM;
          int J6stepDif = J6StepM - J6futStepM;
          int TRstepDif = 0;

          // determine motor directions
          if (J1stepDif <= 0)
          {
            J1dir = 1;
          }
          else
          {
            J1dir = 0;
          }

          if (J2stepDif <= 0)
          {
            J2dir = 1;
          }
          else
          {
            J2dir = 0;
          }

          if (J3stepDif <= 0)
          {
            J3dir = 1;
          }
          else
          {
            J3dir = 0;
          }

          if (J4stepDif <= 0)
          {
            J4dir = 1;
          }
          else
          {
            J4dir = 0;
          }

          if (J5stepDif <= 0)
          {
            J5dir = 1;
          }
          else
          {
            J5dir = 0;
          }

          if (J6stepDif <= 0)
          {
            J6dir = 1;
          }
          else
          {
            J6dir = 0;
          }

          if (TRstepDif <= 0)
          {
            TRdir = 1;
          }
          else
          {
            TRdir = 0;
          }

          // determine if requested position is within axis limits
          if ((J1dir == 1 and (J1StepM + J1stepDif > J1StepLim)) or (J1dir == 0 and (J1StepM - J1stepDif < 0)))
          {
            J1axisFault = 1;
          }
          if ((J2dir == 1 and (J2StepM + J2stepDif > J2StepLim)) or (J2dir == 0 and (J2StepM - J2stepDif < 0)))
          {
            J2axisFault = 1;
          }
          if ((J3dir == 1 and (J3StepM + J3stepDif > J3StepLim)) or (J3dir == 0 and (J3StepM - J3stepDif < 0)))
          {
            J3axisFault = 1;
          }
          if ((J4dir == 1 and (J4StepM + J4stepDif > J4StepLim)) or (J4dir == 0 and (J4StepM - J4stepDif < 0)))
          {
            J4axisFault = 1;
          }
          if ((J5dir == 1 and (J5StepM + J5stepDif > J5StepLim)) or (J5dir == 0 and (J5StepM - J5stepDif < 0)))
          {
            J5axisFault = 1;
          }
          if ((J6dir == 1 and (J6StepM + J6stepDif > J6StepLim)) or (J6dir == 0 and (J6StepM - J6stepDif < 0)))
          {
            J6axisFault = 1;
          }
          if ((TRdir == 1 and (TRStepM + TRstepDif > TRStepLim)) or (TRdir == 0 and (TRStepM - TRstepDif < 0)))
          {
            TRaxisFault = 1;
          }
          TotalAxisFault = J1axisFault + J2axisFault + J3axisFault + J4axisFault + J5axisFault + J6axisFault + TRaxisFault;

          // send move command if no axis limit error
          if (TotalAxisFault == 0 && KinematicError == 0)
          {
            resetEncoders();
            driveMotorsJ(abs(J1stepDif), abs(J2stepDif), abs(J3stepDif), abs(J4stepDif), abs(J5stepDif), abs(J6stepDif), abs(TRstepDif), J1dir, J2dir, J3dir, J4dir, J5dir, J6dir, TRdir, SpeedType, SpeedVal, ACCspd, DCCspd, ACCramp);
            checkEncoders();
            updatePos();
          }

          // stop loop if any serial command is recieved - but the expected command is "S" to stop the loop.

          char recieved = Serial.read();
          inData += recieved;
          if (recieved == '\n')
          {
            break;
          }

          // end loop
        }

        // send move command if no axis limit error
        if (TotalAxisFault == 0 && KinematicError == 0)
        {
          sendRobotPos();
        }
        else if (KinematicError == 1)
        {
          Alarm = "ER";
          delay(5);
          Serial.println(Alarm);
        }
        else
        {
          Alarm = "EL" + String(J1axisFault) + String(J2axisFault) + String(J3axisFault) + String(J4axisFault) + String(J5axisFault) + String(J6axisFault) + String(TRaxisFault);
          delay(5);
          Serial.println(Alarm);
        }

        inData = ""; // Clear recieved buffer
        ////////MOVE COMPLETE///////////
      }

      //----- LIVE TOOL JOG  ---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "LT")
      {
        delay(5);
        Serial.println();

        updatePos();

        int J1dir;
        int J2dir;
        int J3dir;
        int J4dir;
        int J5dir;
        int J6dir;
        int TRdir;

        int J1axisFault = 0;
        int J2axisFault = 0;
        int J3axisFault = 0;
        int J4axisFault = 0;
        int J5axisFault = 0;
        int J6axisFault = 0;
        int TRaxisFault = 0;
        int TotalAxisFault = 0;

        float Xtool = Robot_Kin_Tool[0];
        float Ytool = Robot_Kin_Tool[1];
        float Ztool = Robot_Kin_Tool[2];
        float RZtool = Robot_Kin_Tool[3];
        float RYtool = Robot_Kin_Tool[4];
        float RXtool = Robot_Kin_Tool[5];

        bool JogInPoc = true;
        Alarm = "0";

        int VStart = inData.indexOf("V");
        int SPstart = inData.indexOf("S");
        int AcStart = inData.indexOf("Ac");
        int DcStart = inData.indexOf("Dc");
        int RmStart = inData.indexOf("Rm");
        int WristConStart = inData.indexOf("W");

        float Vector = inData.substring(VStart + 1, SPstart).toFloat();
        String SpeedType = inData.substring(SPstart + 1, SPstart + 2);
        float SpeedVal = inData.substring(SPstart + 2, AcStart).toFloat();
        float ACCspd = 100;
        float DCCspd = 100;
        float ACCramp = 100;

        WristCon = inData.substring(WristConStart + 1);
        WristCon.trim();

        inData = ""; // Clear recieved buffer

        while (JogInPoc = true)
        {

          Xtool = Robot_Kin_Tool[0];
          Ytool = Robot_Kin_Tool[1];
          Ztool = Robot_Kin_Tool[2];
          RXtool = Robot_Kin_Tool[3];
          RYtool = Robot_Kin_Tool[4];
          RZtool = Robot_Kin_Tool[5];

          if (Vector == 10)
          {
            Robot_Kin_Tool[0] = Robot_Kin_Tool[0] - 1;
          }
          if (Vector == 11)
          {
            Robot_Kin_Tool[0] = Robot_Kin_Tool[0] + 1;
          }

          if (Vector == 20)
          {
            Robot_Kin_Tool[1] = Robot_Kin_Tool[1] - 1;
          }
          if (Vector == 21)
          {
            Robot_Kin_Tool[1] = Robot_Kin_Tool[1] + 1;
          }

          if (Vector == 30)
          {
            Robot_Kin_Tool[2] = Robot_Kin_Tool[2] - 1;
          }
          if (Vector == 31)
          {
            Robot_Kin_Tool[2] = Robot_Kin_Tool[2] + 1;
          }

          if (Vector == 60)
          {
            Robot_Kin_Tool[3] = Robot_Kin_Tool[3] - 1 * M_PI / 180;
          }
          if (Vector == 61)
          {
            Robot_Kin_Tool[3] = Robot_Kin_Tool[3] + 1 * M_PI / 180;
          }

          if (Vector == 50)
          {
            Robot_Kin_Tool[4] = Robot_Kin_Tool[4] - 1 * M_PI / 180;
          }
          if (Vector == 51)
          {
            Robot_Kin_Tool[4] = Robot_Kin_Tool[4] + 1 * M_PI / 180;
          }

          if (Vector == 40)
          {
            Robot_Kin_Tool[5] = Robot_Kin_Tool[5] - 1 * M_PI / 180;
          }
          if (Vector == 41)
          {
            Robot_Kin_Tool[5] = Robot_Kin_Tool[5] + 1 * M_PI / 180;
          }

          JangleIn[0] = (J1StepM - J1zeroStep) / J1StepDeg;
          JangleIn[1] = (J2StepM - J2zeroStep) / J2StepDeg;
          JangleIn[2] = (J3StepM - J3zeroStep) / J3StepDeg;
          JangleIn[3] = (J4StepM - J4zeroStep) / J4StepDeg;
          JangleIn[4] = (J5StepM - J5zeroStep) / J5StepDeg;
          JangleIn[5] = (J6StepM - J6zeroStep) / J6StepDeg;

          xyzuvw_In[0] = xyzuvw_Out[0];
          xyzuvw_In[1] = xyzuvw_Out[1];
          xyzuvw_In[2] = xyzuvw_Out[2];
          xyzuvw_In[3] = xyzuvw_Out[3];
          xyzuvw_In[4] = xyzuvw_Out[4];
          xyzuvw_In[5] = xyzuvw_Out[5];

          SolveInverseKinematic();

          Robot_Kin_Tool[0] = Xtool;
          Robot_Kin_Tool[1] = Ytool;
          Robot_Kin_Tool[2] = Ztool;
          Robot_Kin_Tool[3] = RXtool;
          Robot_Kin_Tool[4] = RYtool;
          Robot_Kin_Tool[5] = RZtool;

          // calc destination motor steps
          int J1futStepM = (JangleOut[0] + J1axisLimNeg) * J1StepDeg;
          int J2futStepM = (JangleOut[1] + J2axisLimNeg) * J2StepDeg;
          int J3futStepM = (JangleOut[2] + J3axisLimNeg) * J3StepDeg;
          int J4futStepM = (JangleOut[3] + J4axisLimNeg) * J4StepDeg;
          int J5futStepM = (JangleOut[4] + J5axisLimNeg) * J5StepDeg;
          int J6futStepM = (JangleOut[5] + J6axisLimNeg) * J6StepDeg;

          // calc delta from current to destination
          int J1stepDif = J1StepM - J1futStepM;
          int J2stepDif = J2StepM - J2futStepM;
          int J3stepDif = J3StepM - J3futStepM;
          int J4stepDif = J4StepM - J4futStepM;
          int J5stepDif = J5StepM - J5futStepM;
          int J6stepDif = J6StepM - J6futStepM;
          int TRstepDif = 0;

          // determine motor directions
          if (J1stepDif <= 0)
          {
            J1dir = 1;
          }
          else
          {
            J1dir = 0;
          }

          if (J2stepDif <= 0)
          {
            J2dir = 1;
          }
          else
          {
            J2dir = 0;
          }

          if (J3stepDif <= 0)
          {
            J3dir = 1;
          }
          else
          {
            J3dir = 0;
          }

          if (J4stepDif <= 0)
          {
            J4dir = 1;
          }
          else
          {
            J4dir = 0;
          }

          if (J5stepDif <= 0)
          {
            J5dir = 1;
          }
          else
          {
            J5dir = 0;
          }

          if (J6stepDif <= 0)
          {
            J6dir = 1;
          }
          else
          {
            J6dir = 0;
          }

          if (TRstepDif <= 0)
          {
            TRdir = 1;
          }
          else
          {
            TRdir = 0;
          }

          // determine if requested position is within axis limits
          if ((J1dir == 1 and (J1StepM + J1stepDif > J1StepLim)) or (J1dir == 0 and (J1StepM - J1stepDif < 0)))
          {
            J1axisFault = 1;
          }
          if ((J2dir == 1 and (J2StepM + J2stepDif > J2StepLim)) or (J2dir == 0 and (J2StepM - J2stepDif < 0)))
          {
            J2axisFault = 1;
          }
          if ((J3dir == 1 and (J3StepM + J3stepDif > J3StepLim)) or (J3dir == 0 and (J3StepM - J3stepDif < 0)))
          {
            J3axisFault = 1;
          }
          if ((J4dir == 1 and (J4StepM + J4stepDif > J4StepLim)) or (J4dir == 0 and (J4StepM - J4stepDif < 0)))
          {
            J4axisFault = 1;
          }
          if ((J5dir == 1 and (J5StepM + J5stepDif > J5StepLim)) or (J5dir == 0 and (J5StepM - J5stepDif < 0)))
          {
            J5axisFault = 1;
          }
          if ((J6dir == 1 and (J6StepM + J6stepDif > J6StepLim)) or (J6dir == 0 and (J6StepM - J6stepDif < 0)))
          {
            J6axisFault = 1;
          }
          if ((TRdir == 1 and (TRStepM + TRstepDif > TRStepLim)) or (TRdir == 0 and (TRStepM - TRstepDif < 0)))
          {
            TRaxisFault = 1;
          }
          TotalAxisFault = J1axisFault + J2axisFault + J3axisFault + J4axisFault + J5axisFault + J6axisFault + TRaxisFault;

          // send move command if no axis limit error
          if (TotalAxisFault == 0 && KinematicError == 0)
          {
            resetEncoders();
            driveMotorsJ(abs(J1stepDif), abs(J2stepDif), abs(J3stepDif), abs(J4stepDif), abs(J5stepDif), abs(J6stepDif), abs(TRstepDif), J1dir, J2dir, J3dir, J4dir, J5dir, J6dir, TRdir, SpeedType, SpeedVal, ACCspd, DCCspd, ACCramp);
            checkEncoders();
            updatePos();
          }

          // stop loop if any serial command is recieved - but the expected command is "S" to stop the loop.

          char recieved = Serial.read();
          inData += recieved;
          if (recieved == '\n')
          {
            break;
          }

          // end loop
        }

        // send move command if no axis limit error
        if (TotalAxisFault == 0 && KinematicError == 0)
        {
          sendRobotPos();
        }
        else if (KinematicError == 1)
        {
          Alarm = "ER";
          delay(5);
          Serial.println(Alarm);
        }
        else
        {
          Alarm = "EL" + String(J1axisFault) + String(J2axisFault) + String(J3axisFault) + String(J4axisFault) + String(J5axisFault) + String(J6axisFault) + String(TRaxisFault);
          delay(5);
          Serial.println(Alarm);
        }

        inData = ""; // Clear recieved buffer
        ////////MOVE COMPLETE///////////
      }

      //----- MOVE J IN JOINTS  ---------------------------------------------------
      //-----------------------------------------------------------------------

      if (function == "RJ")
      {
        int J1dir;
        int J2dir;
        int J3dir;
        int J4dir;
        int J5dir;
        int J6dir;
        int TRdir;

        int J1axisFault = 0;
        int J2axisFault = 0;
        int J3axisFault = 0;
        int J4axisFault = 0;
        int J5axisFault = 0;
        int J6axisFault = 0;
        int TRaxisFault = 0;
        int TotalAxisFault = 0;

        int J1stepStart = inData.indexOf("A");
        int J2stepStart = inData.indexOf("B");
        int J3stepStart = inData.indexOf("C");
        int J4stepStart = inData.indexOf("D");
        int J5stepStart = inData.indexOf("E");
        int J6stepStart = inData.indexOf("F");
        int tStart = inData.indexOf("G");
        int SPstart = inData.indexOf("S");
        int AcStart = inData.indexOf("Ac");
        int DcStart = inData.indexOf("Dc");
        int RmStart = inData.indexOf("Rm");
        int WristConStart = inData.indexOf("W");

        float J1Angle;
        float J2Angle;
        float J3Angle;
        float J4Angle;
        float J5Angle;
        float J6Angle;
        float xyzuvw_In[6];
        float railpos;

        J1Angle = inData.substring(J1stepStart + 1, J2stepStart).toFloat();
        J2Angle = inData.substring(J2stepStart + 1, J3stepStart).toFloat();
        J3Angle = inData.substring(J3stepStart + 1, J4stepStart).toFloat();
        J4Angle = inData.substring(J4stepStart + 1, J5stepStart).toFloat();
        J5Angle = inData.substring(J5stepStart + 1, J6stepStart).toFloat();
        J6Angle = inData.substring(J6stepStart + 1, tStart).toFloat();
        xyzuvw_In[6] = inData.substring(tStart + 1, SPstart).toFloat();
        String SpeedType = inData.substring(SPstart + 1, SPstart + 2);
        float SpeedVal = inData.substring(SPstart + 2, AcStart).toFloat();
        float ACCspd = inData.substring(AcStart + 2, DcStart).toFloat();
        float DCCspd = inData.substring(DcStart + 2, RmStart).toFloat();
        float ACCramp = inData.substring(RmStart + 2, WristConStart).toFloat();

        int J1futStepM = (J1Angle + J1axisLimNeg) * J1StepDeg;
        int J2futStepM = (J2Angle + J2axisLimNeg) * J2StepDeg;
        int J3futStepM = (J3Angle + J3axisLimNeg) * J3StepDeg;
        int J4futStepM = (J4Angle + J4axisLimNeg) * J4StepDeg;
        int J5futStepM = (J5Angle + J5axisLimNeg) * J5StepDeg;
        int J6futStepM = (J6Angle + J6axisLimNeg) * J6StepDeg;
        int TRfutStepM = (xyzuvw_In[6] + TRaxisLimNeg) * TRStepDeg;

        // calc delta from current to destination
        int J1stepDif = J1StepM - J1futStepM;
        int J2stepDif = J2StepM - J2futStepM;
        int J3stepDif = J3StepM - J3futStepM;
        int J4stepDif = J4StepM - J4futStepM;
        int J5stepDif = J5StepM - J5futStepM;
        int J6stepDif = J6StepM - J6futStepM;
        int TRstepDif = TRStepM - TRfutStepM;

        // determine motor directions
        if (J1stepDif <= 0)
        {
          J1dir = 1;
        }
        else
        {
          J1dir = 0;
        }

        if (J2stepDif <= 0)
        {
          J2dir = 1;
        }
        else
        {
          J2dir = 0;
        }

        if (J3stepDif <= 0)
        {
          J3dir = 1;
        }
        else
        {
          J3dir = 0;
        }

        if (J4stepDif <= 0)
        {
          J4dir = 1;
        }
        else
        {
          J4dir = 0;
        }

        if (J5stepDif <= 0)
        {
          J5dir = 1;
        }
        else
        {
          J5dir = 0;
        }

        if (J6stepDif <= 0)
        {
          J6dir = 1;
        }
        else
        {
          J6dir = 0;
        }

        if (TRstepDif <= 0)
        {
          TRdir = 1;
        }
        else
        {
          TRdir = 0;
        }

        // determine if requested position is within axis limits
        if ((J1dir == 1 and (J1StepM + J1stepDif > J1StepLim)) or (J1dir == 0 and (J1StepM - J1stepDif < 0)))
        {
          J1axisFault = 1;
        }
        if ((J2dir == 1 and (J2StepM + J2stepDif > J2StepLim)) or (J2dir == 0 and (J2StepM - J2stepDif < 0)))
        {
          J2axisFault = 1;
        }
        if ((J3dir == 1 and (J3StepM + J3stepDif > J3StepLim)) or (J3dir == 0 and (J3StepM - J3stepDif < 0)))
        {
          J3axisFault = 1;
        }
        if ((J4dir == 1 and (J4StepM + J4stepDif > J4StepLim)) or (J4dir == 0 and (J4StepM - J4stepDif < 0)))
        {
          J4axisFault = 1;
        }
        if ((J5dir == 1 and (J5StepM + J5stepDif > J5StepLim)) or (J5dir == 0 and (J5StepM - J5stepDif < 0)))
        {
          J5axisFault = 1;
        }
        if ((J6dir == 1 and (J6StepM + J6stepDif > J6StepLim)) or (J6dir == 0 and (J6StepM - J6stepDif < 0)))
        {
          J6axisFault = 1;
        }
        if ((TRdir == 1 and (TRStepM + TRstepDif > TRStepLim)) or (TRdir == 0 and (TRStepM - TRstepDif < 0)))
        {
          TRaxisFault = 1;
        }
        TotalAxisFault = J1axisFault + J2axisFault + J3axisFault + J4axisFault + J5axisFault + J6axisFault + TRaxisFault;

        // send move command if no axis limit error
        if (TotalAxisFault == 0 && KinematicError == 0)
        {
          resetEncoders();
          driveMotorsJ(abs(J1stepDif), abs(J2stepDif), abs(J3stepDif), abs(J4stepDif), abs(J5stepDif), abs(J6stepDif), abs(TRstepDif), J1dir, J2dir, J3dir, J4dir, J5dir, J6dir, TRdir, SpeedType, SpeedVal, ACCspd, DCCspd, ACCramp);
          checkEncoders();
          sendRobotPos();
        }
        else if (KinematicError == 1)
        {
          Alarm = "ER";
          delay(5);
          Serial.println(Alarm);
        }
        else
        {
          Alarm = "EL" + String(J1axisFault) + String(J2axisFault) + String(J3axisFault) + String(J4axisFault) + String(J5axisFault) + String(J6axisFault) + String(TRaxisFault);
          delay(5);
          Serial.println(Alarm);
        }

        inData = ""; // Clear recieved buffer
        ////////MOVE COMPLETE///////////
      }

      //----- Jog T ---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "JT")
      {
        int J1dir;
        int J2dir;
        int J3dir;
        int J4dir;
        int J5dir;
        int J6dir;
        int TRdir;

        float Xtool = Robot_Kin_Tool[0];
        float Ytool = Robot_Kin_Tool[1];
        float Ztool = Robot_Kin_Tool[2];
        float RZtool = Robot_Kin_Tool[3];
        float RYtool = Robot_Kin_Tool[4];
        float RXtool = Robot_Kin_Tool[5];

        int J1axisFault = 0;
        int J2axisFault = 0;
        int J3axisFault = 0;
        int J4axisFault = 0;
        int J5axisFault = 0;
        int J6axisFault = 0;
        int TotalAxisFault = 0;

        String Alarm = "0";

        int SPstart = inData.indexOf('S');
        int AcStart = inData.indexOf('G');
        int DcStart = inData.indexOf('H');
        int RmStart = inData.indexOf('I');

        String Dir = inData.substring(0, 2); // this should be Z0 or Z1
        float Dist = inData.substring(2, SPstart).toFloat();
        String SpeedType = inData.substring(SPstart + 1, SPstart + 2);
        float SpeedVal = inData.substring(SPstart + 3, AcStart).toFloat();
        float ACCspd = inData.substring(AcStart + 1, DcStart).toInt();
        float DCCspd = inData.substring(DcStart + 1, RmStart).toInt();
        float ACCramp = inData.substring(RmStart + 1).toInt();

        if (Dir == "X0")
        {
          Robot_Kin_Tool[0] = Robot_Kin_Tool[0] + Dist;
        }
        else if (Dir == "X1")
        {
          Robot_Kin_Tool[0] = Robot_Kin_Tool[0] - Dist;
        }
        else if (Dir == "Y0")
        {
          Robot_Kin_Tool[1] = Robot_Kin_Tool[1] + Dist;
        }
        else if (Dir == "Y1")
        {
          Robot_Kin_Tool[1] = Robot_Kin_Tool[1] - Dist;
        }
        else if (Dir == "Z0")
        {
          Robot_Kin_Tool[2] = Robot_Kin_Tool[2] + Dist;
        }
        else if (Dir == "Z1")
        {
          Robot_Kin_Tool[2] = Robot_Kin_Tool[2] - Dist;
        }
        else if (Dir == "R0")
        {
          Robot_Kin_Tool[5] = Robot_Kin_Tool[5] + Dist * M_PI / 180;
        }
        else if (Dir == "R1")
        {
          Robot_Kin_Tool[5] = Robot_Kin_Tool[5] - Dist * M_PI / 180;
        }
        else if (Dir == "P0")
        {
          Robot_Kin_Tool[4] = Robot_Kin_Tool[4] + Dist * M_PI / 180;
        }
        else if (Dir == "P1")
        {
          Robot_Kin_Tool[4] = Robot_Kin_Tool[4] - Dist * M_PI / 180;
        }
        else if (Dir == "W0")
        {
          Robot_Kin_Tool[3] = Robot_Kin_Tool[3] + Dist * M_PI / 180;
        }
        else if (Dir == "W1")
        {
          Robot_Kin_Tool[3] = Robot_Kin_Tool[3] - Dist * M_PI / 180;
        }

        JangleIn[0] = (J1StepM - J1zeroStep) / J1StepDeg;
        JangleIn[1] = (J2StepM - J2zeroStep) / J2StepDeg;
        JangleIn[2] = (J3StepM - J3zeroStep) / J3StepDeg;
        JangleIn[3] = (J4StepM - J4zeroStep) / J4StepDeg;
        JangleIn[4] = (J5StepM - J5zeroStep) / J5StepDeg;
        JangleIn[5] = (J6StepM - J6zeroStep) / J6StepDeg;

        xyzuvw_In[0] = xyzuvw_Out[0];
        xyzuvw_In[1] = xyzuvw_Out[1];
        xyzuvw_In[2] = xyzuvw_Out[2];
        xyzuvw_In[3] = xyzuvw_Out[3];
        xyzuvw_In[4] = xyzuvw_Out[4];
        xyzuvw_In[5] = xyzuvw_Out[5];

        SolveInverseKinematic();

        Robot_Kin_Tool[0] = Xtool;
        Robot_Kin_Tool[1] = Ytool;
        Robot_Kin_Tool[2] = Ztool;
        Robot_Kin_Tool[3] = RZtool;
        Robot_Kin_Tool[4] = RYtool;
        Robot_Kin_Tool[5] = RXtool;

        // calc destination motor steps
        int J1futStepM = (JangleOut[0] + J1axisLimNeg) * J1StepDeg;
        int J2futStepM = (JangleOut[1] + J2axisLimNeg) * J2StepDeg;
        int J3futStepM = (JangleOut[2] + J3axisLimNeg) * J3StepDeg;
        int J4futStepM = (JangleOut[3] + J4axisLimNeg) * J4StepDeg;
        int J5futStepM = (JangleOut[4] + J5axisLimNeg) * J5StepDeg;
        int J6futStepM = (JangleOut[5] + J6axisLimNeg) * J6StepDeg;

        // calc delta from current to destination
        int J1stepDif = J1StepM - J1futStepM;
        int J2stepDif = J2StepM - J2futStepM;
        int J3stepDif = J3StepM - J3futStepM;
        int J4stepDif = J4StepM - J4futStepM;
        int J5stepDif = J5StepM - J5futStepM;
        int J6stepDif = J6StepM - J6futStepM;
        int TRstepDif = 0;

        // determine motor directions
        if (J1stepDif <= 0)
        {
          J1dir = 1;
        }
        else
        {
          J1dir = 0;
        }

        if (J2stepDif <= 0)
        {
          J2dir = 1;
        }
        else
        {
          J2dir = 0;
        }

        if (J3stepDif <= 0)
        {
          J3dir = 1;
        }
        else
        {
          J3dir = 0;
        }

        if (J4stepDif <= 0)
        {
          J4dir = 1;
        }
        else
        {
          J4dir = 0;
        }

        if (J5stepDif <= 0)
        {
          J5dir = 1;
        }
        else
        {
          J5dir = 0;
        }

        if (J6stepDif <= 0)
        {
          J6dir = 1;
        }
        else
        {
          J6dir = 0;
        }

        TRdir = 0;

        // determine if requested position is within axis limits
        if ((J1dir == 1 and (J1StepM + J1stepDif > J1StepLim)) or (J1dir == 0 and (J1StepM - J1stepDif < 0)))
        {
          J1axisFault = 1;
        }
        if ((J2dir == 1 and (J2StepM + J2stepDif > J2StepLim)) or (J2dir == 0 and (J2StepM - J2stepDif < 0)))
        {
          J2axisFault = 1;
        }
        if ((J3dir == 1 and (J3StepM + J3stepDif > J3StepLim)) or (J3dir == 0 and (J3StepM - J3stepDif < 0)))
        {
          J3axisFault = 1;
        }
        if ((J4dir == 1 and (J4StepM + J4stepDif > J4StepLim)) or (J4dir == 0 and (J4StepM - J4stepDif < 0)))
        {
          J4axisFault = 1;
        }
        if ((J5dir == 1 and (J5StepM + J5stepDif > J5StepLim)) or (J5dir == 0 and (J5StepM - J5stepDif < 0)))
        {
          J5axisFault = 1;
        }
        if ((J6dir == 1 and (J6StepM + J6stepDif > J6StepLim)) or (J6dir == 0 and (J6StepM - J6stepDif < 0)))
        {
          J6axisFault = 1;
        }
        TotalAxisFault = J1axisFault + J2axisFault + J3axisFault + J4axisFault + J5axisFault + J6axisFault;

        // send move command if no axis limit error
        if (TotalAxisFault == 0 && KinematicError == 0)
        {
          resetEncoders();
          driveMotorsJ(abs(J1stepDif), abs(J2stepDif), abs(J3stepDif), abs(J4stepDif), abs(J5stepDif), abs(J6stepDif), abs(TRstepDif), J1dir, J2dir, J3dir, J4dir, J5dir, J6dir, TRdir, SpeedType, SpeedVal, ACCspd, DCCspd, ACCramp);
          checkEncoders();
          sendRobotPos();
        }
        else if (KinematicError == 1)
        {
          Alarm = "ER";
          delay(5);
          Serial.println(Alarm);
        }
        else
        {
          Alarm = "EL" + String(J1axisFault) + String(J2axisFault) + String(J3axisFault) + String(J4axisFault) + String(J5axisFault) + String(J6axisFault);
          delay(5);
          Serial.println(Alarm);
        }

        inData = ""; // Clear recieved buffer
        ////////MOVE COMPLETE///////////
      }

      //----- MOVE A (Arc) ---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "MA")
      {

        int J1dir;
        int J2dir;
        int J3dir;
        int J4dir;
        int J5dir;
        int J6dir;
        int TRdir;

        int J1axisFault = 0;
        int J2axisFault = 0;
        int J3axisFault = 0;
        int J4axisFault = 0;
        int J5axisFault = 0;
        int J6axisFault = 0;
        int TotalAxisFault = 0;

        String Alarm = "0";
        float curWayDis;
        float speedSP;
        float Xvect;
        float Yvect;
        float Zvect;
        float calcStepGap;
        float theta;
        float axis[3];
        float axisTemp[3];
        float startVect[3];
        float Rotation[3][3];
        float DestPt[3];
        float a;
        float b;
        float c;
        float d;
        float aa;
        float bb;
        float cc;
        float dd;
        float bc;
        float ad;
        float ac;
        float ab;
        float bd;
        float cd;

        int xMidIndex = inData.indexOf("X");
        int yMidIndex = inData.indexOf("Y");
        int zMidIndex = inData.indexOf("Z");
        int rzIndex = inData.indexOf("Rz");
        int ryIndex = inData.indexOf("Ry");
        int rxIndex = inData.indexOf("Rx");

        int xEndIndex = inData.indexOf("Ex");
        int yEndIndex = inData.indexOf("Ey");
        int zEndIndex = inData.indexOf("Ez");
        int tStart = inData.indexOf("Tr");
        int SPstart = inData.indexOf("S");
        int AcStart = inData.indexOf("Ac");
        int DcStart = inData.indexOf("Dc");
        int RmStart = inData.indexOf("Rm");
        int WristConStart = inData.indexOf("W");

        updatePos();

        float xBeg = xyzuvw_Out[0];
        float yBeg = xyzuvw_Out[1];
        float zBeg = xyzuvw_Out[2];
        float rzBeg = xyzuvw_Out[3];
        float ryBeg = xyzuvw_Out[4];
        float rxBeg = xyzuvw_Out[5];

        float xMid = inData.substring(xMidIndex + 1, yMidIndex).toFloat();
        float yMid = inData.substring(yMidIndex + 1, zMidIndex).toFloat();
        float zMid = inData.substring(zMidIndex + 1, rzIndex).toFloat();

        float rz = inData.substring(rzIndex + 2, ryIndex).toFloat();
        float ry = inData.substring(ryIndex + 2, rxIndex).toFloat();
        float rx = inData.substring(rxIndex + 2, xEndIndex).toFloat();

        float RZvect = rzBeg - rz;
        float RYvect = ryBeg - ry;
        float RXvect = rxBeg - rx;

        float xEnd = inData.substring(xEndIndex + 2, yEndIndex).toFloat();
        float yEnd = inData.substring(yEndIndex + 2, zEndIndex).toFloat();
        float zEnd = inData.substring(zEndIndex + 2, tStart).toFloat();
        xyzuvw_In[6] = inData.substring(tStart + 2, SPstart).toFloat();
        String SpeedType = inData.substring(SPstart + 1, SPstart + 2);
        float SpeedVal = inData.substring(SPstart + 2, AcStart).toFloat();
        float ACCspd = inData.substring(AcStart + 2, DcStart).toFloat();
        float DCCspd = inData.substring(DcStart + 2, RmStart).toFloat();
        float ACCramp = inData.substring(RmStart + 2, WristConStart).toFloat();
        WristCon = inData.substring(WristConStart + 1);
        WristCon.trim();

        // determine length between each point (lengths of triangle)
        Xvect = xEnd - xMid;
        Yvect = yEnd - yMid;
        Zvect = zEnd - zMid;
        float aDist = pow((pow((Xvect), 2) + pow((Yvect), 2) + pow((Zvect), 2)), .5);
        Xvect = xEnd - xBeg;
        Yvect = yEnd - yBeg;
        Zvect = zEnd - zBeg;
        float bDist = pow((pow((Xvect), 2) + pow((Yvect), 2) + pow((Zvect), 2)), .5);
        Xvect = xMid - xBeg;
        Yvect = yMid - yBeg;
        Zvect = zMid - zBeg;
        float cDist = pow((pow((Xvect), 2) + pow((Yvect), 2) + pow((Zvect), 2)), .5);
        // use lengths between each point (lengths of triangle) to determine radius
        float s = (aDist + bDist + cDist) / 2;
        float Radius = aDist * bDist * cDist / 4 / sqrt(s * (s - aDist) * (s - bDist) * (s - cDist));
        // find barycentric coordinates of triangle (center of triangle)
        float BCx = pow(aDist, 2) * (pow(bDist, 2) + pow(cDist, 2) - pow(aDist, 2));
        float BCy = pow(bDist, 2) * (pow(cDist, 2) + pow(aDist, 2) - pow(bDist, 2));
        float BCz = pow(cDist, 2) * (pow(aDist, 2) + pow(bDist, 2) - pow(cDist, 2));
        // find center coordinates of circle - convert barycentric coordinates to cartesian coordinates - dot product of 3 points and barycentric coordiantes divided by sum of barycentric coordinates
        float Px = ((BCx * xBeg) + (BCy * xMid) + (BCz * xEnd)) / (BCx + BCy + BCz);
        float Py = ((BCx * yBeg) + (BCy * yMid) + (BCz * yEnd)) / (BCx + BCy + BCz);
        float Pz = ((BCx * zBeg) + (BCy * zMid) + (BCz * zEnd)) / (BCx + BCy + BCz);
        // define start vetor
        startVect[0] = (xBeg - Px);
        startVect[1] = (yBeg - Py);
        startVect[2] = (zBeg - Pz);
        // get 3 vectors from center of circle to begining target, mid target and end target then normalize
        float vect_Amag = pow((pow((xBeg - Px), 2) + pow((yBeg - Py), 2) + pow((zBeg - Pz), 2)), .5);
        float vect_Ax = (xBeg - Px) / vect_Amag;
        float vect_Ay = (yBeg - Py) / vect_Amag;
        float vect_Az = (zBeg - Pz) / vect_Amag;
        float vect_Bmag = pow((pow((xMid - Px), 2) + pow((yMid - Py), 2) + pow((zMid - Pz), 2)), .5);
        float vect_Bx = (xMid - Px) / vect_Bmag;
        float vect_By = (yMid - Py) / vect_Bmag;
        float vect_Bz = (zMid - Pz) / vect_Bmag;
        float vect_Cmag = pow((pow((xEnd - Px), 2) + pow((yEnd - Py), 2) + pow((zEnd - Pz), 2)), .5);
        float vect_Cx = (xEnd - Px) / vect_Cmag;
        float vect_Cy = (yEnd - Py) / vect_Cmag;
        float vect_Cz = (zEnd - Pz) / vect_Cmag;
        // get cross product of vectors a & c than apply to axis matrix
        float CrossX = (vect_Ay * vect_Bz) - (vect_Az * vect_By);
        float CrossY = (vect_Az * vect_Bx) - (vect_Ax * vect_Bz);
        float CrossZ = (vect_Ax * vect_By) - (vect_Ay * vect_Bx);
        axis[0] = CrossX / sqrt((CrossX * CrossX) + (CrossY * CrossY) + (CrossZ * CrossZ));
        axis[1] = CrossY / sqrt((CrossX * CrossX) + (CrossY * CrossY) + (CrossZ * CrossZ));
        axis[2] = CrossZ / sqrt((CrossX * CrossX) + (CrossY * CrossY) + (CrossZ * CrossZ));
        // get radian angle between vectors using acos of dot product
        float ABradians = acos((vect_Ax * vect_Bx + vect_Ay * vect_By + vect_Az * vect_Bz) / (sqrt(pow(vect_Ax, 2) + pow(vect_Ay, 2) + pow(vect_Az, 2)) * sqrt(pow(vect_Bx, 2) + pow(vect_By, 2) + pow(vect_Bz, 2))));
        float BCradians = acos((vect_Bx * vect_Cx + vect_By * vect_Cy + vect_Bz * vect_Cz) / (sqrt(pow(vect_Bx, 2) + pow(vect_By, 2) + pow(vect_Bz, 2)) * sqrt(pow(vect_Cx, 2) + pow(vect_Cy, 2) + pow(vect_Cz, 2))));
        // get total degrees of both arcs
        float ABdegrees = degrees(ABradians + BCradians);
        // get arc length and calc way pt gap

        float anglepercent = ABdegrees / 360;
        float circumference = 2 * 3.14159265359 * Radius;
        float lineDist = circumference * anglepercent;
        float wayPts = lineDist / linWayDistSP;

        float wayPerc = 1 / wayPts;
        // cacl way pt angle
        float theta_Deg = (ABdegrees / wayPts);

        // determine steps
        int HighStep = lineDist / .05;
        float ACCStep = HighStep * (ACCspd / 100);
        float NORStep = HighStep * ((100 - ACCspd - DCCspd) / 100);
        float DCCStep = HighStep * (DCCspd / 100);

        // set speed for seconds or mm per sec
        if (SpeedType == "s")
        {
          speedSP = (SpeedVal * 1000000) * .2;
        }
        else if (SpeedType == "m")
        {
          speedSP = ((lineDist / SpeedVal) * 1000000) * .2;
        }

        // calc step gap for seconds or mm per sec
        if (SpeedType == "s" or SpeedType == "m")
        {
          float zeroStepGap = speedSP / HighStep;
          float zeroACCstepInc = (zeroStepGap * (100 / ACCramp)) / ACCStep;
          float zeroACCtime = ((ACCStep)*zeroStepGap) + ((ACCStep - 9) * (((ACCStep) * (zeroACCstepInc / 2))));
          float zeroNORtime = NORStep * zeroStepGap;
          float zeroDCCstepInc = (zeroStepGap * (100 / ACCramp)) / DCCStep;
          float zeroDCCtime = ((DCCStep)*zeroStepGap) + ((DCCStep - 9) * (((DCCStep) * (zeroDCCstepInc / 2))));
          float zeroTOTtime = zeroACCtime + zeroNORtime + zeroDCCtime;
          float overclockPerc = speedSP / zeroTOTtime;
          calcStepGap = zeroStepGap * overclockPerc;
          if (calcStepGap <= minSpeedDelay)
          {
            calcStepGap = minSpeedDelay;
            speedViolation = "1";
          }
        }

        // calc step gap for percentage
        else if (SpeedType == "p")
        {
          calcStepGap = ((maxSpeedDelay - ((SpeedVal / 100) * maxSpeedDelay)) + minSpeedDelay);
        }

        // calculate final step increments
        float calcACCstepInc = (calcStepGap * (100 / ACCramp)) / ACCStep;
        float calcDCCstepInc = (calcStepGap * (100 / ACCramp)) / DCCStep;
        float calcACCstartDel = (calcACCstepInc * ACCStep) * 2;
        float calcDCCendDel = (calcDCCstepInc * DCCStep) * 2;

        // calc way pt speeds
        float ACCwayPts = wayPts * (ACCspd / 100);
        float NORwayPts = wayPts * ((100 - ACCspd - DCCspd) / 100);
        float DCCwayPts = wayPts * (DCCspd / 100);

        // calc way inc for lin way steps
        float ACCwayInc = (calcACCstartDel - calcStepGap) / ACCwayPts;
        float DCCwayInc = (calcDCCendDel - calcStepGap) / DCCwayPts;

        // set starting delsy
        float curDelay = calcACCstartDel;

        // set starting angle first way pt
        float cur_deg = theta_Deg;

        /////////////////////////////////////
        // loop through waypoints
        ////////////////////////////////////

        resetEncoders();

        for (int i = 0; i <= wayPts - 1; i++)
        {

          theta = radians(cur_deg);
          // use euler rodrigues formula to find rotation vector
          a = cos(theta / 2.0);
          b = -axis[0] * sin(theta / 2.0);
          c = -axis[1] * sin(theta / 2.0);
          d = -axis[2] * sin(theta / 2.0);
          aa = a * a;
          bb = b * b;
          cc = c * c;
          dd = d * d;
          bc = b * c;
          ad = a * d;
          ac = a * c;
          ab = a * b;
          bd = b * d;
          cd = c * d;
          Rotation[0][0] = aa + bb - cc - dd;
          Rotation[0][1] = 2 * (bc + ad);
          Rotation[0][2] = 2 * (bd - ac);
          Rotation[1][0] = 2 * (bc - ad);
          Rotation[1][1] = aa + cc - bb - dd;
          Rotation[1][2] = 2 * (cd + ab);
          Rotation[2][0] = 2 * (bd + ac);
          Rotation[2][1] = 2 * (cd - ab);
          Rotation[2][2] = aa + dd - bb - cc;

          // get product of current rotation and start vector
          DestPt[0] = (Rotation[0][0] * startVect[0]) + (Rotation[0][1] * startVect[1]) + (Rotation[0][2] * startVect[2]);
          DestPt[1] = (Rotation[1][0] * startVect[0]) + (Rotation[1][1] * startVect[1]) + (Rotation[1][2] * startVect[2]);
          DestPt[2] = (Rotation[2][0] * startVect[0]) + (Rotation[2][1] * startVect[1]) + (Rotation[2][2] * startVect[2]);

          ////DELAY CALC/////
          if (i <= ACCwayPts)
          {
            curDelay = curDelay - (ACCwayInc);
          }
          else if (i >= (wayPts - DCCwayPts))
          {
            curDelay = curDelay + (DCCwayInc);
          }
          else
          {
            curDelay = calcStepGap;
          }

          // shift way pts back to orignal origin and calc kinematics for way pt movement
          float curWayPerc = wayPerc * i;
          xyzuvw_In[0] = (DestPt[0]) + Px;
          xyzuvw_In[1] = (DestPt[1]) + Py;
          xyzuvw_In[2] = (DestPt[2]) + Pz;
          xyzuvw_In[3] = rzBeg - (RZvect * curWayPerc);
          xyzuvw_In[4] = ryBeg - (RYvect * curWayPerc);
          xyzuvw_In[5] = rxBeg - (RXvect * curWayPerc);

          SolveInverseKinematic();

          // calc destination motor steps
          int J1futStepM = (JangleOut[0] + J1axisLimNeg) * J1StepDeg;
          int J2futStepM = (JangleOut[1] + J2axisLimNeg) * J2StepDeg;
          int J3futStepM = (JangleOut[2] + J3axisLimNeg) * J3StepDeg;
          int J4futStepM = (JangleOut[3] + J4axisLimNeg) * J4StepDeg;
          int J5futStepM = (JangleOut[4] + J5axisLimNeg) * J5StepDeg;
          int J6futStepM = (JangleOut[5] + J6axisLimNeg) * J6StepDeg;

          // calc delta from current to destination
          int J1stepDif = J1StepM - J1futStepM;
          int J2stepDif = J2StepM - J2futStepM;
          int J3stepDif = J3StepM - J3futStepM;
          int J4stepDif = J4StepM - J4futStepM;
          int J5stepDif = J5StepM - J5futStepM;
          int J6stepDif = J6StepM - J6futStepM;
          int TRstepDif = 0;

          // determine motor directions
          if (J1stepDif <= 0)
          {
            J1dir = 1;
          }
          else
          {
            J1dir = 0;
          }

          if (J2stepDif <= 0)
          {
            J2dir = 1;
          }
          else
          {
            J2dir = 0;
          }

          if (J3stepDif <= 0)
          {
            J3dir = 1;
          }
          else
          {
            J3dir = 0;
          }

          if (J4stepDif <= 0)
          {
            J4dir = 1;
          }
          else
          {
            J4dir = 0;
          }

          if (J5stepDif <= 0)
          {
            J5dir = 1;
          }
          else
          {
            J5dir = 0;
          }

          if (J6stepDif <= 0)
          {
            J6dir = 1;
          }
          else
          {
            J6dir = 0;
          }

          TRdir = 0;

          // determine if requested position is within axis limits
          if ((J1dir == 1 and (J1StepM + J1stepDif > J1StepLim)) or (J1dir == 0 and (J1StepM - J1stepDif < 0)))
          {
            J1axisFault = 1;
          }
          if ((J2dir == 1 and (J2StepM + J2stepDif > J2StepLim)) or (J2dir == 0 and (J2StepM - J2stepDif < 0)))
          {
            J2axisFault = 1;
          }
          if ((J3dir == 1 and (J3StepM + J3stepDif > J3StepLim)) or (J3dir == 0 and (J3StepM - J3stepDif < 0)))
          {
            J3axisFault = 1;
          }
          if ((J4dir == 1 and (J4StepM + J4stepDif > J4StepLim)) or (J4dir == 0 and (J4StepM - J4stepDif < 0)))
          {
            J4axisFault = 1;
          }
          if ((J5dir == 1 and (J5StepM + J5stepDif > J5StepLim)) or (J5dir == 0 and (J5StepM - J5stepDif < 0)))
          {
            J5axisFault = 1;
          }
          if ((J6dir == 1 and (J6StepM + J6stepDif > J6StepLim)) or (J6dir == 0 and (J6StepM - J6stepDif < 0)))
          {
            J6axisFault = 1;
          }
          TotalAxisFault = J1axisFault + J2axisFault + J3axisFault + J4axisFault + J5axisFault + J6axisFault;

          // send move command if no axis limit error
          if (TotalAxisFault == 0 && KinematicError == 0)
          {
            driveMotorsL(abs(J1stepDif), abs(J2stepDif), abs(J3stepDif), abs(J4stepDif), abs(J5stepDif), abs(J6stepDif), abs(TRstepDif), J1dir, J2dir, J3dir, J4dir, J5dir, J6dir, TRdir, curDelay);
          }
          else if (KinematicError == 1)
          {
            Alarm = "ER";
            delay(5);
            Serial.println(Alarm);
          }
          else
          {
            Alarm = "EL" + String(J1axisFault) + String(J2axisFault) + String(J3axisFault) + String(J4axisFault) + String(J5axisFault) + String(J6axisFault);
            delay(5);
            Serial.println(Alarm);
          }

          // increment angle
          cur_deg += theta_Deg;
        }
        checkEncoders();
        sendRobotPos();

        inData = ""; // Clear recieved buffer
        ////////MOVE COMPLETE///////////
      }

      //----- MOVE C (Cirlce) ---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "MC")
      {

        int J1dir;
        int J2dir;
        int J3dir;
        int J4dir;
        int J5dir;
        int J6dir;
        int TRdir;

        int J1axisFault = 0;
        int J2axisFault = 0;
        int J3axisFault = 0;
        int J4axisFault = 0;
        int J5axisFault = 0;
        int J6axisFault = 0;
        int TotalAxisFault = 0;

        String Alarm = "0";
        float curWayDis;
        float speedSP;
        float Xvect;
        float Yvect;
        float Zvect;
        float calcStepGap;
        float theta;
        int Cdir;
        float axis[3];
        float axisTemp[3];
        float startVect[3];
        float Rotation[3][3];
        float DestPt[3];
        float a;
        float b;
        float c;
        float d;
        float aa;
        float bb;
        float cc;
        float dd;
        float bc;
        float ad;
        float ac;
        float ab;
        float bd;
        float cd;

        int xStart = inData.indexOf("Cx");
        int yStart = inData.indexOf("Cy");
        int zStart = inData.indexOf("Cz");
        int rzStart = inData.indexOf("Rz");
        int ryStart = inData.indexOf("Ry");
        int rxStart = inData.indexOf("Rx");
        int xMidIndex = inData.indexOf("Bx");
        int yMidIndex = inData.indexOf("By");
        int zMidIndex = inData.indexOf("Bz");
        int xEndIndex = inData.indexOf("Px");
        int yEndIndex = inData.indexOf("Py");
        int zEndIndex = inData.indexOf("Pz");
        int tStart = inData.indexOf("Tr");
        int SPstart = inData.indexOf("S");
        int AcStart = inData.indexOf("Ac");
        int DcStart = inData.indexOf("Dc");
        int RmStart = inData.indexOf("Rm");
        int WristConStart = inData.indexOf("W");

        float xBeg = inData.substring(xStart + 2, yStart).toFloat();
        float yBeg = inData.substring(yStart + 2, zStart).toFloat();
        float zBeg = inData.substring(zStart + 2, rzStart).toFloat();
        float rzBeg = inData.substring(rzStart + 2, ryStart).toFloat();
        float ryBeg = inData.substring(ryStart + 2, rxStart).toFloat();
        float rxBeg = inData.substring(rxStart + 2, xMidIndex).toFloat();
        float xMid = inData.substring(xMidIndex + 2, yMidIndex).toFloat();
        float yMid = inData.substring(yMidIndex + 2, zMidIndex).toFloat();
        float zMid = inData.substring(zMidIndex + 2, xEndIndex).toFloat();
        float xEnd = inData.substring(xEndIndex + 2, yEndIndex).toFloat();
        float yEnd = inData.substring(yEndIndex + 2, zEndIndex).toFloat();
        float zEnd = inData.substring(zEndIndex + 2, tStart).toFloat();
        xyzuvw_In[6] = inData.substring(tStart + 2, SPstart).toFloat();
        String SpeedType = inData.substring(SPstart + 1, SPstart + 2);
        float SpeedVal = inData.substring(SPstart + 2, AcStart).toFloat();
        float ACCspd = inData.substring(AcStart + 2, DcStart).toFloat();
        float DCCspd = inData.substring(DcStart + 2, RmStart).toFloat();
        float ACCramp = inData.substring(RmStart + 2, WristConStart).toFloat();
        WristCon = inData.substring(WristConStart + 1);
        WristCon.trim();

        // calc vector from start point of circle (mid) to center of circle (beg)
        Xvect = xMid - xBeg;
        Yvect = yMid - yBeg;
        Zvect = zMid - zBeg;
        // get radius - distance from first point (center of circle) to second point (start point of circle)
        float Radius = pow((pow((Xvect), 2) + pow((Yvect), 2) + pow((Zvect), 2)), .5);

        // set center coordinates of circle to first point (beg) as this is the center of our circle
        float Px = xBeg;
        float Py = yBeg;
        float Pz = zBeg;

        // define start vetor (mid) point is start of circle
        startVect[0] = (xMid - Px);
        startVect[1] = (yMid - Py);
        startVect[2] = (zMid - Pz);
        // get vectors from center of circle to  mid target (start) and end target then normalize
        float vect_Bmag = pow((pow((xMid - Px), 2) + pow((yMid - Py), 2) + pow((zMid - Pz), 2)), .5);
        float vect_Bx = (xMid - Px) / vect_Bmag;
        float vect_By = (yMid - Py) / vect_Bmag;
        float vect_Bz = (zMid - Pz) / vect_Bmag;
        float vect_Cmag = pow((pow((xEnd - Px), 2) + pow((yEnd - Py), 2) + pow((zEnd - Pz), 2)), .5);
        float vect_Cx = (xEnd - Px) / vect_Cmag;
        float vect_Cy = (yEnd - Py) / vect_Cmag;
        float vect_Cz = (zEnd - Pz) / vect_Cmag;
        // get cross product of vectors b & c than apply to axis matrix
        float CrossX = (vect_By * vect_Cz) - (vect_Bz * vect_Cy);
        float CrossY = (vect_Bz * vect_Cx) - (vect_Bx * vect_Cz);
        float CrossZ = (vect_Bx * vect_Cy) - (vect_By * vect_Cx);
        axis[0] = CrossX / sqrt((CrossX * CrossX) + (CrossY * CrossY) + (CrossZ * CrossZ));
        axis[1] = CrossY / sqrt((CrossX * CrossX) + (CrossY * CrossY) + (CrossZ * CrossZ));
        axis[2] = CrossZ / sqrt((CrossX * CrossX) + (CrossY * CrossY) + (CrossZ * CrossZ));
        // get radian angle between vectors using acos of dot product
        float BCradians = acos((vect_Bx * vect_Cx + vect_By * vect_Cy + vect_Bz * vect_Cz) / (sqrt(pow(vect_Bx, 2) + pow(vect_Cy, 2) + pow(vect_Bz, 2)) * sqrt(pow(vect_Cx, 2) + pow(vect_Cy, 2) + pow(vect_Cz, 2))));
        // get arc degree
        float ABdegrees = degrees(BCradians);
        // get direction from angle
        if (ABdegrees > 0)
        {
          Cdir = 1;
        }
        else
        {
          Cdir = -1;
        }

        // get circumference and calc way pt gap
        float lineDist = 2 * 3.14159265359 * Radius;
        float wayPts = lineDist / linWayDistSP;

        float wayPerc = 1 / wayPts;
        // cacl way pt angle
        float theta_Deg = ((360 * Cdir) / (wayPts));

        // determine steps
        int HighStep = lineDist / .05;
        float ACCStep = HighStep * (ACCspd / 100);
        float NORStep = HighStep * ((100 - ACCspd - DCCspd) / 100);
        float DCCStep = HighStep * (DCCspd / 100);

        // set speed for seconds or mm per sec
        if (SpeedType == "s")
        {
          speedSP = (SpeedVal * 1000000) * .2;
        }
        else if (SpeedType == "m")
        {
          speedSP = ((lineDist / SpeedVal) * 1000000) * .2;
        }

        // calc step gap for seconds or mm per sec
        if (SpeedType == "s" or SpeedType == "m")
        {
          float zeroStepGap = speedSP / HighStep;
          float zeroACCstepInc = (zeroStepGap * (100 / ACCramp)) / ACCStep;
          float zeroACCtime = ((ACCStep)*zeroStepGap) + ((ACCStep - 9) * (((ACCStep) * (zeroACCstepInc / 2))));
          float zeroNORtime = NORStep * zeroStepGap;
          float zeroDCCstepInc = (zeroStepGap * (100 / ACCramp)) / DCCStep;
          float zeroDCCtime = ((DCCStep)*zeroStepGap) + ((DCCStep - 9) * (((DCCStep) * (zeroDCCstepInc / 2))));
          float zeroTOTtime = zeroACCtime + zeroNORtime + zeroDCCtime;
          float overclockPerc = speedSP / zeroTOTtime;
          calcStepGap = zeroStepGap * overclockPerc;
          if (calcStepGap <= minSpeedDelay)
          {
            calcStepGap = minSpeedDelay;
            speedViolation = "1";
          }
        }

        // calc step gap for percentage
        else if (SpeedType == "p")
        {
          calcStepGap = ((maxSpeedDelay - ((SpeedVal / 100) * maxSpeedDelay)) + minSpeedDelay);
        }

        // calculate final step increments
        float calcACCstepInc = (calcStepGap * (100 / ACCramp)) / ACCStep;
        float calcDCCstepInc = (calcStepGap * (100 / ACCramp)) / DCCStep;
        float calcACCstartDel = (calcACCstepInc * ACCStep) * 2;
        float calcDCCendDel = (calcDCCstepInc * DCCStep) * 2;

        // calc way pt speeds
        float ACCwayPts = wayPts * (ACCspd / 100);
        float NORwayPts = wayPts * ((100 - ACCspd - DCCspd) / 100);
        float DCCwayPts = wayPts * (DCCspd / 100);

        // calc way inc for lin way steps
        float ACCwayInc = (calcACCstartDel - calcStepGap) / ACCwayPts;
        float DCCwayInc = (calcDCCendDel - calcStepGap) / DCCwayPts;

        // set starting delsy
        float curDelay = calcACCstartDel;

        // set starting angle first way pt
        float cur_deg = theta_Deg;

        /////////////////////////////////////
        // loop through waypoints
        ////////////////////////////////////

        resetEncoders();

        for (int i = 1; i <= wayPts; i++)
        {

          theta = radians(cur_deg);
          // use euler rodrigues formula to find rotation vector
          a = cos(theta / 2.0);
          b = -axis[0] * sin(theta / 2.0);
          c = -axis[1] * sin(theta / 2.0);
          d = -axis[2] * sin(theta / 2.0);
          aa = a * a;
          bb = b * b;
          cc = c * c;
          dd = d * d;
          bc = b * c;
          ad = a * d;
          ac = a * c;
          ab = a * b;
          bd = b * d;
          cd = c * d;
          Rotation[0][0] = aa + bb - cc - dd;
          Rotation[0][1] = 2 * (bc + ad);
          Rotation[0][2] = 2 * (bd - ac);
          Rotation[1][0] = 2 * (bc - ad);
          Rotation[1][1] = aa + cc - bb - dd;
          Rotation[1][2] = 2 * (cd + ab);
          Rotation[2][0] = 2 * (bd + ac);
          Rotation[2][1] = 2 * (cd - ab);
          Rotation[2][2] = aa + dd - bb - cc;

          // get product of current rotation and start vector
          DestPt[0] = (Rotation[0][0] * startVect[0]) + (Rotation[0][1] * startVect[1]) + (Rotation[0][2] * startVect[2]);
          DestPt[1] = (Rotation[1][0] * startVect[0]) + (Rotation[1][1] * startVect[1]) + (Rotation[1][2] * startVect[2]);
          DestPt[2] = (Rotation[2][0] * startVect[0]) + (Rotation[2][1] * startVect[1]) + (Rotation[2][2] * startVect[2]);

          ////DELAY CALC/////
          if (i <= ACCwayPts)
          {
            curDelay = curDelay - (ACCwayInc);
          }
          else if (i >= (wayPts - DCCwayPts))
          {
            curDelay = curDelay + (DCCwayInc);
          }
          else
          {
            curDelay = calcStepGap;
          }

          // shift way pts back to orignal origin and calc kinematics for way pt movement
          xyzuvw_In[0] = (DestPt[0]) + Px;
          xyzuvw_In[1] = (DestPt[1]) + Py;
          xyzuvw_In[2] = (DestPt[2]) + Pz;
          xyzuvw_In[3] = rzBeg;
          xyzuvw_In[4] = ryBeg;
          xyzuvw_In[5] = rxBeg;

          SolveInverseKinematic();

          // calc destination motor steps
          int J1futStepM = (JangleOut[0] + J1axisLimNeg) * J1StepDeg;
          int J2futStepM = (JangleOut[1] + J2axisLimNeg) * J2StepDeg;
          int J3futStepM = (JangleOut[2] + J3axisLimNeg) * J3StepDeg;
          int J4futStepM = (JangleOut[3] + J4axisLimNeg) * J4StepDeg;
          int J5futStepM = (JangleOut[4] + J5axisLimNeg) * J5StepDeg;
          int J6futStepM = (JangleOut[5] + J6axisLimNeg) * J6StepDeg;

          // calc delta from current to destination
          int J1stepDif = J1StepM - J1futStepM;
          int J2stepDif = J2StepM - J2futStepM;
          int J3stepDif = J3StepM - J3futStepM;
          int J4stepDif = J4StepM - J4futStepM;
          int J5stepDif = J5StepM - J5futStepM;
          int J6stepDif = J6StepM - J6futStepM;
          int TRstepDif = 0;

          // determine motor directions
          if (J1stepDif <= 0)
          {
            J1dir = 1;
          }
          else
          {
            J1dir = 0;
          }

          if (J2stepDif <= 0)
          {
            J2dir = 1;
          }
          else
          {
            J2dir = 0;
          }

          if (J3stepDif <= 0)
          {
            J3dir = 1;
          }
          else
          {
            J3dir = 0;
          }

          if (J4stepDif <= 0)
          {
            J4dir = 1;
          }
          else
          {
            J4dir = 0;
          }

          if (J5stepDif <= 0)
          {
            J5dir = 1;
          }
          else
          {
            J5dir = 0;
          }

          if (J6stepDif <= 0)
          {
            J6dir = 1;
          }
          else
          {
            J6dir = 0;
          }

          TRdir = 0;

          // determine if requested position is within axis limits
          if ((J1dir == 1 and (J1StepM + J1stepDif > J1StepLim)) or (J1dir == 0 and (J1StepM - J1stepDif < 0)))
          {
            J1axisFault = 1;
          }
          if ((J2dir == 1 and (J2StepM + J2stepDif > J2StepLim)) or (J2dir == 0 and (J2StepM - J2stepDif < 0)))
          {
            J2axisFault = 1;
          }
          if ((J3dir == 1 and (J3StepM + J3stepDif > J3StepLim)) or (J3dir == 0 and (J3StepM - J3stepDif < 0)))
          {
            J3axisFault = 1;
          }
          if ((J4dir == 1 and (J4StepM + J4stepDif > J4StepLim)) or (J4dir == 0 and (J4StepM - J4stepDif < 0)))
          {
            J4axisFault = 1;
          }
          if ((J5dir == 1 and (J5StepM + J5stepDif > J5StepLim)) or (J5dir == 0 and (J5StepM - J5stepDif < 0)))
          {
            J5axisFault = 1;
          }
          if ((J6dir == 1 and (J6StepM + J6stepDif > J6StepLim)) or (J6dir == 0 and (J6StepM - J6stepDif < 0)))
          {
            J6axisFault = 1;
          }
          TotalAxisFault = J1axisFault + J2axisFault + J3axisFault + J4axisFault + J5axisFault + J6axisFault;

          if (TotalAxisFault == 0 && KinematicError == 0)
          {
            driveMotorsL(abs(J1stepDif), abs(J2stepDif), abs(J3stepDif), abs(J4stepDif), abs(J5stepDif), abs(J6stepDif), abs(TRstepDif), J1dir, J2dir, J3dir, J4dir, J5dir, J6dir, TRdir, curDelay);
          }
          else if (KinematicError == 1)
          {
            Alarm = "ER";
            delay(5);
            Serial.println(Alarm);
          }
          else
          {
            Alarm = "EL" + String(J1axisFault) + String(J2axisFault) + String(J3axisFault) + String(J4axisFault) + String(J5axisFault) + String(J6axisFault);
            delay(5);
            Serial.println(Alarm);
          }

          // increment angle
          cur_deg += theta_Deg;
        }

        checkEncoders();
        sendRobotPos();

        inData = ""; // Clear recieved buffer
        ////////MOVE COMPLETE///////////
      }

      //----- MOVE L ---------------------------------------------------
      //-----------------------------------------------------------------------
      if (function == "ML")
      {
        int J1dir;
        int J2dir;
        int J3dir;
        int J4dir;
        int J5dir;
        int J6dir;
        int TRdir;

        int J1axisFault = 0;
        int J2axisFault = 0;
        int J3axisFault = 0;
        int J4axisFault = 0;
        int J5axisFault = 0;
        int J6axisFault = 0;
        int TRaxisFault = 0;
        int TotalAxisFault = 0;

        String Alarm = "0";

        float curWayDis;
        float speedSP;

        int xStart = inData.indexOf("X");
        int yStart = inData.indexOf("Y");
        int zStart = inData.indexOf("Z");
        int rzStart = inData.indexOf("Rz");
        int ryStart = inData.indexOf("Ry");
        int rxStart = inData.indexOf("Rx");
        int tStart = inData.indexOf("Tr");
        int SPstart = inData.indexOf("S");
        int AcStart = inData.indexOf("Ac");
        int DcStart = inData.indexOf("Dc");
        int RmStart = inData.indexOf("Rm");
        int WristConStart = inData.indexOf("W");

        xyzuvw_In[0] = inData.substring(xStart + 1, yStart).toFloat();
        xyzuvw_In[1] = inData.substring(yStart + 1, zStart).toFloat();
        xyzuvw_In[2] = inData.substring(zStart + 1, rzStart).toFloat();
        xyzuvw_In[3] = inData.substring(rzStart + 2, ryStart).toFloat();
        xyzuvw_In[4] = inData.substring(ryStart + 2, rxStart).toFloat();
        xyzuvw_In[5] = inData.substring(rxStart + 2, tStart).toFloat();
        xyzuvw_In[6] = inData.substring(tStart + 2, SPstart).toFloat();

        String SpeedType = inData.substring(SPstart + 1, SPstart + 2);
        float SpeedVal = inData.substring(SPstart + 2, AcStart).toFloat();
        float ACCspd = inData.substring(AcStart + 2, DcStart).toFloat();
        float DCCspd = inData.substring(DcStart + 2, RmStart).toFloat();
        float ACCramp = inData.substring(RmStart + 2, WristConStart).toFloat();

        WristCon = inData.substring(WristConStart + 1);
        WristCon.trim();

        // vector
        float Xvect = xyzuvw_In[0] - xyzuvw_Out[0];
        float Yvect = xyzuvw_In[1] - xyzuvw_Out[1];
        float Zvect = xyzuvw_In[2] - xyzuvw_Out[2];
        float RZvect = xyzuvw_In[3] - xyzuvw_Out[3];
        float RYvect = xyzuvw_In[4] - xyzuvw_Out[4];
        float RXvect = xyzuvw_In[5] - xyzuvw_Out[5];

        // start pos
        float Xstart = xyzuvw_Out[0];
        float Ystart = xyzuvw_Out[1];
        float Zstart = xyzuvw_Out[2];
        float RZstart = xyzuvw_Out[3];
        float RYstart = xyzuvw_Out[4];
        float RXstart = xyzuvw_Out[5];

        // line dist and determine way point gap
        float lineDist = pow((pow((Xvect), 2) + pow((Yvect), 2) + pow((Zvect), 2) + pow((RZvect), 2) + pow((RYvect), 2) + pow((RXvect), 2)), .5);
        if (lineDist > 0)
        {

          float wayPts = lineDist / linWayDistSP;
          float wayPerc = 1 / wayPts;

          // pre calculate entire move and speeds

          SolveInverseKinematic();
          // calc destination motor steps for precalc
          int J1futStepM = (JangleOut[0] + J1axisLimNeg) * J1StepDeg;
          int J2futStepM = (JangleOut[1] + J2axisLimNeg) * J2StepDeg;
          int J3futStepM = (JangleOut[2] + J3axisLimNeg) * J3StepDeg;
          int J4futStepM = (JangleOut[3] + J4axisLimNeg) * J4StepDeg;
          int J5futStepM = (JangleOut[4] + J5axisLimNeg) * J5StepDeg;
          int J6futStepM = (JangleOut[5] + J6axisLimNeg) * J6StepDeg;

          // calc delta from current to destination fpr precalc
          int J1stepDif = J1StepM - J1futStepM;
          int J2stepDif = J2StepM - J2futStepM;
          int J3stepDif = J3StepM - J3futStepM;
          int J4stepDif = J4StepM - J4futStepM;
          int J5stepDif = J5StepM - J5futStepM;
          int J6stepDif = J6StepM - J6futStepM;

          // FIND HIGHEST STEP FOR PRECALC
          int HighStep = J1stepDif;
          if (J2stepDif > HighStep)
          {
            HighStep = J2stepDif;
          }
          if (J3stepDif > HighStep)
          {
            HighStep = J3stepDif;
          }
          if (J4stepDif > HighStep)
          {
            HighStep = J4stepDif;
          }
          if (J5stepDif > HighStep)
          {
            HighStep = J5stepDif;
          }
          if (J6stepDif > HighStep)
          {
            HighStep = J6stepDif;
          }

          /////PRE CALC SPEEDS//////
          float calcStepGap;

          // determine steps
          float ACCStep = HighStep * (ACCspd / 100);
          float NORStep = HighStep * ((100 - ACCspd - DCCspd) / 100);
          float DCCStep = HighStep * (DCCspd / 100);

          // set speed for seconds or mm per sec
          if (SpeedType == "s")
          {
            speedSP = (SpeedVal * 1000000) * .2;
          }
          else if ((SpeedType == "m"))
          {
            speedSP = ((lineDist / SpeedVal) * 1000000) * .2;
          }

          // calc step gap for seconds or mm per sec
          if (SpeedType == "s" or SpeedType == "m")
          {
            float zeroStepGap = speedSP / HighStep;
            float zeroACCstepInc = (zeroStepGap * (100 / ACCramp)) / ACCStep;
            float zeroACCtime = ((ACCStep)*zeroStepGap) + ((ACCStep - 9) * (((ACCStep) * (zeroACCstepInc / 2))));
            float zeroNORtime = NORStep * zeroStepGap;
            float zeroDCCstepInc = (zeroStepGap * (100 / ACCramp)) / DCCStep;
            float zeroDCCtime = ((DCCStep)*zeroStepGap) + ((DCCStep - 9) * (((DCCStep) * (zeroDCCstepInc / 2))));
            float zeroTOTtime = zeroACCtime + zeroNORtime + zeroDCCtime;
            float overclockPerc = speedSP / zeroTOTtime;
            calcStepGap = zeroStepGap * overclockPerc;
            if (calcStepGap <= minSpeedDelay)
            {
              calcStepGap = minSpeedDelay;
              speedViolation = "1";
            }
          }

          // calc step gap for percentage
          else if (SpeedType == "p")
          {
            calcStepGap = ((maxSpeedDelay - ((SpeedVal / 100) * maxSpeedDelay)) + minSpeedDelay);
          }

          // calculate final step increments
          float calcACCstepInc = (calcStepGap * (100 / ACCramp)) / ACCStep;
          float calcDCCstepInc = (calcStepGap * (100 / ACCramp)) / DCCStep;
          float calcACCstartDel = (calcACCstepInc * ACCStep) * 2;
          float calcDCCendDel = (calcDCCstepInc * DCCStep) * 2;

          // calc way pt speeds
          float ACCwayPts = wayPts * (ACCspd / 100);
          float NORwayPts = wayPts * ((100 - ACCspd - DCCspd) / 100);
          float DCCwayPts = wayPts * (DCCspd / 100);

          // calc way inc for lin way steps
          float ACCwayInc = (calcACCstartDel - calcStepGap) / ACCwayPts;
          float DCCwayInc = (calcDCCendDel - calcStepGap) / DCCwayPts;

          // set starting delsy
          float curDelay = calcACCstartDel;

          // calc track way pt moves
          int TRfutStepM = (xyzuvw_In[6] + TRaxisLimNeg) * TRStepDeg;
          int TRstepDif = (TRStepM - TRfutStepM) / (wayPts - 1);
          if (TRstepDif <= 0)
          {
            TRdir = 1;
          }
          else
          {
            TRdir = 0;
          }

          resetEncoders();
          /////////////////////////////////////////////////
          // loop through waypoints
          for (int i = 1; i <= wayPts; i++)
          {

            ////DELAY CALC/////
            if (i <= ACCwayPts)
            {
              curDelay = curDelay - (ACCwayInc);
            }
            else if (i >= (wayPts - DCCwayPts))
            {
              curDelay = curDelay + (DCCwayInc);
            }
            else
            {
              curDelay = calcStepGap;
            }

            if (debugg == 1)
            {
              curDelay = 0;
            }

            float curWayPerc = wayPerc * i;
            xyzuvw_In[0] = Xstart + (Xvect * curWayPerc);
            xyzuvw_In[1] = Ystart + (Yvect * curWayPerc);
            xyzuvw_In[2] = Zstart + (Zvect * curWayPerc);
            xyzuvw_In[3] = RZstart + (RZvect * curWayPerc);
            xyzuvw_In[4] = RYstart + (RYvect * curWayPerc);
            xyzuvw_In[5] = RXstart + (RXvect * curWayPerc);

            SolveInverseKinematic();

            // calc destination motor steps
            int J1futStepM = (JangleOut[0] + J1axisLimNeg) * J1StepDeg;
            int J2futStepM = (JangleOut[1] + J2axisLimNeg) * J2StepDeg;
            int J3futStepM = (JangleOut[2] + J3axisLimNeg) * J3StepDeg;
            int J4futStepM = (JangleOut[3] + J4axisLimNeg) * J4StepDeg;
            int J5futStepM = (JangleOut[4] + J5axisLimNeg) * J5StepDeg;
            int J6futStepM = (JangleOut[5] + J6axisLimNeg) * J6StepDeg;

            // calc delta from current to destination
            int J1stepDif = J1StepM - J1futStepM;
            int J2stepDif = J2StepM - J2futStepM;
            int J3stepDif = J3StepM - J3futStepM;
            int J4stepDif = J4StepM - J4futStepM;
            int J5stepDif = J5StepM - J5futStepM;
            int J6stepDif = J6StepM - J6futStepM;

            // determine motor directions
            if (J1stepDif <= 0)
            {
              J1dir = 1;
            }
            else
            {
              J1dir = 0;
            }

            if (J2stepDif <= 0)
            {
              J2dir = 1;
            }
            else
            {
              J2dir = 0;
            }

            if (J3stepDif <= 0)
            {
              J3dir = 1;
            }
            else
            {
              J3dir = 0;
            }

            if (J4stepDif <= 0)
            {
              J4dir = 1;
            }
            else
            {
              J4dir = 0;
            }

            if (J5stepDif <= 0)
            {
              J5dir = 1;
            }
            else
            {
              J5dir = 0;
            }

            if (J6stepDif <= 0)
            {
              J6dir = 1;
            }
            else
            {
              J6dir = 0;
            }

            // determine if requested position is within axis limits
            if ((J1dir == 1 and (J1StepM + J1stepDif > J1StepLim)) or (J1dir == 0 and (J1StepM - J1stepDif < 0)))
            {
              J1axisFault = 1;
            }
            if ((J2dir == 1 and (J2StepM + J2stepDif > J2StepLim)) or (J2dir == 0 and (J2StepM - J2stepDif < 0)))
            {
              J2axisFault = 1;
            }
            if ((J3dir == 1 and (J3StepM + J3stepDif > J3StepLim)) or (J3dir == 0 and (J3StepM - J3stepDif < 0)))
            {
              J3axisFault = 1;
            }
            if ((J4dir == 1 and (J4StepM + J4stepDif > J4StepLim)) or (J4dir == 0 and (J4StepM - J4stepDif < 0)))
            {
              J4axisFault = 1;
            }
            if ((J5dir == 1 and (J5StepM + J5stepDif > J5StepLim)) or (J5dir == 0 and (J5StepM - J5stepDif < 0)))
            {
              J5axisFault = 1;
            }
            if ((J6dir == 1 and (J6StepM + J6stepDif > J6StepLim)) or (J6dir == 0 and (J6StepM - J6stepDif < 0)))
            {
              J6axisFault = 1;
            }
            if ((TRdir == 1 and (TRStepM + TRstepDif > TRStepLim)) or (TRdir == 0 and (TRStepM - TRstepDif < 0)))
            {
              TRaxisFault = 1;
            }
            TotalAxisFault = J1axisFault + J2axisFault + J3axisFault + J4axisFault + J5axisFault + J6axisFault + TRaxisFault;

            // send move command if no axis limit error
            if (TotalAxisFault == 0 && KinematicError == 0)
            {
              driveMotorsL(abs(J1stepDif), abs(J2stepDif), abs(J3stepDif), abs(J4stepDif), abs(J5stepDif), abs(J6stepDif), abs(TRstepDif), J1dir, J2dir, J3dir, J4dir, J5dir, J6dir, TRdir, curDelay);
              updatePos();
            }
            else if (KinematicError == 1)
            {
              Alarm = "ER";
              delay(5);
              Serial.println(Alarm);
            }
            else
            {
              Alarm = "EL" + String(J1axisFault) + String(J2axisFault) + String(J3axisFault) + String(J4axisFault) + String(J5axisFault) + String(J6axisFault) + String(TRaxisFault);
              delay(5);
              Serial.println(Alarm);
            }
          }
        }

        inData = ""; // Clear recieved buffer
        ////////MOVE COMPLETE///////////
        delay(5);
        checkEncoders();
        sendRobotPos();
      }
      else
      {
        inData = ""; // Clear recieved buffer
      }
    }
  }
}