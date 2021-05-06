#include "config.h"

#include <Servo.h>

#include <ESP8266WiFi.h>

#include <ESP8266WebServer.h>

 
const char* ssid = "***";

const char* pass = "**********";


ESP8266WebServer server(80);

Servo myservo;

AdafruitIO_Feed *counter = io.feed("switch");

void setup(void){

  Serial.begin(115200);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");

  }

  io.connect();
    
  Serial.println(".");
  
  counter->onMessage(handleMessage);

  while(io.mqttStatus() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  counter->get();

  server.begin();

  Serial.println("HTTP server started");

  myservo.attach(D5);   // Servo attached to D5 pin

  myservo.write(25);

}
void handleMessage(AdafruitIO_Data *data) {
  
  Serial.print("received <- ");
  Serial.println(data->value());
  int angle =30;
  int state = data->toInt();
  if(state == 0) {
    myservo.attach(D5);
   for(angle = 30; angle < 58; angle++) { 
    myservo.write(angle); 
    delay(15); 
      } 
    myservo.write(40);
    delay(15); 
    myservo.detach();
  }
  else if(state == 1) {
    myservo.attach(D5);
    for(angle = 40; angle > 12; angle--) { 
    myservo.write(angle); 
    delay(15); 
      } 
    myservo.write(20);
    delay(15); 
    myservo.detach();
  }
}
 
void loop(void){

  server.handleClient();
  io.run();

}
