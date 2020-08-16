#include <Wire.h>
#include <MySensors_NodeManager.h>

#if defined(REPORT_BATTERY_LEVEL)
    #include <sensors/SensorBattery.h>
    SensorBattery battery;
#endif

#include <sensors/SensorMCP9808.h>
SensorMCP9808 mcp9808;

void before() {
#if defined(DONE_PIN)
    pinMode(DONE_PIN, OUTPUT);
    digitalWrite(DONE_PIN, LOW);
#endif

#if defined(REPORT_BATTERY_LEVEL)
    battery.setMinVoltage(2.7);
    battery.setMaxVoltage(3.0);
    battery.setReportIntervalMinutes(90);
#endif

    nodeManager.setReportIntervalMinutes(30);
    nodeManager.setSleepMinutes(30);
    nodeManager.before();
}

void presentation() {
    nodeManager.presentation();
}

void setup() {
    nodeManager.setSleepBetweenSend(500);

#if !defined(REPORT_BATTERY_LEVEL)
    nodeManager.setADCOff();
#endif

    nodeManager.setup();
}

void loop() {
    nodeManager.loop();

#if defined(DONE_PIN)

    while (1) {
        digitalWrite(DONE_PIN, HIGH);
        delay(1);
        digitalWrite(DONE_PIN, LOW);
        delay(1);
    }

#endif
}
