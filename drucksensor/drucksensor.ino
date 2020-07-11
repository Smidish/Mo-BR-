#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* SSID = "FRITZ!Box 6490 Cable";
const char* PSK = "24770879063129528315";
const char* MQTT_BROKER = "192.168.178.69";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

//Drucksensor
const int FSR_PIN = A0;
int val = 0;
const char* sittingLevel = "0";


//Zeitmessung
unsigned long CurrentTime;
unsigned long ElapsedTime;
unsigned long StartTime;

void setup(){
   Serial.begin(115200);
    setup_wifi();
    client.setServer(MQTT_BROKER, 1883);
    StartTime = millis();
    pinMode(FSR_PIN, INPUT);
}

void loop(){

   //Drucksensor auslesen
   int val = analogRead(FSR_PIN);
   //Serial.println(val);
   CurrentTime = millis();
      ElapsedTime = CurrentTime - StartTime;
   //Feststellen, wie lange Person sitzt / welches Pausenlevel erreicht ist
   if(val>1000){
      
      Serial.println("time: ");
       Serial.println(ElapsedTime);

      //weniger als 20 Minuten (120000 milis)
      if(ElapsedTime>1200000){
        sittingLevel="1";
      }
      //30min
      else if(ElapsedTime>1800000){
        sittingLevel="2";
      }
      //45min
      else if(ElapsedTime>2700000){
        sittingLevel="3";
      }
      //60min
      else if(ElapsedTime>3600000){
        sittingLevel="4";
      }  
      
   }
   else{
        //actually I should connect and read a message her from the MQTT fml
        sittingLevel = "0";
      }
   
   if(sittingLevel != 0&& val<1){
    resetPressure();
  }
   
   

   //send message to Brooker
   if (!client.connected()) {
        reconnect();
    }
    client.loop();

    //some logs to see if shit is working
    //snprintf (msg, 50, "Alive since %ld milliseconds", millis());
    //Serial.print("Publish message: ");
    //Serial.println(sittingLevel);
    
    //publish message on MQTT
    client.publish("/pc", sittingLevel);
    
    //reicht aus, ab und zu zu checken, ob noch Druck drauf ist (5s for a start?)
    delay(5000);
}

void resetPressure(){
    sittingLevel = 0;
    CurrentTime = 0;
    ElapsedTime = 0;
    StartTime = millis();
  }


//MQTT & WiFi Stuff

void setup_wifi() {
    delay(10);
    //Serial.println();
    //Serial.print("Connecting to ");
    //Serial.println(SSID);
 
    WiFi.begin(SSID, PSK);
 
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        //Serial.print(".");
    }
 
    //Serial.println("");
    //Serial.println("WiFi connected");
    //Serial.println("IP address: ");
    //Serial.println(WiFi.localIP());
}
 
void reconnect() {
    while (!client.connected()) {
        //Serial.print("Reconnecting...");
        if (!client.connect("ESP8266Client")) {
            //Serial.print("failed, rc=");
            //Serial.print(client.state());
           // Serial.println(" retrying in 5 seconds");
            delay(5000);
        }
    }
}
