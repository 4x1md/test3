#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <LiquidCrystal.h>

#define DEBOUNCE 2
#define ENC_INT 1
#define ENC_INT_PIN 3
#define ENC_DATA_PIN A5
#define TEMP_PIN A1

#define CMD_CONNECT 0x00
#define CMD_ECHO 0x01
#define CMD_ENC 0x02
#define CMD_TEMP 0x03
#define CMD_DISCONNECT 0xFF

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
EthernetUDP Udp;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(11, 0, 0, 50);
unsigned int localPort = 666;

char rxBuffer[UDP_TX_PACKET_MAX_SIZE];
char txBuffer[16];
byte bufferSize = 0;

bool conn = false;
unsigned int temp = 0;
boolean encoderChanged = false;
boolean encoderData = false;
byte encoderVal = 0x00;

void setup()
{
	lcd.begin(8, 2);
	lcd.print("NC");
	lcd.setCursor(6, 0);
	lcd.print("  ");
	
	Ethernet.begin(mac, ip);
	Udp.begin(localPort);

	Serial.begin(115200);
	
	// LOW - henever the pin is low, CHANGE - whenever the pin changes value, RISING, FALLING
	attachInterrupt(ENC_INT, encoderInt, FALLING);
	pinMode(ENC_INT_PIN, INPUT_PULLUP);
	pinMode(ENC_DATA_PIN, INPUT_PULLUP);
	
	analogReference(INTERNAL);
	pinMode(TEMP_PIN, INPUT);
	//pinMode(TEMP_PIN, INPUT_PULLUP);
}

void loop()
{
	if (encoderChanged)
	{
		delay(DEBOUNCE);
		encoderData = digitalRead(ENC_DATA_PIN);
		if (encoderData)
		{
			encoderVal++;
		}
		else
		{
			encoderVal--;
		}
		byte bh = (encoderVal & 0xF0) >> 4;
		bh = charFromByte(bh);
		byte bl = encoderVal & 0x0F;
		bl = charFromByte(bl);
		
		lcd.setCursor(6, 0);
		lcd.print(char(bh));
		lcd.print(char(bl));
		
		Serial.print("VAL=");
		Serial.print(char(bh));
		Serial.print(char(bl));
		Serial.print(" ED=");
		Serial.println(encoderData);
	
		encoderChanged = false;
		encoderData = false;
		return;
	}
  
	int packetSize = Udp.parsePacket();
	if (packetSize)
	{
		Serial.print("RX ");
		Serial.print(packetSize);
		Serial.print("B, SRC=");
		IPAddress remote = Udp.remoteIP();
		for (int i = 0; i < 4; i++)
		{
		  Serial.print(remote[i], DEC);
		  if (i < 3)
		  {
			Serial.print(".");
		  }
		}
		Serial.print(":");
		Serial.println(Udp.remotePort());
		
		// read the packet into packetBufffer
		lcd.setCursor(0, 1);
		for (int i = 0; i < 16; i++)
		{
		  rxBuffer[i] = 0;
		  txBuffer[i] = 0;
		  lcd.print(' ');
		}

		Udp.read(rxBuffer, UDP_TX_PACKET_MAX_SIZE);
		Serial.print("RX: ");
		Serial.println(rxBuffer);
		
		switch ((unsigned char)rxBuffer[0])
		{
			case CMD_CONNECT:
				lcd.setCursor(0, 0);
				lcd.print("PC");
				Serial.println("PC");
				txBuffer[0] = CMD_CONNECT;
				bufferSize = 1;
				conn = true;
				break;
			case CMD_ECHO:
				Serial.println("ECHO");
				for (byte i = 0; i < 16; i++)
				{
					txBuffer[i] = rxBuffer[i];
				}
				bufferSize = packetSize;
				break;
			case CMD_ENC:
				Serial.println("ENC");
				txBuffer[0] = CMD_ENC;
				txBuffer[1] = encoderVal;
				bufferSize = 2;
				break;
			case CMD_TEMP:
				Serial.print("TEMP = ");
				Serial.println(temp);
				txBuffer[0] = CMD_TEMP;
				txBuffer[1] = (temp >> 8) & 0xFF;
				txBuffer[2] = temp & 0xFF;
				bufferSize = 3;
				break;
			case CMD_DISCONNECT:
				lcd.setCursor(0, 0);
				lcd.print("NC");
				Serial.println("NC");
				txBuffer[0] = CMD_DISCONNECT;
				bufferSize = 1;
				conn = false;
				break;
			default:
				lcd.setCursor(0, 1);
				lcd.print(rxBuffer);
				Serial.println("default");
				txBuffer[0] = CMD_DISCONNECT;
				bufferSize = 1;
				break;
		}
		// Send a reply
		Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
		Udp.write(txBuffer, bufferSize);
		Udp.endPacket();
	}

	temp = analogRead(TEMP_PIN);
	delay(100);
  
}

void encoderInt()
{
	encoderChanged = true;
}

char charFromByte(byte val)
{
	if (val < 10)
	{
		val += 48;
	}
	else
	{
		val += 55;
	}
	return val;
}


/*
  Processing sketch to run with this example
 =====================================================

 // Processing UDP example to send and receive string data from Arduino
 // press any key to send the "Hello Arduino" message


 import hypermedia.net.*;

 UDP udp;  // define the UDP object


 void setup() {
 udp = new UDP( this, 6000 );  // create a new datagram connection on port 6000
 //udp.log( true ); 		// <-- printout the connection activity
 udp.listen( true );           // and wait for incoming message
 }

 void draw()
 {
 }

 void keyPressed() {
 String ip       = "192.168.1.177";	// the remote IP address
 int port        = 8888;		// the destination port

 udp.send("Hello World", ip, port );   // the message to send

 }

 void receive( byte[] data ) { 			// <-- default handler
 //void receive( byte[] data, String ip, int port ) {	// <-- extended handler

 for(int i=0; i < data.length; i++)
 print(char(data[i]));
 println();
 }
 */


