#include <Arduino.h>
#include <functional>
#include <Wire.h>
#include <SPI.h>

void AppPrint(const char* key, const char* value = NULL, const char* value2 = NULL) {
    Serial.print(key); 
    if (value) { Serial.print(": "); Serial.print(value); }
    if (value2) { Serial.print(" "); Serial.print(value2); }
    Serial.println();
}

void AppPrintSeparator(const char* key, const char* value, const char* value2 = NULL) {
    String output = String(value) + String(value2);
    AppPrint("\n####################", key, output.c_str());
}

void AppPrintSeparator(const char* key, String value) {
    AppPrintSeparator(key, value.c_str());
}

void AppPrint(const char* key, String value) {
    AppPrint(key, value.c_str());
}

void AppPrint(const char* key, uint64_t value) {
    char val[22];
    sprintf(val, "%lu", value);
    AppPrint(key, val);
}

void AppPrintHex(const void *data, int len, char separator = ' ') {
    uint8_t* arr = (uint8_t*)data;

    for (int i=0; i<len; i++) {
        Serial.print(arr[i], HEX);
        if (i<len-1) Serial.print(separator);
    }
    Serial.println();
}

void AppPrintHex(const char* data, size_t len, char separator = ' ') {
    uint8_t* arr = (uint8_t*)data;

    for (int i=0; i<len; i++) {
        Serial.print(arr[i], HEX);
        if (i<strlen(data)-1) Serial.print(separator);
    }
    Serial.println();
}


void AppPrintHex(const char* data) {
    AppPrintHex(data, strlen(data), ' ');
    // uint8_t* arr = (uint8_t*)data;

    // for (int i=0; i<strlen(data); i++) {
    //     Serial.print(arr[i], HEX);
    //     if (i<strlen(data)-1) Serial.print(separator);
    // }
    // Serial.println();
}


void AppPrintChars(const char *data, int len, char separator = ' ') {
    for (int i=0; i<len; i++) {
        Serial.print((char)data[i]);
        if (i<len-1) Serial.print(separator);
    }       
    Serial.println();
}

void AppPrintHeap() {
    uint32_t freeHeap = ESP.getFreeHeap();
    AppPrint("\n[FreeHeap]", freeHeap);
}

#include "TimeoutItem.h"
TimerItem appTimer0;

#include "PinConfig.h"
#include "IntCycle.h"
#include "PinWritable.h"
#include "Button.h"         // need TimeoutItem.h
#include "PinSequence.h"
#include "AsyncTimer.h"
#include "ClockItem.h"
#include "SerialControl.h"
#include "ExtraSerial.h"
#include "AppQueue.h"
#include "Behavior.h"
#include "BaseComponent.h"

#include <7Services.h>