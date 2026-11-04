#include"Animator.h"
#include<ESP8266WiFi.h>
#include<FastLED.h>
#include<WiFiUdp.h>

#define NUM_LEDS 60
#define DATA_PIN 4

CRGB leds[NUM_LEDS];


//  * ENTER YOUR WIFI INFORMATION HERE *
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

const char* ssid1 = "YOUR_NETWORK_1";
const char* password1 = "YOUR_PASSWORD_1";

const char* ssid2 = "YOUR_NETWORK_2";
const char* password2 = "YOUR_PASSWORD_2";

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

WiFiUDP udp;
const unsigned int localUdpPort = 4210;
char incomingPacket[255];

// Instantiate the animator
Animator animator(leds, NUM_LEDS);
//  CONTROLS
int animationIndex = 1;
uint8_t hue = 0;
float speed = 1.0;

void setup() {
  // Fire up serial and connect to wifi
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  scanAndConnect();

  // Setup the LEDs
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  // Start up UDP
  udp.begin(localUdpPort);
}

void scanAndConnect() {
  Serial.println("Scanning WiFi networks...");
  int n = WiFi.scanNetworks();

  bool found1 = false;
  bool found2 = false;

  for (int i = 0; i < n; i++) {
    String foundSSID = WiFi.SSID(i);
    if (foundSSID == ssid1) {
      found1 = true;
    } else if (foundSSID == ssid2) {
      found2 = true;
    }
  }

  if (found1) {
    Serial.println("Connecting to: " + String(ssid1));
    WiFi.begin(ssid1, password1);
  } else if (found2) {
    Serial.println("Connecting to: " + String(ssid2));
    WiFi.begin(ssid2, password2);
  } else {
    Serial.println("Neither network found.");
    return;
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }

  Serial.print("\nConnected! IP: ");
  Serial.println(WiFi.localIP());
}

void handleUDP() {
  // Read the UDP packet
  int packetSize = udp.parsePacket();
  if (packetSize) {
    int len = udp.read(incomingPacket, 254);
    if (len > 0) {
      incomingPacket[len] = '\0';
    }

    // Parse UDP and search for commands
    String msg = String(incomingPacket);

    // FOR PINGING
    /*=================================*/
    if (msg.indexOf("LEDPing:;") != -1) {
      Serial.println("PINGED!");
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      udp.write("PONG:;");
      udp.endPacket();
    }
    /*=================================*/

    //  Commands to control animation speed
    if (msg.indexOf("doubleTime:;") != -1) {
      speed = speed / 2.0;
    }
    if (msg.indexOf("halfTime:;") != -1) {
      speed = speed * 2.0;
    }
    if (msg.indexOf("defaultTime:;") != -1) {
      speed = 1.0;
    }
    
    int animIndex = msg.indexOf("animation:");
    int hueIndex = msg.indexOf("hue:");
    //  Run if animation change command is received
    if (animIndex != -1) {
      animator.reset();
      int semiIndex = msg.indexOf(";", animIndex);
      animationIndex = msg.substring(animIndex + 10, semiIndex).toInt();
    }
    //  Run if hue change command is received
    if (hueIndex != -1) {
      animator.reset();
      int semiIndex = msg.indexOf(";", hueIndex);
      hue = msg.substring(hueIndex + 4, semiIndex).toInt();
    }
  }
}

void loop() {
  handleUDP();
  switch (animationIndex) {
    case 1:
      animator.fastScroll(10 * speed, hue);
      break;
    case 2:
      animator.smoothScroll(10 * speed, hue);
      break;
    case 3:
      animator.smoothScroll(10 * speed, 0, 0);
      break;
    case 4:
      animator.solidStrobe(15 * speed, hue);
      break;
    case 5:
      animator.solidStrobe(15 * speed, 0, 0);
      break;
    case 6:
      animator.strobe(60 * speed, 0, 0);
      break;
  }
  FastLED.show();
}