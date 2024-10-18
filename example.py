from IoTFleet import IO

output_pin = IO('OUT', 'd', 3, '192.168.1.140')
print(output_pin) # print pin state
output_pin = 1 # set pin state to 1
print(output_pin) # print pin state

input_pin = IO('IN', 'd', 2, '192.168.1.140')
print(input_pin) # print pin state