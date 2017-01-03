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

#include "TrainController.h"

TrainController::TrainController(uint8_t pin, uint8_t channel) : _pf(pin, channel), _train(0) { }

void TrainController::add(Train *train) {
    Train *ptr = _train;

    train->next = 0;

    if (ptr == 0) {
        _train = train;
    } else {
        while (ptr->next != 0) {
            ptr = ptr->next;
        }
        ptr->next = train;
    }
}

void TrainController::set_speed(struct tag_t tag, uint8_t speed) {
    int n;
    Train *ptr = _train;

    if (ptr == 0) {
        return;
    }

    while (ptr->next != 0) {
        n = memcmp(ptr->tag.id, tag.id, sizeof(struct tag_t));
        if (n == 0) {
            break;
        }
        ptr = ptr->next;
    }
    n = memcmp(ptr->tag.id, tag.id, sizeof(struct tag_t));
    if (n == 0) {
        single_pwm(ptr->output, speed);
    }
}

void TrainController::single_pwm(uint8_t output, uint8_t speed) {
    _pf.single_pwm(output, speed);
    if (speed == PWM_BRK) {
        delay(30);
        _pf.single_pwm(output, PWM_FLT);
    }
}

train_t::train_t(uint8_t driver, byte *tag_id) : output(driver) {
    memcpy(tag.id, tag_id, sizeof(struct tag_t));
}
