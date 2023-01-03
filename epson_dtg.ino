#include <Bounce2.h>

const int iPinStartPrintSensor = 2;     //limit switch behind pump
const int iPinTableStop = 4;           //limit switch table stop
const int iPinTableStart = 3;           //limit switch table start

const int oPinPESensor = 5;             //paper sensor
const int oPinClutch = 6;               //relay clutch
const int oPinPWSensor = 7;             //sensor near printhead

const int CLUTCH_DELAY_AFTER_PE = 200;
const int PE_DELAY = 500;                 //DON'T CHANGE

Bounce debouncerPinStartPrintSensor = Bounce();
Bounce debouncerPinTableStop = Bounce();

volatile bool startedPrinting = false;

void setup() {

  //Serial.begin(115200);

  pinMode(iPinStartPrintSensor, INPUT_PULLUP);
  pinMode(iPinTableStart, INPUT_PULLUP);
  pinMode(iPinTableStop, INPUT_PULLUP);

  pinMode(oPinPWSensor, OUTPUT);
  pinMode(oPinPESensor, OUTPUT);
  pinMode(oPinClutch, OUTPUT);

  debouncerPinStartPrintSensor.attach(iPinStartPrintSensor);
  debouncerPinStartPrintSensor.interval(2);

  debouncerPinTableStop.attach(iPinTableStop);
  debouncerPinTableStop.interval(2);


  digitalWrite(oPinPWSensor, LOW);                            //sensor near printhead
  digitalWrite(oPinPESensor, LOW); //my pcb -> LOW == OFF    //paper sensor
  digitalWrite(oPinClutch, LOW);//my pcb -> HIGH == ON

}

void loop() {

  debouncerPinStartPrintSensor.update();
  debouncerPinTableStop.update();

  if (debouncerPinStartPrintSensor.fell())
  {
    //Serial.println("z tylu pompki");
    startedPrinting = true;
  }

  if (!(digitalRead(iPinTableStart)) && startedPrinting) //if start table limit switch is switched
  {
    //Serial.println("CLUTCH ON");
    delay(PE_DELAY);
    digitalWrite(oPinPESensor, HIGH); //PE SENSOR relay ON
    delay(CLUTCH_DELAY_AFTER_PE);
    digitalWrite(oPinClutch, HIGH); //clutch


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

  if (debouncerPinTableStop.fell()) //if table switched limit switch - table stop
  {
    //Serial.println("stop printing");
    digitalWrite(oPinClutch, LOW); //clutch OFF
    digitalWrite(oPinPESensor, LOW); //PE SENSOR OFF
  }

}
