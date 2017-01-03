/*
 * Copyright 2016 Plastic Objects Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <Arduino.h>
#include <Wire.h>
#include <util/delay.h>

#include "Points/Points.h"
#include "SignalController/SignalController.h"
#include "TrainController/TrainController.h"
#include "DispatchQueue/DispatchQueue.h"

#define PIN_SERVO_LEFT 2
#define PIN_SERVO_RIGHT 3
#define PIN_SERVO_POWER 5

#define PIN_LED_GREEN 4
#define PIN_LED_RED 6

#define PIN_595_DATA  9
#define PIN_CLOCK 12
#define PIN_LATCH 11

#define I2C_ADDRESS_SELF 1

Points *left;
Points *right;

Train *passenger;
Train *freight;

DispatchQueue *dispatch_queue;
SignalController *signal_controller;
TrainController *train_controller;

struct tag_t *train_in_section;

unsigned long greenOffTime;
unsigned long redOffTime;

byte blueTagId[7] = {0x04, 0x8B, 0x54, 0x52, 0xA5, 0x48, 0x80};
byte redTagId[7] = {0x04, 0x81, 0x54, 0x52, 0xA5, 0x48, 0x80};

void setup() {
    Serial.begin(9600);
    while (!Serial);

    Serial.print(F("Initialising..."));

    train_in_section = 0;

    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_RED, OUTPUT);

    left = new Points(PIN_SERVO_LEFT, PIN_SERVO_POWER, LEFT);
    right = new Points(PIN_SERVO_RIGHT, PIN_SERVO_POWER, RIGHT);

    signal_controller = new SignalController(PIN_595_DATA, PIN_CLOCK, PIN_LATCH);

    train_controller = new TrainController(13, 0);

    freight = new Train(BLUE, blueTagId);
    train_controller->add(freight);

    passenger = new Train(RED, redTagId);
    train_controller->add(passenger);

    dispatch_queue = new DispatchQueue;

    Wire.begin(I2C_ADDRESS_SELF);
    Wire.onReceive(receiveEvent);

    Serial.println(F("done!"));
}

void loop() {
    resetLED(PIN_LED_GREEN, greenOffTime);
    resetLED(PIN_LED_RED, redOffTime);

    signal_controller->poll();

    delay(100);
}

void receiveEvent(int count) {
    struct tag_t tag;
    int i = 0;

    Serial.print(F("> "));

    if (count != 8) {
        return;
    }

    uint8_t loop = (uint8_t) Wire.read();

    while (Wire.available() > 0) {
        byte b = (byte) Wire.read();
        tag.id[i++] = b;
    }

    Serial.print(F("detected "));
    Serial.print(loop, DEC);
    dump_byte_array(tag.id, sizeof(struct tag_t));

    if (memcmp(train_in_section, &tag.id, sizeof(struct tag_t)) == 0) {
        Serial.print(F(", train leaving section"));
        delete (train_in_section);
        train_in_section = 0;
    }

    // stop the train for now...
    train_controller->set_speed(tag, PWM_BRK);

    // ...then add it to the dispatch queue
    if (dispatch_queue->queue(loop, tag)) {
        // green led
        digitalWrite(PIN_LED_GREEN, HIGH);
        greenOffTime = millis() + 1500;
        Serial.println(F("...queued!"));
    } else {
        digitalWrite(PIN_LED_RED, HIGH);
        redOffTime = millis() + 1500;
        Serial.println(F("...rejected!"));
    }

    // dispatch the next train in queue only when no train in section
    if (train_in_section == 0) {
        dispatch_queue->dispatch(&dispatch_handler);
    }
}

bool dispatch_handler(request_t request) {
    delay(1000);

    // set points as required
    switch (request.loop) {
        case (uint8_t) 2:
            left->set(NORMAL);
            right->set(NORMAL);
            break;
        case (uint8_t) 3:
        default:
            left->set(REVERSE);
            right->set(REVERSE);
            break;
    }

    delay(1000);

    signal_controller->set(request.loop, SIGNAL_CLEAR);

    delay(1500);

    // start next train
    train_in_section = new tag_t;
    memcpy(train_in_section, request.tag.id, sizeof(struct tag_t));

    train_controller->set_speed(request.tag, PWM_FWD4);

    Serial.print(F("train entering section: "));
    Serial.print(request.loop, DEC);
    dump_byte_array(request.tag.id, sizeof(struct tag_t));
    Serial.println(F("."));

    return true;
}

void resetLED(uint8_t pin, unsigned long offTime) {
    if (digitalRead(pin) > 0 && millis() > offTime) {
        digitalWrite(pin, LOW);
    }
}

void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}