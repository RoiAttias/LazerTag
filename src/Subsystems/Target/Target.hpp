#ifndef TARGET_HPP
#define TARGET_HPP

#include <Arduino.h>
#include "Constants/Constants_Vest.h"
#include "Components/IRremoteESP32/IRremoteESP32.hpp"

void IRAM_ATTR recvISR_0();
void IRAM_ATTR recvISR_1();
void IRAM_ATTR recvISR_2();

void IRAM_ATTR (*recvISRs[])() = {
    recvISR_0, recvISR_1, recvISR_2
};

namespace Target
{
    IRreceiver irReceivers[] = {
        IRreceiver(recvPins[0], recvISRs[0], recvValid),
        IRreceiver(recvPins[1], recvISRs[1], recvValid),
        IRreceiver(recvPins[2], recvISRs[2], recvValid)
    };
    size_t irReceiversCount = 3;

    void init() {
        for (int i = 0; i < irReceiversCount; i++) {
            irReceivers[i].init();
        }
    }

    void loop() {
        for (int i = 0; i < irReceiversCount; i++) {
            if (irReceivers[i].available()) {
                NEC_DATA receivedData = irReceivers[i].read();
                Serial.print(i);
                Serial.print(" - Received NEC Data: 0x");
                Serial.println(receivedData.data, HEX);
            }
        }
    }

}

void IRAM_ATTR recvISR_0() {
    Target::irReceivers[0].decodeNEC();
}

void IRAM_ATTR recvISR_1() {
    Target::irReceivers[1].decodeNEC();
}

void IRAM_ATTR recvISR_2() {
    Target::irReceivers[2].decodeNEC();
}

#endif // TARGET_HPP