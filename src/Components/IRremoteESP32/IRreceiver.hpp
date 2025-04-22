/**
 * @file IRreceiver.hpp
 * @brief IR NEC protocol receiver for ESP32 using PushButton interrupts and PacketBuffer.
 *
 * Implements a state machine to decode incoming NEC IR signals, validate data,
 * and enqueue complete frames into a PacketBuffer for later retrieval.
 */

 #ifndef IRRECEIVER_HPP
 #define IRRECEIVER_HPP
 
 #include "IRremoteESP32.hpp"
 #include "Components/Pushbutton/PushButton.hpp"
 #include "Utilities/PacketBuffer.hpp"
 
 /**
  * @class IRreceiver
  * @brief Decodes NEC IR commands via GPIO interrupts and buffers results.
  */
 class IRreceiver {
 private:
   int recvPin;                       ///< GPIO pin connected to IR receiver.
   Pushbutton isr;                    ///< Debounced ISR handler for signal edges.
   volatile uint32_t dataRead;       ///< Accumulated raw bits.
   volatile uint32_t lastTime;       ///< Timestamp of last edge.
   volatile NEC_STAGE nec_stage;     ///< Current NEC decoding stage.
   volatile int bitCount;            ///< Number of bits received so far.
   bool validateData;                ///< True to enforce address/command inverse checks.
   PacketBuffer<NEC_DATA> buffer;    ///< FIFO buffer of decoded frames.
 
 public:
   /**
    * @brief Constructs an IRreceiver.
    *
    * @param pin GPIO pin number for IR input.
    * @param isrPointer Function pointer for interrupt handling.
    * @param validate True to enable NEC data validation.
    */
   IRreceiver(int pin, ISRpointer isrPointer, bool validate = false)
     : recvPin(pin)
     , isr(pin, NEC_BOUNCE_STOP_FILTER, true, isrPointer)
     , dataRead(0)
     , lastTime(0)
     , nec_stage(headerMark)
     , bitCount(0)
     , validateData(validate)
     , buffer(IR_RECEIVER_BUFFER_SIZE)
   {}
 
   /** @brief Destructor clears pending buffer. */
   ~IRreceiver() {}
 
   /**
    * @brief Initializes the receiver pin and ISR; call this in setup().
    */
   void init() {
     lastTime = micros();
     nec_stage = headerMark;
     pinMode(recvPin, INPUT);
     isr.init(true, true);
   }
 
   /** @brief Number of decoded frames available. */
   int available() const { return buffer.size(); }
 
   /**
    * @brief Dequeues the next NEC_DATA frame from buffer.
    * @return NEC_DATA frame (undefined if none available).
    */
   NEC_DATA read() { NEC_DATA d; buffer.dequeue(d); return d; }
 
   /** @brief Returns raw 32-bit frame data. */
   uint32_t readFull()      { return read().data; }
 
   /**
    * @brief Reads and returns validated command+address (16-bit).
    * @return [command<<8 | address]
    */
   uint16_t readValid() {
     NEC_DATA d = read();
     return (static_cast<uint16_t>(d.command) << 8) | d.address;
   }
 
   /** @brief To be called from ISR: handles edge detection. */
   void handleInterrupt() { isr.handleInterrupt(); }
 
   /**
    * @brief Main decode routine; call repeatedly (e.g., in loop()).
    *
    * Processes timing intervals to reconstruct bits, validates NEC checksum,
    * and enqueues frames into the buffer when complete.
    */
   void decodeNEC() {
     if (buffer.isFull()) return;
     uint32_t now = micros();
     uint32_t duration = now - lastTime;
     lastTime = now;
 
     if (duration < NEC_BOUNCE_STOP_FILTER) return;
 
     switch (nec_stage) {
       case headerMark:
         if (compare(duration, NEC_HEADER_MARK, NEC_THRESHOLD)) {
           nec_stage = headerSpace;
           bitCount = 0;
           dataRead = 0;
         }
         break;
 
       case headerSpace:
         if (compare(duration, NEC_HEADER_SPACE, NEC_THRESHOLD)) {
           nec_stage = bitMark;
         } else {
           nec_stage = headerMark;
         }
         break;
 
       case bitMark:
         if (compare(duration, NEC_BIT_MARK, NEC_THRESHOLD)) {
           nec_stage = bitSpace;
         } else {
           nec_stage = headerMark;
         }
         break;
 
       case bitSpace:
         if (compare(duration, NEC_ONE_SPACE, NEC_THRESHOLD)) {
           dataRead = (dataRead << 1) | 1;
           bitCount++;
         } else if (compare(duration, NEC_ZERO_SPACE, NEC_THRESHOLD)) {
           dataRead = (dataRead << 1);
           bitCount++;
         } else {
           nec_stage = headerMark;
         }
 
         if (bitCount == NEC_BITS) {
           bool valid = true;
           if (validateData) {
             valid = (((dataRead >> 24) & 0xFF) ^ 0xFF) == ((dataRead >> 16) & 0xFF)
                   && (((dataRead >> 8) & 0xFF) ^ 0xFF) == (dataRead & 0xFF);
           }
           if (valid) {
             buffer.enqueue(NEC_DATA(dataRead));
           }
           nec_stage = headerMark;
         } else {
           nec_stage = bitMark;
         }
         break;
     }
   }
 };
 
 #endif // IRRECEIVER_HPP 