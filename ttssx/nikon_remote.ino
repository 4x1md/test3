    // This sketch will send out a Nikon D50 trigger signal (probably works with most Nikons)
    // See the full tutorial at http://www.ladyada.net/learn/sensors/ir.html
    // this code is public domain, please enjoy!
     
    int IRledPin =  3;    // LED connected to digital pin 13
     
    // The setup() method runs once, when the sketch starts
     
    void setup()   {                
      // initialize the IR digital pin as an output:
      pinMode(IRledPin, OUTPUT);      
      pinMode(2, INPUT);      
    }
     
    void loop()                     
    {

//  PORTD = B00000000; // digital 4~7 HIGH, digital 3~0 LOW
//  delay(1000);
//  PORTD = B00001000; // digital 4~7 LOW, digital 3~0 HIGH
//  delay(1000);

      SendNikonCode();
      delay(2*1000);  // wait one minute (60 seconds * 1000 milliseconds)
    }
     
    // This procedure sends a 38KHz pulse to the IRledPin 
    // for a certain # of microseconds. We'll use this whenever we need to send codes
    void pulseIR(long microsecs) {
      // we'll count down from the number of microseconds we are told to wait
     
      cli();  // this turns off any background interrupts
     
      while (microsecs > 0) {
        // 38 kHz is about 13 microseconds high and 13 microseconds low


       PORTD = B00001000; // digital 3 HIGH
       //digitalWrite(IRledPin, HIGH);  // this takes about 3 microseconds to happen
       delayMicroseconds(13);         // hang out for 10 microseconds, you can also change this to 9 if its not working
       PORTD = B00000000; // digital 3 LOW
       //digitalWrite(IRledPin, LOW);   // this also takes about 3 microseconds
       delayMicroseconds(13);         // hang out for 10 microseconds, you can also change this to 9 if its not working
     
       // so 26 microseconds altogether
       microsecs -= 26;
      }
     
      sei();  // this turns them back on
    }
     
    void SendNikonCode() {
      // This is the code for my particular Nikon, for others use the tutorial
      // to 'grab' the proper code from the remote
     
      pulseIR(2080);
      delay(27);
      pulseIR(440);
      delayMicroseconds(1500);
      pulseIR(460);
      delayMicroseconds(3440);
      pulseIR(480);
     
     
      delay(65); // wait 65 milliseconds before sending it again
     
      pulseIR(2080);
      delay(27);
      pulseIR(440);
      delayMicroseconds(1500);
      pulseIR(460);
      delayMicroseconds(3440);
      pulseIR(480);
    }
