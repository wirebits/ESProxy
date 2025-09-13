# 📶ESProxy
An ESP32 tool which locate BLE devices using proximity.

# ✨Features
- Minimal Setup.
- Simply controlled by Serial Console.
- It shows distance and stenght of BLE devices.
- It scans and tracks contineously.

# 👍Recommended
- Use those ESP32 boards which has at least `4MB` flash memory.

# 📦Requirements
- `1` NodeMCU ESP-32S V1.1 38-Pins
- `1` Micro-B USB / Type-C USB Cable with data transfer support

# ⚙️Setup `ESProxy`
## By Arduino
1. Download `Arduino IDE 2.X.X` from [here](https://www.arduino.cc/en/software/) according to your Operating System.
2. Install it.
3. Go to `File` → `Preferences` → `Additional Boards Manager URLs`.
4. Paste the following link :
```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```
5. Click on `OK`.
6. Go to `Tools` → `Board` → `Board Manager`.
7. Wait for sometimes and search `esp32` by `Espressif Systems`.
8. Simply install latest version.
   - Wait for sometime and after that it is installed.
9. Done! Arduino IDE with required boards and libraries is ready.
10. Download or Clone the Repository.
11. Open the folder and then open `ESProxy` folder and just double click on `ESProxy.ino` file.
   - It opens in Arduino IDE.
15. Compile the code.
16. Select the correct board from the `Tools` → `Board` → `esp32`.
  - It is generally `NodeMCU-32S`.
17. Select the correct port number of that board.
18. Upload the code.
   - Wait for sometime to upload.
19. Done!

# 🏃🏻‍♂️Run `ESProxy`
2. Open Serial Console from [here](https://wirebits.github.io/SerialConsole/).
3. Select baud rate to `115200`.
4. Click on `Connect` button.
5. In pop-up, select your device.
6. Type `help` and hit enter to get list of supported commands.
