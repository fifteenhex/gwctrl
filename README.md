[![Build Status](https://travis-ci.com/fifteenhex/gwctrl.svg?branch=master)](https://travis-ci.com/fifteenhex/gwctrl)

This is a simple MQTT remote control interface for a LoRaWAN or similar gateway.


# Heartbeats

The gateway will periodically send heartbeats with some helpful meta-data to 
`gwctrl/<gwid>/heartbeat`.


```json
{
  "location" : {
    "lat" : 0,
    "lon" : 0
  },
  "thermal" : {
    "sensors" : [
      {
        "name" : "28-0417619818ff",
        "millidegrees" : 24062
      }
    ]
  },
  "sysinfo" : {
    "uptime" : 670858
  }
}
```

# Gateway control commands

## Rebooting

```
gwctrl/<gwid>/ctrl/reboot
```
