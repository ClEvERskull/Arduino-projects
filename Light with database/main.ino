#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>

WebServer server(80);

const char* ssid = "";
const char* password = "";

const int pin = 32;
const int frequency = 5000;
const int channel = 4;
const int resolution = 8;

String lastPWMvalue="";
String svieti="";

String SendHTML(){
  String ptr = "<!DOCTYPE html><html>\n";
  ptr +="<head><title>LED</title>\n";
  ptr +="<style> body{box-sizing: border-box;margin: 0; padding: 0;height: 1080px;background-image:linear-gradient(to bottom, white,#3399ff);}\n";
  ptr +="h1{text-align:center;font-family:'Calibri',sans-serif; color:#3399ff;font-size: 30px;}\n";
  ptr +=".input-area{display:flex;flex-direction:column; justify-content: center;align-items: center;margin-top: 20px;}\n";
  ptr +="label,p{font-weight: bold;font-size: 20px; font-family:'Calibri',sans-serif;}\n";
  ptr +=".input-area input{ width: 90px;height: 35px;border-radius: 50px;margin: 10px;padding-left:10px;font-size: 20px;}</style>\n";
  ptr +="</head>\n";
  ptr +="<body><h1>Zmena stavu LED</h1>\n";
  ptr +="<center><form action='/test' method='get'>\n";
  ptr +="<div class='input-area'>";
  ptr +="<input type='submit' value='Zmenit stav'>\n";
  ptr +="</form><center>\n";
  ptr +="</div>";

  WiFiClient tcpclient;
  String hodnota;
  if (tcpclient.connect("192.168.0.106", 80)){
    String request="GET /manager.php?func=insert HTTP/1.1";
    tcpclient.println(request);
    Serial.println(request);
    tcpclient.println("Host: 192.168.0.106");
    tcpclient.println("Connection: closed");
    tcpclient.println();
    while (tcpclient.connected() || tcpclient.available()) {
      if (tcpclient.available()){
        String vysledok = tcpclient.readStringUntil('\n');
        Serial.println(vysledok);
        strcpy(hodnota, vysledok);
      }
    }
  tcpclient.stop();
  }

  ptr +="<p>Aktualna hodnota LED: "+hodnota+"</p>";
  ptr +="</body></html>\n\n";
  return ptr;
}

String test(){
  for (int i = 0; i < server.args(); i++) {
    svieti += "Hodnota: " + (String)i + ": "; 
    svieti += server.argName(i) + "=";
    svieti = server.arg(i);
  }
  return message;
}





void handle_Test_GET() {server.send(200, "text/plain",test());}

void handle_OnConnect() {server.send(200, "text/html", SendHTML());}

void handle_NotFound() {
  String message = "Page Not Found\n\n";
  server.send(404, "text/plain", message);
}

void setup(){ 
  
Serial.begin(115200);
  /*WiFi*/
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");  
  }
  Serial.println("IP siete: ");
  Serial.print(WiFi.localIP());

  /*Server*/
  server.onNotFound(handle_NotFound);
  server.on("/", handle_OnConnect);
  server.on("/test", HTTP_GET, handle_Test_GET);
  server.begin();
}



void loop() {
  server.handleClient();
}