import requests

class Output:
    def __init__(self, pin, ip):
        self.pin = pin
        self.ip = ip

        response = requests.post(
            f"http://{self.ip}/initPin", 
            params={'type': 'd', 'pin': self.pin, 'mode': 'OUT'}
        )
        
        if response.status_code != 200:
            raise Exception(f"Failed to initialize pin {self.pin}. Status code: {response.status_code}")
        
        print(response.text)

    def __setattr__(self, name, value):
        if name == 'state':
            response = requests.post(
                f"http://{self.ip}/setPin", 
                params={'type': 'd', 'pin': self.pin, 'state': value}
            )
            
            if response.status_code != 200:
                raise Exception(f"Failed to set state {value} for pin {self.pin}. Status code: {response.status_code}")
        
            print(response.text)
        else:
            super().__setattr__(name, value)

output_pin = Output(pin=3, ip='192.168.1.140')
output_pin.state = 1  # This will set the pin state to 1 and print the response