#include <ArduinoJson.h>

#define dutchBucketTxPin 8
#define dutchBucketRxPin 9
#define ebbFlowTxPin 12
#define ebbFlowRxPin 13

const short dutchBucketNeedWaterPin = 2;
const short ebbFlowNeedWaterPin = 3;

unsigned long previousWaterLevelMillis;
unsigned long waterLevelInterval = 10*1000L;
 
long checkReservoirLevel(short rx, short tx);
void valveHandling(long waterLevel, String reservoirId);
void getWaterLevels();
long microsecondsToCentimeters(long microseconds);

void setup()
{
    pinMode(dutchBucketNeedWaterPin, OUTPUT);
    pinMode(ebbFlowNeedWaterPin, OUTPUT);
    previousWaterLevelMillis = 0;

    Serial.begin(115200);
    while (!Serial) continue;
    Serial.println("Monitorering starter om 15 sekunder");
    delay(5000);
    digitalWrite(dutchBucketNeedWaterPin, LOW);
    digitalWrite(ebbFlowNeedWaterPin, LOW);
}

void loop() {

    if(millis() - previousWaterLevelMillis >= waterLevelInterval)
    {
        getWaterLevels();
    }
}

void getWaterLevels() {
    long dutchBucketReservoirLevel = 0;
    long ebbFlowReservoirLevel = 0;
    dutchBucketReservoirLevel = checkReservoirLevel(dutchBucketRxPin, dutchBucketTxPin);
    delay(200);
    ebbFlowReservoirLevel = checkReservoirLevel(ebbFlowRxPin, ebbFlowTxPin);
    Serial.print("Dutch: ");
    Serial.println(dutchBucketReservoirLevel);
    Serial.print("EbbFlow: ");
    Serial.println(ebbFlowReservoirLevel);
    delay(200);
    valveHandling(dutchBucketReservoirLevel, "DutchBucket");
    delay(200);
    valveHandling(ebbFlowReservoirLevel, "EbbFlow");
    previousWaterLevelMillis += waterLevelInterval;
}

void valveHandling(long waterLevel, String reservoirId) {
    if(reservoirId == "DutchBucket") {
        if(waterLevel < 6.0) {
            digitalWrite(dutchBucketNeedWaterPin, LOW);
        }
        else if(waterLevel > 22.0) {
            digitalWrite(dutchBucketNeedWaterPin, HIGH);
        }
    }
    if(reservoirId == "EbbFlow") {
        if(waterLevel < 6.0) {
            digitalWrite(ebbFlowNeedWaterPin, LOW);
        }
        else if(waterLevel > 22.0) {
            digitalWrite(ebbFlowNeedWaterPin, HIGH);
        }
    }
}

long checkReservoirLevel(short rx, short tx) {
    float distance = 0;
    float duration = 0;
    pinMode(rx, INPUT);
    pinMode(tx, OUTPUT);
    // The sensor is triggered by a falling edge of a HIGH pulse that 
    // is more than 60 microseconds in duration.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:

    digitalWrite(tx, LOW);
    delayMicroseconds(5);
    digitalWrite(tx, HIGH);
    delayMicroseconds(10);
    digitalWrite(tx, LOW);
    
    // Read the signal from the sensor: a HIGH pulse whose
    // duration is the time (in microseconds) from the sending
    // of the ping to the reception of its echo off of an object.
    // If no object detected, fixed pulse width of 35ms is sent
    // by the sensor.
    
    duration = pulseIn(rx, HIGH);
    // Serial.print("Duration: ");
    // Serial.println(duration);

    // Convert the pulse width duration into a distance
    distance = microsecondsToCentimeters(duration);
    return distance;
}