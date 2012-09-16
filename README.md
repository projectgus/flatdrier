Flatdrier is a system for controlling & monitoring the dehumidifier in
our flat. It comprises a microcontroller program (written in plain C
but designed to run on Arduino board), a Python script, and a web
interface.

This is just a silly overblown personal project, but I'm putting it on
github in the hope some pieces of the code may be of use elsewhere.

# Components

* *mcu/* directory contains a C program that runs on the Arduino board:

    + It monitors the onboard DHT-11 sensor, and up to 3 remote "noname brand" weather station senders (433Mhz band.)

    + It controls the remote power point (Powertran A0342 brand) to turn the dehumidifer on and off.

    + It is deliberately dumb, controlled via one-character serial commands ('r' to read all data, 'o'/'f' to turn power on/off.)

* *flatshield/* is a quick & simple "Arduino shield" breakout board design (made in Kicad) for the 433Mhz sender & receiver modules, and a DHT-11 temp & humidity sensor.

* *poll_flatdrier.py* is a Python script that talks to the Arduino and logs the temperature data to CSV text files. It also makes the decision about when to turn the dehumidifer on or off at a given time, based on the current temperatures & dew points.

* *www/* contains a Web interface that allows browsing of temperature & humidity data. The web interface runs client-only so I can host it on my OpenWRT router without needing to set up any server-side stuff.

# Dependencies

The Kicad .brd files use nicholasclewis' very handy Arduino Shield Modules for Kicad
http://www.thingiverse.com/thing:9630

The web interface uses a number of very useful third party free software libraries, all included in the source tree under www/lib/:
* [Bootstrap](http://twitter.github.com/bootstrap/) by Twitter
* [Twitter Bootstrap Date Range Picker](http://www.dangrossman.info/2012/08/20/a-date-range-picker-for-twitter-bootstrap/) by Dan Grossman
* [Flot](http://www.flotcharts.org/) by IOLA and Ole Laursen
* [Coffeescript Compiler](http://coffeescript.org/) by Jeremy Ashkenas


# Known Issues

* The PCB layout is extremely quick and nasty, and isolates the GND
  pin of the DHT-11 requiring a fix wire. It's only attached here so
  I don't lose track of it.


# License

Except for libraries previously mentioned, flatdrier's unique content
is Copyright (c) 2012, Angus Gratton.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the flatdrier nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ANGUS GRATTON BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.



