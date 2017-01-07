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

#ifndef LEGO_RAILWAY_DISPATCH_H
#define LEGO_RAILWAY_DISPATCH_H

#include <Arduino.h>
#include "TrainController.h"

#define QUEUE_MAX_LENGTH 6

struct request_t {
    uint8_t loop;
    struct tag_t tag;
};

class DispatchQueue {
public:
    DispatchQueue();

    bool queue(uint8_t loop, tag_t tag);

    void dispatch(bool(*dispatch_handler)(request_t));

private:
    bool queued(tag_t tag);

    struct request_t _queue[QUEUE_MAX_LENGTH];
    uint8_t _head = 0;
    uint8_t _tail = 0;
    uint8_t _count = 0;
};

#endif //LEGO_RAILWAY_DISPATCH_H
