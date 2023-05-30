# Getting Bluetooth Headphones working and (usually) auto-connecting


## Set up software
```
sudo apt install mpg123
sudo apt update
sudo apt upgrade
sudo apt install git
sudo apt install pulseaudio pulseaudio-module-bluetooth
sudo usermod -G bluetooth -a pi
pulseaudio --start
sudo reboot
```

## Get bluetooth headphones to connect
(using these instructions: https://gist.github.com/actuino/9548329d1bba6663a63886067af5e4cb)

```bluetoothctl```

this opens a [bluetooth] terminal where you type in different commands. Below are things you type in within that terminal:
```
power on
agent on
default-agent
scan on
```

Watch for the devices it finds. Take note of the headphones you’re looking for.

Might need to turn the headphones on and off again, to figure out which one it is

(you can use tab for auto-completion)

Once you've found your device, type in the below

[`<dev>` is the name of the device (usually a bunch of HEX characters like 20:45:CF:... etc]
  
```
pair <dev>
trust <dev>
connect <dev>
quit
```

## Removing a device
You may need to do this is you are switching headphones for a pi, or setting up a new cloned pi.

Withing `bluetoothctl`
```
devices
```
Find the id of the device to remove
```
remove <dev>
```

## Auto-Connect headphones on startup

edit `/etc/pulse/default.pa` , add (at the end):
```
# automatically switch to newly-connected devices
load-module module-switch-on-connect
```
edit `/etc/bluetooth/main.conf`, 
at the end of the file, add AutoEnable in the existing Policy section (this might already be set up like this):
```
[Policy]
AutoEnable=true
```

At command line, type
```
loginctl enable-linger pi
```


Edit `/lib/systemd/user/pulseaudio.service`
Add option `–exit-idle-time=-1` to ExecStart line, after –daemonize=no

