# Solar Car 1 Motor Controller Controller
*Authors: Wilson Guo, Sebastien Criqui, Declan Stanton. \
 Mbed Studio Instructions stolen from pod-embedded-nucleo (Authors: Ezra Boley, Eric Udlis)*

Repository for the Motor Controller Controller board powered by a Nucleo-L432KC. **__Running on Mbed OS v6.15.1__**

## Getting Started

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

### To import the repo
1. Go to file, then import. 
2. Paste the http github repo link in the url field.
3. I strongly recommend having a single copy of Mbed OS that is linked to and shared between projects. To do this, clone an instance of it from [the official repo](https://github.com/ARMmbed/mbed-os/tree/mbed-os-6.15.1) and link to the folder.
