
## Oyster growing automation project
the goal of this project is to improve the measuring and controlling the process of growing mushrooms in a controlled environment

## Table of Contents
- [Summary of the Growing Process](#summary-of-the-growing-process)
- [Build Overview](#build-overview)
  - [UI](#ui)
- [Electrical Circuit Overview](#electrical-circuit-overview)
  - [Diagram](#circuit overview)
- [How to Install](#how-to-install)
- [Dependencies](#dependencies)
- [How to Use](#how-to-use)
  - [Setting Up CAN](#setting-up-can)

**summary of the process**

Oyster mushroom growing is done in a dark room.

First the seeds are put in a mixture of organic material
The incubation period the mushroom mycelium grow and spread through the substrate.
Aftter that the next phase is fruiting, the relative humidity should be high, between 80%-95% and they need a lot of fresh air thus keeping the co2 level low.
The growing of the mushroom is influenced by co2, temperature and moisture.

**The controlling can be done by various instruments;**
**ventilator:** 
Lets in fresh air from outside trough a vent.
This affects; co2, temperature and humidity.

**air conditioner:**
it lowers the temperature, hot air gets vented outside.

**air humidifier:**
vibrating metal plate in shallow water creates tiny droplets in the air, a simple ventilator spreads it further in the room and increases humidity.

# build overview

A raspberry-pi 5 runs a server which contains the main program logic made in node-red.

The node-red server has a gui in where the user can see measurements in graphs
and set the settings for automatically controlling the enviroment.

The node-red program changes various outputs by comparing the set-goal process-value and the actual measured process-value
for example it switches on the moisturizer when the set-goal moisture-value is 70% and the measured value is 60%



## ui
**graphs page**

![graph1](https://github.com/user-attachments/assets/ab44abb5-f7f7-415c-ac4e-e018b33c205c)

**home page**

![home_page](https://github.com/user-attachments/assets/5c770ff9-78f0-47a3-8079-64137895af45)




# electrical circuit overview

Communication between microcontrollers and raspberry pi is done by CAN-BUS

1 esp32 microcontroller with all measuring devices and 2 outputs is placed in the growing room.

**Components:**

Raspberry pi 5

ESP32 microcontroller

CAN-controller+transeiver - MPC2551 (transeiver TJA1050 integrated)

CAN-transeiver CJMCU-2551

**Sensors:**

BME680 - humidity, temperature, pressure

MHZC19 - co2 measuring

bi-directional level-shifter, 3v <-> 5v


# circuit overview

![IMG-20241106-WA0000](https://github.com/user-attachments/assets/de03d8e4-49be-420d-9d4e-5c1c2054cd67)





# how to install
project is done on Linux pc

git clone git@github.com:Rikkopanda/Oester_zwammen_meet_en_regel.git

the server is made in docker container, using docker compose

node-red is run on the server


# dependencies

the node-red flow depends on multiple packages; these are located on package.json
and can be installed with:
```
npm install in /data/projects/NODE-RED.../
```
/data/projects/NODE-RED.../ is where the noder-red flows and package.json is located in the container

settings.json is an important file where various settigs are configured such as allowing projects from /data/projects folder.

# how to use
```
cd oesterzwam_containers
docker compose up -d
```
this will start container in the background, it restarts up always automatically from startup after.

go to 127.0.0.1:1880 in the browser

upload program on the esp32

**Setting up CAN**

Configure CAN-controller;
```
/boot/config.txt file:(add line)
dtoverlay=mcp2515-can0,oscillator=8000000,interrupt=25
```
Set up the can-interface on linux:
```
auto can0
iface can0 inet manual
    pre-up /sbin/ip link set can0 type can bitrate 25000 triple-sampling on restart-ms 100
    up /sbin/ifconfig can0 up
    down /sbin/ifconfig can0 down
```
lowering bitrate from 500000 to 25000 allowed longer cable lenght


