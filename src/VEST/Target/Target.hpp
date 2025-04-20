#ifndef TARGET_HPP
#define TARGET_HPP

#include <Arduino.h>
#include "VEST/Constants_Vest.h"
#include "Components/IRremoteESP32/IRremoteESP32.hpp"
#include "Utilities/HyperList.hpp"
#include "Utilities/HyperMap.hpp"

void IRAM_ATTR recvISR_0();
void IRAM_ATTR recvISR_1();
void IRAM_ATTR recvISR_2();

void IRAM_ATTR (*recvISRs[])() = {
    recvISR_0, recvISR_1, recvISR_2
};

struct ReceivedFireSignal
{
    NEC_DATA data;
    uint32_t lastTime;

    bool shouldStore() {
        return (millis() - lastTime) > NEC_VALID_TIME_MS;
    }
};

namespace Target
{
    IRreceiver irReceivers[] = {
        IRreceiver(recvPins[0], recvISRs[0], recvValid),
        IRreceiver(recvPins[1], recvISRs[1], recvValid),
        IRreceiver(recvPins[2], recvISRs[2], recvValid)
    };
    size_t irReceiversCount = 3;

    HyperList<NEC_DATA> hits;
    HyperList<ReceivedFireSignal> receivedFireSignals;

    void init() {
        for (int i = 0; i < irReceiversCount; i++) {
            irReceivers[i].init();
        }
    }

    void loop() {
        bool skip = false, receivedContains = false;
        for (int i = 0; i < irReceiversCount; i++) {
            if (irReceivers[i].available()) {
                NEC_DATA receivedData = irReceivers[i].read();

                if (!hits.contains(receivedData)) {

                    for (int j = 0; j < receivedFireSignals.size(); j++) {
                        if (receivedFireSignals[j].data == receivedData) {
                            receivedContains = true;

                            if (receivedFireSignals[j].shouldStore()) {
                                ReceivedFireSignal temp = receivedFireSignals[j];
                                temp.lastTime = millis();
                                receivedFireSignals.remove(j);
                                receivedFireSignals.insert(j, temp);
                                skip = false;
                            } else {
                                skip = true;
                            }

                        }
                        else {
                            skip = false;
                            receivedContains = false;
                        }
                    }


                    if (skip) {
                        continue;
                    }
                    else {
                        hits.addend(receivedData);
                        if (!receivedContains) {
                            ReceivedFireSignal fireSignal = {receivedData, millis()};
                            receivedFireSignals.addend(fireSignal);
                        }
                    }
                }
            }
        }
    }

    int hasHit() {
        return hits.size();
    }

    NEC_DATA readHit() {
        auto value = hits[0];
        hits.remove(0);
        return value;
    }

    void clear() {
        hits.clear();
        receivedFireSignals.clear();
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