#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char *ssid = "Your_SSID";
const char *password = "Your_PASSWORD";

const int ledPin = 2; // Change to your desired GPIO pin
const int photoresistorPin = 34; // Change to your desired ADC pin

int ledBrightness = 0;
int photoresistorValue = 0;

AsyncWebServer server(80);

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(photoresistorPin, INPUT);

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<html><body>";
    html += "<h1>LED Controller</h1>";
    html += "<p>LED Brightness: " + String(ledBrightness) + "</p>";
    html += "<input type='range' min='0' max='255' value='" + String(ledBrightness) + "' id='ledSlider' onchange='changeLED(this.value)'>";
    html += "<p>Photoresistor Value: " + String(photoresistorValue) + "</p>";
    html += "</body><script>function changeLED(value) { fetch('/led?brightness=' + value); }</script></html>";
    request->send(200, "text/html", html);
  });

  server.on("/led", HTTP_GET, [](AsyncWebServerRequest *request){
    String brightnessStr = request->arg("brightness");
    ledBrightness = brightnessStr.toInt();
    analogWrite(ledPin, ledBrightness);
    request->send(200, "text/plain", "LED brightness set to " + brightnessStr);
  });

  server.on("/photoresistor", HTTP_GET, [](AsyncWebServerRequest *request){
    photoresistorValue = analogRead(photoresistorPin);
    request->send(200, "text/plain", String(photoresistorValue));
  });

  server.begin();
}

void loop() {
  // Add any other tasks or logic here as needed
}