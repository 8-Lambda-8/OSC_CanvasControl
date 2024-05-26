# OSC Canvas Control

WT32-eth01 project for positon controlling a motorized canvas


Canvas Port: 8001
Console Port: 8000

## OSC Commands

Positions are managed as seconds motor switched on

### /CanvasControl/move/to
moves to position given as integers

### /CanvasControl/move/by
moves by distance given as integers

### /CanvasControl/get
returns the current position value as integer

### /CanvasControl/setHome
returns the position to 0

