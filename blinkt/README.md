# blinkt

Turning the blinkt on and off.

## Setup

Create menu item Accessories > Pimoroni Dashboard (seems optional):
```
sudo apt-get install pimoroni
```

Install the actual software:
```
sudo apt-get install python3-blinkt
```

## systemd setup

If you want light 0 to be green when the system is running, execute the following:

```
./install-service
sudo systemctl enable blinkt.service
sudo systemctl start  blinkt.service
```
