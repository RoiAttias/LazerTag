#ifndef IRRECEIVER_HPP
#define IRRECEIVER_HPP

#include "IRremoteESP32.hpp"
#include "Components/Pushbutton/PushButton.hpp"

class IRreceiver
{
private:
  int recvPin;
  Pushbutton isr;

  NEC_DATA data;

  uint32_t dataRead;
  uint32_t lastTime;
  uint32_t currentTime;
  uint32_t duration;
  int bitCount;
  NEC_STAGE nec_stage;

  bool validateData;  // Flag to enable/disable validation

  NEC_DATA buffer[IR_RECEIVER_BUFFER_SIZE]; // Buffer to store received data
  int bufferIndex;

public:
  IRreceiver(int pin, ISRpointer isrPointer, bool validate = false)
  : recvPin(pin), isr(pin, NEC_BOUNCE_STOP_FILTER, true, isrPointer),
    data(0UL), dataRead(0UL), lastTime(0), currentTime(0), duration(0),
    bitCount(0), nec_stage(headerMark), validateData(validate),
    bufferIndex(0)
  {
    memset(buffer, 0, sizeof(buffer));
  }

  ~IRreceiver() {}

  void init()
  {
    lastTime = micros();
    nec_stage = headerMark;

    pinMode(recvPin, INPUT_PULLUP);

    isr.enablePressEvent(true);
    isr.enableReleaseEvent(true);
    isr.init();
  }

  int available()
  {
    return bufferIndex;
  }

  NEC_DATA read()
  {
    NEC_DATA value;
    if (bufferIndex > 0)
    {
      value = buffer[0];
      // Shift the buffer contents
      for (int i = 1; i < bufferIndex; i++)
      {
        buffer[i - 1] = buffer[i];
      }
      bufferIndex--;
    }
    return value;
  }

  uint32_t readFull()
  {
    return read().data;
  }

  uint16_t readValid()
  {
    NEC_DATA input = read();
    uint16_t result = input.command;
    result = (result << 8) | input.address;
    return result;
  }

  void decodeNEC()
  {
    if (bufferIndex >= IR_RECEIVER_BUFFER_SIZE)
      return;

    if (!isr.hasPressed() && !isr.hasReleased())
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
          if (bufferIndex < IR_RECEIVER_BUFFER_SIZE)
          {
            buffer[bufferIndex++].data = dataRead;
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