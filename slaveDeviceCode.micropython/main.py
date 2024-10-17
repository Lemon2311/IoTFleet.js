# import lib
from MicroAPIgRESTion import *
# importing Pin for use in future handler
from machine import Pin, ADC

# using device pins
@POST('/initPin', 'type', 'pin', 'mode')
async def initPin(type, pin, mode):
    
    pin = int(pin)
    
    if mode == 'IN':
        if type == 'd':
            Pin(pin, Pin.IN)
        elif type != 'a':
            return "Invalid type"
                
    elif mode == 'OUT':
        if type == 'd':
            Pin(pin, Pin.OUT)
        elif type != 'a':
            return "Invalid type"
  
    else:
        return "Invalid mode"
  
    return f"{type}{pin} initialized as {mode}"

@POST('/setPin', 'type', 'pin', 'state')
async def setPin(type, pin, state):
    
    pin = int(pin)
    state = int(state)

    if state > 0:
        if(type == 'a'):
            Pin(pin).value(state)
        elif(type == 'd'):
            Pin(pin).value(1)
        else:
            return "Invalid type"
    
    elif state <= 0:
        if(type == 'a'):
            Pin(pin).value(state)
        elif(type == 'd'):
            Pin(pin).value(0)
        else:
            return "Invalid type"
    
    return f"{type}{pin} set to {state}"

@GET('/pinState', 'type', 'pin')
async def pinState(type, pin):
    
    pin = int(pin)
    
    if type == 'd':
        return Pin(pin).value()
    elif type == 'a':
        return ADC(Pin(pin)).read()
    else:
        return "Invalid type"

asyncio.run(main())
