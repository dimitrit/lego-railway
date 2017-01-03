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

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>

#define RST_PIN       0x3
#define SS_PIN        0x2
#define LED_PIN       0xA

#define I2C_ADDRESS_SELF    0x2
#define I2C_ADDRESS_TARGET  0x1

MFRC522 mfrc522(SS_PIN, RST_PIN);

unsigned long ledOffTime;

void setup() {
    Serial.begin(9600);
    while (!Serial);

    Wire.begin(I2C_ADDRESS_SELF);

    SPI.begin();
    mfrc522.PCD_Init();

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    ledOffTime = 0;

    Serial.print(F("RFID ["));
    Serial.print(I2C_ADDRESS_SELF, DEC);
    Serial.println(F("] ready..."));
}

void loop() {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        digitalWrite(LED_PIN, HIGH);
        ledOffTime = millis() + 1500;

        Wire.beginTransmission(I2C_ADDRESS_TARGET);
        Wire.write(I2C_ADDRESS_SELF);
        Wire.write(mfrc522.uid.uidByte, mfrc522.uid.size);
        Wire.endTransmission();

        Serial.print(F("id:"));
        dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
        Serial.println();

        mfrc522.PICC_HaltA();
    }

    if (digitalRead(LED_PIN) > 0 && millis() > ledOffTime) {
        digitalWrite(LED_PIN, LOW);
    }
}

void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}