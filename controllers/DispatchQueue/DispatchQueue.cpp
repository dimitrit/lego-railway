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

#include "DispatchQueue.h"

DispatchQueue::DispatchQueue(): _head(0), _tail(0), _count(0) { }

bool DispatchQueue::queue(uint8_t loop, tag_t tag) {
    if (_count == QUEUE_MAX_LENGTH) {
        return false;
    }

    _queue[_head].loop = loop;
    memcpy(_queue[_head].tag.id, tag.id, sizeof(struct tag_t));

    _head = (_head + 1) % QUEUE_MAX_LENGTH;
    _count++;

    return true;
}

void DispatchQueue::dispatch(bool(*dispatch_handler)(request_t request)) {
    if (_count > 0 && dispatch_handler(_queue[_tail])) {
        _tail = (_tail + 1) % QUEUE_MAX_LENGTH;
        _count--;
    }
}