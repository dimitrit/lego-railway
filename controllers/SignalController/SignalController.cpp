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

#include "SignalController.h"

SignalController::SignalController(uint8_t dataPin, uint8_t clockPin, uint8_t latchPin) : _data(dataPin), _clock(clockPin), _latch(latchPin) {
    pinMode(_data, OUTPUT);
    pinMode(_clock, OUTPUT);
    pinMode(_latch, OUTPUT);

    danger();
}

void SignalController::set(uint8_t index, uint8_t signal) {
    _positions = (~(3 << (index * 2)) & _positions) | signal << (index * 2);

    update();

    if (signal == SIGNAL_CLEAR) {
        _dangerAt = millis() + 3000;
    }
}

void SignalController::poll() {
    if (_dangerAt > 0 && millis() > _dangerAt) {
        danger();
    }
}

void SignalController::danger() {
    // set all signals at danger
    _positions = 0b10101010;
    _dangerAt = 0;
    update();
}

void SignalController::update() {

    digitalWrite(_latch, LOW);
    shiftOut(_data, _clock, MSBFIRST, _positions);
    digitalWrite(_latch, HIGH);
}