#ifndef GUN_MAIN_HPP
#define GUN_MAIN_HPP

#include <Arduino.h>
#include "Components/Pushbutton/Pushbutton.hpp"

#include "Modules/Gun.hpp"  
#include "Modules/Player.hpp"

#include "Subsystems/Shooter/Shooter.hpp"
#include "Subsystems/GUI_Gun/GUI_Gun.hpp"

#include "Constants/Constants_Gun.h"

void gun_trigger_interrupt();

Player player = Player();
Pushbutton trigger(triggerPin, 0, false, gun_trigger_interrupt);
Shooter shooter = Shooter();

void gun_trigger_interrupt()
{
    trigger.handleInterrupt();
}

void gun_setup()
{
    Serial.begin(115200);
    trigger.init();
    shooter.init(&player);
    GUI::init(&player);

    player.setHP(60);
    trigger.enablePressEvent(true);
    shooter.start();

}

void gun_loop()
{
    shooter.loop();
    GUI::loop();

    if (player.gun.getAmmo() == 0)
    {
        shooter.reload();
    }

    if (trigger.hasPressed() || (shooter.isFullAuto() && trigger.isPressed()))
    {
        shooter.shoot();
    }

    if (shooter.shouldRender())
    {
        GUI::callRender();
    }
}

#endif // GUN_MAIN_HPP