/*
MIT License 
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
/*
 * Written By Dennis Ho 2018 May.
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include "conf.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ADS1115.h"
#include <FS.h>

#define SOFTAPMODE
#define HIDESSID false
#define MAXSTATION 4
#define CASTCH 13   
#define OLED_RESET D0

int node = 0;
char ssid [16]; 
char password [16] ; 
String localip = "0.0.0.0";

Adafruit_SSD1306 display(OLED_RESET);

ESP8266WebServer server(80);

ADS1115 ads;
char setupmode = 0;
const int led = D4;
const int SW1 = D5;
const int SW2 = D6;
const int SW3 = D7;
const int SW4 = D8;
int swlev1 = HIGH;
int swlev2 = HIGH;
int swlev3 = HIGH;
int swlev4 = HIGH;
int FLASHKEY = 10; 

String getContentType(String filename); 
bool handleFileRead(String path);       

void handleRoot() {
  int adc0;
  String buf = "{";
  digitalWrite(led, 1);
  ads.getAddr_ADS1115(ADS1115_GND_ADDRESS);
  Wire.beginTransmission(ADS1115_GND_ADDRESS);
  int error = Wire.endTransmission();
  if (error == 0)
  {
    int ad0 = ads.Measure_SingleEnded(0);
    int ad3 = ads.Measure_SingleEnded(3);
    if (ad0 & 0x8000 ) ad0 = 0;
    if (ad3 & 0x8000 ) ad3 = 0;
    buf += " \"ch0\" : ";
    buf += ad0 * 1.875 / 1000 ;
    buf += ",";
    buf += " \"vcc\" : ";
    buf += ad3 * 1.875 / 10000;
    buf += ",";
    buf += " \"status\" : \"good\" , ";
  } else {
    buf += " \"status\" : \"bad\" , ";
  }
  buf += " \"sw1\" : ";
  if ( swlev1 == HIGH ) {
    buf += "\"ON\"";
  } else {
    buf += "\"OFF\"";
  }
  buf += ", ";
  buf += " \"sw2\" : ";
  if ( swlev2 == HIGH ) {
    buf += "\"ON\"";
  } else {
    buf += "\"OFF\"";
  }
  buf += ", ";
  buf += " \"sw3\" : ";
  if ( swlev3 == HIGH ) {
    buf += "\"ON\"";
  } else {
    buf += "\"OFF\"";
  }
  buf += ", ";
  buf += " \"sw4\" : ";
  if ( swlev4 == HIGH ) {
    buf += "\"ON\"";
  } else {
    buf += "\"OFF\"";
  }
  buf += ", ";
  buf += " \"key\" : ";
  if ( digitalRead (FLASHKEY) == HIGH ) {
    buf += "\"RELEASE\"";
  } else {
    buf += "\"PRESS\"";
  }
  buf += ", ";
  buf += " \"node\" : " + String(node);
  buf += " }";
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Max-Age", "1");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS, PUT");
  server.sendHeader("Access-Control-Allow-Headers", "accept, authorization,  content-type, x-requested-with");
  server.sendHeader("Access-Control-Allow-Credentials", "true");
  server.send(200, "application/json", buf);
  digitalWrite(led, 0);
}

bool handleFileRead(String path) { 
  if (path.endsWith("/")) path += "index.html";        
  String contentType = getContentType(path);           
  if (SPIFFS.exists(path)) {                            
    File file = SPIFFS.open(path, "r");                 
    size_t sent = server.streamFile(file, contentType); 
    file.close();                                       
    return true;
  }
  return false;                                         
}

String getContentType(String filename) {
  if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

void handleSW1() {
  String msg = "switched: ";
  swlev1 = (swlev1 == HIGH ) ? LOW : HIGH;
  msg += ( swlev1 == HIGH ) ? "ON" : "OFF";
  msg += "\n";
  server.send(200, "text/plain", msg );
  digitalWrite (SW1 , swlev1);
}

void handleSW2() {
  String msg = "switched: ";
  swlev2 = (swlev2 == HIGH ) ? LOW : HIGH;
  msg += ( swlev2 == HIGH ) ? "ON" : "OFF";
  msg += "\n";
  server.send(200, "text/plain", msg );
  digitalWrite (SW1 , swlev2);
}

void handleSW3() {
  String msg = "switched: ";
  swlev3 = (swlev3 == HIGH ) ? LOW : HIGH;
  msg += ( swlev3 == HIGH ) ? "ON" : "OFF";
  msg += "\n";
  server.send(200, "text/plain", msg );
  digitalWrite (SW1 , swlev3);
}

void handleSW4() {
  String msg = "switched: ";
  swlev4 = (swlev4 == HIGH ) ? LOW : HIGH;
  msg += ( swlev4 == HIGH ) ? "ON" : "OFF";
  msg += "\n";
  server.send(200, "text/plain", msg );
  digitalWrite (SW1 , swlev4);
}


void handleOptions () {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Max-Age", "1");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS, PUT");
  server.sendHeader("Access-Control-Allow-Headers", "accept, authorization,  content-type, x-requested-with");
  server.sendHeader("Access-Control-Allow-Credentials", "true");
  server.send(200, "text/plain", "" );
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void wifiaccess () {
  IPAddress addr (192, 168, 1, 201);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress gateway(0, 0, 0, 0);
  WiFi.disconnect ();
  display.clearDisplay();
  display.setCursor (0, 0);
  display.printf ("MASTER NODE = %d\n", node);
  WiFi.softAPdisconnect(true); 
  delay (300);
  WiFi.softAPdisconnect(false); 
  delay (300);
  WiFi.softAPConfig(addr, gateway, subnet);
  WiFi.softAP(ssid, password, CASTCH,  HIDESSID, MAXSTATION); 
  WiFi.config(addr, gateway, subnet);
  WiFi.begin();
  display.setCursor (0, 8);
  display.printf ("%s\n", ssid);
  localip = WiFi.localIP().toString();
  display.printf ("IP=%s:\n", localip.c_str());
  display.println ("Web Console ");
  display.println ("http://192.168.1.201");
  display.display();
}

const String etype[] = { "NA", "NA", "TKIP", "NA", "WPA", "WEP", "NA", "NONE", "AUTO"};
void wificonnect () {
  IPAddress addr (192, 168, 1, 200 + node);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress gateway(0, 0, 0, 0);
  int rec = 0;
  WiFi.disconnect ();
  delay (500);
  display.clearDisplay();
  display.setCursor (0, 0);
  display.printf ("STATION NODE = %d\n", node);
  display.display();
  WiFi.mode (WIFI_STA);
  WiFi.config(addr, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    display.setCursor (0, 56);
    rec ++;
    display.printf ("TRY connecting %d\n", rec % 2);
    display.display();
    if (digitalRead (led) > 0) {
      digitalWrite (led , LOW);
    } else {
      digitalWrite (led, HIGH);
    }
  }
  display.clearDisplay();
  display.setCursor (0, 0);
  display.printf ("STATION NODE = %d\n", node);
  display.setCursor (0, 16);
  display.printf ("%s\n", ssid);
  localip = WiFi.localIP().toString();
  display.printf ("IP=%s:\n", localip.c_str());
  display.println ("Web Console ");
  display.println ("http://192.168.1.201");
  display.display();
}

char *readline () {
  char dbuf[32];
  memset (dbuf, 0, 32);
  uint16_t idx = 0;
  uint8_t ret = 0;
  while (ret == 0) {
    if (Serial.available () > 0) {
      while (Serial.available () > 0 ) {
        dbuf[idx] = Serial.read ();
        Serial.print (dbuf[idx]);
        if (dbuf[idx] == 13 || dbuf[idx] == 10 ) {
          dbuf[idx] = 0;
          ret ++;
          continue;
        }
        if (dbuf[idx] != 8) {
          idx ++;
        } else {
          dbuf[idx] = 0;
        }
        if (idx > 15) idx = 15;
      }
    }
  }
  Serial.printf ("====%s\n", dbuf);
  return dbuf;
}

void setupMenu () {
  String ssid_s;
  String pass_s;
  String node_s;
  int z;
  display.setCursor (0, 0);
  display.clearDisplay();
  display.printf ("***CONFIG MODE***\n");
  display.printf ("Connect setup cable\n");
  display.display ();
  while (true) {
    while (Serial.available ()) {
      Serial.read ();
    }
    Serial.println ("  ");
    Serial.println ("-------------SETUP MENU-------" );
    Serial.println ("Setup SSID press 's'");
    Serial.println ("Setup Password press 'p'");
    Serial.println ("Setup Node press 'n'");
    Serial.println ("Read setup press 'r'");
    Serial.println ("Save and Reboot press 'x'");
    Serial.println ("------------------------------");
    while (!Serial.available());
    char cmd = toupper (Serial.read());
    if (cmd == 'S') {
      Serial.println ("SSID set: <your SSID, 8Chars> and press <enter>");
      ssid_s = readline ();
      if (ssid_s.length() < 1) continue;
      char ebuf[16];
      memset (ebuf, 0 , 16);
      for (z = 0; z < 16; z++) {
        char c = ssid_s.charAt (z);
        if (c != 0) {
          EEPROM.write (z, c);
        } else {
          EEPROM.write (z, 0);
        }
      }
      EEPROM.commit ();
      Serial.printf ("***updated***\n");
    }
    if (cmd == 'P') {
      Serial.println ("Password set: <your password, 8Chars> and press <enter>");
      pass_s = readline();
      if (pass_s.length() < 1) continue;
      char ebuf[16];
      memset (ebuf, 0 , 16);
      for (z = 0; z < 16; z++) {

        char c = pass_s.charAt (z);
        if (c != 0) {
          EEPROM.write (z + 16, c);
        } else {
          EEPROM.write (z + 16, 0);
        }
      }
      EEPROM.commit ();
      Serial.printf ("***updated***\n");
    }
    if (cmd == 'N') {
      Serial.println ("Node number (1-40): <Node number> and press <enter>");
      node_s = readline();
      int n  =  node_s.toInt ();
      if (node_s.toInt () < 1 || node_s.toInt () > 40) {
        Serial.printf ("Error, node number must be 1 to 40\n");
      } else {
        //
        EEPROM.write ( 32, n);
        EEPROM.commit ();
        Serial.printf ("***updated***\n");
      }
    }
    if (cmd == 'R') {
      char ebuf[16];
      memset (ebuf, 0 , 16);
      for (z = 0; z < 16; z++) {
        ebuf[z] = (char)EEPROM.read(z);
      }
      Serial.printf("Current SSID : %s\r\n", ebuf);
      //
      memset (ebuf, 0 , 16);
      for (z = 0; z < 16; z++) {
        ebuf[z] = (char)EEPROM.read(z + 16);
      }
      Serial.printf("Current Password : %s\r\n", ebuf);
      //
      memset (ebuf, 0 , 16);
      int nn = (int)EEPROM.read(32);
      Serial.printf("Current Node Number = %d\r\n", nn);
      Serial.println ("......");
    }
    if (cmd == 'X') {
      Serial.printf ("Restart\n");
      ESP.restart();
    }
  }
}

void readSetup () {
  int z;
  char tbuf [16];
  memset (tbuf, 0, 16);
  for (z = 0; z < 16; z++) {
    char p  = (char)EEPROM.read(z);
    if (p == 0) break;
    tbuf[z] = p;
  }
  sprintf (ssid, "%s", tbuf);
  //
  memset (tbuf, 0, 16);
  for (z = 0; z < 16; z++) {
    char p  = (char)EEPROM.read(z + 16);
    if ( p == 0) break;
    tbuf[z] = p;
  }
  sprintf (password, "%s", tbuf);
  //
  node = (int)EEPROM.read(32);
  Serial.printf ("\r\n\n\n");
  Serial.printf ("SSID:%s\n\r", ssid);
  Serial.printf ("PASS:%s\n\r", password );
  for (z = 0; z < 16; z ++) {
    Serial.printf ("%d,", password[z]);
  }
  Serial.printf ("Node:%d\n\r", node);
}

void setup(void) {
  int z;
  Serial.begin(115200);
  SPIFFS.begin();
  EEPROM.begin (64);
  pinMode(led, OUTPUT);
  pinMode(SW1, OUTPUT);
  pinMode(SW2, OUTPUT);
  pinMode(SW3, OUTPUT);
  pinMode(SW4, OUTPUT);
  pinMode(FLASHKEY, INPUT);
  digitalWrite (SW1, swlev1);
  digitalWrite (SW2, swlev2);
  digitalWrite (SW3, swlev3);
  digitalWrite (SW4, swlev4);
  digitalWrite(led, LOW);
  readSetup();
  ads.begin();
  ads.getAddr_ADS1115(ADS1115_GND_ADDRESS);
  ads.setGain(GAIN_TWOTHIRDS);
  ads.setMode(MODE_CONTIN);
  ads.setRate(RATE_475);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 0);
  display.println("Start waiting");
  display.display();
  if (digitalRead (FLASHKEY) == 0 || node < 1 || node > 40) {
    setupMenu ();
  } else {
    if (node > 1) {
      wificonnect ();
    } else
    {
      wifiaccess ();
    }
    server.on("/data/", HTTP_GET, handleRoot);
    server.on("/", HTTP_OPTIONS, handleOptions);
    server.on("/sw1", HTTP_GET, handleSW1);
    server.on("/sw1", HTTP_OPTIONS, handleOptions);
    server.on("/sw2", HTTP_GET, handleSW2);
    server.on("/sw2", HTTP_OPTIONS, handleOptions);
    server.on("/sw3", HTTP_GET, handleSW3);
    server.on("/sw3", HTTP_OPTIONS, handleOptions);
    server.on("/sw4", HTTP_GET, handleSW4);
    server.on("/sw4", HTTP_OPTIONS, handleOptions);
    server.onNotFound([]() {                              
      if (!handleFileRead(server.uri()))                  
        server.send(404, "text/plain", "404: Not Found"); 
    });
    server.begin();
  }
}

void loop(void) {
  if (node == 1) {
    display.setCursor(0, 48);
    display.printf ("Station:%d\n", WiFi.softAPgetStationNum());
    display.display();
  } else {
    if (WiFi.status() == WL_CONNECTION_LOST || WiFi.status() == WL_DISCONNECTED ) {
      wificonnect();
    } else {
      display.setCursor (0, 56);
      display.printf ("RSSI:%d dB  \n", WiFi.RSSI());
      display.display ();
    }
  }
  server.handleClient();

}
