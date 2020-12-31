/*
code	color
0x0000	Black
0xFFFF	White
0xBDF7	Light Gray
0x7BEF	Dark Gray
0xF800	Red
0xFFE0	Yellow
0xFBE0	Orange
0x79E0	Brown
0x7E0	Green
0x7FF	Cyan
0x1F	Blue
0xF81F	Pink

 */

#include <WiFi.h>
#include "time.h"

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>

const char* ssid       = "d";
const char* password   = "marhar#2";
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 16*3600;
const int   daylightOffset_sec = 3600;
struct tm timeinfo;

void printLocalTime()
{
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print(timeinfo.tm_hour);
  Serial.print(":");
  Serial.print(timeinfo.tm_min);
  Serial.print(":");
  Serial.print(timeinfo.tm_sec);
  Serial.println("");
}

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h


byte omm = 99;
boolean initial = 1;
byte xcolon = 0;
unsigned int colour = 0;

void setup(void) {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK); // Note: the new fonts do not draw the background colour
  
  //connect to WiFi
  Serial.printf("\n\nConnecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  //WiFi.disconnect(true);
  //WiFi.mode(WIFI_OFF);
}

void loop() {
  uint8_t hh, mm, ss;
  printLocalTime();
  hh = timeinfo.tm_hour;
  mm = timeinfo.tm_min;
  ss = timeinfo.tm_sec;
  delay(1000);

  if (ss==0 || initial) {
    initial = 0;
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setCursor (8, 52);
    tft.print(__DATE__); // This uses the standard ADAFruit small font

    tft.setTextColor(TFT_BLUE, TFT_BLACK);
    tft.drawCentreString("It is windy",120,48,2); // Next size up font 2

    //tft.setTextColor(0xF81F, TFT_BLACK); // Pink
    //tft.drawCentreString("12.34",80,100,6); // Large font 6 only contains characters [space] 0 1 2 3 4 5 6 7 8 9 . : a p m
  }

  // Update digital time
  byte xpos = 6;
  byte ypos = 0;
  if (omm != mm) { // Only redraw every minute to minimise flicker
    // Uncomment ONE of the next 2 lines, using the ghost image demonstrates text overlay as time is drawn over it
    tft.setTextColor(0x39C4, TFT_BLACK);  // Leave a 7 segment ghost image, comment out next line!
    //tft.setTextColor(TFT_BLACK, TFT_BLACK); // Set font colour to black to wipe image
    // Font 7 is to show a pseudo 7 segment display.
    // Font 7 only contains characters [space] 0 1 2 3 4 5 6 7 8 9 0 : .
    tft.drawString("88:88",xpos,ypos,7); // Overwrite the text to clear it
    tft.setTextColor(0xFBE0, TFT_BLACK); // Orange
    omm = mm;

    if (hh<10) xpos+= tft.drawChar('0',xpos,ypos,7);
    xpos+= tft.drawNumber(hh,xpos,ypos,7);
    xcolon=xpos;
    xpos+= tft.drawChar(':',xpos,ypos,7);
    if (mm<10) xpos+= tft.drawChar('0',xpos,ypos,7);
    tft.drawNumber(mm,xpos,ypos,7);

#if 0
    // left off here
    xpos+= tft.drawChar(':',xpos,ypos,7);
    if (ss<10) xpos+= tft.drawChar('0',xpos,ypos,7);
    tft.drawNumber(ss,xpos,ypos,7);
#endif      
  }

  if (ss%2) { // Flash the colon
    tft.setTextColor(0x39C4, TFT_BLACK);
    xpos+= tft.drawChar(':',xcolon,ypos,7);
    tft.setTextColor(0xFBE0, TFT_BLACK);
  }
  else {
    tft.drawChar(':',xcolon,ypos,7);
    colour = random(0xFFFF);
    // Erase the old text with a rectangle, the disadvantage of this method is increased display flicker
    tft.fillRect (0, 64, 160, 20, TFT_BLACK);
    tft.setTextColor(colour);
    tft.drawRightString("Colour",75,64,4); // Right justified string drawing to x position 75
    String scolour = String(colour,HEX);
    scolour.toUpperCase();
    char buffer[20];
    scolour.toCharArray(buffer,20);
    tft.drawString(buffer,82,64,4);
  }
}
