// Test code for file server information display
// Able to display information from the file server to the web page

#include "SPIFFS.h"
 
void setup() {

  // Set baud rate
  Serial.begin(115200);
  
  if(!SPIFFS.begin(true)){
    Serial.println("SPIFFS mount error.");
    return;
  }
  
  File file = SPIFFS.open("/testData.txt");
  if(!file){
    Serial.println("Failed to open file for reading.");
    return;
  }
  
  Serial.println("File Content:");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}
 
void loop() {

}
