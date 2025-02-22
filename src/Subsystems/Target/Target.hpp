#ifndef TARGET_HPP
#define TARGET_HPP

#include <Arduino.h>
#include "Constants/Constants_Vest.h"
#include "Components/IRremoteESP32/IRremoteESP32.hpp"

void IRAM_ATTR recvISR_0();

void IRAM_ATTR (*recvISRs[])() = {
    recvISR_0
};

namespace Target
{
    IRreceiver recv0(recvPins[0], recvISRs[0], recvValid);

    IRreceiver * irReceivers = {
        &recv0
    };
    size_t irReceiversCount = 1;

    void init() {
        for (int i = 0; i < irReceiversCount; i++) {
            irReceivers[i]->init();
        }
    }

    void loop() {
        for (int i = 0; i < irReceiversCount; i++) {
            if (irReceivers[i]->available()) {
                NEC_DATA receivedData = irReceivers->read();
                Serial.print("Received NEC Data: 0x");
                Serial.println(receivedData.data, HEX);
            }
        }
    }

};

void IRAM_ATTR recvISR_0() {
    Target::recv0.decodeNEC();
}

#endif // TARGET_HPP