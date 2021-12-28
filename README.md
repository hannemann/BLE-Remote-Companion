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
* {String} method keyup|keydown
* {Object} params
* {Number} params.type 1|2|3|4
* 
Example:
```json
{
    "method": "keydown",
    "params": {
        "type": 1,
        "code": "KEYCODE_ENTER"
    } 
}
```
### keypress
#### Parameters
* {String} method keypress
* {Object} params
* {String} params.type KEYBOARD|CONSUMER|APP_LAUNCHER|APP_CONTROL
* {String} params.code one of the defined keycodes in keys.h
* {Number} (params.longpress) 1|0 optional longpress

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
### Websocket out example:
```JSON
[
    {
        "id": "56db6351ef85b5e2",
        "type": "tab",
        "label": "BLE-RC out",
        "disabled": false,
        "info": "",
        "env": []
    },
    {
        "id": "07f492947b224677",
        "type": "websocket out",
        "z": "56db6351ef85b5e2",
        "name": "BLE-RC out",
        "server": "",
        "client": "3ee14b8f32b1ba3d",
        "x": 450,
        "y": 80,
        "wires": []
    },
    {
        "id": "74c9515399442738",
        "type": "server-events",
        "z": "56db6351ef85b5e2",
        "name": "Android TV Control",
        "server": "efbddbd9.15bcd8",
        "version": 1,
        "event_type": "android_tv_control",
        "exposeToHomeAssistant": false,
        "haConfig": [
            {
                "property": "name",
                "value": ""
            },
            {
                "property": "icon",
                "value": ""
            }
        ],
        "waitForRunning": true,
        "outputProperties": [
            {
                "property": "payload",
                "propertyType": "msg",
                "value": "",
                "valueType": "eventData"
            }
        ],
        "x": 130,
        "y": 80,
        "wires": [
            [
                "7e9ed5ba6993d1c5"
            ]
        ]
    },
    {
        "id": "7e9ed5ba6993d1c5",
        "type": "change",
        "z": "56db6351ef85b5e2",
        "name": "Transform",
        "rules": [
            {
                "t": "move",
                "p": "payload.event",
                "pt": "msg",
                "to": "temp",
                "tot": "msg"
            },
            {
                "t": "set",
                "p": "payload",
                "pt": "msg",
                "to": "{\"method\":\"keypress\"}",
                "tot": "json"
            },
            {
                "t": "move",
                "p": "temp",
                "pt": "msg",
                "to": "payload.params",
                "tot": "msg"
            }
        ],
        "action": "",
        "property": "",
        "from": "",
        "to": "",
        "reg": false,
        "x": 300,
        "y": 80,
        "wires": [
            [
                "07f492947b224677"
            ]
        ]
    },
    {
        "id": "3ee14b8f32b1ba3d",
        "type": "websocket-client",
        "path": "ws://192.168.178.218:81/jsonrpc",
        "tls": "",
        "wholemsg": "false",
        "hb": "10"
    },
    {
        "id": "efbddbd9.15bcd8",
        "type": "server",
        "name": "Home Assistant",
        "version": 2,
        "addon": true,
        "rejectUnauthorizedCerts": true,
        "ha_boolean": "y|yes|true|on|home|open",
        "connectionDelay": true,
        "cacheJson": true,
        "heartbeat": false,
        "heartbeatInterval": 30
    }
]
```
### Websocket in (with longpress) example:
```JSON
[
    {
        "id": "56db6351ef85b5e2",
        "type": "tab",
        "label": "BLE-RC in",
        "disabled": false,
        "info": "",
        "env": []
    },
    {
        "id": "d4ef4d94e2560bb3",
        "type": "websocket in",
        "z": "56db6351ef85b5e2",
        "name": "BLERC in",
        "server": "",
        "client": "3ee14b8f32b1ba3d",
        "x": 100,
        "y": 160,
        "wires": [
            [
                "f01479fe2d692999"
            ]
        ]
    },
    {
        "id": "f01479fe2d692999",
        "type": "json",
        "z": "56db6351ef85b5e2",
        "name": "",
        "property": "payload",
        "action": "",
        "pretty": false,
        "x": 110,
        "y": 200,
        "wires": [
            [
                "bb66a22acdc8a961"
            ]
        ]
    },
    {
        "id": "da400108e061ada0",
        "type": "api-call-service",
        "z": "56db6351ef85b5e2",
        "name": "Kodi",
        "server": "efbddbd9.15bcd8",
        "version": 3,
        "debugenabled": false,
        "service_domain": "script",
        "service": "start_kodi",
        "entityId": "",
        "data": "",
        "dataType": "jsonata",
        "mergecontext": "",
        "mustacheAltTags": false,
        "outputProperties": [],
        "queue": "none",
        "x": 410,
        "y": 280,
        "wires": [
            []
        ]
    },
    {
        "id": "bb66a22acdc8a961",
        "type": "switch",
        "z": "56db6351ef85b5e2",
        "name": "",
        "property": "payload.method",
        "propertyType": "msg",
        "rules": [
            {
                "t": "eq",
                "v": "keydown",
                "vt": "str"
            },
            {
                "t": "eq",
                "v": "keyup",
                "vt": "str"
            }
        ],
        "checkall": "true",
        "repair": false,
        "outputs": 2,
        "x": 230,
        "y": 200,
        "wires": [
            [
                "f83a4bb0a6965f4d"
            ],
            [
                "7ba7eb2af3c8515b",
                "92c8f1ebc10d8fcb"
            ]
        ]
    },
    {
        "id": "7ba7eb2af3c8515b",
        "type": "switch",
        "z": "56db6351ef85b5e2",
        "name": "",
        "property": "payload.params.key",
        "propertyType": "msg",
        "rules": [
            {
                "t": "eq",
                "v": "KEYCODE_PROG_RED",
                "vt": "str"
            },
            {
                "t": "eq",
                "v": "KEYCODE_PROG_GREEN",
                "vt": "str"
            },
            {
                "t": "eq",
                "v": "KEYCODE_PROG_YELLOW",
                "vt": "str"
            },
            {
                "t": "eq",
                "v": "KEYCODE_PROG_BLUE",
                "vt": "str"
            }
        ],
        "checkall": "true",
        "repair": false,
        "outputs": 4,
        "x": 110,
        "y": 340,
        "wires": [
            [
                "1d1f693a7b112ed8"
            ],
            [
                "de98910465a6bd1a"
            ],
            [
                "80a0be027878c684"
            ],
            [
                "ec7fe963b6aa0c4b"
            ]
        ]
    },
    {
        "id": "de98910465a6bd1a",
        "type": "api-call-service",
        "z": "56db6351ef85b5e2",
        "name": "Youtube",
        "server": "efbddbd9.15bcd8",
        "version": 3,
        "debugenabled": false,
        "service_domain": "script",
        "service": "start_youtube",
        "entityId": "",
        "data": "",
        "dataType": "jsonata",
        "mergecontext": "",
        "mustacheAltTags": false,
        "outputProperties": [],
        "queue": "none",
        "x": 260,
        "y": 320,
        "wires": [
            []
        ]
    },
    {
        "id": "80a0be027878c684",
        "type": "api-call-service",
        "z": "56db6351ef85b5e2",
        "name": "Disney",
        "server": "efbddbd9.15bcd8",
        "version": 3,
        "debugenabled": false,
        "service_domain": "script",
        "service": "start_disney",
        "entityId": "",
        "data": "",
        "dataType": "jsonata",
        "mergecontext": "",
        "mustacheAltTags": false,
        "outputProperties": [],
        "queue": "none",
        "x": 250,
        "y": 360,
        "wires": [
            []
        ]
    },
    {
        "id": "ec7fe963b6aa0c4b",
        "type": "api-call-service",
        "z": "56db6351ef85b5e2",
        "name": "Prime",
        "server": "efbddbd9.15bcd8",
        "version": 3,
        "debugenabled": false,
        "service_domain": "script",
        "service": "schlazi_tv_prime",
        "entityId": "",
        "data": "",
        "dataType": "jsonata",
        "mergecontext": "",
        "mustacheAltTags": false,
        "outputProperties": [],
        "queue": "none",
        "x": 250,
        "y": 400,
        "wires": [
            []
        ]
    },
    {
        "id": "1d1f693a7b112ed8",
        "type": "function",
        "z": "56db6351ef85b5e2",
        "name": "",
        "func": "let kodi = null;\nlet netflix = null;\n\nif (Date.now() - flow.get(\"blercKeyDown\") > 500) {\n    netflix = msg;\n} else {\n    kodi = msg;\n}\n\nreturn [kodi,netflix];",
        "outputs": 2,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 260,
        "y": 280,
        "wires": [
            [
                "da400108e061ada0"
            ],
            [
                "4665622f7505ffbb"
            ]
        ]
    },
    {
        "id": "4665622f7505ffbb",
        "type": "api-call-service",
        "z": "56db6351ef85b5e2",
        "name": "Netflix",
        "server": "efbddbd9.15bcd8",
        "version": 3,
        "debugenabled": false,
        "service_domain": "script",
        "service": "start_netflix",
        "entityId": "",
        "data": "",
        "dataType": "jsonata",
        "mergecontext": "",
        "mustacheAltTags": false,
        "outputProperties": [],
        "queue": "none",
        "x": 410,
        "y": 320,
        "wires": [
            []
        ]
    },
    {
        "id": "92c8f1ebc10d8fcb",
        "type": "delay",
        "z": "56db6351ef85b5e2",
        "name": "50ms",
        "pauseType": "delay",
        "timeout": "50",
        "timeoutUnits": "milliseconds",
        "rate": "1",
        "nbRateUnits": "1",
        "rateUnits": "second",
        "randomFirst": "1",
        "randomLast": "5",
        "randomUnits": "seconds",
        "drop": false,
        "allowrate": false,
        "outputs": 1,
        "x": 410,
        "y": 220,
        "wires": [
            [
                "150d4f79efbad0a3"
            ]
        ]
    },
    {
        "id": "f83a4bb0a6965f4d",
        "type": "change",
        "z": "56db6351ef85b5e2",
        "name": "Timestamp Keydown",
        "rules": [
            {
                "t": "set",
                "p": "blercKeyDown",
                "pt": "flow",
                "to": "",
                "tot": "date"
            }
        ],
        "action": "",
        "property": "",
        "from": "",
        "to": "",
        "reg": false,
        "x": 460,
        "y": 180,
        "wires": [
            []
        ]
    },
    {
        "id": "150d4f79efbad0a3",
        "type": "change",
        "z": "56db6351ef85b5e2",
        "name": "Remove Timestamp",
        "rules": [
            {
                "t": "delete",
                "p": "blercKeyDown",
                "pt": "flow"
            }
        ],
        "action": "",
        "property": "",
        "from": "",
        "to": "",
        "reg": false,
        "x": 580,
        "y": 220,
        "wires": [
            []
        ]
    },
    {
        "id": "3ee14b8f32b1ba3d",
        "type": "websocket-client",
        "path": "ws://192.168.178.218:81/jsonrpc",
        "tls": "",
        "wholemsg": "false",
        "hb": "10"
    },
    {
        "id": "efbddbd9.15bcd8",
        "type": "server",
        "name": "Home Assistant",
        "version": 2,
        "addon": true,
        "rejectUnauthorizedCerts": true,
        "ha_boolean": "y|yes|true|on|home|open",
        "connectionDelay": true,
        "cacheJson": true,
        "heartbeat": false,
        "heartbeatInterval": 30
    }
]
```

## Keyboard Codes 0x07 (Type: KEYBOARD / 1)
| id | code | HID_USAGE | Android Keycode (ADB) |
|----|------|-----------|-----------------------|
|0|KEYCODE_A|0x0004|0x001d|
|1|KEYCODE_B|0x0005|0x001e|
|2|KEYCODE_C|0x0006|0x001f|
|3|KEYCODE_D|0x0007|0x0020|
|4|KEYCODE_E|0x0008|0x0021|
|5|KEYCODE_F|0x0009|0x0022|
|6|KEYCODE_G|0x000a|0x0023|
|7|KEYCODE_H|0x000b|0x0024|
|8|KEYCODE_I|0x000c|0x0025|
|9|KEYCODE_J|0x000d|0x0026|
|10|KEYCODE_K|0x000e|0x0027|
|11|KEYCODE_L|0x000f|0x0028|
|12|KEYCODE_M|0x0010|0x0029|
|13|KEYCODE_N|0x0011|0x002a|
|14|KEYCODE_O|0x0012|0x002b|
|15|KEYCODE_P|0x0013|0x002c|
|16|KEYCODE_Q|0x0014|0x002d|
|17|KEYCODE_R|0x0015|0x002e|
|18|KEYCODE_S|0x0016|0x002f|
|19|KEYCODE_T|0x0017|0x0030|
|20|KEYCODE_U|0x0018|0x0031|
|21|KEYCODE_V|0x0019|0x0032|
|22|KEYCODE_W|0x001a|0x0033|
|23|KEYCODE_X|0x001b|0x0034|
|24|KEYCODE_Y|0x001c|0x0035|
|25|KEYCODE_Z|0x001d|0x0036|
|26|KEYCODE_1|0x001e|0x0008|
|27|KEYCODE_2|0x001f|0x0009|
|28|KEYCODE_3|0x0020|0x000a|
|29|KEYCODE_4|0x0021|0x000b|
|30|KEYCODE_5|0x0022|0x000c|
|31|KEYCODE_6|0x0023|0x000d|
|32|KEYCODE_7|0x0024|0x000e|
|33|KEYCODE_8|0x0025|0x000f|
|34|KEYCODE_9|0x0026|0x0010|
|35|KEYCODE_0|0x0027|0x0007|
|36|KEYCODE_ENTER|0x0028|0x0042|
|37|KEYCODE_ESCAPE|0x0029|0x006f|
|38|KEYCODE_DEL|0x002a|0x0043|
|39|KEYCODE_TAB|0x002b|0x003d|
|40|KEYCODE_SPACE|0x002c|0x003e|
|41|KEYCODE_MINUS|0x002d|0x0045|
|42|KEYCODE_EQUALS|0x002e|0x0046|
|43|KEYCODE_LEFT_BRACKET|0x002f|0x0047|
|44|KEYCODE_RIGHT_BRACKET|0x0030|0x0048|
|45|KEYCODE_BACKSLASH|0x0031|0x0049|
|46|KEYCODE_SEMICOLON|0x0033|0x004a|
|47|KEYCODE_APOSTROPHE|0x0034|0x004b|
|48|KEYCODE_GRAVE|0x0035|0x0044|
|49|KEYCODE_COMMA|0x0036|0x0037|
|50|KEYCODE_PERIOD|0x0037|0x0038|
|51|KEYCODE_SLASH|0x0038|0x004c|
|52|KEYCODE_CAPS_LOCK|0x0039|0x0073|
|53|KEYCODE_F1|0x003a|0x0083|
|54|KEYCODE_F2|0x003b|0x0084|
|55|KEYCODE_F3|0x003c|0x0085|
|56|KEYCODE_F4|0x003d|0x0086|
|57|KEYCODE_F5|0x003e|0x0087|
|58|KEYCODE_F6|0x003f|0x0088|
|59|KEYCODE_F7|0x0040|0x0089|
|60|KEYCODE_F8|0x0041|0x008a|
|61|KEYCODE_F9|0x0042|0x008b|
|62|KEYCODE_F10|0x0043|0x008c|
|63|KEYCODE_F11|0x0044|0x008d|
|64|KEYCODE_F12|0x0045|0x008e|
|65|KEYCODE_SYSRQ|0x0046|0x0078|
|66|KEYCODE_SCROLL_LOCK|0x0047|0x0074|
|67|KEYCODE_BREAK|0x0048|0x0079|
|68|KEYCODE_INSERT|0x0049|0x007c|
|69|KEYCODE_MOVE_HOME|0x004a|0x007a|
|70|KEYCODE_PAGE_UP|0x004b|0x005c|
|71|KEYCODE_FORWARD_DEL|0x004c|0x0070|
|72|KEYCODE_MOVE_END|0x004d|0x007b|
|73|KEYCODE_PAGE_DOWN|0x004e|0x005d|
|74|KEYCODE_DPAD_RIGHT|0x004f|0x0016|
|75|KEYCODE_DPAD_LEFT|0x0050|0x0015|
|76|KEYCODE_DPAD_DOWN|0x0051|0x0014|
|77|KEYCODE_DPAD_UP|0x0052|0x0013|
|78|KEYCODE_NUM_LOCK|0x0053|0x008f|
|79|KEYCODE_NUMPAD_DIVIDE|0x0054|0x009a|
|80|KEYCODE_NUMPAD_MULTIPLY|0x0055|0x009b|
|81|KEYCODE_NUMPAD_SUBTRACT|0x0056|0x009c|
|82|KEYCODE_NUMPAD_ADD|0x0057|0x009d|
|83|KEYCODE_NUMPAD_ENTER|0x0058|0x00a0|
|84|KEYCODE_NUMPAD_1|0x0059|0x0091|
|85|KEYCODE_NUMPAD_2|0x005a|0x0092|
|86|KEYCODE_NUMPAD_3|0x005b|0x0093|
|87|KEYCODE_NUMPAD_4|0x005c|0x0094|
|88|KEYCODE_NUMPAD_5|0x005d|0x0095|
|89|KEYCODE_NUMPAD_6|0x005e|0x0096|
|90|KEYCODE_NUMPAD_7|0x005f|0x0097|
|91|KEYCODE_NUMPAD_8|0x0060|0x0098|
|92|KEYCODE_NUMPAD_9|0x0061|0x0099|
|93|KEYCODE_NUMPAD_0|0x0062|0x0090|
|94|KEYCODE_NUMPAD_DOT|0x0063|0x009e|
|95|KEYCODE_MENU|0x0065|0x0052|
|96|KEYCODE_POWER|0x0066|0x001a|
|97|KEYCODE_NUMPAD_EQUALS|0x0067|0x00a1|
|98|KEYCODE_MEDIA_STOP|0x0078|0x0056|
|99|KEYCODE_VOLUME_MUTE|0x007f|0x00a4|
|100|KEYCODE_VOLUME_UP|0x0080|0x0018|
|101|KEYCODE_VOLUME_DOWN|0x0081|0x0019|
|102|KEYCODE_NUMPAD_COMMA|0x0085|0x009f|
|103|KEYCODE_NUMPAD_LEFT_PAREN|0x00b6|0x00a2|
|104|KEYCODE_NUMPAD_RIGHT_PAREN|0x00b7|0x00a3|
|105|KEYCODE_CTRL_LEFT|0x00e0|0x0071|
|106|KEYCODE_SHIFT_LEFT|0x00e1|0x003b|
|107|KEYCODE_ALT_LEFT|0x00e2|0x0039|
|108|KEYCODE_META_LEFT|0x00e3|0x0075|
|109|KEYCODE_CTRL_RIGHT|0x00e4|0x0072|
|110|KEYCODE_SHIFT_RIGHT|0x00e5|0x003c|
|111|KEYCODE_ALT_RIGHT|0x00e6|0x003a|
|112|KEYCODE_META_RIGHT|0x00e7|0x0076|
|113|KEYCODE_MEDIA_PLAY_PAUSE|0x00e8|0x0055|
|114|KEYCODE_MEDIA_PREVIOUS|0x00ea|0x0058|
|115|KEYCODE_MEDIA_NEXT|0x00eb|0x0057|
|116|KEYCODE_MEDIA_EJECT|0x00ec|0x0081|
|117|KEYCODE_EXPLORER|0x00f0|0x0040|
|118|KEYCODE_BACK|0x00f1|0x0004|
|119|KEYCODE_FORWARD|0x00f2|0x007d|
|120|KEYCODE_CALCULATOR|0x00fb|0x00d2|

## Keyboard Codes 0x0c (Type: CONSUMER / 2)
| id | code | HID_USAGE |
|----|------|--------|
|0|KEYCODE_POWER|0x0034|
|1|KEYCODE_MENU|0x0040|
|2|KEYCODE_WINDOW|0x0067|
|3|KEYCODE_PROG_RED|0x0069|
|4|KEYCODE_PROG_GREEN|0x006a|
|5|KEYCODE_PROG_BLUE|0x006b|
|6|KEYCODE_PROG_YELLOW|0x006c|
|7|KEYCODE_LAST_CHANNEL|0x0083|
|8|KEYCODE_TV|0x0089|
|9|KEYCODE_EXPLORER|0x008a|
|10|KEYCODE_CALL|0x008c|
|11|KEYCODE_GUIDE|0x008d|
|12|KEYCODE_DVR|0x009a|
|13|KEYCODE_CAPTIONS|0x0061|
|14|KEYCODE_CHANNEL_UP|0x009c|
|15|KEYCODE_CHANNEL_DOWN|0x009d|
|16|KEYCODE_MEDIA_PLAY|0x00b0|
|17|KEYCODE_BREAK|0x00b1|
|18|KEYCODE_MEDIA_RECORD|0x00b2|
|19|KEYCODE_MEDIA_FAST_FORWARD|0x00b3|
|20|KEYCODE_MEDIA_REWIND|0x00b4|
|21|KEYCODE_MEDIA_NEXT|0x00b5|
|22|KEYCODE_MEDIA_PREVIOUS|0x00b6|
|23|KEYCODE_MEDIA_STOP|0x00b7|
|24|KEYCODE_MEDIA_EJECT|0x00b8|
|25|KEYCODE_MEDIA_PLAY_PAUSE|0x00cd|
|26|KEYCODE_VOLUME_MUTE|0x00e2|
|27|KEYCODE_VOLUME_UP|0x00e9|
|28|KEYCODE_VOLUME_DOWN|0x00ea|
|29|KEYCODE_MEDIA_AUDIO_TRACK|0x0173|
|30|KEYCODE_BOOKMARK|0x0182|
|31|KEYCODE_MUSIC|0x0183|
|32|KEYCODE_ENVELOPE|0x018a|
|33|KEYCODE_CONTACTS|0x018d|
|34|KEYCODE_CALENDAR|0x018e|
|35|KEYCODE_CALCULATOR|0x0192|
|36|KEYCODE_EXPLORER|0x0196|
|37|KEYCODE_POWER|0x019e|
|38|KEYCODE_HEADSETHOOK|0x01b6|
|39|KEYCODE_MUSIC|0x01b7|
|40|KEYCODE_SEARCH|0x0221|
|41|KEYCODE_HOME|0x0223|
|42|KEYCODE_BACK|0x0224|
|43|KEYCODE_FORWARD|0x0225|
|44|KEYCODE_MEDIA_STOP|0x0226|
|45|KEYCODE_BOOKMARK|0x022a|
|46|KEYCODE_PAGE_UP|0x0233|
|47|KEYCODE_PAGE_DOWN|0x0234|
## Keyboard Codes 0x0c (Type: APP_LAUNCHER / 3)
| id | code | HID_USAGE |
|----|------|--------|
|0|AL Launch Button Config. Tool|0x0181|
|1|AL Programmable Button Config.|0x0182|
|2|AL Consumer Control Config.|0x0183|
|3|AL Word Processor|0x0184|
|4|AL Text Editor|0x0185|
|5|AL Spreadsheet|0x0186|
|6|AL Graphics Editor|0x0187|
|7|AL Presentation App|0x0188|
|8|AL Database App|0x0189|
|9|AL Email Reader|0x018a|
|10|AL Newsreader|0x018b|
|11|AL Voicemail|0x018c|
|12|AL Contacts / Address Book|0x018d|
|13|AL Calendar / Schedule|0x018e|
|14|AL Task / Project Manager|0x018f|
|15|AL Log / Journal / Timecard|0x0190|
|16|AL Checkbook / Finance|0x0191|
|17|AL Calculator|0x0192|
|18|AL A/V Capture / Playback|0x0193|
|19|AL Local Machine Browser|0x0194|
|20|AL LAN/WAN Browser|0x0195|
|21|AL Internet Browser|0x0196|
|22|AL Remote Networking/ISP Connect|0x0197|
|23|AL Network Conference|0x0198|
|24|AL Network Chat|0x0199|
|25|AL Telephony / Dialer|0x019a|
|26|AL Logon|0x019b|
|27|AL Logoff|0x019c|
|28|AL Logon / Logoff|0x019d|
|29|AL Terminal Lock / Screensaver|0x019e|
|30|AL Control Panel|0x019f|
|31|AL Command Line Processor / Run|0x01a0|
|32|AL Process / Task Manager|0x01a1|
|33|AL Select Task / Application|0x01a2|
|34|AL Next Task / Application|0x01a3|
|35|AL Previous Task / Application|0x01a4|
|36|AL Preemptive Halt Task / App.|0x01a5|
|37|AL Integrated Help Center|0x01a6|
|38|AL Documents|0x01a7|
|39|AL Thesaurus|0x01a8|
|40|AL Dictionary|0x01a9|
|41|AL Desktop|0x01aa|
|42|AL Spell Check|0x01ab|
|43|AL Grammar Check|0x01ac|
|44|AL Wireless Status|0x01ad|
|45|AL Keyboard Layout|0x01ae|
|46|AL Virus Protection|0x01af|
|47|AL Encryption|0x01b0|
|48|AL Screen Saver|0x01b1|
|49|AL Alarms|0x01b2|
|50|AL Clock|0x01b3|
|51|AL File Browser|0x01b4|
|52|AL Power Status|0x01b5|
|53|AL Image Browser|0x01b6|
|54|AL Audio Browser|0x01b7|
|55|AL Movie Browser|0x01b8|
|56|AL Digital Rights Manager|0x01b9|
|57|AL Digital Wallet|0x01ba|
|58|AL Instant Messaging|0x01bc|
|59|AL OEM Features / Tips Browser|0x01bd|
|60|AL OEM Help|0x01be|
|61|AL Online Community|0x01bf|
|62|AL Entertainment Content Browser|0x01c0|
|63|AL Online Shopping Browser|0x01c1|
|64|AL SmartCard Information / Help|0x01c2|
|65|AL Market / Finance Browser|0x01c3|
|66|AL Customized Corp. News Browser|0x01c4|
|67|AL Online Activity Browser|0x01c5|
|68|AL Research / Search Browser|0x01c6|
|69|AL Audio Player|0x01c|

## Keyboard Codes 0x0c (Type: APP_CONTROL / 4)
| id | code | HID_USAGE |
|----|------|--------|
|0|AC New|0x0201|
|1|AC Open|0x0202|
|2|AC Close|0x0203|
|3|AC Exit|0x0204|
|4|AC Maximize|0x0205|
|5|AC Minimize|0x0206|
|6|AC Save|0x0207|
|7|AC Print|0x0208|
|8|AC Properties|0x0209|
|9|AC Undo|0x021a|
|10|AC Copy|0x021b|
|11|AC Cut|0x021c|
|12|AC Paste|0x021d|
|13|AC Select All|0x021e|
|14|AC Find|0x021f|
|15|AC Find and Replace|0x0220|
|16|AC Search|0x0221|
|17|AC Go To|0x0222|
|18|AC Home|0x0223|
|19|AC Back|0x0224|
|20|AC Forward|0x0225|
|21|AC Stop|0x0226|
|22|AC Refresh|0x0227|
|23|AC Previous Link|0x0228|
|24|AC Next Link|0x0229|
|25|AC Bookmarks|0x022a|
|26|AC History|0x022b|
|27|AC Subscriptions|0x022c|
|28|AC Zoom In|0x022d|
|29|AC Zoom Out|0x022e|
|30|AC Zoom|0x022f|
|31|AC Full Screen View|0x0230|
|32|AC Normal View|0x0231|
|33|AC View Toggle|0x0232|
|34|AC Scroll Up|0x0233|
|35|AC Scroll Down|0x0234|
|36|AC Pan Left|0x0236|
|37|AC Pan Right|0x0237|
|38|AC New Window|0x0239|
|39|AC Tile Horizontally|0x023a|
|40|AC Tile Vertically|0x023b|
|41|AC Format|0x023c|
|42|AC Edit|0x023d|
|43|AC Bold|0x023e|
|44|AC Italics|0x023f|
|45|AC Underline|0x0240|
|46|AC Strikethrough|0x0241|
|47|AC Subscript|0x0242|
|48|AC Superscript|0x0243|
|49|AC All Caps|0x0244|
|50|AC Rotate|0x0245|
|51|AC Resize|0x0246|
|52|AC Flip horizontal|0x0247|
|53|AC Flip Vertical|0x0248|
|54|AC Mirror Horizontal|0x0249|
|55|AC Mirror Vertical|0x024a|
|56|AC Font Select|0x024b|
|57|AC Font Color|0x024c|
|58|AC Font Size|0x024d|
|59|AC Justify Left|0x024e|
|60|AC Justify Center H|0x024f|
|61|AC Justify Right|0x0250|
|62|AC Justify Block H|0x0251|
|63|AC Justify Top|0x0252|
|64|AC Justify Center V|0x0253|
|65|AC Justify Bottom|0x0254|
|66|AC Justify Block V|0x0255|
|67|AC Indent Decrease|0x0256|
|68|AC Indent Increase|0x0257|
|69|AC Numbered List|0x0258|
|70|AC Restart Numbering|0x0259|
|71|AC Bulleted List|0x025a|
|72|AC Promote|0x025b|
|73|AC Demote|0x025c|
|74|AC Yes|0x025d|
|75|AC No|0x025e|
|76|AC Cancel|0x025f|
|77|AC Catalog|0x0260|
|78|AC Buy / Checkout|0x0261|
|79|AC Add to Cart|0x0262|
|80|AC Expand|0x0263|
|81|AC Expand All|0x0264|
|82|AC Collapse|0x0265|
|83|AC Collapse All|0x0266|
|84|AC Print Preview|0x0267|
|85|AC Paste Special|0x0268|
|86|AC Insert Mode|0x0269|
|87|AC Delete|0x026a|
|88|AC Lock|0x026b|
|89|AC Unlock|0x026c|
|90|AC Protect|0x026d|
|91|AC Unprotect|0x026e|
|92|AC Attach Comment|0x026f|
|93|AC Delete Comment|0x0270|
|94|AC View Comment|0x0271|
|95|AC Select Word|0x0272|
|96|AC Select Sentence|0x0273|
|97|AC Select Paragraph|0x0274|
|98|AC Select Column|0x0275|
|99|AC Select Row|0x0276|
|100|AC Select Table|0x0277|
|101|AC Select Object|0x0278|
|102|AC Redo / Repeat|0x0279|
|103|AC Sort|0x027a|
|104|AC Sort Ascending|0x027b|
|105|AC Sort Descending|0x027c|
|106|AC Filter|0x027d|
|107|AC Set Clock|0x027e|
|108|AC View Clock|0x027f|
|109|AC Select Time Zone|0x0280|
|110|AC Edit Time Zones|0x0281|
|111|AC Set Alarm|0x0282|
|112|AC Clear Alarm|0x0283|
|113|AC Snooze Alarm|0x0284|
|114|AC Reset Alarm|0x0285|
|115|AC Synchronize|0x0286|
|116|AC Send/Receive|0x0287|
|117|AC Send To|0x0288|
|118|AC Reply|0x0289|
|119|AC Reply All|0x028a|
|120|AC Forward Msg|0x028b|
|121|AC Send|0x028c|
|122|AC Attach File|0x028d|
|123|AC Upload|0x028e|
|124|AC Download (Save Target As)|0x028f|
|125|AC Set Borders|0x0290|
|126|AC Insert Row|0x0291|
|127|AC Insert Column|0x0292|
|128|AC Insert File|0x0293|
|129|AC Insert Picture|0x0294|
|130|AC Insert Object|0x0295|
|131|AC Insert Symbol|0x0296|
|132|AC Save and Close|0x0297|
|133|AC Rename|0x0298|
|134|AC Merge|0x0299|
|135|AC Split|0x029a|
|136|AC Distribute Horizontally|0x029b|
|137|AC Distribute Vertically|0x029|
## TODO
* feedback if key was learned, indicate key learn, cancel lear/delete
* load all keycodes for configuration
* configurable ws timeout
* case for d1 Mini style with TSOP (reset button)
* case for Node MCU style with TSOP (reset button)