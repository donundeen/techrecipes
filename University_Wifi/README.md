Setting up WIFI on a university WIFI system 

(in this case, Concordia University)

`sudo emacs /etc/wpa_supplicant/wpa_supplicant.conf`
add:
```
network={ 
ssid="ConcordiaUniversity" 
scan_ssid=1 
key_mgmt=WPA-EAP 
pairwise=CCMP TKIP 
group=CCMP TKIP 
eap=PEAP 
identity="<my university login>" 
password="<my University password>" 
phase1="peapver=0" 
phase2="MSCHAPV2" 
}
```

Reboot:

```ping google.com``` 

to confirm wifi working

NOTE: the password above is in clear text. It's better to use an encrypted password here instead. (need to look up how I did that...)
