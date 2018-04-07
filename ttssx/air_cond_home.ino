// IR DEFINITIONS
#define IR_PIN       3
#define IR_PIN_HIGH  B00001000             // Data located in which list? 0: OFF, 1: ON
#define IR_PIN_LOW   B00000000             // Data located in which list? 0: OFF, 1: ON

int IrSignalOn[] = {
	311, 388,
	192, 98,
	95, 195,
	95, 98,
	95, 98,
	192, 98,
	101, 94,
	92, 98,
	102, 91,
	102, 188,
	200, 90,
	95, 195,
	192, 98,
	95, 98,
	101, 95,
	92, 98,
	102, 91,
	95, 98,
	101, 95,
	98, 95,
	98, 95,
	92, 98,
	101, 94,
	98, 95,
	99, 91,
	95, 98,
	95, 98,
	102, 91,
	102, 188,
	198, 95,
	299, 388,
	199, 92,
	101, 192,
	92, 98,
	102, 91,
	192, 98,
	95, 98,
	95, 98,
	102, 91,
	102, 188,
	192, 98,
	102, 188,
	192, 98,
	101, 95,
	100, 91,
	102, 91,
	95, 98,
	102, 91,
	102, 91,
	102, 91,
	95, 98,
	102, 91,
	101, 95,
	92, 98,
	95, 98,
	102, 92,
	101, 95,
	98, 95,
	92, 195,
	192, 98,
	295, 395,
	192, 98,
	95, 195,
	102, 92,
	95, 98,
	198, 94,
	92, 98,
	95, 98,
	95, 98,
	95, 195,
	192, 98,
	95, 195,
	199, 92,
	101, 95,
	98, 92,
	95, 98,
	102, 91,
	95, 98,
	101, 95,
	92, 98,
	101, 95,
	92, 98,
	101, 95,
	92, 98,
	95, 98,
	95, 98,
	102, 92,
	101, 95,
	99, 188,
	198, 92,
	399, 0
  };
  
int IrSignalOff[] = {
	439, 439,
	54, 0
  };




// This procedure sends a 38KHz pulse to the IRledPin 
// for a certain # of microseconds. We'll use this whenever we need to send codes
void pulseIR(long microsecs)
{
  // we'll count down from the number of microseconds we are told to wait
 
  cli();  // this turns off any background interrupts
 
  while (microsecs > 0)
  {
    // 38 kHz is about 13 microseconds high and 13 microseconds low
   PORTD = IR_PIN_HIGH;
   delayMicroseconds(13);  // 13
   PORTD = IR_PIN_LOW;
   delayMicroseconds(13);  // 13
 
   // so 26 microseconds altogether
   microsecs -= 26;
  }
 
  sei();  // this turns them back on
}


void SendIRCodeOn()
{
  int arraySize = sizeof(IrSignalOn)/2;
  for (int i = 0; i < arraySize; i++)
  {
    pulseIR(IrSignalOn[i++] * 10);
    delayMicroseconds(IrSignalOn[i] * 10);
  }
}

void SendIRCodeOff()
{
  int arraySize = sizeof(IrSignalOff)/2;
  for (int i = 0; i < arraySize; i++)
  {
    pulseIR(IrSignalOff[i++] * 10);
    delayMicroseconds(IrSignalOff[i] * 10);
  }
}

void setup(void)
{
  pinMode(IR_PIN, OUTPUT);      
}

void loop(void)
{
    SendIRCodeOn();
    delay(2500);
    //SendIRCodeOff();
    //delay(5000);
}
