// Code to display images in photo library on web application [second iteration]
// Using collected images

#ifdef ESP32
  #include <WiFi.h>
  #include <ESPAsyncWebServer.h>
  #include <SPIFFS.h>
#else
  #include <Arduino.h>
  #include <ESP8266WiFi.h>
  #include <Hash.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #include "FS.h"
#endif

const char* ssid = "LANcasterYfi";  // Enter SSID here
const char* password = "";  

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
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
    // Set action buttons
    <button class="button" onclick="button1Clicked()">Download Images</button>
    <button class="button" onclick="button2Clicked()">Clear Storage</button>
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
    function button1Clicked() {
      // Insert download image function
      alert("Download Images button clicked");
    }

    function button2Clicked() {
      // Insert clear SD card (at a later stage)
      alert("Clear Storage button clicked");
    }
  </script>
</body>   

</html>)rawliteral";


void setup(){
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  if(!SPIFFS.begin()){
        Serial.println("SPIFFS Error. Rerun.");
        return;
  }

  // Prints ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Server requests for images
  // As if images already saved in file system
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/image1", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/image1.jpg", "image/jpeg");
  });

  server.on("/image2", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/image2.jpg", "image/jpeg");
  });

  server.on("/image3", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/image3.jpg", "image/jpeg");
  });

  server.on("/image4", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/image4.jpg", "image/jpeg");
  });

  server.on("/image5", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/image5.jpg", "image/jpeg");
  });

  server.on("/image6", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/image6.jpg", "image/jpeg");
  });

  server.on("/image7", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/image7.jpg", "image/jpeg");
  });

  server.on("/image8", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/image8.jpg", "image/jpeg");
  });

  server.on("/image9", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/image9.jpg", "image/jpeg");
  });

  // Start server
  server.begin();
}
 
void loop(){
  
}
