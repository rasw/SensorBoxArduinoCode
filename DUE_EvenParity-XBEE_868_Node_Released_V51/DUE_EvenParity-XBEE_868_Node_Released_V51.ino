 /* DUO VERSION
 =================================================================================
  XBEE 868Mhz -  RS485 Node Transparent Mode - With RX/TX and power LEDs
 =================================================================================
    - Pin 20 used for RS485 direction control 
    - Pin 13 turned off
    - Pin A1 LED blinks when data is transmitted
    - Pin A0 LED blinks when data is received
    - Pin A2 Software power indicator LED 
 =================================================================================
  Written for the Arduino DUO Board.
  Richard Wilson 
  AKVA Group Scotland Ltd
  Software Revision 5.0.1 Released
  Parity Version (868Mhz Xbee) with commands
 =================================================================================
*/

#define XBEE_RX 16              // Serial Receive pin (RS485 R Pin)
#define XBEE_TX 17              // Serial Transmit pin (RS485 D Pin)
#define SSerialTxControl 20     // RS485 Direction control (Pin 20)
#define RS485Transmit HIGH      // define constant HIGH
#define RS485Receive LOW        // define constant LOW
#define Pin13LED 13             // define flag pin

#define RX_LED A0               // define receive LED pin (BLUE)
#define TX_LED A1               // define transmit LED pin (GREEN)
#define PowerLED A2             // define software power LED pin (RED)
#define IsMaster 12             // Master / Slave Jumper  HIGH = Slave Mode


byte byteFromXBEE;
byte byteFromRS485;
byte Send2Xbee;
byte Send2RS485;

int PingFlag = 0;
int i = 0;
bool skipThisSend = false;
int DayCounter = 0;
unsigned long milliSeconds;


void LoopLEDs()
{
  for (i=0; i < 10; i++)
  {
    digitalWrite(TX_LED, 1);
    delay(200);
    digitalWrite(TX_LED, 0);
    delay(200);
    digitalWrite(RX_LED, 1);
    delay(200);
    digitalWrite(RX_LED, 0);
    delay(200);
    digitalWrite(PowerLED, 1);
    delay(200);
    digitalWrite(PowerLED, 0);
    delay(200);
  }

   digitalWrite(TX_LED, 0);
   digitalWrite(RX_LED, 0);
   digitalWrite(PowerLED, 1);
}


void setup() 
{ 
   pinMode(Pin13LED, OUTPUT);                           // setup IO pins
   pinMode(RX_LED, OUTPUT);
   pinMode(TX_LED, OUTPUT); 
   pinMode(PowerLED, OUTPUT); 
   pinMode(SSerialTxControl, OUTPUT); 
   pinMode(IsMaster,INPUT_PULLUP);                      // LOW = Master   (default HIGH using no jumper with pullups)
  
  // Serial.begin(9600);                                  // P USB Port
   Serial1.begin(9600,SERIAL_8E1);                      // RS485 Port (Sensor)
   Serial2.begin(9600);                                 // XBEE Port (XBEE)
   digitalWrite(SSerialTxControl, RS485Receive);        // Set RS485 Testing only
   digitalWrite(PowerLED,HIGH);                         // Turn on power LED
   digitalWrite(TX_LED,LOW);                            // Turn off power LED
   digitalWrite(RX_LED,LOW);                            // Turn off power LED
   digitalWrite(Pin13LED,LOW);                          // Turn off Pin 13 LED
   
   LoopLEDs();                                          // LT Leds and wait for connections to stabilise  
}


void loop() {

//  Each character takes 104 microseconds to transmit

 delayMicroseconds(500); // MUST BE 500 for AKVA CONNECT !!!
  
  // RS485 Port Read  ==============================================
  if (Serial1.available()) 
  {
    Send2Xbee = Serial1.read();                     // Read the byte 
    digitalWrite(RX_LED, HIGH);                     // Show activity
    delayMicroseconds(80);  
    Serial2.write(Send2Xbee);        
    //delayMicroseconds(1200); // was 80
    digitalWrite(RX_LED, LOW);
  }

  // XBee Port Read  ===============================================
  if (Serial2.available()) 
  { 
    digitalWrite(TX_LED, HIGH); 
    digitalWrite(SSerialTxControl, RS485Transmit);      // Disable RS485 Transmit 
    Send2RS485 = Serial2.read();                        // Read the byte 
    //delayMicroseconds(1200); 
    Serial1.write(Send2RS485);                         // Send btye to RS485
    delayMicroseconds(2000);                           // <<< -- Vital must be at least 2000 (AKVA COPNNECT)
    digitalWrite(SSerialTxControl, RS485Receive);      // reset RS485 to receive
    digitalWrite(TX_LED, LOW); 
  }
  else
  {
     digitalWrite(SSerialTxControl, RS485Receive);      // reset RS485 to receive
     delayMicroseconds(2000);                           // <<< -- Vital must be at least 2000 (AKVA COPNNECT)
  }
 
}

