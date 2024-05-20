// Code for image library [thrid iteration]

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
const char* password = ""; // quicker for testing

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
      color: #444444;
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
      width: 100%; /* Increase image width */
      margin-bottom: 10px;
    }

    /* Style for field container */
    .field-container {
      text-align: center;
      margin-top: 20px;
    }

    /* Style for field label */
    .field-label {
      font-family: Helvetica;
      color: #444444;
    }

    /* Style for field value */
    .field-value {
      font-family: Helvetica;
      color: #000000;
      font-weight: bold;
    }

    /* Style for subtext */
    .subtext {
      font-family: Helvetica;
      color: #888888;
      font-size: 12px;
    }
  </style>
</head>
<body>
  <!-- Heading -->
  <h2>System Health Check</h2>

  <!-- Button container -->
  <div class="button-container">
    <button class="button" onclick="takePhoto()">Take Photograph</button>
  </div>

  <!-- Image container -->
  <div class="image-container">
    <img src="image1">
  </div>

  <!-- Field container -->
  <div class="field-container">
    <span class="field-label">Battery Power:</span>
    <span class="field-value" id="battery-power">78</span>
    <span class="subtext">%</span>
  </div>

  <!-- JavaScript for button actions -->
  <script>
    function takePhoto() {
      // Implement the action for taking a photograph here
      alert("Take Photograph button clicked");
    }
  </script>
</body>   

</html>)rawliteral";


void setup(){
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  if(!SPIFFS.begin()){
        Serial.println("SPIFFS Error");
        return;
  }

  Serial.println(WiFi.localIP());

  // server requests 
  // testing on non-static images - works?
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/image1", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/image1.jpg", "image/jpeg");
  });

  // Start server
  server.begin();
}
 
void loop(){
  
}
