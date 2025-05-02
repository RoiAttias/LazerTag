🎉 ESP32 LazerTag Project v1.0.0 Release 🎉

The ESP32 LazerTag Project, developed by Roi Attias, is a modular, interactive laser-tag gaming system powered by the ESP32 microcontroller.
The project integrates wireless and infrared communication, responsive graphical user interfaces (GUIs), and dynamic visual feedback to deliver an immersive gaming experience.
During development, several reusable libraries and modules were created, significantly benefiting future projects.
Note: This initial release (v1.0.0) is functional and feature-rich but includes some known bugs and stability issues. These will be fully addressed in the upcoming stable release v1.0.1.

🚀 Key Features
LuminaUI GUI Helper Library: A custom GUI framework for embedded devices, simplifying the creation of interactive and visually appealing interfaces. LuminaUI handles screen activities, navigation, and custom components, enhancing productivity and user experience.
LuminaUI - GUI Manager: Intuitive TFT touchscreen interface (ST7796 + XPT2046) for efficient game management.
LuminaUI - GUI_Gun: OLED SSD1306 display (128×64) for real-time messages, ammunition status, health, and reload indicators.
Modular Game Management: Structured modules (Game, Player, and Gun) manage player health, ammunition, game states, and hit detection. Designed for flexibility and expandability.
Nexus Protocol (ESP-NOW): Peer-to-peer wireless communication based on Espressif's ESP-NOW protocol, ensuring fast, reliable exchange of data (player health, game status, ammunition updates) between devices without external network hardware.
IRremoteESP32 Integration: Accurate infrared communication using NEC protocol at 38 kHz for responsive hit detection via IR signals.
Visualizer Animation Manager: Dynamic animation management for NeoPixel (WS2812) LEDs, providing visual feedback for gameplay events (shooting, hits, reload, victory, defeat).
Interrupt-based Pushbutton Class: Custom interrupt-driven input handling with debounce logic, offering precise and responsive trigger actions.
Flexible Device Type Switching: Simple compile-time configuration to switch device roles (Gun, Vest, Manager), facilitating deployment and configuration.
LazerTag Packet for Nexus: Structured communication packets supporting future updates, ensuring maintainability and extensibility.

📦 Device Subsystems
Gun: Trigger interrupts, IR transmitter, OLED GUI (gui_gun), ammunition management (Gun module).
Vest: Multiple IR receivers (Target), NeoPixel ring (Ring), visual and tactile feedback systems.
Manager: Device scanning, pairing, and game setup via touchscreen (gui_manager), with comprehensive game-state oversight and control.

🐞 Known Issues
Minor bugs related to GUI responsiveness, flags, and packet handling, scheduled for resolution in version 1.0.1.

🔮 Upcoming v1.0.1 Update
Bug fixes for improved stability and responsiveness.
Enhanced wireless communication reliability and GUI rendering.
Advanced GUI interaction with additional basic and custom elements via LuminaUI.
Full utilization of the existing GunData structure for dynamic gun configuration selection per player.
Addition of a dedicated physical reload button on the Gun device.

📚 Credits and External Libraries
This project leverages the powerful PlatformIO ecosystem and several essential external libraries:
PlatformIO – Embedded development IDE and build system.
Arduino Core for ESP32 – ESP32 integration with Arduino framework.
TFT_eSPI by Bodmer – High-performance TFT display library.
U8g2 by olikraus – Universal graphics library for OLED displays.
Adafruit NeoPixel – WS2812 NeoPixel LED control library.
ESP-NOW via ESP32 Arduino Core – High-speed peer-to-peer wireless communication.
FreeFonts from Adafruit_GFX – Custom fonts for graphic displays.

Enjoy exploring the first stable feature-complete release, and stay tuned for v1.0.1 for a polished, optimized experience! 🌟
