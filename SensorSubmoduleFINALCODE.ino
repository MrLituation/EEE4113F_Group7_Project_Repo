#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>
#include "Arducam_Mega.h"

// ESP-wroom-32 MASTER VSPI pins 
#define PIN_MISO 19
#define PIN_MOSI 23
#define PIN_SCK 18
// VSPI pins for SD chip select
#define PIN_SS_SD 5
// VSPI pins for camera chip select
#define PIN_SS_CAM1 26  // Arducam as SPI slave
#define PIN_SS_CAM2 27  // ESP32-CAM as SPI slave 

#define MOTION_SENSOR_PIN 2  // HC-SR505 pin, GPIO pin 2 has a pull down resistor attached to it in schematic so maybe try GPIO 4 if noisy
#define LIGHT_SENSOR_PIN 0

// Instance of SPI class 
SPIClass vspi(VSPI);
// Instance of Arducam_Mega class with its CS pin
Arducam_Mega myCAM(PIN_SS_CAM1);  
// Instance of RTC_DS3231 class creation
RTC_DS3231 rtc;

void setup() {
  Serial.begin(115200);
  pinMode(MOTION_SENSOR_PIN, INPUT);
  pinMode(LIGHT_SENSOR_PIN, INPUT);

  // Initialize the VSPI for cameras and SD card
  vspi.begin(PIN_SCK, PIN_MISO, PIN_MOSI);

  // CAM initialization
  myCAM.begin();  


  pinMode(PIN_SS_SD, OUTPUT);
  pinMode(PIN_SS_CAM1, OUTPUT);
  pinMode(PIN_SS_CAM2, OUTPUT);

  

  if (!SD.begin(PIN_SS_SD, vspi)) {
    Serial.println("Card Mount Failed");
    return;
  }
  Serial.println("SD Card is successfully initialized.");

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    return;
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, time is not set!");
    //Date and time of RTC set to the date and time at which the code was compiled.
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  
  }

  Serial.println("RTC initialized.");
}

void loop() {
  //Code only runs should motion be detected
  if (digitalRead(MOTION_SENSOR_PIN) == HIGH) {
    delay(5S0);  // Debounce

    //Identification to tell if ArduCam or ESPCam was used
    String cameraID = "";
    // Get the current date and time
    DateTime now = rtc.now();
    //
    char filename[32];
    //The file was saved with a filename that 
    //includes real time second in order to avoid 
    //having the same filename for different images
    sprintf(filename, "/image_%04d%02d%02d_%02d%02d%02d.jpg", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
    File imgFile = SD.open(filename, FILE_WRITE);

    if (digitalRead(LIGHT_SENSOR_PIN) == HIGH) {
      Serial.println("Capturing Image with Arducam...");
      //ArduCam Mega take picture function just needs required image 
      //resolution and the preferred format as inputs
      myCAM.takePicture(CAM_IMAGE_MODE_WQXGA2, CAM_IMAGE_PIX_FMT_JPG);
      cameraID = "Arducam";

      if (!imgFile) {
        Serial.println("Failed to open file for writing");
        return;
      }

      // Loop until all image bytes are received
      while (myCAM.getReceivedLength()) {  
      // Read image data from camera and write to SD card
        uint8_t data = myCAM.readByte();
        imgFile.write(data);
      }

      imgFile.close();
      Serial.println("\nImage saved to SD card as " + String(filename));
      Serial.println("Captured using " + cameraID);
    } 
    //Case if light sensor pin is low indicating NIGHTTIME
    else {
      Serial.println("Capturing Image with ESP-Cam...");
      cameraID = "ESP-Cam";

      // Simulation code for SPI communication with ESP32-CAM
      // Assuming a fixed image size which is not the case IRL
      for (uint32_t i = 0; i < 70000; i++) {  
        // Read a byte from the ESP32-CAM (placeholder)
        byte data = vspi.transfer(0x00);      
        imgFile.write(data);
      }

      imgFile.close();
      Serial.println("\nImage received and saved to SD card as " + String(filename));
      Serial.println("Captured using " + cameraID);
    }
  }
}

