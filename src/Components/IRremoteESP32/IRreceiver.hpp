#ifndef IRRECEIVER_HPP
#define IRRECEIVER_HPP

#include "IRremoteESP32.hpp"
#include "Components/Pushbutton/PushButton.hpp"
#include "Utilities/PacketBuffer.hpp" // Include the PacketBuffer header

class IRreceiver
{
private:
  int recvPin; // Pin for receiving IR signals
  Pushbutton isr; // Pushbutton instance for handling interrupts

  NEC_DATA data; // NEC data structure

  volatile uint32_t dataRead; // Variable to store the read data
  volatile uint32_t lastTime; // Variable to store the last time a signal was received
  volatile uint32_t currentTime; // Variable to store the current time
  volatile uint32_t duration; // Variable to store the duration between signals
  volatile int bitCount; // Variable to count the number of bits received
  volatile NEC_STAGE nec_stage; // Variable to store the current stage of NEC decoding

  bool validateData; // Flag to enable/disable validation

  PacketBuffer<NEC_DATA> buffer; // PacketBuffer to store received data

public:
  // Constructor to initialize the IR receiver
  IRreceiver(int pin, ISRpointer isrPointer, bool validate = false)
  : recvPin(pin), isr(pin, NEC_BOUNCE_STOP_FILTER, true, isrPointer),
    data(0UL), dataRead(0UL), lastTime(0), currentTime(0), duration(0),
    bitCount(0), nec_stage(headerMark), validateData(validate),
    buffer(IR_RECEIVER_BUFFER_SIZE)
  {}

  // Destructor
  ~IRreceiver() {}

  // Method to initialize the IR receiver
  void init()
  {
    lastTime = micros(); // Initialize lastTime with the current time
    nec_stage = headerMark; // Set the initial stage to headerMark

    pinMode(recvPin, INPUT); // Set the receive pin as input

    isr.init(true, true); // Initialize the ISR
  }

  // Method to check if data is available in the buffer
  int available()
  {
    return buffer.size();
  }

  // Method to read data from the buffer
  NEC_DATA read()
  {
    NEC_DATA value;
    buffer.dequeue(value); // Dequeue data from the buffer
    return value;
  }

  // Method to read the full data
  uint32_t readFull()
  {
    return read().data;
  }

  // Method to read and validate the data
  uint16_t readValid()
  {
    NEC_DATA input = read();
    uint16_t result = input.command;
    result = (result << 8) | input.address;
    return result;
  }

  void handleInterrupt()
  {
    isr.handleInterrupt();
  }

  // Method to decode NEC signals
  void decodeNEC()
  {
    if (buffer.size() >= IR_RECEIVER_BUFFER_SIZE)
      return;

    currentTime = micros();

    duration = currentTime - lastTime;
    if (duration < NEC_BOUNCE_STOP_FILTER)
      return;

    lastTime = currentTime;

    switch (nec_stage)
    {
    case headerMark:
      if (compare(duration, NEC_HEADER_MARK, NEC_THRESHOLD))
      {
        nec_stage = headerSpace;
      }
      else
      {
        nec_stage = headerMark; // Reset to header mark if not matched
      }
      break;

    case headerSpace:
      if (compare(duration, NEC_HEADER_SPACE, NEC_THRESHOLD))
      {
        nec_stage = bitMark;
        bitCount = 0;
        dataRead = 0;
      }
      else
      {
        nec_stage = headerMark; // Reset to header mark if not matched
      }
      break;

    case bitMark:
      if (compare(duration, NEC_BIT_MARK, NEC_THRESHOLD))
      {
        nec_stage = bitSpace;
      }
      else
      {
        nec_stage = headerMark; // Reset to header mark if not matched
      }
      break;

    case bitSpace:
      if (compare(duration, NEC_ONE_SPACE, NEC_THRESHOLD))
      {
        dataRead = (dataRead << 1) | 1; // Received a '1'
        bitCount++;
      }
      else if (compare(duration, NEC_ZERO_SPACE, NEC_THRESHOLD))
      {
        dataRead = (dataRead << 1) | 0; // Received a '0'
        bitCount++;
      }
      else
      {
        nec_stage = headerMark; // Reset to header mark if not matched
      }

      if (bitCount == NEC_BITS)
      { 
        bool valid = true;
        if (validateData)
        {
          // Check if the second byte is the inverse of the first, and the fourth is inverse of third
          uint8_t byte1 = (dataRead >> 24) & 0xFF;
          uint8_t byte2 = (dataRead >> 16) & 0xFF;
          uint8_t byte3 = (dataRead >> 8) & 0xFF;
          uint8_t byte4 = dataRead & 0xFF;

          valid = (byte1 == (byte2 ^ 0xFF)) && (byte3 == (byte4 ^ 0xFF));
        }

        if (valid)
        {
          if (buffer.size() < IR_RECEIVER_BUFFER_SIZE)
          {
            NEC_DATA newData;
            newData.data = dataRead;
            buffer.enqueue(newData); // Enqueue the valid data to the buffer
          }
        }
        
        nec_stage = headerMark; // Reset for next signal
      }
      else
      {
        nec_stage = bitMark; // Go back to wait for next bit mark
      }
      break;
    }
  }
};

#endif // IRRECEIVER_HPP