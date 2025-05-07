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

6/5
- 