# Bluetooth LE Remote Companion

Remotely control devices via Bluetooth with an IR remote, from your Browser, Homeassistant or Node-RED with no noticable latency (depends on IR protocol). 

## Features
This software turns an ESP32 into a virtual keyboard and mouse running a websocket server and/or Home Assistant websocket API client.

Button presses sent by an IR remote can also be broadcasted to Home Assistant or all connected websocket clients making it possible to start your favourite streaming app via adb or toggle lights or...

A simple keyboard and mouse is available in the web frontend. Point your browser to the esp's IP and check out.

The web frontend offers an easy, self-explanatory way to configure your IR remote.

An easy to print housing for a d1 mini style esp32 can be found in the `case/D1-Mini` subfolder.

## Supported Hardware
* ESP32

### (Strongly) Recommended Peripherals
* TSOP4838 on PIN 23 (pin can be configured in prod_env.ini see example)

## Installation
### PlatformIO
* Checkout Project
* Connect ESP32
* Upload

## Configuration
### WIFI
Connect to the WIFI 'BLERC' with the password 0987654321 and enter your WIFI Credentials. Device reboots and than connects to your WIFI. Look up the IP in your router and than point your browser to it.

### Bluetooth
Bind the device you want to control to `BLE Remote Companion`.  
It happend to me sometinmes that the mouse did not work instantly. A rebind always helped in that cases.

### Other
You find a configuration page in the menu of the frontend.
|Config|Description|
-|-
|General||
|Room  |Give it a name (needed for HA Websocket API)|
|Home Assistant API||
|Enable|Enable HA API support|
|IP|IP Address of machine running Home Assistant|
|Port|Home Assistant frontend port (8123)|
|Token|Long lived API Token|
|Send assigned IR Codes|Send IR Button presses that are already assigned to a keypress also|
|Websocket Broadcasts||
|Enable|Enable Websocket broadcast messages|
|Send assigned IR Codes|Send IR Button presses that are already assigned to a keypress also|

## Home Assistant
BLE Remote Companion utilizes the [Home Assistant Websocket API](https://developers.home-assistant.io/docs/api/websocket) if enabled. Simply add the IP address, port and a [Long-lived access token](https://developers.home-assistant.io/docs/auth_api/#long-lived-access-token) to be prepared.

The client calls the script `ble_rc_to_ha`. The parameters for the script contain all information needed to invoke an arbitrary action within your Home Assistant. I prefer a simple script that once invoked fires an event to trigger automations. Using it this way enables you to create automations like you would normally do for any other trigger.

#### Script `ble_rc_to_ha`
```yaml
ble_rc_to_ha:
  fields:
    method:
      description: "Keydown or up"
      example: "keydown"
      required: true
    protocol:
      description: "The ir protocol id"
      example: "1"
      required: true
    code:
      description: "The ir code"
      example: "131578875"
      required: true
    room:
      description: "The ble-rc device room"
      example: "Living"
      required: true
  sequence:
    - event: ble_rc_to_ha
      event_data:
        method: "{{ method }}"
        protocol: "{{ protocol }}"
        code: "{{ code }}"
        room: "{{ room }}"

```
An automation that gets triggered by this event could look like this:
```yaml
- id: "91243432-708f-11ec-bf72-93a5ed09c06e"
  alias: Do Something
  trigger:
    - platform: event
      event_type:
        - ble_rc_to_ha
  condition: >
    {{
      trigger.event.data.room == 'Living' and 
      trigger.event.data.method == 'keyup' and
      trigger.event.data.protocol == 3 and
      trigger.event.data.code == 45957311
    }}
  action:
    service: script.do_something
  mode: single
```
You can pick up the condition parameters by listenening to the `ble_rc_to_ha` event in the Home Assistant developer tools events section.

The other way round BLE Remote Companion listens to the event `ha_to_ble_rc`  
Expected event data:
```yaml
- event: ha_to_ble_rc
  event_data:
    room: "Living"        # mandatory
    method: "keypress"    # mandatory
    type: "KEYBOARD"      # mandatory
    code: "KEYCODE_ENTER" # mandatory
    longpress: false      # optional (keypress only)
```
Have a look at the [keycode tables below](#keycode-tables) to find out what to send.
Available methods are
* keypress (recommended)
* keydown (don't forget to send a keyup also...)
* keyup

Keypress is a combination of `keydown` followed by `keyup`. The optional `longpress` parameter adds a delay of 500ms in between.

Note: I don't have a use case for `keydown` or `keyup` yet. These methods may be deprecated and dissappear in future releases.

## Websocket Server
As an alternative to the Home Assistant client Remote Companion exposes a websocket server (default port: 81). Once connected you can send and receive Button presses with e.g. Node-RED (easiest IMO if you are not using Home Assistant)
```
ws://1.2.3.4:81/jsonrpc
```
Supported commands:
* keypress
  * sends a keypress with optional longpress
* keydown
  * sends keydown, don't forget to send a keyup afterwards ;)
* keyup
  * sends keyup
* learn
  * learn keycode
* clear
  * clear IR configuration
* reboot
### keypress/keyup/keydown
#### Parameters
* {String} method <i>keypress|keyup|keydown</i>
* {Object} params
* {String} params.type <i>KEYBOARD|CONSUMER|APP_LAUNCHER|APP_CONTROL</i>
* {String} params.code <i>one of the keycodes as shown in [tables below](#keycode-tables)</i>
* {Boolean} (params.longpress) <i>true|false optional longpress (keypress method only)</i>

Keypress is a combination of `keydown` followed by `keyup`. The optional `longpress` parameter adds a delay of 500ms in between.

Note: I don't have a use case for `keydown` or `keyup` yet. These methods may be deprecated and dissappear in future releases.

Example payload:
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
## Node-RED (wip)
You can use the Node-RED websocket nodes to send and receive messages to/from your Remote Companion.  
This enables you to send and receive keycodes to/from BLE Remote Companion.  
You could use it to have a remote in your home automation system or start Apps via ADB.
### Configuration
* add Websocket out and configure accordingly
  * activate Send Heartbeat checkbox for automatic reconnect
* add Websocket in to receive messages if a key is pressed

### [Websocket out example flow](/doc/node-red/websocket-out-flow.json)
### [Websocket in (with longpress) example flow](/doc/node-red/websocket-in-flow.json)

## Keycode Tables
* [Keyboard Codes 0x07 (Type: KEYBOARD)](/doc/keycodes/keyboard.md)
* [Keyboard Codes 0x0c (Type: CONSUMER)](/doc/keycodes/consumer.md)
* [Keyboard Codes 0x0c (Type: APP_LAUNCHER)](/doc/keycodes/app-launcher.md)
* [Keyboard Codes 0x0c (Type: APP_CONTROL)](/doc/keycodes/app-control.md)
* [Named and Decimal Keycodes from Android Open Source Project](/doc/keycodes/AOS-Project.md)
## TODO:
* send button presses from frontend to Home Assistant
* general purpose Node-RED example flows with screenshots
* change Wifi MAC also changes BT MAC? Test... this could solve issues if device is connected to an unknown bt client
* documentation
* Mouse Mode -> needs testing
  * https://github.com/T-vK/ESP32-BLE-Mouse
  * https://github.com/olegos76/nimble_kbdhid_example/blob/main/main/gatt_vars.c)
* handle ir errors when learning
* layout onfiguration
* configurable ws timeout
* case for Node MCU style with TSOP (reset button)
* start BLE Advertising via tactile button longpress
* Basic Auth
* configurable device name/room visible in frontend/title
* deeplinks to layouts for iframes