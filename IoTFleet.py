import requests

class IO:
    def __init__(self, mode, type, pin, ip):
        self.mode = mode
        self.type = type
        self.pin = pin
        self.ip = ip

        response = requests.post(
            f"http://{self.ip}/initPin", 
            params={'type': self.type, 'pin': self.pin, 'mode': self.mode}
        )
        
        if response.status_code != 200:
            raise Exception(f"Failed to initialize pin {self.pin}. Status code: {response.status_code}")
        
        print(response.text)

    def __setattr__(self, name, value):
        if name == 'state':
            value = int(value)

            response = requests.post(
                f"http://{self.ip}/setPin", 
                params={'type': self.type, 'pin': self.pin, 'state': value}
            )
            
            if response.status_code != 200:
                raise Exception(f"Failed to set state {value} for pin {self.pin}. Status code: {response.status_code}")
            
            self.__dict__['state'] = value
            print(response.text)
        else:
            super().__setattr__(name, value)

    def __str__(self):
        if self.mode == 'IN':
            response = requests.get(
                f"http://{self.ip}/pinState", 
                params={'type': self.type, 'pin': self.pin}
            )
            
            if response.status_code != 200:
                raise Exception(f"Failed to get state for pin {self.pin}. Status code: {response.status_code}")
            
            state = int(response.text)
        elif self.mode == 'OUT':
            state = self.__dict__.get('state', 0)
        else:
            state = 0

        return str(state)

    def __int__(self):
        return self.__dict__.get('state', 0)
