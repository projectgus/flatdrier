EESchema Schematic File Version 2  date Sun 02 Sep 2012 17:09:22 EST
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:arduino_shieldsNCL
LIBS:flatshield-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 1 1
Title ""
Date "2 sep 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	5750 2800 5600 2800
Wire Wire Line
	5600 2800 5600 2950
Wire Wire Line
	5450 2450 5350 2450
Wire Wire Line
	5750 2150 5600 2150
Wire Wire Line
	5600 2150 5600 2250
Wire Wire Line
	5050 1500 5050 1950
Wire Wire Line
	5050 1500 4750 1500
Wire Wire Line
	5550 1500 5750 1500
Wire Wire Line
	5750 1300 5750 1250
Wire Wire Line
	5750 1250 5550 1250
Wire Wire Line
	5550 1250 5550 1300
Wire Wire Line
	2850 1600 2650 1600
Wire Wire Line
	2850 1800 2850 1900
Wire Wire Line
	2850 1900 2650 1900
Wire Wire Line
	2650 1900 2650 2000
Wire Wire Line
	4750 1200 4950 1200
Wire Wire Line
	4950 1200 4950 1250
Wire Wire Line
	4750 1400 5750 1400
Wire Wire Line
	5750 1600 5500 1600
Wire Wire Line
	5050 1950 5750 1950
Connection ~ 5350 1950
Wire Wire Line
	5550 1850 5750 1850
Wire Wire Line
	5350 2450 5350 2600
Wire Wire Line
	5350 2600 5750 2600
Wire Wire Line
	4750 1600 5000 1600
Wire Wire Line
	5000 1600 5000 2700
Wire Wire Line
	5000 2700 5750 2700
NoConn ~ 5750 2050
NoConn ~ 4750 1100
NoConn ~ 4750 2700
NoConn ~ 4750 2600
NoConn ~ 4750 2500
NoConn ~ 4750 2400
NoConn ~ 4750 2300
NoConn ~ 4750 2200
NoConn ~ 4750 2100
NoConn ~ 4750 2000
NoConn ~ 4750 1800
NoConn ~ 4750 1700
NoConn ~ 4750 1300
NoConn ~ 2850 2700
NoConn ~ 2850 2600
NoConn ~ 2850 2500
NoConn ~ 2850 2400
NoConn ~ 2850 2300
$Comp
L ARDUINO_SHIELD SHIELD1
U 1 1 5034C2C0
P 3800 1900
F 0 "SHIELD1" H 3450 2850 60  0000 C CNN
F 1 "ARDUINO_SHIELD" H 3850 950 60  0000 C CNN
	1    3800 1900
	1    0    0    -1  
$EndComp
NoConn ~ 2850 2200
NoConn ~ 2850 2000
NoConn ~ 2850 1700
NoConn ~ 2850 1500
$Comp
L GND #PWR01
U 1 1 5034C6D4
P 5600 2950
F 0 "#PWR01" H 5600 2950 30  0001 C CNN
F 1 "GND" H 5600 2880 30  0001 C CNN
	1    5600 2950
	1    0    0    -1  
$EndComp
$Comp
L CONN_3 RX1
U 1 1 5034C6BD
P 6100 2700
F 0 "RX1" V 6050 2700 50  0000 C CNN
F 1 "CONN_3" V 6150 2700 40  0000 C CNN
	1    6100 2700
	1    0    0    -1  
$EndComp
$Comp
L +3,3V #PWR02
U 1 1 5034C61E
P 2650 1600
F 0 "#PWR02" H 2650 1560 30  0001 C CNN
F 1 "+3,3V" H 2650 1710 30  0000 C CNN
	1    2650 1600
	0    -1   -1   0   
$EndComp
$Comp
L +3,3V #PWR03
U 1 1 5034C5D6
P 5450 2450
F 0 "#PWR03" H 5450 2410 30  0001 C CNN
F 1 "+3,3V" H 5450 2560 30  0000 C CNN
	1    5450 2450
	0    1    1    0   
$EndComp
$Comp
L R R1
U 1 1 5034C5B0
P 5350 2200
F 0 "R1" V 5430 2200 50  0000 C CNN
F 1 "10K" V 5350 2200 50  0000 C CNN
	1    5350 2200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 5034C588
P 5600 2250
F 0 "#PWR04" H 5600 2250 30  0001 C CNN
F 1 "GND" H 5600 2180 30  0001 C CNN
	1    5600 2250
	1    0    0    -1  
$EndComp
$Comp
L +3,3V #PWR05
U 1 1 5034C564
P 5550 1850
F 0 "#PWR05" H 5550 1810 30  0001 C CNN
F 1 "+3,3V" H 5550 1960 30  0000 C CNN
	1    5550 1850
	0    -1   -1   0   
$EndComp
$Comp
L CONN_4 DHT1
U 1 1 5034C54F
P 6100 2000
F 0 "DHT1" V 6050 2000 50  0000 C CNN
F 1 "CONN_4" V 6150 2000 50  0000 C CNN
	1    6100 2000
	1    0    0    -1  
$EndComp
$Comp
L +3,3V #PWR06
U 1 1 5034C49B
P 5550 1500
F 0 "#PWR06" H 5550 1460 30  0001 C CNN
F 1 "+3,3V" H 5550 1610 30  0000 C CNN
	1    5550 1500
	0    -1   -1   0   
$EndComp
$Comp
L CONN_1 ANT1
U 1 1 5034C389
P 5350 1600
F 0 "ANT1" H 5430 1600 40  0000 L CNN
F 1 "CONN_1" H 5350 1655 30  0001 C CNN
	1    5350 1600
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR07
U 1 1 5034C35F
P 4950 1250
F 0 "#PWR07" H 4950 1250 30  0001 C CNN
F 1 "GND" H 4950 1180 30  0001 C CNN
	1    4950 1250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 5034C33B
P 2650 2000
F 0 "#PWR08" H 2650 2000 30  0001 C CNN
F 1 "GND" H 2650 1930 30  0001 C CNN
	1    2650 2000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR09
U 1 1 5034C313
P 5550 1300
F 0 "#PWR09" H 5550 1300 30  0001 C CNN
F 1 "GND" H 5550 1230 30  0001 C CNN
	1    5550 1300
	1    0    0    -1  
$EndComp
$Comp
L CONN_4 TX1
U 1 1 5034C2E9
P 6100 1450
F 0 "TX1" V 6050 1450 50  0000 C CNN
F 1 "CONN_4" V 6150 1450 50  0000 C CNN
	1    6100 1450
	1    0    0    -1  
$EndComp
$EndSCHEMATC
