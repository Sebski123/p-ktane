# P-KTANE
## Sources
* https://github.com/dlareau/KTANE-physical
* https://imgur.com/gallery/J6HdM0v
* https://old.reddit.com/r/ktane/comments/a3fgyt/i_made_a_physical_version_of_the_video_game_keep/
* https://github.com/jgillick/avr-discobus


## Modules
* [Wires](https://i.imgur.com/xLXpCYH.png)              (MAIN)
* [Button](https://i.imgur.com/Leew7du.png)             (MAIN)
* [Keypads](https://i.imgur.com/Zy6ScEo.png)
* [Simon Says](https://i.imgur.com/uX7BzED.png)         (MAIN)
* [Who's on First](https://i.imgur.com/YisE1go.png)
* [Memory](https://i.imgur.com/bvGt5gz.png)             (MAIN)
* [Morse Code](https://i.imgur.com/V4XAdVZ.png)
* [Complicated Wires](https://i.imgur.com/jJb2Kp9.png)
* [Wire Sequences](https://i.imgur.com/1drzKTO.png)
* [Mazes](https://i.imgur.com/xXMdvLr.png)              (MAIN)
* [Passwords](https://i.imgur.com/us0XRBs.png)


### Needy Modules
* [Venting Gas](https://i.imgur.com/f2eua3p.png)
* [Capacitor Discharge](https://i.imgur.com/Q0VJkoo.png)
* [Knobs](https://i.imgur.com/hlpkEUD.png)

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