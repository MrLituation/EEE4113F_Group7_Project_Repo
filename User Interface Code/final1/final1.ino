// Second iteration of final
// Completed, but needs integration with some of SD features 

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

// ESP32 Access Point Network Credentials 
const char* ssid = "ESP32";         // WiFi network name
const char* password = "12345678";  // Password (given to user)

IPAddress local_ip(192,168,1,1);    // User input IP address into browser
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

// Start web server
AsyncWebServer server(80);

// Function Declarations 
String getHomePageContent();
String getLibraryPageContent();
String getSystemHealthPageContent();
void handleTakePhotograph();
void handleDownloadImages();
void handleClearStorage();

void setup() {
  // Initialise serial communication
  Serial.begin(115200);

  // Set up WiFi access point
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  // Launch SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("SPIFFS Setup Error");
    return;
  }

  // Define HTTP routes and request handlers
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", getHomePageContent());
  });

  server.on("/library", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", getLibraryPageContent());
  });

  server.on("/system_health", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", getSystemHealthPageContent());
  });

  server.on("/takePhoto", HTTP_GET, [](AsyncWebServerRequest *request){
    handleTakePhotograph();
    request->send(200, "text/plain", "Camera check...");
  });

  server.on("/downloadImages", HTTP_GET, [](AsyncWebServerRequest *request){
    handleDownloadImages();
    request->send(200, "text/plain", "Downloading images...");
  });

  server.on("/clearStorage", HTTP_GET, [](AsyncWebServerRequest *request){
    handleClearStorage();
    request->send(200, "text/plain", "Clearing storage...");
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
    String html = """
        <!DOCTYPE html>
        <html>
        <head>
            <meta name = "viewport" content = "width=device-width, initial-scale = 1.0, user-scalable = no">
            <title>Welcome</title>
            <link rel = "stylesheet" type = "text/css" href = "homeStyle.css">
        </head>
        <body>
            <h1>Welcome</h1>
            <img src = "logo.png" alt = "Logo">
            <p>Select an option below:</p>
            <a class = "download-button" href = "/download">Download Images</a>
            <a class = "sensorpage-button" href = "/sensor">System Health Check</a>
            <a class = "librarypage-button" href = "/library">Image Library</a>
        </body>
        </html>
        """;
    
    return html;
}



String getDownloadPageContent() {
  String html = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    /* Style for heading */
    h2 {
      font-family: Helvetica;
      text-align: center;
      color: 444444;
      margin-top: 20px;
    }

    /* Style for button container */
    .button-container {
      margin-top: 20px;
      text-align: center;
    }

    /* Style for button */
    .button {
      background-color: grey;
      color: white;
      padding: 10px 20px;
      margin: 0 10px;
      border: none;
      border-radius: 4px;
      cursor: pointer;
    }

    /* Style for image container */
    .image-container {
      display: flex;
      flex-wrap: wrap;
      justify-content: space-around;
      margin-top: 20px;
    }

    /* Style for individual image */
    .image-container img {
      width: 30%;
      margin-bottom: 10px;
    }
  </style>
</head>
<body>
  <!-- Heading -->
  <h2>Image Library</h2>

  <!-- Button container -->
  <div class="button-container">
    <button class="button" onclick="downloadImages()">Download Images</button>
    <button class="button" onclick="clearStorage()">Clear Storage</button>
  </div>

  <!-- Add a space -->
  <br>

  <!-- Image container -->
  <div class="image-container">
    <img src="image1">
    <img src="image2">
    <img src="image3">
    <img src="image4">
    <img src="image5">
    <img src="image6">
    <img src="image7">
    <img src="image8">
    <img src="image9">
  </div>

  <!-- JavaScript for button actions -->
  <script>
    function downloadImages() {
      fetch("/downloadImages")
        .then(response => response.text())
        .then(data => {
          alert(data);
        })
        .catch(error => {
          console.error('Error:', error);
        });
    }

    function clearStorage() {
      fetch("/clearStorage")
        .then(response => response.text())
        .then(data => {
          alert(data);
        })
        .catch(error => {
          console.error('Error:', error);
        });
    }
  </script>
</body>   
</html>
)rawliteral";
  
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

String getLibraryPageContent() {
  String html = "<!DOCTYPE html> <html>\n";
  html += "<head><title>Image Library</title></head>\n";
  html += "<body><h1>Image Library</h1>\n";
  // Content for the library page
  html += "</body></html>\n";
  
  return html;
}

String getSystemHealthPageContent() {
  String html = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    // add CSS style
  </style>
</head>
<body>
  <h2>System Health Check</h2>
  <p>Add your system health check details here.</p>
</body>
</html>
)rawliteral";
  
  return html;
}

void handleDownloadImages() {
  // Add SD download feature here at a later stage
}

void handleClearStorage() {
  // Add SD clear feature here
}
