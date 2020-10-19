EESchema Schematic File Version 4
LIBS:Config-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Module:ESP32-DevKitC-V4 U1
U 1 1 5F6DA698
P 5550 3500
F 0 "U1" H 5950 2150 50  0000 C CNN
F 1 "ESP32-DevKitC-V4" H 5100 2150 50  0000 C CNN
F 2 "" H 5550 3500 50  0001 C CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32-devkitc-v4_reference_design.zip" H 5250 3550 50  0001 C CNN
	1    5550 3500
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Male J1
U 1 1 5F6DDF96
P 3850 3200
F 0 "J1" H 3822 3082 50  0000 R CNN
F 1 "comm_header" H 3822 3173 50  0000 R CNN
F 2 "" H 3850 3200 50  0001 C CNN
F 3 "~" H 3850 3200 50  0001 C CNN
	1    3850 3200
	-1   0    0    1   
$EndComp
$Comp
L power:VCC #PWR02
U 1 1 5F6DDF9C
P 3500 3000
F 0 "#PWR02" H 3500 2850 50  0001 C CNN
F 1 "VCC" V 3518 3127 50  0000 L CNN
F 2 "" H 3500 3000 50  0001 C CNN
F 3 "" H 3500 3000 50  0001 C CNN
	1    3500 3000
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR03
U 1 1 5F6DDFA2
P 3500 3300
F 0 "#PWR03" H 3500 3050 50  0001 C CNN
F 1 "GND" V 3505 3172 50  0000 R CNN
F 2 "" H 3500 3300 50  0001 C CNN
F 3 "" H 3500 3300 50  0001 C CNN
	1    3500 3300
	0    1    1    0   
$EndComp
Wire Wire Line
	3650 3300 3500 3300
Wire Wire Line
	3500 3000 3650 3000
Wire Wire Line
	3650 3100 3500 3100
Wire Wire Line
	3500 3200 3650 3200
Text Label 3500 3200 2    50   ~ 0
sda
Text Label 3500 3100 2    50   ~ 0
scl
$Comp
L power:VCC #PWR01
U 1 1 5F6DF798
P 5550 2000
F 0 "#PWR01" H 5550 1850 50  0001 C CNN
F 1 "VCC" V 5568 2127 50  0000 L CNN
F 2 "" H 5550 2000 50  0001 C CNN
F 3 "" H 5550 2000 50  0001 C CNN
	1    5550 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5F6E046E
P 5550 5000
F 0 "#PWR04" H 5550 4750 50  0001 C CNN
F 1 "GND" V 5555 4872 50  0000 R CNN
F 2 "" H 5550 5000 50  0001 C CNN
F 3 "" H 5550 5000 50  0001 C CNN
	1    5550 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 3800 6300 3800
Wire Wire Line
	6300 3700 6150 3700
Text Label 6300 3700 0    50   ~ 0
sda
Text Label 6300 3800 0    50   ~ 0
scl
Wire Wire Line
	5550 2000 5550 2100
Wire Wire Line
	5550 5000 5550 4900
NoConn ~ 4950 3500
NoConn ~ 4950 3600
NoConn ~ 4950 3700
NoConn ~ 4950 3800
NoConn ~ 4950 3900
NoConn ~ 4950 4000
NoConn ~ 6150 4600
NoConn ~ 6150 4500
NoConn ~ 6150 4400
NoConn ~ 6150 4300
NoConn ~ 6150 4200
NoConn ~ 6150 4100
NoConn ~ 6150 4000
NoConn ~ 6150 3900
NoConn ~ 6150 3600
NoConn ~ 6150 3500
NoConn ~ 6150 3400
NoConn ~ 6150 3300
NoConn ~ 6150 3200
NoConn ~ 6150 3100
NoConn ~ 6150 3000
NoConn ~ 6150 2900
NoConn ~ 6150 2800
NoConn ~ 6150 2700
NoConn ~ 6150 2600
NoConn ~ 6150 2500
NoConn ~ 6150 2400
NoConn ~ 6150 2300
NoConn ~ 5650 2100
NoConn ~ 4950 2300
NoConn ~ 4950 2500
NoConn ~ 4950 2600
$EndSCHEMATC
