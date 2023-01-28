# MQTT Example for OpenHab
- [Basic directions](https://community.openhab.org/t/oh3-mqtt-setup-and-configuration) for Mosquitto and OpenHab
- See [OpenHab Things](https://www.openhab.org/docs/configuration/things.html) and [OpenHab Items](https://www.openhab.org/docs/configuration/items.html) for infomation on setting up items.
- Uses current sensor to monitor power consumed by the machines to determine their states.
  - TP-Link [KP125](https://www.kasasmart.com/us/products/smart-plugs/kasa-smart-plug-slim-energy-monitoring-kp125) or equivalent with energy monitoring.
  - Needs to have an binding available for OpenHab to monitor
    - [List](https://www.openhab.org/addons/) of available OpenHab bindings
	- [TP-Link binding](https://www.openhab.org/addons/bindings/tplinksmarthome/) for OpenHab, includes examples of things and items.
Provided as example with no guarantees express or implied.


## Rule Files
washer.rules
 - Washing machine rules to update status based on wattage used
 - Machine uses less than 2.5 watts when off, wattages exceeding this will be considered "on".
 - Updates WashingMachineStatus to publish update to MQTT topic
dryer.rules
 - Clothes dryer rules to update status based on wattage used
 
## OpenHab Things & Items
 - WashingMachine_Power item is linked to current power used thing on current sensor
 - WashingMachineStatus is item linked to the MQTT thing "washer"
   - Washer thing has channel called WasherStatus
   - WashingMachineStatus is linked to the WasherStatus channel
   - Updates sent to WashingMachineStatus gets published to MQTT topic sensors/washerstatus

**Images of my setup**
### MQTT broker binding
![MQTT broker binding](/assets/images/MQTT%20Broker%20Binding1.png)
![MQTT broker binding code](/assets/images/MQTT%20Broker%20Binding2.png)

### MQTT washer thing
**Washer thing tab**
![MQTT washer thing](/assets/images/MQTT%20Washer%20Thing.png)

**Channels tab on washer thing**
![MQTT washer channels](/assets/images/MQTT%20Washer%20Channels.png)

**Code tab on washer thing**
 - Configures many of the settings on other two tabs.
![MQTT washer code](/assets/images/MQTT%20Washer%20Code.png)
