# clockesp

Uses esp8266, zeroseg

Schematics d5.115




```
jupyter nbconvert --to script clockesp.ipynb 
```

## Status codes

```
401 Initiating Wifi connect
402 Initiating NTP update
403 NTP update complete
404 Programming mode
```

## Programming mode

Normal operation is not started, but the device is put into programming mode. 
To activate programming mode, hold down right switch whilst turning on the 
device.

## Setup of ltp (local time protocol) daemon

Check that systemd has enabled rc.local:
```
systemctl status rc-local.service
```

sudo vim /etc/rc.local:
```
nohup python3 /home/mcarter/repos/rpi/clockesp/ltpd.py &
exit 0
```

Set permissions:
```
sudo chown root /etc/rc.local
sudo chmod 755 /etc/rc.local
```

If the process is already hogging the port:
```
fuser -k 1762/tcp
```

Test it:
```
/etc/rc.local
telnet pi2 1762

```

Reboot and see if telnetting is still OK.

## See also


* <li><a href="https://gist.github.com/blippy/ea7709597cae84c3bcde02d2bc18b1e5">lcd-clock-esp.py</a> includes fancy LCD commands</li>
