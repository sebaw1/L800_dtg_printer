const int iPinStartPrintSensor = 2;
const int iPinTableStop = 12;
const int iPinTableStart = 4;
const int oPinPESensor = 5;
const int oPinClutch = 6;
const int oPinPWSensor = 7;
bool startedPrinting = false;//, stoppedPrinting = false;
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

  digitalWrite(oPinPWSensor, LOW);
  digitalWrite(oPinPESensor, LOW); //moj pcb -> LOW == OFF
  digitalWrite(oPinClutch, LOW);//moj pcb -> HIGH == ON

 
  attachInterrupt(digitalPinToInterrupt(iPinStartPrintSensor), startPrint_INTERRUPT, FALLING); //czujnik za stacją dokującą. wykrycie zbocza opadającego
  
 // attachInterrupt(digitalPinToInterrupt(iPinTableStop), stopPrint_INTERRUPT, FALLING);
  
}

void loop() {

  if(startedPrinting)
  {
    if(!(digitalRead(iPinTableStart))) //jesli wcisnieta krańcówka startu druku
    {
          delay(PE_DELAY);
          digitalWrite(oPinPESensor, HIGH); //zewrzyj styki PE sensora
          delay(CLUTCH_DELAY_AFTER_PE);
          digitalWrite(oPinClutch,HIGH);//sprzęgło
          //Serial.println("CLUTCH ON");
          
          //delay(1000);
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

  if(!(digitalRead(iPinTableStop))) //jesli wcisnieta krańcówka stopu druku
  {
    digitalWrite(oPinClutch, LOW); //odlacz sprzegło
    digitalWrite(oPinPESensor, LOW); //czujnik PE wylacz
  }
 
}

//czujnik za stacją dokującą (zbocze opadające)
//eliminacja drgań styków
void startPrint_INTERRUPT(){
  static unsigned long lastTime;
  unsigned long timeNow = millis();
  
  if (timeNow - lastTime < 450)
    return;
    
  startedPrinting = true;
  
  //Serial.println("PRINTING!");
  lastTime = timeNow;
}
