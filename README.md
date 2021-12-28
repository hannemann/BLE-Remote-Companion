# Bluetooth LE Remote Companion

Remotely control devices via Bluetooth from your Browser, Homeassistant via NodeRed or any IR Remote 

This software runs on an ESP32 making it into a virtual keyboard running a websocket server, this allows clients to connect via websocket and send named keycodes.

Keycodes sent are also broadcasted to all connected websocket clients making it possible to react 
e.g. using Homeassistant via NodeRed and start your favourite streaming app via adb or toggle lights or...

A simple keyboard is also available in the web frontend. Point your browser to the esp's IP and check out.

The web frontend offers an easy way to configure your IR remote.

## Supported Hardware
* ESP32

### Recommended Peripherals
* TSOP4838 on PIN 23 (pin can be configured in prod_env.ini see example)

## Installation
### PlatformIO
* Checkout Project
* Connect ESP32
* Upload

No configuration needed.

## Websocket Server
BLE-RC exposes a websocket server (default port: 81).
```
ws://1.2.3.4:81/jsonrpc
```
Supported commands:
* keydown
  * sends keydown
* keyup
  * sends keyup
* keypress
  * sends a keypress with optional longpress
* learn
  * learn keycode
* clear
  * clear IR configuration
* reboot

### keyup/keydown
#### Parameters
* {String} method <i>keyup|keydown</i>
* {Object} params
* {Number} params.type <i>1|2|3|4</i>
* {Number} params.key <i>id of key as shown in [tables below](#keycode-tables)</i>

Example:
```jsonby
{
    "method": "keydown",
    "params": {
        "type": 1,
        "key": 40
    } 
}
```
### keypress
#### Parameters
* {String} method keypress
* {Object} params
* {String} params.type <i>KEYBOARD|CONSUMER|APP_LAUNCHER|APP_CONTROL</i>
* {String} params.code <i>one of the keycodes as shown in [tables below](#keycode-tables)</i>
* {Number} (params.longpress) <i>1|0 optional longpress</i>

Example:
```json
{
    "method": "keypress",
    "params": {
        "type": "KEYBOARD",
        "code": "KEYCODE_ENTER",
        "longpress": 1
    } 
}
```
## Node-RED
You can use the Node-RED websocket nodes to send and receive messages to/from your BLE-RC.  
This enables you to send all keycodes defined in keys.h and also receive the keycodes sent by BLE-RC if you use your remote or the web frontend.  
I use it to have a remote in my Homeassistant and also start Apps via ADB by pressing the Red/Green/Yellow/Blu Buttons on my IR Remote.
### Configuration
* add Websocket out and configure accordingly
  * activate Send Heartbeat checkbox for automatic reconnect
* add Websocket in to receive messages if a key is pressed

### Home Assistant script example:
Observe `android_tv_control` with Node-RED `events:all` node to send messages via websocket to your BLE-RC
```yaml
android_tv_enter:
  sequence:
    - event: android_tv_control
      event_data:
        type: KEYBOARD
        code: KEYCODE_ENTER

android_tv_enter_longpress:
  sequence:
    - event: android_tv_control
      event_data:
        type: KEYBOARD
        code: KEYCODE_ENTER
        longpress: 1
```
### [Websocket out example flow](/doc/node-red/websocket-out-flow.json)
### [Websocket in (with longpress) example flow](/doc/node-red/websocket-in-flow.json)

## Keycode Tables
* [Keyboard Codes 0x07 (Type: KEYBOARD / 1)](/doc/keycodes/keyboard.md)
* [Keyboard Codes 0x0c (Type: CONSUMER / 2)](/doc/keycodes/consumer.md)
* [Keyboard Codes 0x0c (Type: APP_LAUNCHER / 3)](/doc/keycodes/app-launcher.md)

* [Keyboard Codes 0x0c (Type: APP_CONTROL / 4)](/doc/keycodes/app-control.md)
## TODO
* documentation
* feedback if key was learned (send result), indicate key learn, cancel lear/delete
* load all keycodes for configuration
* configurable ws timeout
* case for d1 Mini style with TSOP (reset button)
* case for Node MCU style with TSOP (reset button)