#include "Adafruit_WS2801.h"
#include "ESPMy.h"
#define topicDeckenleuchte "/wohnzimmer/licht"
#define topicTuer "/wohnzimmer/tuer"
#define tuerPin D3
#define dataPin 4
#define clockPin 5
#define LEDLEN 34
#define SETCOLORLEN 6
#define ConLedPin 2
#define HOME 1

ESPMy myESP;
Adafruit_WS2801 strip = Adafruit_WS2801(LEDLEN, dataPin, clockPin);
unsigned int red = 255;
unsigned int green = 255;
unsigned int blue = 255;
boolean lastStatus = false;
void setup() {
  Serial.begin(115200);
  strip.begin();
  pinMode(ConLedPin, OUTPUT);
  pinMode(tuerPin, INPUT_PULLUP);
  strip.show();
#ifdef HOME
  myESP = ESPMy("FB_OLD", "1 2 3 4 5 6 7 8 0");
  myESP.setConnectionLED(ConLedPin);
  myESP.connect(cb, "192.168.11.10");
#else
  myESP = ESPMy("shp", "shpshpshp");
  myESP.setConnectionLED(ConLedPin);
  myESP.connect(cb, "192.168.1.1");
#endif 

 
  myESP.subscribe(topicDeckenleuchte);
}

void loop() {
  myESP.loop();
  boolean newStatus = digitalRead(tuerPin);
  if(newStatus != lastStatus)
  {
    lastStatus = newStatus;
    Serial.println(newStatus);
    if(newStatus){
       myESP.publish(topicTuer, "1");
    } else {
       myESP.publish(topicTuer, "0");
    }
    delay(100);
   
  }
}


void cb(char* topic, byte* payload, unsigned int length) {
  Serial.println(topic);
  String sTopic(topic);

  if (sTopic.equals(topicDeckenleuchte)) {
    Serial.println("Deckenleuchte!");
    String payloadS;
    if (length == SETCOLORLEN) {
      
      payloadS = (char)payload[0];
      payloadS += (char)payload[1];
      red = strtoul(payloadS.c_str(), NULL, 16);
      
      payloadS = (char)payload[2];
      payloadS += (char)payload[3];
      green = strtoul(payloadS.c_str(), NULL, 16);
      
      payloadS = (char)payload[4];
      payloadS += (char)payload[5];
      blue = strtoul(payloadS.c_str(), NULL, 16);
      setColor(red,  green, blue);
      
    } else if (length == 1) {
      Serial.println("length 1");
      if (payload[0] == '1') {
        setColor(red, green, blue);
      } else if (payload[0] == '0') {
        setColor(0,0,0);
      }
    }


  }

}


void setColor(byte red, byte green, byte blue) {
  int i;
  uint32_t c = Color(red, blue,green );
  for (i = 0; i < LEDLEN; i++) {
    strip.setPixelColor(i, c);
    
  }
  strip.show();
}

/* Helper functions */
// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}
