#include <SPI.h>
#include <SD.h>

// Set the pins used
#define cardSelect 4

char filename[15];
File logfile;

// blink out an error code
void error(uint8_t errno) {
  while(1) {
    uint8_t i;
    for (i=0; i<errno; i++) {
      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);
      delay(100);
    }
    for (i=errno; i<10; i++) {
      delay(200);
    }
  }
}

// This line is not needed if you have Adafruit SAMD board package 1.6.2+
//   #define Serial SerialUSB

void setup() {
  // connect at 115200
  Serial.begin(115200);
  //wait 10 seconds to see if a computer wants to hear hello world
  while(!Serial){if(millis()>10000)break;}
  Serial.println("\r\nAnalog logger test");
  pinMode(13, OUTPUT);


  // see if the card is present and can be initialized:
  if (!SD.begin(cardSelect)) {
    Serial.println("Card init. failed!");
    error(2);
  }
  strcpy(filename, "ANALOG00.TXT");
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = '0' + i/10;
    filename[7] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }

  logfile = SD.open(filename, FILE_WRITE);
  if( ! logfile ) {
    Serial.print("Couldnt create "); 
    Serial.println(filename);
    error(3);
  }
  Serial.print("Writing to "); 
  Serial.println(filename);

  pinMode(13, OUTPUT);
  pinMode(8, OUTPUT);
  Serial.println("Ready!");
  logfile.close();
  
}

uint8_t i=0;
void loop() {
  digitalWrite(8, HIGH);
  logfile = SD.open(filename, FILE_WRITE);
  logfile.print("A0 = "); logfile.println(analogRead(0));
  logfile.close();
  Serial.print("A0 = "); Serial.println(analogRead(0));
  digitalWrite(8, LOW);
  
  delay(100);
}
