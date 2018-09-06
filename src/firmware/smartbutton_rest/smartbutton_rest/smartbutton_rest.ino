#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char *ssid = "Bache";
const char *password = "xxxxxx";

WebServer server(80);
const int led = 4;

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/on", handleLedOn);
  server.on("/off", handleLedOff);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void handleRoot() {
  server.send(200, "text/html", "Hello");
}

void handleLedOff() {
  digitalWrite(led, 0);
  server.send(200, "text/html", "LED turned OFF");
}

void handleLedOn() {
  digitalWrite(led, 1);
  server.send(200, "text/html", "LED turned ON");
}

void handleNotFound() {
  server.send(404, "text/plain", "NOT FOUND");
}

void loop(void) {
  server.handleClient();
}
