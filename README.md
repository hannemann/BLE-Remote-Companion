# Bluetooth LE Remote Companion

Remotely control devices via Bluetooth from your Browser, Homeassistant via NodeRed or any IR Remote 

This software runs on an ESP32 making it into a virtual keyboard running a websocket server, this allows clients to connect via websocket and send named keycodes.

Keycodes sent are also broadcasted to all connected websocket clients making it possible to react 
e.g. using Homeassistant via NodeRed and start your favourite streaming app via adb ore toggle lights or...

A simple keyboard is also available in the web frontend. Point your browser to the esp's IP and check out.

The web frontend offers an easy way to configure your IR remote.

#### Supported Hardware
* ESP32

#### Recommended Peripherals
* TSOP4838 on PIN 23 (pin can be configured in prod_env.ini see example)

#### Installation
##### PlatformIO
* Checkout Project
* Connect ESP32
* Upload

No configuration needed.


#### TODO
* Reinit Websocket (visibilitychange)
* add Home button to frontend
* load all keycodes for configuration