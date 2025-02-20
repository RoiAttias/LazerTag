#ifndef SHOOTER_HPP
#define SHOOTER_HPP

#include "Components/IRremoteESP32.hpp"
#include "Components/Visualizer.hpp"
#include "Modules/Gun.hpp"

class Shooter
{
private:
    IRsender irSender(irPin, irChannel, irFrequency);
    Visualizer visualizer(stripLength, stripPin, stripFrameIntervalMS);
    Gun gun();
public:
    init(){
        irSender.init();
        visualizer.init(stripBrightness);
    }

    void updateGunData(GunData data){
        gun = Gun(data);
    }

    void shoot()
    {
        
    }
    void reload();
};

#endif // SHOOTER_HPP