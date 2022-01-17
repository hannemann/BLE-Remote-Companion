# Bluetooth LE Remote Companion

Remotely control devices via Bluetooth with an IR remote, from a Browser, Homeassistant or Node-RED. 

## Features
This software turns an ESP32 into a virtual keyboard and mouse running a websocket server and/or Home Assistant websocket API client.

Button presses sent by an IR remote can also be broadcasted to Home Assistant or all connected websocket clients making it possible to start your favourite streaming app via adb or toggle lights or...

A simple keyboard and mouse is available in the web frontend. Point your browser to the esp's IP and check out.

The web frontend offers an easy, self-explanatory way to configure your IR remote. It is also possible to change the default keycode that is sent by a button.

An easy to print housing for a d1 mini style esp32 can be found in the `case/D1-Mini` subfolder.

## Supported Hardware
* ESP32
  * developed and tested on D1 Mini Style ESP32:
    * Module: ESP32-WROOM-32
    * Chipset: ESP32-D0WDQ6
    * 4MB Flash
    * 520KB RAM

### (Strongly) Recommended Peripherals
* TSOP4838 on PIN 23 (pin can be configured in prod_env.ini see example)

## Installation
### PlatformIO
* Checkout Project
* Connect ESP32
* Upload

## Configuration
### WIFI
Connect to the WIFI 'BLERC' with the password 0987654321 and enter your WIFI Credentials. Device reboots and than connects to the WIFI. Look up the IP in the router and than point a browser to it.

### Bluetooth
Bind the device to control to `BLE Remote Companion`.  
It happend to me sometinmes that the mouse did not work instantly. A rebind always helped in that cases.

### Other
You find a configuration page in the menu of the frontend.
|Config|Description|
-|-
|General||
|Room  |Give it a name (recommended to be able to differentiate between several devices when events are evaluated by HA)|
|Keyboard||
|Layout|Match the layout to the layout of the device|
|Home Assistant API||
|Enable|Enable HA API support|
|IP|IP Address of machine running Home Assistant|
|Port|Home Assistant frontend port (8123)|
|Token|Long lived API Token|
|Send assigned IR Codes|Send IR Button presses that are already assigned to a keypress also|
|Websocket Broadcasts||
|Enable|Enable Websocket broadcast messages|
|Send assigned IR Codes|Send IR Button presses that are already assigned to a keypress also|

## Navigation
* Remote
  * Mimics an IR remote.
* Keyboard
  * A simple keyboard (wip)
* Mouse
  * Mimics a trackpad. Tested with Shield and Ubuntu
## Main Menu
* ### Learn IR codes page
  * #### Learn a code  
    Press the button you want to learn. When prompted press the corresponding button on your remote like normal
  * #### Forget a code  
    Press the `Forget a single button` Button. When prompted press the button to be forgotten.
  * #### Delete all learned codes  
    Press and confirm the `Delete the entire configuration` button. Configuration of room, HA etc. is left untouched.
* ### Map Remote Buttons
  * #### Add Mapping
    Hit the Button you want to remap, choose the function in the list. Re-learn the remote button afterwards
  * #### Reset a single Button
    Press the button you want to reset
  * #### Reset all mappings
    Reset all mappings to their default values
* ### All Buttons Page  
  The Page displays a list of all supported buttons. Not all of them are supported on each and every device and not all existing HID functions are supported by Remote Companion. Play around with these. The displayed keycodes can all be used with Home Assistant or a websocket client.

* ### OTA Update
  Keep Remote Companion up to date by uploading a new release  
  I own one device that doesn't like OTA updates (and is alos picky with USB so i assume it's somehow broken). If the Upload fails i reboot the device via menu while being on the update page. Wait for 15 seconds and than try again. Works in most cases.

* ### Reboot

## Home Assistant
BLE Remote Companion utilizes the [Home Assistant Websocket API](https://developers.home-assistant.io/docs/api/websocket) if enabled. Simply add the IP address, port and a [Long-lived access token](https://developers.home-assistant.io/docs/auth_api/#long-lived-access-token) to be prepared. 

The client calls the script `ble_rc_to_ha` if a button on a remote or in the frontend is pressed. The parameters for the script contain all information needed to invoke an arbitrary action within your Home Assistant. I prefer a simple script that once invoked fires an event to trigger automations. Using it this way enables you to create automations like you would normally do for any other trigger. If the pressed button has an ir code assigned the message is omitted unless 'Send assigned IR codes' is enabled in the configuration.

#### Parameters sent
* {String} method *one of keyup|keydown*
* {Number} ir_prototcol *The ir protocol id*
* {Number} ir_code *The ir code*
* {String} type *one of KEYBOARD|CONSUMER|APP_LAUNCHER|APP_CONTROL|unkown*
* {String} code *one of the keycodes as shown in [tables below](#keycode-tables) or unknown*
* {String} room *The configured room*
* {Boolean} longpress *true if pressed for longer than 500ms*

#### Script `ble_rc_to_ha` called by your Remote Companion
```yaml
ble_rc_to_ha:
  fields:
    method:
      description: "Keydown or up"
      example: "keydown"
      required: true
    ir_protocol:
      description: "The ir protocol id"
      example: "1"
      required: true
    ir_code:
      description: "The ir code"
      example: "131578875"
      required: true
    type:
      description: "The keyboard type"
      example: "KEYBOARD"
      required: true
    code:
      description: "The key code"
      example: "KEYBOARD_ENTER"
      required: true
    room:
      description: "The ble-rc device room"
      example: "Living"
      required: true
    longpress:
      description: "Pressed for longer than 500ms"
      example: "true"
      required: true
  sequence:
    - event: ble_rc_to_ha
      event_data:
        method: "{{ method }}"
        ir_protocol: "{{ ir_protocol }}"
        ir_code: "{{ ir_code }}"
        type: "{{ type }}"
        code: "{{ code }}"
        room: "{{ room }}"
        longpress: "{{ longpress }}"
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
or like this if you want to combine multiple triggers in one automation:
```yaml
- id: "91243432-708f-11ec-bf72-93a5ed09c06e"
  alias: Do something or something else
  trigger:
    - platform: event
      event_type:
        - ble_rc_to_ha
  condition: >
    {{
      trigger.event.data.method == 'keyup' and
      trigger.event.data.room == 'Living'
    }}
  action:
    - choose:
        - conditions:
            condition: template
            value_template: >
              {{
                trigger.event.data.ir_protocol == 3 and
                trigger.event.data.ir_code == 45957311
              }}
          sequence:
            service: script.send_adb_intent
        - conditions:
            condition: template
            value_template: >
              {{
                trigger.event.data.ir_protocol == 3 and
                trigger.event.data.ir_code == 45940991
              }}
          sequence:
            service: script.switch_lights_on
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

To add remote buttons to your frontend you can use this script:
```yaml
android_tv_control:
  fields:
    room:
      description: "The Room of the device"
      example: "Living"
      required: true
      selector:
        select:
          options:
            - Living
            - Sleeping
            - Home Cinema
    method:
      description: "The method to invoke"
      example: "keypress"
      required: true
      default: keypress
      selector:
        select:
          options:
            - keypress
            - keydown
            - keyup
    type:
      description: "The Keyboard Type"
      example: "KEYBOARD"
      required: true
      default: KEYBOARD
      selector:
        select:
          options:
            - KEYBOARD
            - CONSUMER
            - APP_LAUNCHER
            - APP_CONTROL
    code:
      description: "The keycode"
      example: "KEYCODE_DPAD_UP"
      required: true
      selector:
        text:
    longpress:
      description: "Longpress"
      required: true
      default: false
      selector:
        boolean:
  sequence:
    - event: ha_to_ble_rc
      event_data:
        room: "{{ room }}"
        method: "{{ method }}"
        type: "{{ type }}"
        code: "{{ code }}"
        longpress: "{{ longpress }}"

```
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
* {String} method *keypress|keyup|keydown*
* {Object} params
* {String} params.type *one of KEYBOARD|CONSUMER|APP_LAUNCHER|APP_CONTROL*
* {String} params.code *one of the keycodes as shown in [tables below](#keycode-tables)*
* {Boolean} (params.longpress) *true|false optional longpress (keypress method only)*

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
## Node-RED
You can use the Node-RED websocket nodes to send and receive messages to/from your Remote Companion.  
You could use it to have a remote in your home automation system or start Apps via ADB.
### Configuration
* add Websocket out and configure accordingly
  * activate Send Heartbeat checkbox for automatic reconnect
* add Websocket in to receive messages if a key is pressed

#### Send to Remote Companion
Refer to Websocket documentation above.

#### Receive messages from Remote Companion
Remote Companion sends messages to all connected websocket clients if a button is pressed on a remote or in the frontend. If the pressed button has an ir code assigned the message is omitted unless 'Send assigned IR codes' is enabled in the configuration.  
#### Message:
* {String} event *ble_rc_to_ws*
* {Object} data
* {String} data.method *one of keyup|keydown*
* {Number} data.ir_prototcol *The ir protocol id*
* {Number} data.ir_code *The ir code*
* {String} data.type *one of KEYBOARD|CONSUMER|APP_LAUNCHER|APP_CONTROL|unknown*
* {String} data.code *one of the keycodes as shown in [tables below](#keycode-tables) or unknown*
* {String} data.room *The configured room*
* {Boolean} data.longpress *true if pressed for longer than 500ms*
```

As a starting point you can import the [Websocket example flow](/doc/node-red/example-flow.json)
![Screenshot](/doc/node-red/Node-RED%20Example%20Flow.png)

## Keycode Tables
* [Keyboard Codes 0x07 (Type: KEYBOARD)](/doc/keycodes/keyboard.md)
* [Consumer Codes 0x0c (Type: CONSUMER)](/doc/keycodes/consumer.md)
* [Consumer Codes 0x0c (Type: APP_LAUNCHER)](/doc/keycodes/app-launcher.md)
* [Consumer Codes 0x0c (Type: APP_CONTROL)](/doc/keycodes/app-control.md)
* [Named and Decimal Keycodes from Android Open Source Project](/doc/keycodes/AOS-Project.md)
## TODO:
* documentation -> wip
* handle ir errors when learning
* case for Node MCU style with TSOP (reset button)
* Basic Auth
* navigate with history api