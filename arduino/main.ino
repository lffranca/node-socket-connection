#include <WiFi.h>

#define SOCKET_TIMEOUT 10
#define TICK_TIMEOUT   100

const char *ssid = "my_ssid";
IPAddress apIP(192, 168, 1, 1);

WiFiServer myServer(2000);
WiFiClient myClient;
unsigned char clientConnected = 0;

void connectWifi() {
  WiFi.disconnect();
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00
  WiFi.softAP(ssid, NULL, 6, 0); //, 1); // no pass, ch=6, ssid=broadcast, max_conns=1
}

void setup() {
  connectWifi();
  myServer.begin();
  Serial.begin(115200);
}

unsigned int cnt=0;
void serverPoll() {
  //
  // Test for new connection
  //
  if (!myClient.connected()) {
    myClient = myServer.available();
    if (myClient.connected()) {
      Serial.write("Connect...");
      cnt=0;
      clientConnected = 1;
      myClient.setNoDelay(true);
      myClient.setTimeout(SOCKET_TIMEOUT);
    }
  }

  if (myClient.connected()) {
    while (myClient.available()) {  // get data from Client
      Serial.write(myClient.read());
    }

    // Write
    char buf[64];
    sprintf(buf, "Hello World %u\r\n", cnt++);
    int len = strlen(buf);

    int rc;
    int bufp = 0;
    {
      rc = myClient.write(buf+bufp, len-bufp);
      if (rc >= 0) {
        bufp += rc;
        if (len > bufp) {
          sprintf(buf, "write() retry rc=%d bufp=%d len=%d\r\n", rc, bufp, len);
          Serial.write(buf);
        }
      } else {
        sprintf(buf, "write() error returned %d\r\n", rc);
        Serial.write(buf);
      }
    } while((len > bufp) && (rc >= 0));

    if (len > bufp) {
      sprintf(buf, "myClient.write(\"Hello World %d\") returned %d\r\n", cnt, rc);
    }

    Serial.write(buf);
  } else {
    myClient.stop();
    if (clientConnected) {
      Serial.write("Disconnect...");
      clientConnected = 0;
    }
  }
}

void loop() {
  static unsigned int last = 0;
  static unsigned int delta = TICK_TIMEOUT;
  unsigned int now = millis();
  unsigned int diff = now - last;
  unsigned int tick = (diff >= delta);
  if (tick) {
    delta = TICK_TIMEOUT;
    last = now;
    serverPoll();
  }
}