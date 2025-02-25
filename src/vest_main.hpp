#include "Subsystems/Target/Target.hpp"
#include "Subsystems/Ring/Ring.hpp"

void vest_setup() {
  Serial.begin(115200);  // Initialize Serial for output

  Target::init();
  Ring::init();
  Ring::load();
}

void vest_loop() {
  Target::loop();
  Ring::loop();

  if (Target::hasHit() > 0) {
    if(Target::readHit().data == 0x69966996) {
      Ring::hit();
    }
  }
}