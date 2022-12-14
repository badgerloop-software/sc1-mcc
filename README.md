# Solar Car 1 Motor Controller Controller
*Author: Wilson Guo. \
 Mbed Studio Instructions stolen from pod-embedded-nucleo (Authors: Ezra Boley, Eric Udlis)*

# Getting Started

### Mbed Studio
As with any project, there are a million ways to flash an ARM chip. I prefer Mbed Studio because of it's all in one approach to building and flashing our STM32s under the hood. If you would like to use the CLI or some other method, feel free.
To get started with Mbed Studio follow this quick guide
1. Install [Mbed Studio](https://os.mbed.com/studio/)
2. Create an [Mbed OS Account](https://os.mbed.com) - They don't send emails so thumbs up for that
3. Log into Mbed studio with your mbed os Account
4. Click File -> Import Program, put the git clone URL in the URL field
5. Install Mbed OS libraries in the bottom libraries tab if needed. Sometimes on first clone an update is needed.
7. Set the target as the "NUCLEO-L432KC"
8. Enjoy programming your Mbed enabled STM Nucleo


### Current Pin Usage. Check [MCC confluence page](https://badgerloop.atlassian.net/wiki/spaces/EL/pages/167378955/Motor+Controller+Controller+Rev+A+Design) for updates
| Pin ID  | Type  | Usage   | Usage   | Type  | Pin ID|
| :---:   | :---: | :---:   | :---:   | :---: | :---: |
| PA9     | Dig   | Speed   |         |       | VIN   |
| PA10    | Dig   |         |         |       | GND   |
| NRST    |       |         |         |       | NRST  |
| GND     |       |         |         |       | 5V    |
| PA12    | Dig   | CANTX   |         | Alg   | PA2   |
| PB0     | Dig   | CrzA    |         | Alg   | PA7   |
| PB7     | Dig   | CrzB    |         | Alg   | PA6   |
| PB6     | Dig   | Crz Set | Acc Out | Alg   | PA5   |
| PB1     | Dig   | Crz Rst |         | Alg   | PA4   |
| D7      | Dig   | Power   |         | Alg   | PA3   |
| D8      | Dig   | Dir     | Brk Sig | Alg   | PA1   |
| PA8     | Dig   | Eco     | Acc Sig | Alg   | PA0   |
| PA11    | Dig   | CANRX   |         |       | AREF  |
| PB5     | Dig   | MC Stat |         |       | 3V3   |
| PB4     | Dig   | Brake   |         | Dig   | PB3   |
