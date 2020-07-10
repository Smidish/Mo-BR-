#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
const char* SSID = "FRITZ!Box 6490 Cable";
const char* PSK = "24770879063129528315";
const char* MQTT_BROKER = "192.168.178.69";
//yeah go ahead and steal my wifi :P

int LED1 = 5;
int LED2 = 4;
int LED3 = 0;
int LED4 = 2;
int LED5 = 14;
int LED6 = 12;
int LED7 = 13;
int LED8 = 15;
int LED9 = 16;
 
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
 
void setup() {
  //lwvwl leds
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);

    //exercise leds
    pinMode(LED5, OUTPUT);
    pinMode(LED6, OUTPUT);
    pinMode(LED7, OUTPUT);
    pinMode(LED8, OUTPUT);
    pinMode(LED9, OUTPUT);
    Serial.begin(115200);
    setup_wifi();
    client.setServer(MQTT_BROKER, 1883);
    client.setCallback(callback);
}
 
void setup_wifi() {
    delay(10);
    
 
    WiFi.begin(SSID, PSK);
 
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
 
    Serial.println(WiFi.localIP());
}
 
void callback(char* topic, byte* payload, unsigned int length) {

    char msg[length+1];
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
        msg[i] = (char)payload[i];
    }
    Serial.println();
 
    msg[length] = '\0';
    Serial.println(msg);

    //LEDs für die Levelanzeige
    
    resetAll();
    if(strcmp(msg,"0")==0){
      resetAll();
      }
    if(strcmp(msg,"1")==0){
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
      randomExercise();
    }
    if(strcmp(msg,"2")==0){
      randomExercise();
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    }
    if(strcmp(msg,"3")==0){
      randomExercise();
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, LOW);
    }
    if(strcmp(msg,"4")==0){
      randomExercise();
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
    }
}

void resetAll(){
  digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
  digitalWrite(LED5, LOW);
    digitalWrite(LED6, LOW);
    digitalWrite(LED7, LOW);
    digitalWrite(LED8, LOW);
    digitalWrite(LED9, LOW);
  }


void randomExercise(){
    
    long ranNum = random(1, 5);
    switch(ranNum){
      case 1:
      digitalWrite(LED5, HIGH);
      break;
      case 2:
      digitalWrite(LED6, HIGH);
      break;
      case 3:
      digitalWrite(LED7, HIGH);
      break;
      case 4:
      digitalWrite(LED8, HIGH);
      break;
      case 5:
      digitalWrite(LED9, HIGH);
      break;
      }
  }
  
void reconnect() {
    while (!client.connected()) {
        Serial.println("Reconnecting MQTT...");
        if (!client.connect("ESP8266Client")) {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" retrying in 5 seconds");
            delay(5000);
        }
    }
    client.subscribe("/pc");
    Serial.println("MQTT Connected...");
}
 
void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}
