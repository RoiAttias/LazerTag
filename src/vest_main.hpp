#include "Subsystems/Target/Target.hpp"

void vest_setup() {
  Serial.begin(115200);  // Initialize Serial for output
  Target::init(); 
}

void vest_loop() {
  Target::loop();
}