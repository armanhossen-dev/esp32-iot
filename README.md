## ESP32 simulator Websites . . .      
[![1. wokwi](https://img.shields.io/badge/WOKWI-1a1a1a?style=for-the-badge)](https://wokwi.com/)
[![2. velxio](https://img.shields.io/badge/velxio-0c2950?style=for-the-badge)](https://velxio.dev/)

# ESP_32 & More     

<!--
![ESP32](https://img.shields.io/badge/ESP32-f84225?style=for-the-badge)
![OLED](https://img.shields.io/badge/OLED-23306b?style=for-the-badge)
![Buzzer](https://img.shields.io/badge/Buzzer-5d6679?style=for-the-badge)
![Wi-Fi](https://img.shields.io/badge/Wi--Fi-00f800?style=for-the-badge)
![Side](https://img.shields.io/badge/S-Side%20Project-39d353?style=for-the-badge)
-->

09-Aug-2026 -- day1    
10-Aug-2026 -- day2 [![Real-Time Digital Clock](https://img.shields.io/badge/Real--Time_Digital_Clock-209be9?style=for-the-badge)](https://github.com/armanhossen-dev/esp32/tree/main/OLED_ESP32/Real-TimeClock)    
11-Aug-2026 -- day3 [![Dinogame](https://img.shields.io/badge/Dinogame-6f6f6f?style=for-the-badge)](https://github.com/armanhossen-dev/esp32/tree/main/game/dinogame)     
12-Aug-2026 -- day4 [![Desk Buddy](https://img.shields.io/badge/Desk_Buddy-f14c07?style=for-the-badge)](https://github.com/armanhossen-dev/esp32/tree/main/Desktop)     
12-Sep-2026 -- day5 [![Upgrade: Clock Buzzer Alarm](https://img.shields.io/badge/Upgrade-Clock_Buzzer_Alarm-209be9?labelColor=39d353&style=for-the-badge)](https://github.com/armanhossen-dev/esp32-iot/blob/main/OLED_ESP32/Real-TimeClock/esp32_clock_alarm2.ino)     

--- 

# Troubleshooting Note:
### Missing ESP32 Serial Port (macOS / CH340)       
If `/dev/cu.usbserial-*` or `/dev/cu.wchusbserial-*` fails to appear under **Tools > Port**:

1. **Unplug** the ESP32 from USB.
2. Press and **HOLD** the `BOOT` (IO0) button on the board.
3. **Plug in** the USB cable while continuing to hold `BOOT`.
4. Hold for **3 seconds**, then release.
5. Open **Tools > Port** in Arduino IDE and select the detected serial port.
