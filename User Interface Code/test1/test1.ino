// First final iteration

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

// ESP32 Access Point Credentials (mention)
const char* ssid = "ESP32";         // WiFi network name
const char* password = "12345678";  // Password

IPAddress local_ip(192,168,1,1);  // IP Address
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

AsyncWebServer server(80);

String cachedLabels;
String cachedData;

// Function Declarations 
String getHomePageContent();
String getDownloadPageContent();
String getCameraPageContent();
String getSensorPageContent();
String getLibraryPageContent();
String getLabels();
String getData();

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  if(!SPIFFS.begin(true)){
    Serial.println("SPIFFS Mount Error"); //
    return;
  }
  
  cachedLabels = getLabels();
  cachedData = getData();
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", getHomePageContent());
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

String getHomePageContent() {
  String html = "<!DOCTYPE html> <html>\n";
  html += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  html += "<title>Welcome</title>\n";
  html += "<style>html { font-family: Helvetica; display: inline-block; margin: 50px auto; text-align: center;}\n"; // Increased margin and centered everything
  html += "body{margin-top: 30px;} h1 {color: #444444;margin: 30px auto 20px; font-size: 30px;}\n"; // Increased margin and font size for h1
  html += "img {width: 120px; margin-bottom: 30px;}\n"; // Increased image width
  html += ".download-button, .button {display: inline-block; width: auto; background-color: #D3D3D3; border: none; color: white; padding: 10px 20px; text-decoration: none; font-size: 20px; margin: 10px; cursor: pointer; border-radius: 4px;}\n"; // Changed button color
  html += ".download-button {background-color: #808080;}\n"; // Changed background color for Download Images button
  html += ".download-button:active, .button:active {background-color: #BEBEBE;}\n"; // Changed active color
  html += ".button {background-color: #9ACD32;}\n"; // Changed background color for System Health Check and Image Library buttons
  html += ".button:active {background-color: #7CFC00;}\n"; // Changed active color
  html += "p {font-size: 16px;color: #888;margin-bottom: 20px;}\n"; // Increased margin-bottom for paragraph
  html += "</style>\n";
  html += "</head>\n";
  html += "<body>\n";
  html += "<h1>Welcome</h1>\n";
  html += "<img src=\"logo.png\" alt=\"Logo\">\n"; // Added image tag
  html += "<p>Select an option below:</p>\n";
  html += "<a class=\"download-button\" href=\"/download\">Download Images</a>\n"; // Changed button class
  html += "<a class=\"button\" href=\"/sensor\">System Health Check</a>\n"; // Changed button class
  html += "<a class=\"button\" href=\"/library\">Image Library</a>\n"; // Changed button class
  html += "</body>\n";
  html += "</html>\n";
  
  return html;
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
  html += "<canvas id=\"myChart\"></canvas>\n";
  html += "<script>";
  html += "var ctx = document.getElementById('myChart').getContext('2d');";
  html += "var myChart = new Chart(ctx, {";
  html += "type: 'line',";
  html += "data: {";
  html += "labels: ["; // Prepare labels
  html += getLabels();
  html += "],";
  html += "datasets: [{";
  html += "label: 'Light Intensity',";
  html += "data: ["; // Prepare data
  html += getData();
  html += "],";
  html += "backgroundColor: 'rgba(255, 99, 132, 0.2)',";
  html += "borderColor: 'rgba(255, 99, 132, 1)',";
  html += "borderWidth: 1";
  html += "}]";
  html += "},";
  html += "options: {";
  html += "scales: {";
  html += "y: {";
  html += "beginAtZero: true";
  html += "}";
  html += "}";
  html += "}";
  html += "});";
  html += "</script>\n";
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

String getLabels() {
  String labels;
  File file = SPIFFS.open("/testData.txt");
  if(file) {
    while(file.available()) {
      String line = file.readStringUntil('\n');
      if (line.startsWith("Light Intensity")) {
        labels += "\"" + line.substring(line.lastIndexOf("=") + 2) + "\",";
      }
    }
    file.close();
  }
  return labels.substring(0, labels.length() - 1); // Remove trailing comma
}

String getData() {
  String data;
  File file = SPIFFS.open("/testData.txt");
  if(file) {
    while(file.available()) {
      String line = file.readStringUntil('\n');
      if (line.startsWith("Light Intensity")) {
        data += line.substring(line.lastIndexOf("=") + 2) + ",";
      }
    }
    file.close();
  }
  return data.substring(0, data.length() - 1); // Remove trailing comma
}
