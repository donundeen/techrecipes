# setting up AP/STA mode
This process will get you a raspberry pi that functions as a WIFI access point that you can connect to like any WiFi. 
You can then set up a webserver that people can access only when they're connected to this Wifi.


## install hostapd and dnsmasq:
```
sudo apt-get install hostapd
sudo apt-get install dnsmasq
```
OR 
```sudo apt-get install hostapd dnsmasq```

Then turn them off:
```    
sudo systemctl stop hostapd
sudo systemctl stop dnsmasq
```

## install iptables

```sudo apt-get install iptables```


## run the install ap/sta script

from: https://github.com/MkLHX/AP_STA_RPI_SAME_WIFI_CHIP
- it's not perfect, and it's being maintained and updated, so results might vary from time to time
- Note: we're using the script from our own repo, so it's stable.

```shell
cd ~/loginlog
sudo ./ap_sta_config.sh --ap [NAME of the AP]  [PW of the AP] --client [EXTERNAL WIFI AP] [EXTERNAL WIFI PW] --country CA --ip [IP OF THE AP, eg 10.0.0.203]
```

- the details of the above part might need to be customized to your situation, eg
    - change the client ssid and pw depending on the network you want the device to be able to connect to to reach the interst
    - the --ip value : maybe that need to be changed for diferent devices?


edit
`sudo nano /etc/udev/rules.d/70-persistent-net.rules`
make sure there's a closing quote at the end


## Add full path to bins in scripts
`sudo nano /bin/manage-ap0-iface.sh`
change service to /usr/sbin/service
[ fixed this in the repo, check next time then delete this step]


## Reboot

`sudo reboot`

## Test

if it worked, you should be able to see the wifi SSID log.local, connect via usb, and connect to the internet


## Backup again Once it works!

# remove pw for the AP
- edit /etc/hostapd/hostapd.conf
    `sudo nano /etc/hostapd/hostapd.conf`
- comment out all lines that start with wpa_ or rsn_
