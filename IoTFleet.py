import requests

class Output:
    def __init__(self, type, pin, ip):
        self.type = type
        self.pin = pin
        self.ip = ip

        response = requests.post(
            f"http://{self.ip}/initPin", 
            params={'type': self.type, 'pin': self.pin, 'mode': 'OUT'}
        )
        
        if response.status_code != 200:
            raise Exception(f"Failed to initialize pin {self.pin}. Status code: {response.status_code}")
        
        print(response.text)

    def __setattr__(self, name, value):
        if name == 'state':
            response = requests.post(
                f"http://{self.ip}/setPin", 
                params={'type': self.type, 'pin': self.pin, 'state': value}
            )
            
            if response.status_code != 200:
                raise Exception(f"Failed to set state {value} for pin {self.pin}. Status code: {response.status_code}")
        
            print(response.text)
        else:
            super().__setattr__(name, value)
