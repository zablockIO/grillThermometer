# **Grill Thermometer**

Grill thermometer with UI using the STM32G070 board thermistor, Type-K thermocouple, and pre-fabricated PCB mount with LEDs, buttons, buzzer, and SSD1306 OLED display. Uses thermistor and thermocouple for measuring the temperature using the ADC to convert voltage and counts to Celsius. Two point calibration was used to improve temperature reading.

Two points  
1. Theoretical Low = 6 degreeC
2. Theoretical High = 94 degreeC
3. Actual Low = -6 degreeC
4. Actual High = 92 degreeC 

Thermometer can measure in both Celsius and Farenheit and is preloaded with Beef, Fish, Lamb, Pork, and Poultry target temperatures for various cuts and doneness. Four custom temperature slots are also available via serial connection using UART. UART protocal can also display target temperature and current temperatures as well as list the loaded custom options. Green LED and buzzer will activate when set target temperature is reached, and blue LED will ramp when target temperature is set but measured temperature is lower. Buttons are utilized to navigate the UI and set, clear, or change target temperature and units. 

## Preloaded meat cuts

| MEAT | OPTIONS |
| ---- | ------- |
| Beef | GROUND, RARE, MEDIUM RARE, MEDIUM, MEDIUM WELL, WELL DONE
| Fish | TUNA & SWORDFISH, OTHER FISH
| Lamb | CHOPS, RARE, MEDIUM RARE, MEDIUM, MEDIUM WELL, WELL DONE
| Pork | RIBS & ALL ELSE, MEDIUM, WELL DONE
| Poultry | GROUND, ALL CUTS