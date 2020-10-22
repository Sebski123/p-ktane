EESchema Schematic File Version 4
LIBS:NeedyCapacitorDischarge-cache
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
L MCU_Microchip_ATmega:ATmega328-PU U1
U 1 1 5F599BB8
P 3800 3250
F 0 "U1" H 3156 3296 50  0000 R CNN
F 1 "ATmega328-PU" H 3156 3205 50  0000 R CNN
F 2 "Package_DIP:DIP-28_W7.62mm" H 3800 3250 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega328_P%20AVR%20MCU%20with%20picoPower%20Technology%20Data%20Sheet%2040001984A.pdf" H 3800 3250 50  0001 C CNN
	1    3800 3250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5F59E059
P 2200 2150
F 0 "C1" H 2315 2196 50  0000 L CNN
F 1 "100nF" H 2315 2105 50  0000 L CNN
F 2 "" H 2238 2000 50  0001 C CNN
F 3 "~" H 2200 2150 50  0001 C CNN
	1    2200 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 1750 3800 1750
$Comp
L power:VCC #PWR08
U 1 1 5F5B8A8F
P 3800 1600
F 0 "#PWR08" H 3800 1450 50  0001 C CNN
F 1 "VCC" H 3817 1773 50  0000 C CNN
F 2 "" H 3800 1600 50  0001 C CNN
F 3 "" H 3800 1600 50  0001 C CNN
	1    3800 1600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR09
U 1 1 5F5B9910
P 3800 4850
F 0 "#PWR09" H 3800 4600 50  0001 C CNN
F 1 "GND" H 3805 4677 50  0000 C CNN
F 2 "" H 3800 4850 50  0001 C CNN
F 3 "" H 3800 4850 50  0001 C CNN
	1    3800 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 1750 3800 1600
Connection ~ 3800 1750
$Comp
L power:VCC #PWR06
U 1 1 5F5BC7FD
P 2200 1900
F 0 "#PWR06" H 2200 1750 50  0001 C CNN
F 1 "VCC" H 2217 2073 50  0000 C CNN
F 2 "" H 2200 1900 50  0001 C CNN
F 3 "" H 2200 1900 50  0001 C CNN
	1    2200 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 4850 3800 4750
$Comp
L power:GND #PWR07
U 1 1 5F5BDC36
P 2200 2400
F 0 "#PWR07" H 2200 2150 50  0001 C CNN
F 1 "GND" H 2205 2227 50  0000 C CNN
F 2 "" H 2200 2400 50  0001 C CNN
F 3 "" H 2200 2400 50  0001 C CNN
	1    2200 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 2400 2200 2300
Wire Wire Line
	2200 2000 2200 1900
$Comp
L power:VCC #PWR05
U 1 1 5F5C1662
P 1600 1950
F 0 "#PWR05" H 1600 1800 50  0001 C CNN
F 1 "VCC" H 1617 2123 50  0000 C CNN
F 2 "" H 1600 1950 50  0001 C CNN
F 3 "" H 1600 1950 50  0001 C CNN
	1    1600 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 2050 1600 1950
Wire Wire Line
	1600 2350 1600 2450
Text Label 1600 2450 0    50   ~ 0
rst
Wire Wire Line
	3200 2200 3150 2200
Text Label 3150 2200 2    50   ~ 0
rst
$Comp
L Connector:Conn_01x05_Female J2
U 1 1 5F5D16D2
P 1750 4150
F 0 "J2" H 1778 4176 50  0000 L CNN
F 1 "prog_header" H 1778 4085 50  0000 L CNN
F 2 "" H 1750 4150 50  0001 C CNN
F 3 "~" H 1750 4150 50  0001 C CNN
	1    1750 4150
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Male J1
U 1 1 5F5D255B
P 1750 3550
F 0 "J1" H 1722 3432 50  0000 R CNN
F 1 "comm_header" H 1722 3523 50  0000 R CNN
F 2 "" H 1750 3550 50  0001 C CNN
F 3 "~" H 1750 3550 50  0001 C CNN
	1    1750 3550
	-1   0    0    1   
$EndComp
$Comp
L power:VCC #PWR01
U 1 1 5F5DC5B2
P 2150 3000
F 0 "#PWR01" H 2150 2850 50  0001 C CNN
F 1 "VCC" V 2168 3127 50  0000 L CNN
F 2 "" H 2150 3000 50  0001 C CNN
F 3 "" H 2150 3000 50  0001 C CNN
	1    2150 3000
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5F5DDC71
P 1400 3650
F 0 "#PWR02" H 1400 3400 50  0001 C CNN
F 1 "GND" V 1405 3522 50  0000 R CNN
F 2 "" H 1400 3650 50  0001 C CNN
F 3 "" H 1400 3650 50  0001 C CNN
	1    1400 3650
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5F5DE488
P 1400 4350
F 0 "#PWR04" H 1400 4100 50  0001 C CNN
F 1 "GND" V 1405 4222 50  0000 R CNN
F 2 "" H 1400 4350 50  0001 C CNN
F 3 "" H 1400 4350 50  0001 C CNN
	1    1400 4350
	0    1    1    0   
$EndComp
Wire Wire Line
	1550 4150 1400 4150
Wire Wire Line
	1550 3950 1400 3950
Wire Wire Line
	1400 4350 1550 4350
Wire Wire Line
	1550 3650 1400 3650
Wire Wire Line
	1400 3350 1550 3350
Text Label 1400 4150 2    50   ~ 0
rst
Wire Wire Line
	4400 3450 4500 3450
Text Label 4500 3450 0    50   ~ 0
scl
Wire Wire Line
	4400 3750 4500 3750
Text Label 4500 3850 0    50   ~ 0
tx
Text Label 4500 3750 0    50   ~ 0
rx
Wire Wire Line
	4500 3850 4400 3850
Wire Wire Line
	1550 3450 1400 3450
Wire Wire Line
	1400 3550 1550 3550
Wire Wire Line
	1550 4050 1400 4050
Wire Wire Line
	1400 4250 1550 4250
Text Label 1400 3550 2    50   ~ 0
sda
Text Label 1400 3450 2    50   ~ 0
scl
Text Label 1400 4250 2    50   ~ 0
rx
Text Label 1400 4050 2    50   ~ 0
tx
$Comp
L Device:R_US R1
U 1 1 5F59B8EB
P 1600 2200
F 0 "R1" H 1668 2246 50  0000 L CNN
F 1 "10k" H 1668 2155 50  0000 L CNN
F 2 "" V 1640 2190 50  0001 C CNN
F 3 "~" H 1600 2200 50  0001 C CNN
	1    1600 2200
	1    0    0    -1  
$EndComp
NoConn ~ 3200 2050
Wire Wire Line
	4400 3350 4500 3350
Text Label 4500 3350 0    50   ~ 0
sda
$Comp
L power:GND #PWR011
U 1 1 5F5AFC00
P 2750 3650
F 0 "#PWR011" H 2750 3400 50  0001 C CNN
F 1 "GND" H 2755 3477 50  0000 C CNN
F 2 "" H 2750 3650 50  0001 C CNN
F 3 "" H 2750 3650 50  0001 C CNN
	1    2750 3650
	-1   0    0    1   
$EndComp
Wire Wire Line
	2750 3650 2750 3750
Wire Wire Line
	2750 4450 2750 4550
Wire Wire Line
	4400 4450 4500 4450
Text Label 4500 4450 0    50   ~ 0
status_red
Text Label 2750 4550 3    50   ~ 0
status_red
$Comp
L Display_Character:DC56-11EWA U3
U 1 1 5F5D7C6C
P 8050 3300
F 0 "U3" H 8050 3967 50  0000 C CNN
F 1 "LTD6640E" H 8050 3876 50  0000 C CNN
F 2 "Display_7Segment:DA56-11SURKWA" H 8070 2650 50  0001 C CNN
F 3 "http://www.kingbrightusa.com/images/catalog/SPEC/DC56-11EWA.pdf" H 7930 3400 50  0001 C CNN
	1    8050 3300
	0    -1   -1   0   
$EndComp
$Comp
L Driver_LED:MAX7219 U2
U 1 1 5F5DAEF3
P 5900 2850
F 0 "U2" H 6150 1900 50  0000 C CNN
F 1 "MAX7219" H 5650 1900 50  0000 C CNN
F 2 "" H 5850 2900 50  0001 C CNN
F 3 "https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf" H 5950 2700 50  0001 C CNN
	1    5900 2850
	1    0    0    1   
$EndComp
$Comp
L Device:R_US RSet1
U 1 1 5F5FE586
P 5650 3900
F 0 "RSet1" H 5718 3946 50  0000 L CNN
F 1 "27k" H 5718 3855 50  0000 L CNN
F 2 "" V 5690 3890 50  0001 C CNN
F 3 "~" H 5650 3900 50  0001 C CNN
	1    5650 3900
	0    1    1    0   
$EndComp
Wire Wire Line
	4400 2050 5500 2050
Wire Wire Line
	5500 2150 4400 2150
Wire Wire Line
	4400 2250 5500 2250
$Comp
L power:VCC #PWR015
U 1 1 5F5AAC2E
P 5900 3950
F 0 "#PWR015" H 5900 3800 50  0001 C CNN
F 1 "VCC" H 5917 4123 50  0000 C CNN
F 2 "" H 5900 3950 50  0001 C CNN
F 3 "" H 5900 3950 50  0001 C CNN
	1    5900 3950
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR014
U 1 1 5F5AC817
P 5900 1750
F 0 "#PWR014" H 5900 1500 50  0001 C CNN
F 1 "GND" H 5905 1577 50  0000 C CNN
F 2 "" H 5900 1750 50  0001 C CNN
F 3 "" H 5900 1750 50  0001 C CNN
	1    5900 1750
	-1   0    0    1   
$EndComp
Wire Wire Line
	5900 1750 5900 1850
Wire Wire Line
	5900 3850 5900 3900
Wire Wire Line
	5500 3900 5500 3650
Wire Wire Line
	5800 3900 5900 3900
Connection ~ 5900 3900
Wire Wire Line
	5900 3900 5900 3950
$Comp
L Device:C C2
U 1 1 5F5B7770
P 5200 2900
F 0 "C2" H 5315 2946 50  0000 L CNN
F 1 "100nF" H 5315 2855 50  0000 L CNN
F 2 "" H 5238 2750 50  0001 C CNN
F 3 "~" H 5200 2900 50  0001 C CNN
	1    5200 2900
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR012
U 1 1 5F5B86DF
P 5200 2650
F 0 "#PWR012" H 5200 2500 50  0001 C CNN
F 1 "VCC" H 5217 2823 50  0000 C CNN
F 2 "" H 5200 2650 50  0001 C CNN
F 3 "" H 5200 2650 50  0001 C CNN
	1    5200 2650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR013
U 1 1 5F5B8DDC
P 5200 3150
F 0 "#PWR013" H 5200 2900 50  0001 C CNN
F 1 "GND" H 5205 2977 50  0000 C CNN
F 2 "" H 5200 3150 50  0001 C CNN
F 3 "" H 5200 3150 50  0001 C CNN
	1    5200 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 3150 5200 3050
Wire Wire Line
	5200 2650 5200 2750
Wire Wire Line
	7650 3900 7650 4000
Wire Wire Line
	7650 4000 7300 4000
Wire Wire Line
	7300 4000 7300 3650
Wire Wire Line
	7300 3650 6300 3650
Wire Wire Line
	7650 2700 7650 2600
Wire Wire Line
	7650 2600 7300 2600
Wire Wire Line
	7300 2600 7300 3650
Connection ~ 7300 3650
Wire Wire Line
	7750 3900 7750 4050
Wire Wire Line
	7750 4050 7250 4050
Wire Wire Line
	7250 4050 7250 3550
Wire Wire Line
	7250 3550 6300 3550
Wire Wire Line
	6300 3450 7200 3450
Wire Wire Line
	7200 3450 7200 4100
Wire Wire Line
	7200 4100 7850 4100
Wire Wire Line
	7850 4100 7850 3900
Wire Wire Line
	7950 3900 7950 4150
Wire Wire Line
	7950 4150 7150 4150
Wire Wire Line
	7150 4150 7150 3350
Wire Wire Line
	7150 3350 6300 3350
Wire Wire Line
	6300 3250 7100 3250
Wire Wire Line
	7100 3250 7100 4200
Wire Wire Line
	7100 4200 8050 4200
Wire Wire Line
	8050 4200 8050 3900
Wire Wire Line
	8150 3900 8150 4250
Wire Wire Line
	8150 4250 7050 4250
Wire Wire Line
	7050 4250 7050 3150
Wire Wire Line
	7050 3150 6300 3150
Wire Wire Line
	7000 4300 8250 4300
Wire Wire Line
	8250 4300 8250 3900
Wire Wire Line
	7750 2700 7750 2550
Wire Wire Line
	7750 2550 7250 2550
Wire Wire Line
	7250 2550 7250 3550
Connection ~ 7250 3550
Wire Wire Line
	7200 3450 7200 2500
Wire Wire Line
	7200 2500 7850 2500
Wire Wire Line
	7850 2500 7850 2700
Connection ~ 7200 3450
Wire Wire Line
	7950 2700 7950 2450
Wire Wire Line
	7950 2450 7150 2450
Wire Wire Line
	7150 2450 7150 3350
Connection ~ 7150 3350
Wire Wire Line
	7100 3250 7100 2400
Wire Wire Line
	7100 2400 8050 2400
Wire Wire Line
	8050 2400 8050 2700
Connection ~ 7100 3250
Wire Wire Line
	8150 2700 8150 2350
Wire Wire Line
	8150 2350 7050 2350
Wire Wire Line
	7050 2350 7050 3150
Connection ~ 7050 3150
Wire Wire Line
	7000 2300 8250 2300
Wire Wire Line
	8250 2300 8250 2700
NoConn ~ 8350 3900
NoConn ~ 8350 2700
Wire Wire Line
	8450 3900 8450 4000
Wire Wire Line
	8450 4000 8600 4000
Wire Wire Line
	8600 4000 8600 2250
Wire Wire Line
	8600 2250 6950 2250
Wire Wire Line
	6950 2250 6950 2850
Wire Wire Line
	6950 2850 6300 2850
Wire Wire Line
	6300 2750 6900 2750
Wire Wire Line
	6900 2750 6900 2200
Wire Wire Line
	6900 2200 8450 2200
Wire Wire Line
	8450 2200 8450 2700
Wire Wire Line
	7000 2300 7000 3050
Text Label 6350 3250 0    50   ~ 0
seg_e
Text Label 6350 3350 0    50   ~ 0
seg_d
Text Label 6350 3450 0    50   ~ 0
seg_c
Text Label 6350 3550 0    50   ~ 0
seg_b
Text Label 6350 3650 0    50   ~ 0
seg_a
Wire Wire Line
	6300 2650 6400 2650
Wire Wire Line
	6300 2550 6400 2550
Text Label 6400 2650 0    50   ~ 0
dig_2
Text Label 6400 2550 0    50   ~ 0
dig_3
NoConn ~ 4400 2350
NoConn ~ 4400 2450
NoConn ~ 4400 2550
NoConn ~ 4400 2650
NoConn ~ 4400 2750
NoConn ~ 4400 2950
NoConn ~ 4400 3050
NoConn ~ 4400 3150
NoConn ~ 4400 3250
Wire Wire Line
	6300 3050 7000 3050
Connection ~ 7000 3050
Wire Wire Line
	7000 3050 7000 4300
NoConn ~ 6300 2050
$Comp
L Device:LED D13
U 1 1 5F749B98
P 2750 3900
F 0 "D13" H 2743 4116 50  0000 C CNN
F 1 "Red" H 2743 4025 50  0000 C CNN
F 2 "" H 2750 3900 50  0001 C CNN
F 3 "~" H 2750 3900 50  0001 C CNN
	1    2750 3900
	0    1    1    0   
$EndComp
NoConn ~ 4400 4350
$Comp
L LED:HDSP-4830_2 BAR1
U 1 1 5F6641B2
P 10050 1200
F 0 "BAR1" H 10050 1867 50  0000 C CNN
F 1 "DC-10SURKWA" H 10050 1776 50  0000 C CNN
F 2 "Display:HDSP-4830" H 10050 400 50  0001 C CNN
F 3 "https://docs.broadcom.com/docs/AV02-1798EN" H 8050 1400 50  0001 C CNN
	1    10050 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 800  9800 800 
Wire Wire Line
	9850 900  9800 900 
Wire Wire Line
	9800 1000 9850 1000
Wire Wire Line
	9850 1100 9800 1100
Wire Wire Line
	9800 1200 9850 1200
Wire Wire Line
	9800 1300 9850 1300
Wire Wire Line
	9800 1400 9850 1400
Wire Wire Line
	9800 1500 9850 1500
Wire Wire Line
	9800 1600 9850 1600
Wire Wire Line
	9800 1700 9850 1700
Wire Wire Line
	10300 800  10250 800 
Wire Wire Line
	10300 900  10250 900 
Wire Wire Line
	10300 1000 10250 1000
Wire Wire Line
	10300 1100 10250 1100
Wire Wire Line
	10300 1200 10250 1200
Wire Wire Line
	10300 1300 10250 1300
Wire Wire Line
	10300 1400 10250 1400
Wire Wire Line
	10300 1500 10250 1500
Wire Wire Line
	10300 1600 10250 1600
Wire Wire Line
	10300 1700 10250 1700
$Comp
L LED:HDSP-4830_2 BAR2
U 1 1 5F6C2551
P 10050 2550
F 0 "BAR2" H 10050 3217 50  0000 C CNN
F 1 "DC-10SURKWA" H 10050 3126 50  0000 C CNN
F 2 "Display:HDSP-4830" H 10050 1750 50  0001 C CNN
F 3 "https://docs.broadcom.com/docs/AV02-1798EN" H 8050 2750 50  0001 C CNN
	1    10050 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 2150 9800 2150
Wire Wire Line
	9850 2250 9800 2250
Wire Wire Line
	9800 2350 9850 2350
Wire Wire Line
	9850 2450 9800 2450
Wire Wire Line
	9800 2550 9850 2550
Wire Wire Line
	9800 2650 9850 2650
Wire Wire Line
	9800 2750 9850 2750
Wire Wire Line
	9800 2850 9850 2850
Wire Wire Line
	9800 2950 9850 2950
Wire Wire Line
	9800 3050 9850 3050
Wire Wire Line
	10300 2150 10250 2150
Wire Wire Line
	10300 2250 10250 2250
Wire Wire Line
	10300 2350 10250 2350
Wire Wire Line
	10300 2450 10250 2450
Wire Wire Line
	10300 2550 10250 2550
Wire Wire Line
	10300 2650 10250 2650
Wire Wire Line
	10300 2750 10250 2750
Wire Wire Line
	10300 2850 10250 2850
Wire Wire Line
	10300 2950 10250 2950
Wire Wire Line
	10300 3050 10250 3050
$Comp
L LED:HDSP-4830_2 BAR3
U 1 1 5F6C96C9
P 10050 3900
F 0 "BAR3" H 10050 4567 50  0000 C CNN
F 1 "DC-10SURKWA" H 10050 4476 50  0000 C CNN
F 2 "Display:HDSP-4830" H 10050 3100 50  0001 C CNN
F 3 "https://docs.broadcom.com/docs/AV02-1798EN" H 8050 4100 50  0001 C CNN
	1    10050 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 3500 9800 3500
Wire Wire Line
	9850 3600 9800 3600
Wire Wire Line
	9800 3700 9850 3700
Wire Wire Line
	9850 3800 9800 3800
Wire Wire Line
	9800 3900 9850 3900
Wire Wire Line
	9800 4000 9850 4000
Wire Wire Line
	9800 4100 9850 4100
Wire Wire Line
	9800 4200 9850 4200
Wire Wire Line
	9800 4300 9850 4300
Wire Wire Line
	9800 4400 9850 4400
Wire Wire Line
	10300 3500 10250 3500
Wire Wire Line
	10300 3600 10250 3600
Wire Wire Line
	10300 3700 10250 3700
Wire Wire Line
	10300 3800 10250 3800
Wire Wire Line
	10300 3900 10250 3900
Wire Wire Line
	10300 4000 10250 4000
Wire Wire Line
	10300 4100 10250 4100
Wire Wire Line
	10300 4200 10250 4200
Wire Wire Line
	10300 4300 10250 4300
Wire Wire Line
	10300 4400 10250 4400
$Comp
L LED:HDSP-4830_2 BAR4
U 1 1 5F6D16D4
P 10050 5250
F 0 "BAR4" H 10050 5917 50  0000 C CNN
F 1 "DC-10SURKWA" H 10050 5826 50  0000 C CNN
F 2 "Display:HDSP-4830" H 10050 4450 50  0001 C CNN
F 3 "https://docs.broadcom.com/docs/AV02-1798EN" H 8050 5450 50  0001 C CNN
	1    10050 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 4850 9800 4850
Wire Wire Line
	9850 4950 9800 4950
Wire Wire Line
	9800 5050 9850 5050
Wire Wire Line
	9850 5150 9800 5150
Wire Wire Line
	9800 5250 9850 5250
Wire Wire Line
	9800 5350 9850 5350
Wire Wire Line
	9800 5450 9850 5450
Wire Wire Line
	9800 5550 9850 5550
Wire Wire Line
	9800 5650 9850 5650
Wire Wire Line
	9800 5750 9850 5750
Wire Wire Line
	10300 4850 10250 4850
Wire Wire Line
	10300 4950 10250 4950
Wire Wire Line
	10300 5050 10250 5050
Wire Wire Line
	10300 5150 10250 5150
Wire Wire Line
	10300 5250 10250 5250
Wire Wire Line
	10300 5350 10250 5350
Wire Wire Line
	10300 5450 10250 5450
Wire Wire Line
	10300 5550 10250 5550
Wire Wire Line
	10300 5650 10250 5650
Wire Wire Line
	10300 5750 10250 5750
Text Label 6350 3150 0    50   ~ 0
seg_f
Text Label 6350 3050 0    50   ~ 0
seg_g
Text Label 6350 2950 0    50   ~ 0
seg_dp
Wire Wire Line
	6350 2950 6300 2950
Wire Wire Line
	6400 2450 6300 2450
Wire Wire Line
	6400 2350 6300 2350
Wire Wire Line
	6400 2250 6300 2250
Text Label 6400 2450 0    50   ~ 0
dig_4
Text Label 6400 2350 0    50   ~ 0
dig_5
Text Label 6400 2250 0    50   ~ 0
dig_6
Text Label 10300 800  0    50   ~ 0
dig_2
Text Label 10300 900  0    50   ~ 0
dig_2
Text Label 10300 1000 0    50   ~ 0
dig_2
Text Label 10300 1100 0    50   ~ 0
dig_2
Text Label 10300 1200 0    50   ~ 0
dig_2
Text Label 10300 1300 0    50   ~ 0
dig_2
Text Label 10300 1400 0    50   ~ 0
dig_2
Text Label 10300 1500 0    50   ~ 0
dig_2
Text Label 10300 1600 0    50   ~ 0
dig_3
Text Label 10300 1700 0    50   ~ 0
dig_3
Text Label 10300 2150 0    50   ~ 0
dig_3
Text Label 10300 2250 0    50   ~ 0
dig_3
Text Label 10300 2350 0    50   ~ 0
dig_3
Text Label 10300 2450 0    50   ~ 0
dig_3
Text Label 10300 2550 0    50   ~ 0
dig_3
Text Label 10300 2650 0    50   ~ 0
dig_3
Text Label 10300 2750 0    50   ~ 0
dig_4
Text Label 10300 2850 0    50   ~ 0
dig_4
Text Label 10300 2950 0    50   ~ 0
dig_4
Text Label 10300 3050 0    50   ~ 0
dig_4
Text Label 10300 3500 0    50   ~ 0
dig_4
Text Label 10300 3600 0    50   ~ 0
dig_4
Text Label 10300 3700 0    50   ~ 0
dig_4
Text Label 10300 3800 0    50   ~ 0
dig_4
Text Label 10300 3900 0    50   ~ 0
dig_5
Text Label 10300 4000 0    50   ~ 0
dig_5
Text Label 10300 4100 0    50   ~ 0
dig_5
Text Label 10300 4200 0    50   ~ 0
dig_5
Text Label 10300 4300 0    50   ~ 0
dig_5
Text Label 10300 4400 0    50   ~ 0
dig_5
Text Label 10300 4850 0    50   ~ 0
dig_5
Text Label 10300 4950 0    50   ~ 0
dig_5
NoConn ~ 6300 2150
Text Label 10300 5050 0    50   ~ 0
dig_6
Text Label 10300 5150 0    50   ~ 0
dig_6
Text Label 10300 5250 0    50   ~ 0
dig_6
Text Label 10300 5350 0    50   ~ 0
dig_6
Text Label 10300 5450 0    50   ~ 0
dig_6
Text Label 10300 5550 0    50   ~ 0
dig_6
Text Label 10300 5650 0    50   ~ 0
dig_6
Text Label 10300 5750 0    50   ~ 0
dig_6
Text Label 9800 800  2    50   ~ 0
seg_a
Text Label 9800 1600 2    50   ~ 0
seg_a
Text Label 9800 2750 2    50   ~ 0
seg_a
Text Label 9800 3900 2    50   ~ 0
seg_a
Text Label 9800 5050 2    50   ~ 0
seg_a
Text Label 9800 900  2    50   ~ 0
seg_b
Text Label 9800 1000 2    50   ~ 0
seg_c
Text Label 9800 1100 2    50   ~ 0
seg_d
Text Label 9800 1200 2    50   ~ 0
seg_e
Text Label 9800 1300 2    50   ~ 0
seg_f
Text Label 9800 1400 2    50   ~ 0
seg_g
Text Label 9800 1500 2    50   ~ 0
seg_dp
Text Label 9800 1700 2    50   ~ 0
seg_b
Text Label 9800 2150 2    50   ~ 0
seg_c
Text Label 9800 2250 2    50   ~ 0
seg_d
Text Label 9800 2350 2    50   ~ 0
seg_e
Text Label 9800 2450 2    50   ~ 0
seg_f
Text Label 9800 2550 2    50   ~ 0
seg_g
Text Label 9800 2650 2    50   ~ 0
seg_dp
Text Label 9800 2850 2    50   ~ 0
seg_b
Text Label 9800 2950 2    50   ~ 0
seg_c
Text Label 9800 3050 2    50   ~ 0
seg_d
Text Label 9800 3500 2    50   ~ 0
seg_e
Text Label 9800 3600 2    50   ~ 0
seg_f
Text Label 9800 3700 2    50   ~ 0
seg_g
Text Label 9800 3800 2    50   ~ 0
seg_dp
Text Label 9800 4000 2    50   ~ 0
seg_b
Text Label 9800 4100 2    50   ~ 0
seg_c
Text Label 9800 4200 2    50   ~ 0
seg_d
Text Label 9800 4300 2    50   ~ 0
seg_e
Text Label 9800 4400 2    50   ~ 0
seg_f
Text Label 9800 4850 2    50   ~ 0
seg_g
Text Label 9800 4950 2    50   ~ 0
seg_dp
Text Label 9800 5150 2    50   ~ 0
seg_b
Text Label 9800 5250 2    50   ~ 0
seg_c
Text Label 9800 5350 2    50   ~ 0
seg_d
Text Label 9800 5450 2    50   ~ 0
seg_e
Text Label 9800 5550 2    50   ~ 0
seg_f
Text Label 9800 5650 2    50   ~ 0
seg_g
Text Label 9800 5750 2    50   ~ 0
seg_dp
$Comp
L Device:LED D1
U 1 1 5F8DB07A
P 5500 4650
F 0 "D1" V 5539 4533 50  0000 R CNN
F 1 "LED" V 5448 4533 50  0000 R CNN
F 2 "" H 5500 4650 50  0001 C CNN
F 3 "~" H 5500 4650 50  0001 C CNN
	1    5500 4650
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_Push_Dual_x2 SW1
U 1 1 5F8DFBFB
P 6050 4500
F 0 "SW1" H 5950 4650 50  0000 L CNN
F 1 "SW_Push_Dual_x2" H 5900 4400 50  0000 L CNN
F 2 "" H 6050 4700 50  0001 C CNN
F 3 "~" H 6050 4700 50  0001 C CNN
	1    6050 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 3950 5500 3950
Wire Wire Line
	5500 3950 5500 4500
$Comp
L power:GND #PWR016
U 1 1 5F8FD49E
P 6350 4500
F 0 "#PWR016" H 6350 4250 50  0001 C CNN
F 1 "GND" H 6355 4327 50  0000 C CNN
F 2 "" H 6350 4500 50  0001 C CNN
F 3 "" H 6350 4500 50  0001 C CNN
	1    6350 4500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6350 4500 6250 4500
$Comp
L Device:R_US R3
U 1 1 5F9260E7
P 5500 5050
F 0 "R3" V 5400 4950 50  0000 L CNN
F 1 "330" V 5600 5000 50  0000 L CNN
F 2 "" V 5540 5040 50  0001 C CNN
F 3 "~" H 5500 5050 50  0001 C CNN
	1    5500 5050
	-1   0    0    1   
$EndComp
Connection ~ 5500 4500
$Comp
L power:VCC #PWR010
U 1 1 5F9310D1
P 5500 5300
F 0 "#PWR010" H 5500 5150 50  0001 C CNN
F 1 "VCC" H 5517 5473 50  0000 C CNN
F 2 "" H 5500 5300 50  0001 C CNN
F 3 "" H 5500 5300 50  0001 C CNN
	1    5500 5300
	-1   0    0    1   
$EndComp
Wire Wire Line
	5500 5300 5500 5200
Wire Wire Line
	5500 4900 5500 4800
$Comp
L Device:R_US R2
U 1 1 5F94E90A
P 2750 4300
F 0 "R2" V 2650 4200 50  0000 L CNN
F 1 "680" V 2850 4250 50  0000 L CNN
F 2 "" V 2790 4290 50  0001 C CNN
F 3 "~" H 2750 4300 50  0001 C CNN
	1    2750 4300
	-1   0    0    1   
$EndComp
Wire Wire Line
	2750 4150 2750 4050
Wire Wire Line
	5500 4500 5850 4500
NoConn ~ 4400 4250
NoConn ~ 4400 4150
NoConn ~ 4400 4050
$Comp
L Device:Polyfuse F1
U 1 1 5F9289D4
P 1900 3000
F 0 "F1" V 1675 3000 50  0000 C CNN
F 1 "Polyfuse" V 1766 3000 50  0000 C CNN
F 2 "" H 1950 2800 50  0001 L CNN
F 3 "~" H 1900 3000 50  0001 C CNN
	1    1900 3000
	0    1    1    0   
$EndComp
Text Label 1400 3950 2    50   ~ 0
UVCC
Text Label 1400 3350 2    50   ~ 0
UVCC
Text Label 1650 3000 2    50   ~ 0
UVCC
Wire Wire Line
	1650 3000 1750 3000
Wire Wire Line
	2050 3000 2150 3000
$EndSCHEMATC
