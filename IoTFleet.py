import requests
import time

class digital:
    def __init__(self, pin, mode, ip):
        self.pin = pin
        self.mode = mode
        self.ip = ip
        
        requests.post(f"http://{self.ip}/initPin", json={'type': 'd', 'pin': self.pin, 'mode': self.mode})

        if self.mode == 'IN':
            self.__class__.__getattr__ = self._getattr
        elif self.mode == 'OUT':
            self.__class__.__setattr__ = self._setattr
        else:
            raise ValueError("Invalid mode. Use 'IN' or 'OUT'.")

    def _setattr(self, name, value):
        if name == 'state':
            response = requests.post(f"http://{self.ip}/setPin", json={'type': 'd', 'pin': self.pin, 'state': value})
            if response.status_code != 200:
                raise Exception("Failed to write pin state")
        else:
            super().__setattr__(name, value)

    def _getattr(self, name):
        if name == 'state':
            response = requests.get(f"http://{self.ip}/pinState", params={'type': 'd', 'pin': self.pin})
            if response.status_code == 200:
                return response.json()
            else:
                raise Exception("Failed to read pin state")
        else:
            return super().__getattr__(name)

# Example usage
# For an input pin
input_pin = digital(pin=2, mode='IN', ip='192.168.1.140')
print(input_pin.state)  # This will read the pin state

# For an output pin
output_pin = digital(pin=3, mode='OUT', ip='192.168.1.140')
output_pin.state = 1  # This will write to the pin