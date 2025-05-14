/**
 * @file Scanner.hpp
 * @brief “Scanner” Activity for the Manager GUI.
 *
 * Presents a grid of DeviceBox elements and “Scan”/“Next” buttons.
 * Allows the manager to scan for active devices, select at least one gun and one vest,
 * and proceed to player setup.
 */

 #ifndef SCANNER_HPP
 #define SCANNER_HPP
 
 #include "GUI_Manager.hpp"
 #include "Common/Constants_Common.h"
 #include <Arduino.h>
 #include "Components/Nexus/Nexus.hpp"
 #include "Common/LazerTagPacket.hpp"
 #include "PlayerSetup.hpp"
 
 // Forward declarations for touch handlers
 void onScanButtonTouch(ivec2 point, TouchStatus status);
 void onNextButtonTouch(ivec2 point, TouchStatus status);
 void onDeviceBoxTouch(ivec2 point, TouchStatus status);
 
 void initScanner(); ///< Forward declaration for scanner initialization
 void triggerScanner(); ///< Forward declaration for scanner trigger function

 /**
  * @class Scanner
  * @brief Activity that lets the manager discover and select game devices.
  *
  * Layout:
  * - A colored background
  * - Title text “Scanner”
  * - A grid of up to 9 DeviceBox elements (IDs 1–9)
  * - A “Scan” button to initiate ESP-NOW peer discovery
  * - A “Next” button to proceed once at least one gun and one vest are selected
  *
  * On render:
  * - Sends a scan request to each selected DeviceBox’s address
  * - Updates the displayed list of discovered Nexus::devices
  */
 class Scanner : public Activity {
 public:
     Background background;       ///< Full-screen background block
     Text       titleText;        ///< Header text “Scanner”
     Button     scanButton;       ///< “Scan” button
     Button     nextButton;       ///< “Next” button
 
     DeviceBox* deviceBoxes[9];   ///< Grid of DeviceBox pointers
     ivec2      deviceBoxOrigin;  ///< Top-left origin for first box
     ivec2      deviceBoxSize;    ///< Width/height of each box
     ivec2      deviceBoxSpacing; ///< Horizontal/vertical spacing between boxes

     bool NextButtonPressed = false; ///< Flag to track Next button state
 
     /**
      * @brief Construct a Scanner activity.
      *
      * - Creates background, title, Scan and Next buttons at fixed positions.
      * - Allocates 9 DeviceBox instances in a 3×3 grid.
      * - Registers touch handlers and hides boxes initially.
      */
     Scanner()
       : Activity(),
         background(TFT_BROWN),
         titleText(
           Element(ivec2(0,  10), LuminaUI_AUTO, ivec2(480, 40)),
           String("Scanner"), TFT_WHITE, 1, MC_DATUM, 0, &FreeMonoBold24pt7b
         ),
         scanButton(
           Element(ivec2(20, 240), LuminaUI_AUTO, ivec2(200, 70)),
           "Scan", TFT_BLACK, TFT_GREEN, TFT_BLACK, 20, 1, 0.0f,
           &FreeMono24pt7b, true, true
         ),
         nextButton(
           Element(ivec2(250,240), LuminaUI_AUTO, ivec2(200, 70)),
           "Next", TFT_BLACK, TFT_DARKGREY, TFT_BLACK, 20, 1, 0.0f,
           &FreeMono24pt7b, true, true
         ),
         deviceBoxOrigin(   6,  60),
         deviceBoxSize(   150,  50),
         deviceBoxSpacing(   6,   8)
     {
         // Configure Scan button touch
         scanButton.OnTouch_setHandler(onScanButtonTouch);
         scanButton.OnTouch_setEnable(true);
 
         // Configure Next button touch
         nextButton.OnTouch_setHandler(onNextButtonTouch);
         nextButton.OnTouch_setEnable(true);
 
         // Prepare array of element pointers (4 static + 9 dynamic)
         Element* elems[13] = { &background, &titleText, &scanButton, &nextButton };
         ivec2 pos;
         // Instantiate and position 9 DeviceBoxes
         for (int i = 0; i < 9; i++) {
             pos = deviceBoxOrigin +
                   (deviceBoxSize + deviceBoxSpacing).multiply(ivec2(i % 3, i / 3));
             deviceBoxes[i] = new DeviceBox(Element(pos, LuminaUI_AUTO, ivec2(150,50)), i + 1);
             elems[4 + i] = deviceBoxes[i];
         }
 
         init();  // Hide & configure DeviceBoxes
         elements.addFromArray(elems, sizeof(elems)/sizeof(Element*));
     }
 
     /**
      * @brief Initialize DeviceBoxes: hide them and set touch callbacks.
      */
     void init() {
        GUI::gameDevices.clear();
         for (int i = 0; i < 9; i++) {
             deviceBoxes[i]->visible = false;
             deviceBoxes[i]->OnTouch_setHandler(onDeviceBoxTouch);
             deviceBoxes[i]->OnTouch_setEnable(true);
         }
     }
 
     /**
      * @brief Determine if the Next button can be pressed.
      * @return true if ≥1 gun and ≥1 vest are selected.
      */
     bool canNext() {
         int guns = 0, vests = 0;
         for (int i = 0; i < 9; i++) {
             if ((!deviceBoxes[i]->selected) || (!deviceBoxes[i]->visible)) continue;
             if (deviceBoxes[i]->deviceGroup == NEXUS_GROUP_GUN)  guns++;
             if (deviceBoxes[i]->deviceGroup == NEXUS_GROUP_VEST) vests++;
         }
         return guns > 0 && vests > 0;
     }
 
     /**
      * @brief On each render pass:
      * - Builds GUI::gameDevices from currently selected boxes
      * - Sends a scan request to each selected address
      * - Draws all child elements
      */
     Viewport render(const Viewport &viewport) override {
         if (!canNext()) {
            nextButton.background.fillColor = TFT_DARKGREY;
            nextButton.background.borderColor = TFT_BLACK;
            nextButton.text.textColor = TFT_BLACK;
         } else if (canNext() && !NextButtonPressed) {
            nextButton.background.fillColor = TFT_ORANGE;
            nextButton.background.borderColor = TFT_BLACK;
            nextButton.text.textColor = TFT_BLACK;
         }

         return Activity::render(viewport);
     }
 
     /**
      * @brief Update device list when scan completes.
      * - Shows a DeviceBox for each discovered peer
      * - Updates its ID and group flag
      * - Resets selection and button colors
      */
     virtual void updateScannedDevices() {
         for (int i = 0; i < 9; i++) {
             if (i < Nexus::devices.size()) {
                 NexusAddress d = Nexus::devices[i];
                 deviceBoxes[i]->updateInformation(d.deviceID, d.groups);
                 deviceBoxes[i]->setSelected(false);
                 deviceBoxes[i]->visible = true;
             } else {
                 deviceBoxes[i]->updateInformation(0, 0);
                 deviceBoxes[i]->setSelected(false);
                 deviceBoxes[i]->visible = false;
             }
         }
         // Update button colors based on scan completion & selection state
         scanButton.background.fillColor = TFT_GREEN;
         nextButton.background.fillColor = canNext() ? TFT_ORANGE : TFT_DARKGREY;
     }
 };


 Scanner* scanner = new Scanner(); ///< Pointer to the scanner instance

 /** 
  * @brief Handler for Scan button touches.
  * - PRESS: darken button
  * - RELEASE: call Nexus::scan() and update button appearance
  * - READY: restore color to green if scan complete
  */
 void onScanButtonTouch(ivec2 point, TouchStatus status) {
     switch (status) {
         case TouchStatus_PRESS:
             scanner->scanButton.background.fillColor = TFT_DARKGREEN;
             scanner->scanButton.background.borderColor = TFT_BLACK;
             scanner->scanButton.text.textColor     = TFT_WHITE;
             scanner->scanButton.callRender();
             break;
         case TouchStatus_RELEASE:
             GUI::gameDevices.clear();
             Nexus::scan();
             scanner->scanButton.background.fillColor = TFT_DARKGREY;
             scanner->scanButton.background.borderColor = TFT_BLACK;
             scanner->scanButton.text.textColor     = TFT_BLACK;
             scanner->scanButton.callRender();
             break;
         case TouchStatus_READY:
             scanner->scanButton.background.fillColor = Nexus::isScanComplete ? TFT_GREEN : TFT_DARKGREY;
             scanner->scanButton.background.borderColor = TFT_BLACK;
             scanner->scanButton.text.textColor = TFT_BLACK;
             scanner->scanButton.callRender();
             break;
         default:
             break;
     }
 }
 
 /**
  * @brief Handler for Next button touches.
  * - Only active once a gun & vest are selected (canNext())
  * - PRESS/RELEASE: tint button, on RELEASE advance to PlayerSetup
  */
 void onNextButtonTouch(ivec2 point, TouchStatus status) {
    // Set the NextButtonPressed flag to true when the button is pressed

    scanner->NextButtonPressed = false;
    // Show button is disabled if no gun/vest selected
     if (!scanner->canNext()) {
         scanner->nextButton.background.fillColor = TFT_DARKGREY;
         scanner->nextButton.background.borderColor = TFT_BLACK;
         scanner->nextButton.text.textColor = TFT_BLACK;
         scanner->nextButton.callRender();
         return;
     }

     // Change button color based o
     switch (status) {
         case TouchStatus_PRESS:
             scanner->NextButtonPressed = true;
             scanner->nextButton.background.fillColor = TFT_MAROON;
             scanner->nextButton.background.borderColor = TFT_YELLOW;
             scanner->nextButton.text.textColor = TFT_YELLOW;
             scanner->nextButton.callRender();
             break;
         case TouchStatus_RELEASE:
             scanner->nextButton.background.fillColor = TFT_ORANGE;
             scanner->nextButton.background.borderColor = TFT_BLACK;
             scanner->nextButton.text.textColor = TFT_BLACK;
             scanner->nextButton.callRender();

             GUI::gameDevices.clear();
            for (int i = 0; i < 9; i++) {
                uint8_t g = scanner->deviceBoxes[i]->deviceGroup;
                if ((g == NEXUS_GROUP_GUN || g == NEXUS_GROUP_VEST) && scanner->deviceBoxes[i]->selected) {
                    NexusAddress addr = { NEXUS_PROJECT_ID, g, (uint8_t)scanner->deviceBoxes[i]->deviceId };
                    GUI::gameDevices.addend(addr);
                }
            }
             playerSetup->init();
             GUI::selectActivity(GUI_Manager_Activity::PLAYERSETUP);
             break;
         case TouchStatus_READY:
             scanner->nextButton.background.fillColor = TFT_ORANGE;
             scanner->nextButton.background.borderColor = TFT_BLACK;
             scanner->nextButton.text.textColor = TFT_BLACK;
             scanner->nextButton.callRender();
             break;
         default:
             break;
     }
 }
 
 /**
  * @brief Handler for DeviceBox touches.
  * - Toggles selection on PRESS
  * - If newly selected and is gun/vest, sends a “mark” packet to that device
  */
 void onDeviceBoxTouch(ivec2 point, TouchStatus status) {
     if (status != TouchStatus_PRESS) return;
     for (int i = 0; i < 9; i++) {
         auto *box = scanner->deviceBoxes[i];
         if (box->visible && box->inRange(point)) {
             box->invertSelected();
             uint8_t g = box->deviceGroup;
             if (g == NEXUS_GROUP_GUN || g == NEXUS_GROUP_VEST) {
                NexusAddress addr{NEXUS_PROJECT_ID, g, (uint8_t)box->deviceId};
                if (box->selected) {
                    // Send a mark packet to the selected device
                    Nexus::sendData(COMMS_MARK, 0, nullptr, addr);
                } else {
                    // Send a demark packet to the unselected device
                    Nexus::sendData(COMMS_DEMARK, 0, nullptr, addr);
                }
             } else {
                box->setSelected(false);
             }
             break;
         }
     }
     GUI::callRender();
 }

 void initScanner() {
    scanner->init();
} 

 /**
  * @brief Simulate a touch on the scanner button.
  * - Used to trigger a scan from the main loop
  */
 void triggerScanner() {
    GUI::screen.executeTouch(
        scanner->scanButton.getPosition()
        , TouchStatus::TouchStatus_RELEASE
    );

    for (int i = 0; i < 9; i++) {
        scanner->deviceBoxes[i]->visible = false;
        scanner->deviceBoxes[i]->setSelected(false);
    }
 }
 
 #endif // SCANNER_HPP 