This library aims to simplify automation across various devices and interfaces, with a focus
on minimal code requirements. It's ideal for home automation being a good fit for people
with no prior experience in the domain and also serves as solid foundation for advanced
IoT projects utilizeing multiple devices, enabling seamless communication through the 
HTTP protocol.

Actively working on the [FRL1.0](# "FIRSTRELEASE1.0")
 build, which consists of a client side html,css,js ui,
with functionality for connecting and initializeing inputs and outpus on connected esp32
devices, with the posibility of controlling digital, analog IO, and pulse-width-modulation,
other posibilities are also possible, and I accent propositions, feature requests, and external
commits and merges :).

Active implementation initializes the digital outputs added to the outputs[] array,
and creates https endpoints at [deviceIp](# "your esp32`s ip, outputed in serial monitor when running the src/main.cpp on the esp32")/d[x](# "digital pin number")/[on](# "on or off to set the pin to high or low"), to turn the certain pin to high or low which, can be used,
to control all sorts of devices. This can be used with relays to turn on and off any device,
or in combination with other sensors or electronics, and a ui is implemented from which a person can register new digitalOutPins and toggle
their state, to be noted that right now before using the ui the pins must be added to output[] in src/main.cpp. 
