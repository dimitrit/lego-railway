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

#ifndef LEGO_RAILWAY_POINTS_H
#define LEGO_RAILWAY_POINTS_H

#include <Arduino.h>
#include <Servo.h>

#define _SERVO_TRAVEL 30

enum Direction {
    LEFT,
    RIGHT
};

enum Position {
    NORMAL = 0,                 // ie. straight ahead
    REVERSE = _SERVO_TRAVEL     // ie. turn
};

class Points {

public:
    Points(uint8_t pin, uint8_t power, Direction direction);

    void set(Position position);

    Position get();

private:
    uint8_t _power;
    Servo _servo;
    Direction _direction;
    Position _position;
};

#endif //LEGO_RAILWAY_POINTS_H
