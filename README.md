Project LazerTag v1.0.1 - Made By Roi Attias

📝 Enhancements and Fixes:

Added clear function in the Pushbutton class.

Fixed an issue causing all guns to shoot in full auto; now only guns explicitly configured for full auto will behave accordingly.

Introduced Demark animation for Gun and Vest subsystems (Ring subsystem).

Added new communication commands: COMMS_DEMARK and COMMS_GUNNAME.

Manager now sends a Demark command upon device deselection in the Scanner activity.

Manager sends a Mark command when the user selects Player1/2 titles in PlayerSetup and Gameplay activities.

Guns are organized into an array with corresponding gun names.

Gun names are now stored within the Player class.

Gun names are communicated to each gun device at game start.

Game start conditions updated to ensure unique guns and vests for each player.

UI fixes and improvements implemented.

Automatic scanning triggered when returning from PlayerSetup to Scanner.

Added a new parameter to the Gradient element.

Implemented Gun Setup activities.

Scanner automatically initiates scanning after initial usage.

Version information added to the Activation screen.

Nexus scan duration optimized from 2000ms to 500ms.

Introduced clear address functions within the Player class for gun and vest addresses.

Resolved a device box duplication error in PlayerSetup.

Gameplay activity now sends a Mark command when interacting with player titles and HP bars.

Corrected pregame reload logic for guns with more bullets than magazine capacity.

Added textOffset parameter to the Textbox element.

Polygon element vertex mapping fixed.

Completed GunSetup activities implementation.

Increased Gun NeoPixel brightness from 20 to 100.

Gradient background issue fixed in Gameplay activity.

Rendering call fixed when transitioning from the second GunSetup activity.

🐞 Known Issues:

Manager: Rendering issue when transitioning to/from Player 1 Gun Setup activity.

Vest: HP animation does not trigger at the start of the game.
