#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <WebSocketsServer.h>

const char *ssid = "MyAP";
const char *password = "password";

AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void handleRoot(AsyncWebServerRequest *request) {
  StaticJsonDocument<200> doc;
  doc["message"] = "bienvenido a esp32";
  String output;
  serializeJson(doc, output);
  request->send(200, "application/json", output);
}

void handleApi(AsyncWebServerRequest *request) {
  StaticJsonDocument<200> doc;
  doc["num1"] = random(100);
  doc["num2"] = random(100);
  doc["num3"] = random(100);
  String output;
  serializeJson(doc, output);
  request->send(200, "application/json", output);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {
        Serial.printf("[%u] Connected from url: %s\n", num, payload);
        // send message to client
        webSocket.sendTXT(num, "Connected");
      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] Message: %s\n", num, payload);
      break;
    case WStype_BIN:
      Serial.printf("[%u] Message [BINARY]: %u bytes\n", num, length);
      break;
  }
}


void setup() {
  WiFi.softAP(ssid, password);
  server.on("/", HTTP_GET, handleRoot);
  server.on("/api", HTTP_GET, handleApi);
  server.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("Webserver Started!");
}

void loop() {
  webSocket.loop();

  static unsigned long last_time = 0;
  if (millis() - last_time >= 1000) {
    last_time = millis();
    int number1 = random(50,100);
    int number2 = random(50,100);
    int number3 = random(50,100);
    int number4 = random(50,100);

    String message = "{\"number1\": " + String(number1) + ",\"number2\": " + String(number2) + ",\"number3\": " + String(number3) + "}";
    String longMessage= "{\"sections\":{\"a\":{\"a1\":"+String(number1)+",\"a2\":"+String(number2)+",\"a3\":"+String(number3)+",\"a4\":"+String(number4)+"},\"b\":{\"b1\":"+String(number1)+",\"b2\":"+String(number2)+",\"b3\":"+String(number3)+",\"b4\":"+String(number4)+"},\"c\":{\"c1\":"+String(number1)+",\"c2\":"+String(number2)+",\"c3\":"+String(number3)+",\"c4\":"+String(number4)+"}}}";

    webSocket.broadcastTXT(longMessage);
  }
}



