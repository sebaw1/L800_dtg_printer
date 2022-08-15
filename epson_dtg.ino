const int iPinStartPrintSensor = 2;     //limit switch behind pump
const int iPinTableStop = 12;           //limit switch table stop
const int iPinTableStart = 4;           //limit switch table start  
const int oPinPESensor = 5;             //paper sensor
const int oPinClutch = 6;               //relay clutch
const int oPinPWSensor = 7;             //sensor near printhead
bool startedPrinting = false;
const int CLUTCH_DELAY_AFTER_PE = 200;
const int PE_DELAY = 500;                 //DON'T CHANGE

void setup() {
  
  //Serial.begin(115200);

  pinMode(iPinStartPrintSensor, INPUT_PULLUP);
  pinMode(iPinTableStart, INPUT_PULLUP);
  pinMode(iPinTableStop, INPUT_PULLUP);
  pinMode(oPinPWSensor, OUTPUT);
  pinMode(oPinPESensor, OUTPUT);
  pinMode(oPinClutch, OUTPUT);

 // digitalWrite(oPinPESensor, HIGH); //COIL -> HIGH == OFF
 // digitalWrite(oPinClutch, HIGH);//COIL -> low == ON

  digitalWrite(oPinPWSensor, LOW);                            //sensor near printhead
  digitalWrite(oPinPESensor, LOW); //moj pcb -> LOW == OFF    //paper sensor
  digitalWrite(oPinClutch, LOW);//moj pcb -> HIGH == ON

 
  attachInterrupt(digitalPinToInterrupt(iPinStartPrintSensor), startPrint_INTERRUPT, FALLING); //czujnik za stacją dokującą. wykrycie zbocza opadającego
  
 // attachInterrupt(digitalPinToInterrupt(iPinTableStop), stopPrint_INTERRUPT, FALLING);
  
}

void loop() {

  if(startedPrinting)
  {
    if(!(digitalRead(iPinTableStart))) //if start table limit switch is switched
    {
          delay(PE_DELAY);
          digitalWrite(oPinPESensor, HIGH); //PE SENSOR relay ON
          delay(CLUTCH_DELAY_AFTER_PE);
          digitalWrite(oPinClutch,HIGH); //clutch
          //Serial.println("CLUTCH ON");
          
          //delay(1000);
          //sensor near printhead needs to be "cheated"
          digitalWrite(oPinPWSensor, HIGH); //oszukanie czujnika koło głowicy
          delay(1500);
          digitalWrite(oPinPWSensor, LOW); 
          delay(800);
          digitalWrite(oPinPWSensor, HIGH);
          delay(1000);
          digitalWrite(oPinPWSensor, LOW); 
          
          
    }
    startedPrinting = false;   
    
  }

  if(!(digitalRead(iPinTableStop))) //if table switched limit switch - table stop
  {
    digitalWrite(oPinClutch, LOW); //clutch OFF
    digitalWrite(oPinPESensor, LOW); //PE SENSOR OFF
  }
 
}

//czujnik za stacją dokującą (zbocze opadające)
//eliminacja drgań styków

//limit switch behind pump, delete bouncing 
void startPrint_INTERRUPT(){
  static unsigned long lastTime;
  unsigned long timeNow = millis();
  
  if (timeNow - lastTime < 450)
    return;
    
  startedPrinting = true;
  
  //Serial.println("PRINTING!");
  lastTime = timeNow;
}
