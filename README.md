# P-KTANE
## Sources
* https://github.com/dlareau/KTANE-physical
* https://imgur.com/gallery/J6HdM0v
* https://old.reddit.com/r/ktane/comments/a3fgyt/i_made_a_physical_version_of_the_video_game_keep/


## ReDesign
### To-do
- [ ] Add pinheaders to all schematics
- [ ] Figure out how to add battery/indicators
- [ ] Make symbol/footprint for arcade button
- [ ] Redo SimonSays schematic with above symbol
- [ ] Design PCBs
- [ ] Make more modules
- [ ] Organize Repo
- [ ] Clean up README
- [x] Choose which modules to build
- [ ] Double check value of RSet for MAX7219 ICs
- [ ] Figure out how to add radio dial to MorseCode
- [ ] Modules on back?
- [ ] Add presets from KTANE?
- [ ] Variable amount of strikes?
- [ ] Better leaderboard?
- [ ] Way to enter name? (Password module?)
- [ ] Better system for swapping modules?
- [ ] Make e-paper display
  - [ ] Expose PSU enable pin
  - [x] Add SRAM IC
  - [x] Add microSD slot?
  - [ ] White image for storage
  - [ ] Add display/controller to GxEPD2?
- [ ] Figure out how to power device
  - [ ] Wired?
  - [ ] Battery?
  - [ ] Both?
- [ ] Add protection circuits?
  - [ ] Short circuit protection?
  - [ ] Overvoltage protection?
  - [ ] Overcurrent protection?
- [ ] Add TMRpcm and SD to timer?
  - [ ] [This](https://maxoffsky.com/maxoffsky-blog/how-to-play-wav-audio-files-with-arduino-uno-and-microsd-card/)
  - [ ] Or [this](https://circuitdigest.com/microcontroller-projects/arduino-audio-music-player)


### v1 Compromises
| Module       | Problem                                              | Possible solution                              |
|--------------|------------------------------------------------------|------------------------------------------------|
| Button       | Text is separate from button instead of on top       | 3D-print text on button                        |
| Button       | Text is reduced to a single digit                    | Use changeable buttons with 3D-printed text    |
| Edgework     | Serial number is rough (16-segment display)          | Use E-paper display                            |
| Edgework     | All possible indicators are always present           | Use E-paper displays (expensive)               |
| Maze         | Hard to differentiate elements                       | Use RGB led's on PCB                           |
| Maze         | No way to see maze walls                             | Use 11x11 grid to also show walls              |
| Memory       | Press button under display instead of display itself | Place button under display and make it movable |
| Simon Says   | Rough sound                                          | Replace buzzer with proper speaker             |
| Simon Says   | Rough sound                                          | Replace tone() with better sine wave (filter?) |
| Simple wires | Wires are unplugged instead of cut                   | Use magnets to "cut" wires                     |


## Modules
| Module                                                       |Going to build| Coded | Schematic | PCB | Parts | Tested | Built | I2C addr.|
|--------------------------------------------------------------|--------------|-------|---------- |-----|-------|--------|-------|----------|
| Timer (Controller)                                           |      ✔️     |   ✔️  |    🟡    |  ❌ |  ❌  |   ❌   |  ❌  |  MASTER  |
| Config (ESP8266)                                             |      ✔️     |   ✔️  |    ✔️    |  ❌ |  ✔️  |   ✔️   |  ❌  |    0x1   |
| [Button](https://i.imgur.com/Leew7du.png)                    |      ✔️     |   ✔️  |    🟡    |  ❌ |  ❌  |   ❌   |  ❌  |    0x2   |
| [Complicated Wires](https://i.imgur.com/jJb2Kp9.png)         |      ✔️     |   ✔️  |    🟡    |  ❌ |  ❌  |   ❌   |  ❌  |    0x3   |
| [Maze](https://i.imgur.com/xXMdvLr.png)                      |      ✔️     |   🟡  |    🟡    |  ❌ |  🟡  |   ❌   |  ❌  |    0x4   |
| [Memory](https://i.imgur.com/bvGt5gz.png)                    |      ✔️     |   🟡  |    🟡    |  🟡 |  ❌  |   ❌   |  ❌  |    0x5   |
| [Morse Code](https://i.imgur.com/V4XAdVZ.png)                |      ✔️     |   ❌  |    ❌    |  ❌ |  ❌  |   ❌   |  ❌  |    0x6   |
| [Needy Capacitor Discharge](https://i.imgur.com/Q0VJkoo.png) |      ✔️     |   ✔️  |    🟡    |  ❌ |  ❌  |   ❌   |  ❌  |    0x7   |
| [Needy Knob](https://i.imgur.com/hlpkEUD.png)                |      ✔️     |   ✔️  |    🟡    |  ❌ |  ❌  |   ❌   |  ❌  |    0x8   |
| [Needy Venting Gas](https://i.imgur.com/f2eua3p.png)         |      ✔️     |   ✔️  |    🟡    |  ❌ |  ❌  |   ❌   |  ❌  |    0x9   |
| [Password](https://i.imgur.com/us0XRBs.png)                  |      ✔️     |   ❌  |    ❌    |  ❌ |  ❌  |   ❌   |  ❌  |    0xA   | 
| [Simon Says](https://i.imgur.com/uX7BzED.png)                |      ✔️     |   🟡  |    🟡    |  ❌ |  ❌  |   ❌   |  ❌  |    0xB   |
| [Simple Wires](https://i.imgur.com/xLXpCYH.png)              |      ✔️     |   🟡  |    🟡    |  ❌ |  ❌  |   ❌   |  ❌  |    0xC   |
| [Keypads](https://i.imgur.com/Zy6ScEo.png)                   |      ❌     |   ❌  |    ❌    |  ❌ |  ❌  |   ❌   |  ❌  |    N/A   |
| [Who's on First](https://i.imgur.com/YisE1go.png)            |      ❌     |   ❌  |    ❌    |  ❌ |  ❌  |   ❌   |  ❌  |    N/A   |
| [Wire Sequence](https://i.imgur.com/1drzKTO.png)             |      ❌     |   ❌  |    ❌    |  ❌ |  ❌  |   ❌   |  ❌  |    N/A   |


# How to burn bootloader and upload sketches for ATMega328
## How to burn bootloader
### Install boards
1. Open the Arduino IDE.
1. Open the **File > Preferences** menu item.
1. Enter the following URL in **Additional Boards Manager URLs**:

    ```
    https://mcudude.github.io/MiniCore/package_MCUdude_MiniCore_index.json
    ``` 

1. Open the **Tools > Board > Boards Manager...** menu item.
1. Wait for the platform indexes to finish downloading.
1. Scroll down until you see the **MiniCore** entry and click on it.
1. Click **Install**.
1. After installation is complete close the **Boards Manager** window.
1. **Note**: If you plan to use the *PB series, you need the latest version of the Arduino toolchain. This toolchain is available through IDE 1.8.6 or newer. Here's how you install/enable the toolchain:
  -  Open the **Tools > Board > Boards Manager...** menu item.
  -  Wait for the platform indexes to finish downloading.
  -  The top is named **Arduino AVR boards**. Click on this item.
  -  Make sure the latest version is installed and selected
  -  Close the **Boards Manager** window.

### Prepare Arduino
1. Copy [this](https://raw.githubusercontent.com/adafruit/ArduinoISP/master/ArduinoISP.ino) code into the Arduino IDE and upload it to an Arduino normally.
1. Connect Arduino to the ATMega as shown below (Capacitor is important).
![https://i.imgur.com/yCqd9RF.png](https://i.imgur.com/yCqd9RF.png)

### Burn bootloader
1. Select the **Tools > Board > ATmega328** menu item from the MiniCore section.
1. Select the **Tools > Variant > 328 / 328A** menu item.
1. Select the **Tools > Clock > Internal 8MHz** menu item.
1. Select the **Tools > Programmer > Arduino as ISP** menu item.
1. Click **Tools > Burn Bootloader**

## How to upload sketches
### Wiring
1. Remove ATmega328 from Arduino
1. Connect Arduino to the ATMega as shown below.
![https://i.imgur.com/xxt5Faa.png](https://i.imgur.com/xxt5Faa.png)

### Uploading
1. Select the **Tools > Board > ATmega328** menu item from the MiniCore section.
1. Select the **Tools > Variant > 328 / 328A** menu item.
1. Select the **Tools > Clock > Internal 8MHz** menu item.
1. Select the **Tools > Programmer > Arduino as ISP** menu item.
1. Click Upload.