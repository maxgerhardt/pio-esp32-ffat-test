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


void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void open_testfiletxt() {
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
}

void loop() {
    listDir(FFat, "/", 3 /* max depth */);
    open_testfiletxt();
    delay(1000);
}