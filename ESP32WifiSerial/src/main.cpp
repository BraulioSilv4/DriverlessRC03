#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <HardwareSerial.h>

#define RXD1 16
#define TXD1 17

HardwareSerial SerialPort(2);

const char* ssid = "NOS-F5C4";
const char* password = "MGRYFHFQ";

const int ledPin = 2;
const int ESP32Baud = 115200;

enum CommandType {
    SPEED,
    TURN,
    MODE,
    ERROR
};

AsyncWebServer server(80);
AsyncWebSocket ws("/RC03");

CommandType getCommandType(const char* type) {
    if (strcmp(type, "speed") == 0) {
        return SPEED;
    }

    if (strcmp(type, "turn") == 0) {
        return TURN;
    }

    if (strcmp(type, "mode") == 0) {
        return MODE;
    }

    return ERROR;
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_BINARY) {
        uint8_t commandType = data[0];

        int value = data[1];


        switch (commandType) {
            case SPEED:
                SerialPort.printf("S%d\n", value);
                Serial.printf("Speed: %d\n", value);
                break;
            case TURN:
                SerialPort.printf("T%d\n", value);
                Serial.printf("Turn: %d\n", value);
                break;
            case MODE:
                if (value == 0) {
                    SerialPort.printf("M0\n");
                    Serial.printf("Mode: 0\n");
                } else if (value >= 1) {
                    SerialPort.printf("M1\n");
                    Serial.printf("Mode: 1\n");
                } 
                break;
            default:
                Serial.printf("Unknown command type: %d\n", commandType);
                break;
        }
    }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void setup() {
    Serial.begin(ESP32Baud);
    pinMode(ledPin, OUTPUT);

    SerialPort.begin(9600, SERIAL_8N1, RXD1, TXD1);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        digitalWrite(ledPin, HIGH);
    }

    digitalWrite(ledPin, LOW);

    Serial.printf("Current URL Address: ws://%s%s\n", WiFi.localIP().toString().c_str(), ws.url());

    initWebSocket();

    server.begin();
}

void loop() {
    ws.cleanupClients();
}