Changes:
4/5
- added clear function in pushbutton class
- fixed in issue when all guns shot full auto, now only those that full auto enabled can do that
- added demark animation for gun and vest (ring subsystem)
- added commands "COMMS_DEMARK" and "COMMS_GUNNAME"
- manager now sends the demark command when user deselect devices in the scanner activity
- manager now sends the mark command when user presses on the player1/2 title in the PlayerSetup and Gameplay activities
- put guns in array and added gun name array
- gunName is saved inside player class
- the gun name command is send to each gun device when the game begin

5/5
- updated game start conditions: now ensures that guns are different and vest are different
- fixed ui problems

7/5
- going back from PlayerSetup to Scanner trigger scan automatically
- added parameter to Gradient element
- added Gun Setup activities

8/5
- Scanner now automatically scans if its not the first time switching to Scanner
- anded version to activation screen
- changed nexus scan time from 2000ms to 500ms
- added clear address functions in player, one for gun address and one for vest address
- fixed player setup error of devicebox multiplication
- in gameplay activity, now both player title and hp bar sends mark command to the player devices
- fixed pregame reloading for cases with the gun having more bullets that the magazine
- added textOffset to Textbox
- fixed polygon element's vertex mapping
- finished GunSetup activities
10/5
- Increased gun neopixel brightness from 20 to 100
- fixed again gradient background to gameplay activity
- fixed calling render when switching from gun setup activity