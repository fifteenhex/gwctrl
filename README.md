[![Build Status](https://travis-ci.com/fifteenhex/gwctrl.svg?branch=master)](https://travis-ci.com/fifteenhex/gwctrl)

This is a simple MQTT remote control interface for a LoRaWAN or similar gateway.


# Heartbeats

The gateway will periodically send heartbeats with some helpful meta-data

```
gwctrl/<gwid>/heartbeat
```

# Gateway control commands

## Rebooting

```
gwctrl/<gwid>/ctrl/reboot
```
