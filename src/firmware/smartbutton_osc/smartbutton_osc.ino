


#include <ArduinoOSC.h>

WiFiUDP udp;
ArduinoOSCWiFi osc;
const char* ssid = "Bache";
const char* pwd = "xxxxx";
const IPAddress ip(192, 168, 10, 201);
const IPAddress gateway(192, 168, 1, 1);
const IPAddress subnet(255, 255, 255, 0);
const char* host = "192.168.10.174";
const int recv_port = 10000;
const int send_port = 12000;

int buttonPin = 15;
int buttonLedPin = 4;
int buttonState = 0;
int newButtonState = 0;

void setup()
{
  Serial.begin(115200);
  WiFi.disconnect(true);
  WiFi.onEvent(onWiFiEvent);
  WiFi.begin(ssid, pwd);
  WiFi.config(ip, gateway, subnet);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  osc.begin(udp, recv_port);
  osc.addCallback("/button/led/state", &callback);

  digitalWrite(buttonPin, LOW);
  pinMode(buttonPin, INPUT);
  buttonState = digitalRead(buttonPin);

  pinMode( buttonLedPin, OUTPUT);
}

void loop()
{

  newButtonState = digitalRead(buttonPin);
  if ( newButtonState != buttonState ) {
    buttonState = newButtonState;
    OSCMessage msg;
    msg.beginMessage(host, send_port);
    msg.setOSCAddress("/button/state");
    msg.addArgInt32(buttonState);
    osc.send(msg);
    // osc.parse();
    Serial.println( buttonState );
    // delay(10);
  }



  //delay(10);

  osc.parse();
}

void callback(OSCMessage& m)
{
  digitalWrite( buttonLedPin, m.getArgAsInt32(0) );

}

void onWiFiEvent(WiFiEvent_t event)
{
  switch (event)
  {
    case SYSTEM_EVENT_STA_START:
      {
        Serial.println("STA Started");
        break;
      }
    case SYSTEM_EVENT_STA_CONNECTED:
      {
        Serial.println("STA Connected");
        break;
      }
    case SYSTEM_EVENT_STA_GOT_IP:
      {
        Serial.print("STA IPv4: ");
        Serial.println(WiFi.localIP());
        break;
      }
    case SYSTEM_EVENT_STA_DISCONNECTED:
      {
        Serial.println("STA Lost Connection");
        break;
      }
    case SYSTEM_EVENT_STA_STOP:
      {
        Serial.println("STA Stopped");
        break;
      }
    default:
      {
        break;
      }
  }
}
