# MCC Analog, CAN, and GPIO Implementation Details
Signals and pins are grouped by type. [Mbed OS](https://os.mbed.com/docs/mbed-os/v6.15/apis/index.html) is used to abstract pin configuration and read/write operations from registers. Generally, the following steps will be followed for any given signal.

1. Determine the pin the signal is on.
2. Determine the type of the signal. This will dictate the proper file the signal's code should be included in. 
3. Determine the appropriate MbedOS class to declare the pin as an instance of. Will likely be in the *Input/Output drivers* or *Other drivers* categories. 
4. Declare it as such and add any additional setup to the **init** function.
5. Determine the datastructure where the current status of the signal to be stored. Analog will likely need a new float. GPIO will use a slot in the bitmap. If a new datastructure is required and the message is needed by other boards, a unique CAN offset will need to be assigned to the signal. See **can.h** for more details
6. Add code to update reads/writes on the pin as necessary. 
7. Add details about your new signal to the comments in the proper header files. If a unique CAN offset was assigned, add the address to the bottom of the README



# Main Motor State Machine

## States

### Off (0)

Should be in this state until the car is powered and the start motor signal from the switch is received. Enter idle mode with direction forward.

### Idle (1)

Will be used as "neutral" state as safety between Forward and Reverse. State can be entered from main on motor start signal. Must be in this state to change motor direction. Automatically enter this state from Forward and Reverse states once Power (or speed?) is below a threshold configured at boot determined by high voltage. Automatically leave this state once the accelerator pedal is pressed. Can enter off mode if motor off switch signal received.

### Forward (2)

Normal driving state with motor acceleration voltage controlled by pedal in forward direction. State entered when forward is chosen in idle and accelerator pedal is engaged. Allow enabling of both cruise control modes from this state. Return to this state when cruise control is disabled. 

### Reverse (3)

Normal driving state with motor acceleration voltage controlled by pedal in reverse direction. State entered when reverse is chosen in idle and accelerator pedal is engaged. DO NOT allow cruise control to be entered from this mode. Possible speed limiter. 

### Cruise Control (4)

Driving mode where acceleration voltage is controlled by PID (Another state machine TBD). Enterable from Forward state only by Crz on signal. Returns to Forward immediately on off signal or braking. DO NOT allow direct switching between cruise control modes. 