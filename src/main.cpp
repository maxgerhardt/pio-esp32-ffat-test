#include <Arduino.h>
#include <FS.h>
#include <FFat.h>

void setup() {
    Serial.begin(115200);
    bool formatOnFail = false;
    const char* mountPath = "/ffat";
    int maxOpenFiles = 10; 
    const char* partitionLabel = "ffat"; // needs to match used partition table
    if(!FFat.begin(formatOnFail, mountPath, maxOpenFiles, partitionLabel)) {
        Serial.println("Failed to initialize FFat partiton.");
        Serial.println("Did you do 'Upload Filesystem Image' beforehand?");
        while(1) {delay(100);}
    }
    Serial.println("FFat filesystem initialized.");
    Serial.println("Total bytes: " + String(FFat.totalBytes()));
    Serial.println("Used bytes: " + String(FFat.usedBytes()));
    Serial.println("Free bytes: " + String(FFat.freeBytes()));
}

void loop() {
    File testfile = FFat.open("/testfile.txt", "r");
    if(!testfile) {
        Serial.println("Coudln't open testfile.txt!");
        delay(1000);
        return;
    }
    String content = testfile.readString();
    Serial.println("Content of file (" + String(testfile.position()) + " bytes)");
    Serial.println(content);
    testfile.close();
    delay(1000);
}