# Washer Status Indicator
Washing Machine status indicator using a ESP32-C3 based Adafruit QT Py

## Features

- Simple LED indicator for at a glance washing machine and clothes dryer status.
  - Green LED for "OFF / Ready"
  - Red LED for "ON/ In Use"
  - Could be used to indicate status of other things.
- Open source hardware
- Software uses [PubSubClient](https://pubsubclient.knolleary.net/) to communicate with MQTT server
  - Uses [OpenHab](https://www.openhab.org/) home automation sever for source of MQTT data.
    - Requires MQTT server such as [Mosquitto](https://mosquitto.org/).
	  - [Basic directions](https://community.openhab.org/t/oh3-mqtt-setup-and-configuration) for Mosquitto and OpenHab
    - Follow directions for configuring [MQTT binding](https://www.openhab.org/addons/bindings/mqtt/).
  - Could be used with other MQTT sources like [Home Assistant](https://www.home-assistant.io/).

## Boards
The schematic folder contains the board and schematic files in Eagle format
- Designs for 1206 SMT LEDs and resistors or through hole components
![MQTT washer thing](/assets/images/smtboard.png)
![MQTT washer thing](/assets/images/washerboardrender.png)

## Code
The code folder contains the program code to compile and upload to the ESP32
- Use [Platform.io](https://docs.platformio.org/en/latest/integration/ide/vscode.html) installed on [Visual Studio Code](https://code.visualstudio.com/) AKA: VSCode to compile and upload.
- Open the code folder in VSCode then compile, PlatformIO should download any dependencies.