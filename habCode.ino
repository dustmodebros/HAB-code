#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <Adafruit_MPL3115A2.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <string.h>
#include <util/crc16.h>

/*
   This code requires the use of SoftwareSerial, and assumes that you have a
   9600-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
   It also assumes that you have a NTX2B transmitter with its data line 
   connected to pin 13, feel free to change the below definition to whichever
   is most convenient for your layout.
   for the MPL3115A2, connect the SCL pin to I2C Clock (Analog 5 on an UNO) 
   and the SDA pin to I2C Data (Analog 4 on an UNO)
*/
#define RADIOPIN 13

static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600;

char datastring[80];
char sz[32];
File myFile;
float initHeight;

// The Barometer object
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();

// The GPS object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  pinMode(RADIOPIN,OUTPUT);
  baro.begin();
  SD.begin(10);
  Serial.begin(115200); //
  ss.begin(GPSBaud);
  initHeight = baro.getAltitude();
  Serial.println("new restart");
  myFile = SD.open("results.txt", FILE_WRITE);
  myFile.println("new restart");
  myFile.close();
}

void loop()
{
  //need to reopen and close the file on each loop to ensure saving
  myFile = SD.open("results.txt", FILE_WRITE);

  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);

  sprintf(sz,  "%02d:%02d:%02d ", gps.time.hour() + 1, gps.time.minute(), gps.time.second());
  Serial.println(sz);
  myFile.println(sz);
  float altm = baro.getAltitude() - initHeight;
  float temp = baro.getTemperature();
  float pres = baro.getPressure();
  myFile.print(altm);
  myFile.println(" meters");
  Serial.print(altm);
  Serial.println(" meters");
  myFile.close();
  sprintf(datastring, "%s\n%f meters\n%f degC\n%f Pa\n\n", sz, altm, temp, pres);
  sprintf(checksum_str, "*%04X\n", checksum(datastring));
  strcat(datastring,checksum_str);
  rtty_txstring (datastring);
  smartDelay(5000);
 
  if (millis() > 5000 && gps.charsProcessed() < 10)
      Serial.println(F("No GPS data received: check wiring"));
}

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  { //calculates spacing for nice printing of floating point numbers wrt number of significant figures
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}

static void printDateTime(TinyGPSTime &t)
{
  if (!t.isValid())
  {
    Serial.print(F("******** "));
  }
  else
  {
    sprintf(sz, "%02d:%02d:%02d ", t.hour() + 1, t.minute(), t.second());
    Serial.print(sz);
  }
  smartDelay(0);
}

static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartDelay(0);
}

void rtty_txstring (char *string)
{
 // Sends one char at a time to rtty_txbyte
  char c;
  c = *string++;
  while ( c != '\0')
  {
    rtty_txbyte (c);
    c = *string++;
  }
}

void rtty_txbyte (char c)
{
  /* Sends each bit of a char to 
    ** rtty_txbit function. 
    ** NB The bits are sent Least Significant Bit first
    **
    ** All chars should be preceded with a 0 and 
    ** proceded with a 1. 0 = Start bit; 1 = Stop bit
    */
  int i;
  rtty_txbit (0); // Start bit
  // Send bits for for char LSB first 
  for (i=0;i<7;i++)
  {
    if (c & 1) rtty_txbit(1); 
    else rtty_txbit(0); 
    c = c >> 1;
  }
  rtty_txbit (1); // Stop bit
  rtty_txbit (1); // Stop bit
}

void rtty_txbit (int bit)
{
  if (bit)
  {
    // high
    digitalWrite(RADIOPIN, HIGH);
  }
  else
  {
    // low
    digitalWrite(RADIOPIN, LOW);
  }
 
  //delayMicroseconds(3370); // 300 baud
  delayMicroseconds(10000); // Both for 50 Baud 
  delayMicroseconds(10150); 
}

uint16_t checksum (char *string)
{ //borrowed checksum code from RTTY spec.
  size_t i;
  uint16_t crc;
  uint8_t c;
  crc = 0xFFFF;
  // Calculate checksum ignoring the first two $s
  for (i = 2; i < strlen(string); i++)
  {
    c = string[i];
    crc = _crc_xmodem_update (crc, c);
  }
  return crc;
}    
