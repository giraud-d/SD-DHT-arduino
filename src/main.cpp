#include <DS3231.h>
#include <Wire.h>
#include <DHT.h>
#include <SD.h>

#define DHTPIN 7
#define DHTTYPE DHT11  // or DHT22  (AM2302)

DHT dht(DHTPIN, DHTTYPE); /// Initialize DHT sensor for normal 16mhz Arduino
RTClib myRTC;
DateTime now;
File myFile;
bool sdError = true;

void initSDCard() {
    Serial.print("Initializing SD card...");
    SD.end();
    if (SD.begin(53, 51, 52, 50)) {
        Serial.println("initialization done!");
        sdError = false;
    } else {
        Serial.println("initialization failed.");
    }
}

void setup() {
    Serial.begin(9600);
    dht.begin();
    Wire.begin();
    initSDCard();
}

void loop() {
    if (sdError)
        initSDCard();
    else {
        myFile = SD.open("timestamp-humidite-temperature.csv", O_APPEND | FILE_WRITE);
        if (myFile) {
            if (myFile.print(RTClib::now().unixtime()) > 0) {
                myFile.print(';');
                myFile.print(dht.readHumidity());
                myFile.print(';');
                myFile.println(dht.readTemperature());
            } else
                sdError = true;
            myFile.close();;
        } else
            sdError = true;
    }
    delay(10000);
}