#include <Arduino.h> // biblioteca nativa (plataformIO) para utilizar o framework do arduíno
#include <PubSubClient.h> // biblioteca nativa (plataformIO) para utilização do mqtt na ESP32
#include <WiFi.h> // // biblioteca nativa (plataformIO) para utilização do WiFi

const char* ssid = "WIFI_MESH_IST";
const char* pass = "ac1ce0ss6_mesh";
const char* brokerUser = "Default";
const char* brokerPass = "7bbecaaf-b03f-43ad-b8c7-137a20587c5b";
const char* broker = "mqtt.tago.io";
const char* outTopic = "esp32";

WiFiClient espClient;
PubSubClient client(espClient);

long currentTime, lastTime;
int count =0;
char messages[50];

void setupWifi(){
    delay(100);
    Serial.print("\nConectando...");
    Serial.println(ssid);

    WiFi.begin(ssid, pass);

    while ((WiFi.status() !=WL_CONNECTED)){
        delay(100);
        Serial.print("-");
    }

    Serial.print("\nSUCESSO - Conectado em:");
    Serial.println(ssid);    
}

void reconnect(){
    while(!client.connected()){
        Serial.print("\nConectando... em");
        Serial.println(broker);
        if(client.connect("", brokerUser, brokerPass)){
            Serial.print("\nSUCESSO, conectado em:");
            Serial.println(broker);
        }
        else {
            Serial.println("Tentando conectar novamente");
            delay(5000);
        }
    }
}

void setup(){

    Serial.begin(115200);
    setupWifi();
    client.setServer(broker, 1883);
}

void loop(){
    if (!client.connected()){
        reconnect();
    }
    client.loop();

    currentTime = millis();
    if(currentTime - lastTime > 2000){
        count++;
        snprintf(messages, 75, "Count: %d", count);
        Serial.print("Sending messages: ");
        Serial.println(messages);
        client.publish(outTopic, messages);
        lastTime = millis();

    }
}