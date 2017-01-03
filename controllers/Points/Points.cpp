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

#include "Points.h"

Points::Points(uint8_t pin, uint8_t power, Direction direction) : _servo(), _power(power), _direction(direction), _position(NORMAL) {
    _servo.attach(pin);

    pinMode(power, OUTPUT);
    digitalWrite(_power, HIGH);

    delay(30);

    set(_position);
}

void Points::set(Position position) {
    digitalWrite(_power, HIGH);

    if (_direction == RIGHT) {
        _servo.write(position);
    } else {
        _servo.write(_SERVO_TRAVEL - position);
    }

    digitalWrite(_power, LOW);
}

Position Points::get() {
    return _position;
}