Adafruit Python MCP9808
=======================

Python library for accessing the MCP9808 precision temperature sensor on a Raspberry Pi or Beaglebone Black.

Designed specifically to work with the Adafruit MCP9808 sensor ----> https://www.adafruit.com/product/1782

To install, first make sure some dependencies are available by running the following commands (on a Raspbian
or Beaglebone Black Debian install):

````
sudo apt-get update
sudo apt-get install build-essential python-dev python-smbus
````

Then download the library by clicking the download zip link to the right and unzip the archive somewhere on your Raspberry Pi or Beaglebone Black.  Then execute the following command in the directory of the library:

````
sudo python setup.py install
````

Make sure you have internet access on the device so it can download the required dependencies.

See examples of usage in the examples folder.

Adafruit invests time and resources providing this open source code, please support Adafruit and open-source hardware by purchasing products from Adafruit!

Written by Tony DiCola for Adafruit Industries.
MIT license, all text above must be included in any redistribution
