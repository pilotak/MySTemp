#include <Wire.h>
#include <MySensors_NodeManager.h>

#if defined(REPORT_BATTERY_LEVEL)
    #include <sensors/SensorBattery.h>
    SensorBattery battery;
#endif

#include <sensors/SensorMCP9808.h>
SensorMCP9808 mcp9808;

void before() {
    battery.setMinVoltage(2.7);
    battery.setMaxVoltage(3.0);
    nodeManager.before();
}

void presentation() {
    nodeManager.presentation();
}

void setup() {
    pinMode(DONE_PIN, OUTPUT);
    digitalWrite(DONE_PIN, LOW);

    nodeManager.setSleepOrWait(true);
    nodeManager.setSleepBetweenSend(500);

#if !defined(REPORT_BATTERY_LEVEL)
    nodeManager.setADCOff();
#endif

    nodeManager.setup();
}

void loop() {
    nodeManager.loop();

    while (1) {
        digitalWrite(DONE_PIN, HIGH);
        delay(1);
        digitalWrite(DONE_PIN, LOW);
        delay(1);
    }
}
