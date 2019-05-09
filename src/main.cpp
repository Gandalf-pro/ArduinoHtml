#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

//Modem Settings
const char *ssid = "Gunes";
const char *password = "718718718A";

//Server
ESP8266WebServer server(80);
bool relay1 = false;
bool relay2 = false;


//Relays
#define rel1 D3
#define rel2 D4

//seting the routing
void handleGet();
void handlePost();
void setRouting(){
	server.on("/", HTTP_GET, handleGet);
	server.on("/", HTTP_POST, handlePost);
}


void setup()
{
	Serial.begin(96000);
	// Relay settings
	pinMode(rel1, OUTPUT);
	pinMode(rel2, OUTPUT);
	digitalWrite(rel1, HIGH);
	digitalWrite(rel2, HIGH);

	//connecting to wifi
	Serial.print("Connecting to ");
	Serial.println(ssid);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println("Wifi connected.");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());

	//http server
	setRouting();
	server.begin();
	Serial.println("Http server is running");
}

void setRelay1(bool state){
	if(state && !relay1){
		digitalWrite(rel1, LOW);
		relay1 = true;
	}else if(!state && relay1){
		digitalWrite(rel1, HIGH);
		relay1 = false;
	}
}

void setRelay2(bool state){
	if(state && !relay2){
		digitalWrite(rel2, LOW);
		relay2 = true;
	}else if(!state && relay2){
		digitalWrite(rel2, HIGH);
		relay2 = false;
	}
}


void loop()
{
	server.handleClient();
}

void handleGet(){
	String bam;
	bam = "Relay 1:";
	if(relay1){
		bam += "on";
	}else {
		bam += "off";
	}
	bam += "  Relay 2:";
	if(relay2){
		bam += "on";
	}else {
		bam += "off";
	}
	server.send(200, "text/html", bam);
}
	StaticJsonDocument<100> doc;
void handlePost(){
	if(server.hasArg("plain")){
		deserializeJson(doc,server.arg("plain"));
		setRelay1(doc["relay1"]);
		setRelay2(doc["relay2"]);
	}
	doc.clear();
	server.send(200, "text/html", "ok");
}