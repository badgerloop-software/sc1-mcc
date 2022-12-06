# MCC Analog, CAN, and GPIO Implementation Details
Signals and pins are grouped by type. [Mbed OS](https://os.mbed.com/docs/mbed-os/v6.15/apis/index.html) is used to abstract pin configuration and read/write operations from registers. Generally, the following steps will be followed for any given signal.

1. Determine the pin the signal is on.
2. Determine the type of the signal. This will dictate the proper file the signal's code should be included in. 
3. Determine the appropriate MbedOS class to declare the pin as an instance of. Will likely be in the *Input/Output drivers* or *Other drivers* categories. 
4. Declare it as such and add any additional setup to the **init** function.
5. Determine the datastructure where the current status of the signal to be stored. Analog will likely need a new float. GPIO will use a slot in the bitmap. If a new datastructure is required and the message is needed by other boards, a unique CAN offset will need to be assigned to the signal. See **can.h** for more details
6. Add code to update reads/writes on the pin as necessary. 
7. Add details about your new signal to the comments in the proper header files. If a unique CAN offset was assigned, add the address to the bottom of the README