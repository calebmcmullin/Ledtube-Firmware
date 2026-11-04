# LED Tube — WeMos D1 Mini

Firmware for a **WeMos D1 Mini (ESP8266)** controlling **60 WS2812B LEDs** over Wi-Fi/UDP.

## Flashing

1. Install [Arduino IDE](https://www.arduino.cc/en/software).
2. In **Preferences → Additional Boards Manager URLs**, add:
   ```text
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
3. In **Boards Manager**, install **esp8266 by ESP8266 Community**. In **Library Manager**, install **FastLED**.
4. Put `led_tube.ino` and `Animator.h` in the same folder named `led_tube`.
5. Edit the Wi-Fi names/passwords (`ssid1`, `password1`, `ssid2`, `password2`) in `led_tube.ino`. Use 2.4 GHz Wi-Fi.
6. Connect the D1 Mini over USB. Select **Tools → Board → ESP8266 Boards → LOLIN(WEMOS) D1 R2 & mini**, then select its port.
7. Click **Upload**. Open **Serial Monitor** at **115200 baud** to see the IP address.

## Wiring

| WeMos D1 Mini | LED strip |
|---|---|
| D2 (GPIO4) | DIN |
| GND | GND |

Power the LEDs from a suitable **external 5V supply** and connect its GND to the D1 Mini GND. Don't power 60 LEDs from the D1 Mini's 5V pin.

## UDP controls

Send text commands to the device's IP address on **UDP port 4210**:

| Command | Action |
|---|---|
| `animation:1;` through `animation:6;` | Select animation |
| `hue:128;` | Set hue (0–255) |
| `doubleTime:;` | Faster |
| `halfTime:;` | Slower |
| `defaultTime:;` | Reset speed |
| `LEDPing:;` | Replies `PONG:;` |

**Note:** Remove real Wi-Fi passwords from the source before publishing it.
