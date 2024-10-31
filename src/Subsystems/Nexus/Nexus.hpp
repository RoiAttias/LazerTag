#ifndef NEXUS_HPP
#define NEXUS_HPP

#include <esp_wifi.h>

class Nexus
{
protected:
public:
    bool getMacAddress(uint8_t *mac)
    {
        return esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac) == ESP_OK;
    }

    bool getMacString(char *str) {
        uint8_t mac[6];
        bool success = getMacAddress(&mac);
        sprintf(str, "%02X:%02X:%02X:%02X:%02X:%02X", 
          mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        return success;
    }
};

#endif // NEXUS_HPP