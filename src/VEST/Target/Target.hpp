/**
 * @file Target.hpp
 * @brief IR hit detection subsystem for the Vest device.
 *
 * Uses three IRreceiver instances (one per pin/ISR) to decode NEC protocol signals.
 * Implements debouncing logic to avoid duplicate hits within a short time window.
 * Exposes API to initialize, poll, and consume unique hit codes.
 */

 #ifndef TARGET_HPP
 #define TARGET_HPP
 
 #include <Arduino.h>
 #include "VEST/Constants_Vest.h"                     ///< recvPins[], recvValid
 #include "Components/IRremoteESP32/IRremoteESP32.hpp" ///< NEC_DATA, IRreceiver
 #include "Utilities/HyperList.hpp"                   ///< dynamic list of hits
 
 // Forward declarations of the interrupt handlers
 void IRAM_ATTR recvISR_0();
 void IRAM_ATTR recvISR_1();
 void IRAM_ATTR recvISR_2();
 
 // Array of ISR function pointers, indexed by receiver
 static void IRAM_ATTR (*recvISRs[])() = {
     recvISR_0, recvISR_1, recvISR_2
 };
 
 /**
  * @struct ReceivedFireSignal
  * @brief Wraps a received NEC_DATA code with a timestamp.
  *
  * Allows filtering out repeated signals that arrive within NEC_VALID_TIME_MS.
  */
 struct ReceivedFireSignal {
     NEC_DATA data;     ///< The NEC 32-bit code
     uint32_t lastTime; ///< millis() when this code was last accepted
 
     /**
      * @brief Check whether enough time has passed to accept the same signal again.
      * @return True if (millis() - lastTime) > NEC_VALID_TIME_MS
      */
     bool shouldStore() const {
         return (millis() - lastTime) > NEC_VALID_TIME_MS;
     }
 };
 
 namespace Target {
     /// IR receiver instances (one per pin/ISR)
     static IRreceiver irReceivers[] = {
         IRreceiver(recvPins[0], recvISRs[0], recvValid),
         IRreceiver(recvPins[1], recvISRs[1], recvValid),
         IRreceiver(recvPins[2], recvISRs[2], recvValid)
     };
     static const size_t irReceiversCount = 3;
 
     /// Queue of unique hit codes waiting to be processed
     static HyperList<NEC_DATA> hits;
     /// Records recent signals to debounce duplicates
     static HyperList<ReceivedFireSignal> receivedFireSignals;
 
     /**
      * @brief Initialize all IR receivers and attach their ISRs.
      */
     void init() {
         for (size_t i = 0; i < irReceiversCount; ++i) {
             irReceivers[i].init();
         }
     }
 
     /**
      * @brief Poll each receiver, decode new NEC codes, and store unique hits.
      *
      * - Skips codes already in `hits`.
      * - Uses `receivedFireSignals` to debounce repeats within NEC_VALID_TIME_MS.
      */
     void loop() {
         for (size_t i = 0; i < irReceiversCount; ++i) {
             if (!irReceivers[i].available()) continue;
             NEC_DATA code = irReceivers[i].read();
 
             // Already queued? skip
             if (hits.contains(code)) continue;
 
             bool skip = false;
             // Debounce: check if we saw this code recently
             for (size_t j = 0; j < receivedFireSignals.size(); ++j) {
                 auto &entry = receivedFireSignals[j];
                 if (entry.data == code) {
                     if (entry.shouldStore()) {
                         entry.lastTime = millis();
                         skip = false;
                     } else {
                         skip = true;
                     }
                     break;
                 }
             }
             if (skip) continue;
 
             // New hit: enqueue and record timestamp
             hits.addend(code);
             receivedFireSignals.addend({code, millis()});
         }
     }
 
     /**
      * @brief Number of pending hits.
      * @return Integer count.
      */
     int hasHit() {
         return hits.size();
     }
 
     /**
      * @brief Retrieve and remove the oldest hit code.
      * @return The NEC_DATA of the hit.
      */
     NEC_DATA readHit() {
         NEC_DATA code = hits[0];
         hits.remove(0);
         return code;
     }
 
     /**
      * @brief Clear all pending hits and debounce history.
      */
     void clear() {
         hits.clear();
         receivedFireSignals.clear();
     }
 }
 
 // ISR implementations simply invoke the decoder on the corresponding receiver
 void IRAM_ATTR recvISR_0() { Target::irReceivers[0].decodeNEC(); }
 void IRAM_ATTR recvISR_1() { Target::irReceivers[1].decodeNEC(); }
 void IRAM_ATTR recvISR_2() { Target::irReceivers[2].decodeNEC(); }
 
 #endif // TARGET_HPP 