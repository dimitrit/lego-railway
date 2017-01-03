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

#ifndef LEGO_RAILWAY_CONTROLLER_H
#define LEGO_RAILWAY_CONTROLLER_H

#include <PowerFunctions.h>

struct tag_t {
    byte id[7];
};

struct train_t {
    train_t(uint8_t driver, byte *tag_id);

    uint8_t output;
    struct tag_t tag;
    train_t *next;
};

typedef struct train_t Train;

class TrainController {

public:
    TrainController(uint8_t pin, uint8_t channel);

    void add(Train *train);

    void set_speed(struct tag_t tag, uint8_t speed);

private:
    PowerFunctions _pf;
    Train *_train;

    void single_pwm(uint8_t output, uint8_t speed);
};

#endif //LEGO_RAILWAY_CONTROLLER_H