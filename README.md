# IoTFleet.py

Python version of IoTFleet. 

IoTFleet enables control over multiple microcontroller devices running micropython like the Esp32 or Raspberry Pi Pico from a PC remotely.

# Features
- Remotely control multiple slave micropython device over air from a master python device
- Digital & Analog pins supported, analog is experimental
- Coming soon, over-air micropython device code change, etc...

## Slave Device Setup
To get started, insert your wifi credential into the file `config.py` then save `MicroAPIgRESTion.py`, `main.py` & `config.py` onto the slave device.
```
config.py
```
```python
SSID = "SSID" # Replace SSID with your wifi SSID
PASS = "PASS" # Replace PASS with PASSWORD
```
The ip of the device is being output in serial when the device connects to WI-Fi.

## Using slave devices via master device

```python
from IoTFleet import IO

output_pin = IO('OUT', 'd', 3, '192.168.1.140') # initialize digital pin 3 of the device with the ip '192.168.1.140' as output

print(output_pin) # print cached pin state, initially 0, updates with each set
output_pin = 1 # set pin state to 1
print(output_pin) # print newly cached pin state

input_pin = IO('IN', 'a', 2, '192.168.1.140') #initialize analog pin 2 of the device with the ip '192.168.1.140' as input

print(input_pin) # get pin state and print
```
