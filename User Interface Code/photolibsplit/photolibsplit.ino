// Code for photo library testing a split approach with an external html file and integrated wifi access point
// Didn't end up using - preferred integrated html within arduino code

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

// Access point established with set details (mention set password - possibly change?)
const char* ssid = "ESP32";         // SSID - WiFi network name
const char* password = "12345678";  // Password

// Put set IP Address details 
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

AsyncWebServer server(80);

String cachedLabels;
String cachedData;

// Function Declarations 
String getDownloadPageContent();
String getCameraPageContent();
String getSensorPageContent();
String getLibraryPageContent();

void setup() {
    Serial.begin(115200);

    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    delay(100);

    if(!SPIFFS.begin(true)){
        Serial.println("SPIFFS Error");
        return;
    }

    // Reads cache data and stores it
    cachedLabels = "Example Label";
    cachedData = "Example Data";

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/webpage.html", String(), false);
    });

    server.on("/download", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", getDownloadPageContent());
    });

    server.on("/camera", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", getCameraPageContent());
    });

    server.on("/sensor", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", getSensorPageContent());
    });

    server.on("/library", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", getLibraryPageContent());
    });

    server.onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "text/plain", "Not found");
    });

    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
}

String getDownloadPageContent() {
    String html = "<!DOCTYPE html> <html>\n";
    html += "<head><title>Download Images</title></head>\n";
    html += "<body><h1>Download Images</h1>\n";
    // Content for the download page
    html += "</body></html>\n";

    return html;
}

String getCameraPageContent() {
    String html = "<!DOCTYPE html> <html>\n";
    html += "<head><title>Camera Check</title></head>\n";
    html += "<body><h1>Camera Check</h1>\n";
    // Content for the camera page
    html += "</body></html>\n";

    return html;
}

String getSensorPageContent() {
    String html = "<!DOCTYPE html> <html>\n";
    html += "<head><title>Sensor Statistics</title></head>\n";
    html += "<body><h1>Sensor Statistics</h1>\n";
    // Content for the sensor page
    html += "</body></html>\n";

    return html;
}

String getLibraryPageContent() {
    String html = "<!DOCTYPE html> <html>\n";
    html += "<head><title>Image Library</title></head>\n";
    html += "<body><h1>Image Library</h1>\n";
    // Content for the library page
    html += "</body></html>\n";

    return html;
}
