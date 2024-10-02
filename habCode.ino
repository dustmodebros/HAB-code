#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <Adafruit_MPL3115A2.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>

/*
   This code requires the use of SoftwareSerial, and assumes that you have a
   9600-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/

static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600;

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
  //need to reopen and close the file on each loop
  myFile = SD.open("results.txt", FILE_WRITE);

  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);

  sprintf(sz,  "%02d:%02d:%02d ", gps.time.hour() + 1, gps.time.minute(), gps.time.second());
  Serial.println(sz);

  myFile.println(sz);
  float altm = baro.getAltitude() - initHeight;
  myFile.print(altm);
  myFile.println(" meters");
  Serial.print(altm);
  Serial.println(" meters");
  myFile.close();

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
  {
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
