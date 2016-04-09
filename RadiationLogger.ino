#include <EEPROM.h>
#include <SPI.h>
#include <SD.h>

File logFile;

const int indexAddr = 0;
const long threshold = 1500;
const int sensorPin = A0;
const int pauseA = 10;
const int pauseB = 2;
const int pauseC = 300;
const int samples = 5;

long timer = 0;

void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  Serial.print("Initializing SD card...");

  if (!SD.begin(8)) {
    Serial.println("initialization failed!");
    return;
  }

  Serial.println("initialization done.");


  // Get new file index
  byte fileIndex = EEPROM.read(indexAddr);
  fileIndex++;
  EEPROM.write(indexAddr, fileIndex);

  // Caclulate new filename
  String fileName = "data";
  fileName.concat(fileIndex);
  fileName.concat(".txt");

  // Open file
  logFile = SD.open(fileName, FILE_WRITE);

  // if the file opened okay, write to it:
  if (logFile) {
    Serial.print("opening " + fileName + " to write new data");
    logFile.println("testing 1, 2, 3.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening " + fileName);
    while (true);;
  }
}


void loop() {
  if (analogRead(sensorPin) > threshold) {
    timer = micros() + pauseC;
    delayMicroseconds(pauseA);
    long total = 0;
    for (byte i = 0; i < samples ; i++) {
      total += analogRead(sensorPin);
      delayMicroseconds(pauseB);
    }
    double avg = ((double) total) / ((double) samples);
    recordData(avg);
    while(micros() < timer);;
  }
}

void recordData(double mag) {

  logFile.print(millis());
  logFile.print(":");
  logFile.print(mag);
  logFile.println();

  Serial.print("detected value: ");
  Serial.println(mag);

}




