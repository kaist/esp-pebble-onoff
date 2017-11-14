/*
   Captive Portal by: M. Ray Burnette 20150831
   See Notes tab for original code references and compile requirements
   Sketch uses 300,640 bytes (69%) of program storage space. Maximum is 434,160 bytes.
   Global variables use 50,732 bytes (61%) of dynamic memory, leaving 31,336 bytes for local variables. Maximum is 81,920 bytes.
*/
#include <ESP8266WiFi.h>
#include "./DNSServer.h"                  // Patched lib
#include <ESP8266WebServer.h>
#include <FS.h>


const byte        DNS_PORT = 53;          // Capture DNS requests on port 53
IPAddress         apIP(10, 10, 10, 1);    // Private network for server
DNSServer         dnsServer;              // Create the DNS object
ESP8266WebServer  webServer(80);          // HTTP server



                   
String getContentType(String filename){

  if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  return "text/plain";
}



bool handleFileRead(String path){
  if(path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);
  if (path.endsWith("204"))
  {
     File file = SPIFFS.open("/index.html", "r");
    size_t sent = webServer.streamFile(file, "text/html");
    
    file.close();

    return true;  
  };

 if (path.endsWith("on"))
  {
   digitalWrite(D1, LOW);
   webServer.send(200, "text/html", "ok");
    return true;  
  };
  
 if (path.endsWith("off"))
  {
   digitalWrite(D1, HIGH);
   webServer.send(200, "text/html", "ok");
    return true;  
  };

  if( SPIFFS.exists(path)){
    File file = SPIFFS.open(path, "r");
    size_t sent = webServer.streamFile(file, contentType);
    
    file.close();
    return true;
  }

   File file = SPIFFS.open("/index.html", "r");
    size_t sent = webServer.streamFile(file, "text/html");
    
    file.close();
    return true;
  
}




void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  pinMode(D1, OUTPUT);   
  digitalWrite(D1, HIGH);

  char a[20]={
    "ZalomskijLIGHT"
  };

  WiFi.softAP(a);

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  // replay to all requests with same HTML
  SPIFFS.begin();



  webServer.onNotFound([](){
    handleFileRead(webServer.uri());
      
  });
  webServer.begin();
};
void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
};


