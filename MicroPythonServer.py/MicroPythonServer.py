from MicroAPIgRESTion import *
from machine import Pin
from machine import ADC

@POST('/initializeDigitalPin', 'pin', 'mode')
async def digitalPin_init_handler(pin, mode):
    
    pin = int(pin)
    
    if mode == 'input':
        Pin(pin, Pin.IN)
    elif mode == 'output':
        Pin(pin, Pin.OUT)
    else:
        return "Invalid"
    
    return f"Digital pin nr.{pin} initialized as {mode}"

@POST('/digitalOutput', 'pin', 'state')
async def digitalPin_out_handler(pin, state):
    
    pin = int(pin)
    
    if state == 'high':
        Pin(pin).value(1)
    elif state == 'low':
        Pin(pin).value(0)
    else:
        return "Invalid"
    
    return f"Pin nr.{pin} set to {state}"

@GET('/digitalInput', 'pin')
async def digitalPin_in_handler(pin):
    
    pin = int(pin)
    
    pin_status = "HIGH" if Pin(pin).value() else "LOW"
    
    return pin_status

@GET('/analogInput', 'pin', 'type', 'precision')
async def analogPin_in_handler(pin, type, precision):
    
    pin = int(pin)
    
    return map_u16AnalogIn_to_voltage(ADC(Pin(pin)).read_u16())

def map_u16AnalogIn_to_voltage(value):
    scaled_value = value / 65535.0
    mapped_value = scaled_value * 3.3
    return mapped_value

asyncio.run(main())