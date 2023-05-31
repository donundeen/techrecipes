# Using PiJuice

PiJuice is a great piece of hardware for providing uninterrupted power for your pi. Some cool things it can do:
- It provides a battery backup to a plugged-in pi, so a brief unplug doesn't shut down the systen
- It can provide full power to not only the pi, but also other devices. 
- It can provent your pi from becoming "underpowered" which can result in losing Wifi, SSH connections, etc.
- It provides external buttons that can shutdown, start, reboot, etc the Pi. These buttons can be programed to run arbitrary commands on the pi as well.
- It can work with different types of batteries

purchase/info links
https://github.com/PiSupply/PiJuice
https://uk.pi-supply.com/products/pijuice-standard
https://www.pishop.ca/product/pijuice-hat-a-portable-power-platform-for-every-raspberry-pi/

# Installing PiJuice Software
`sudo apt-get install pijuice-base`

run pijuice cli
`pijuice_cli` and look at all the possibilities


# getting better power support
Use TWO power supplies: run one to the PiJuice, and one to the POWER (outer) microusb port on the Pi0. 

# provide power to the "power switch" pins
this lets you power other things without drawing from the pi's power
there are two pins under the PiJuice: power and ground. Attach these to the device you want to power.

run `pijuice_cli`
`Change Power Switch` -> set the value to a current level you need for the device you're attaching
