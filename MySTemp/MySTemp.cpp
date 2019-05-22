#include <MySensors.h>
#include <Wire.h>
#include "Adafruit_MCP9808.h"

Adafruit_MCP9808 mcp;

MyMessage msg(0, V_TEMP);

#if defined(REPORT_BATTERY_LEVEL)
int32_t readVcc() {
    // Read 1.1V reference against AVcc
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);

    delay(2);  // Wait for Vref to settle
    ADCSRA |= _BV(ADSC);  // Start conversion

    while (bit_is_set(ADCSRA, ADSC));  // measuring

    uint8_t low  = ADCL;  // must read ADCL first - it then locks ADCH
    uint8_t high = ADCH;  // unlocks both

    int32_t result = (high << 8) | low;

    result = 1125300L / result;  // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
    return result;  // Vcc in millivolts
}
#endif

void presentation() {
    sendSketchInfo(NODE_NAME, SKETCH_VER);
    present(0, S_TEMP);
}

void goToSleep() {
    while (1) {
        digitalWrite(DONE_PIN, HIGH);
        delay(1);
        digitalWrite(DONE_PIN, LOW);
        delay(1);
    }
}

void setup() {
    pinMode(DONE_PIN, OUTPUT);
    digitalWrite(DONE_PIN, LOW);

#if defined(MY_DEBUG) || defined(APP_DEBUG)
    Serial.begin(MY_BAUD_RATE);
#endif

#if defined(REPORT_BATTERY_LEVEL)
    analogReference(INTERNAL);
#endif

    if (mcp.begin()) {
        mcp.setResolution(2);  // 0.125°C
        mcp.shutdown_wake(0);  // wake up

#if defined(APP_DEBUG)
        Serial.println("MCP9808 ok.");
#endif

    } else {
#if defined(APP_DEBUG)
        Serial.println("Could not init MCP9808.");
#endif
        goToSleep();
    }
}

void loop() {
#if defined(REPORT_BATTERY_LEVEL)
    int32_t batt_voltage = readVcc();
    uint8_t battery_state = 0;

    if (batt_voltage >= 2700) {
        battery_state = map(batt_voltage, 2700, 3000, 0, 100);
        battery_state = constrain(battery_state, 0, 100);
    }

#if defined(APP_DEBUG)
    Serial.print("Battery voltage: ");
    Serial.print(batt_voltage);
    Serial.println(" V");
    Serial.print("Battery percentage: ");
    Serial.print(battery_state);
    Serial.println(" %");
#endif

    sendBatteryLevel(battery_state);
#endif

    sleep(mcp.getResolution());

    float temp = mcp.readTempC();

    mcp.shutdown_wake(1);  // sleep

#if defined(APP_DEBUG)
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" *C");
#endif

    if (!isnan(temp)) {
        send(msg.set(temp, 2));
    }

    goToSleep();
}
