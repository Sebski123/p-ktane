EESchema Schematic File Version 4
LIBS:MorseCode-cache
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
U 1 1 5F833469
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
U 1 1 5F83346C
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
U 1 1 5F83346D
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
U 1 1 5F83346E
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
U 1 1 5F833470
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
P 2250 3050
F 0 "#PWR01" H 2250 2900 50  0001 C CNN
F 1 "VCC" V 2268 3177 50  0000 L CNN
F 2 "" H 2250 3050 50  0001 C CNN
F 3 "" H 2250 3050 50  0001 C CNN
	1    2250 3050
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
L Device:LED_RCBG D13
U 1 1 5F5A1178
P 2750 3950
F 0 "D13" V 2704 4310 50  0000 L CNN
F 1 "LED_RCBG" V 2795 4310 50  0000 L CNN
F 2 "" H 2750 3900 50  0001 C CNN
F 3 "~" H 2750 3900 50  0001 C CNN
	1    2750 3950
	0    1    1    0   
$EndComp
NoConn ~ 2550 4150
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
	2950 4450 2950 4550
Wire Wire Line
	4400 4350 4500 4350
Wire Wire Line
	4400 4450 4500 4450
Text Label 4500 4350 0    50   ~ 0
status_grn
Text Label 4500 4450 0    50   ~ 0
status_red
Text Label 2750 4550 3    50   ~ 0
status_grn
Text Label 2950 4550 3    50   ~ 0
status_red
NoConn ~ 4400 2550
NoConn ~ 4400 2650
NoConn ~ 4400 2750
NoConn ~ 4400 2950
NoConn ~ 4400 3050
NoConn ~ 4400 3150
NoConn ~ 4400 3250
$Comp
L Device:R_US R2
U 1 1 5F833475
P 2750 4300
F 0 "R2" H 2600 4350 50  0000 L CNN
F 1 "1k" H 2600 4250 50  0000 L CNN
F 2 "" V 2790 4290 50  0001 C CNN
F 3 "~" H 2750 4300 50  0001 C CNN
	1    2750 4300
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R3
U 1 1 5F833476
P 2950 4300
F 0 "R3" H 3018 4346 50  0000 L CNN
F 1 "680" H 3018 4255 50  0000 L CNN
F 2 "" V 2990 4290 50  0001 C CNN
F 3 "~" H 2950 4300 50  0001 C CNN
	1    2950 4300
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push_Dual_x2 SW1
U 1 1 5F81C82F
P 5500 4950
F 0 "SW1" H 5500 5235 50  0000 C CNN
F 1 "Left" H 5500 5144 50  0000 C CNN
F 2 "" H 5500 5150 50  0001 C CNN
F 3 "~" H 5500 5150 50  0001 C CNN
	1    5500 4950
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push_Dual_x2 SW2
U 1 1 5F81D454
P 5500 5350
F 0 "SW2" H 5500 5635 50  0000 C CNN
F 1 "Right" H 5500 5544 50  0000 C CNN
F 2 "" H 5500 5550 50  0001 C CNN
F 3 "~" H 5500 5550 50  0001 C CNN
	1    5500 5350
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D1
U 1 1 5F81DDBC
P 6850 5000
F 0 "D1" H 6843 4745 50  0000 C CNN
F 1 "Morse_Light" H 6843 4836 50  0000 C CNN
F 2 "" H 6850 5000 50  0001 C CNN
F 3 "~" H 6850 5000 50  0001 C CNN
	1    6850 5000
	-1   0    0    1   
$EndComp
$Comp
L Driver_LED:MAX7219 U?
U 1 1 5F8536B6
P 5900 2850
AR Path="/5F832F87/5F8536B6" Ref="U?"  Part="1" 
AR Path="/5F8536B6" Ref="U4"  Part="1" 
F 0 "U4" H 6150 1900 50  0000 C CNN
F 1 "MAX7219" H 5650 1900 50  0000 C CNN
F 2 "" H 5850 2900 50  0001 C CNN
F 3 "https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf" H 5950 2700 50  0001 C CNN
	1    5900 2850
	1    0    0    1   
$EndComp
$Comp
L Device:R_US RSet?
U 1 1 5F8536BC
P 5650 3900
AR Path="/5F832F87/5F8536BC" Ref="RSet?"  Part="1" 
AR Path="/5F8536BC" Ref="RSet1"  Part="1" 
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
L power:VCC #PWR?
U 1 1 5F8536C5
P 5900 3950
AR Path="/5F832F87/5F8536C5" Ref="#PWR?"  Part="1" 
AR Path="/5F8536C5" Ref="#PWR014"  Part="1" 
F 0 "#PWR014" H 5900 3800 50  0001 C CNN
F 1 "VCC" H 5917 4123 50  0000 C CNN
F 2 "" H 5900 3950 50  0001 C CNN
F 3 "" H 5900 3950 50  0001 C CNN
	1    5900 3950
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F8536CB
P 5900 1750
AR Path="/5F832F87/5F8536CB" Ref="#PWR?"  Part="1" 
AR Path="/5F8536CB" Ref="#PWR010"  Part="1" 
F 0 "#PWR010" H 5900 1500 50  0001 C CNN
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
L Device:C C?
U 1 1 5F8536D7
P 5200 2900
AR Path="/5F832F87/5F8536D7" Ref="C?"  Part="1" 
AR Path="/5F8536D7" Ref="C2"  Part="1" 
F 0 "C2" H 5315 2946 50  0000 L CNN
F 1 "100nF" H 5315 2855 50  0000 L CNN
F 2 "" H 5238 2750 50  0001 C CNN
F 3 "~" H 5200 2900 50  0001 C CNN
	1    5200 2900
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR?
U 1 1 5F8536DD
P 5200 2650
AR Path="/5F832F87/5F8536DD" Ref="#PWR?"  Part="1" 
AR Path="/5F8536DD" Ref="#PWR012"  Part="1" 
F 0 "#PWR012" H 5200 2500 50  0001 C CNN
F 1 "VCC" H 5217 2823 50  0000 C CNN
F 2 "" H 5200 2650 50  0001 C CNN
F 3 "" H 5200 2650 50  0001 C CNN
	1    5200 2650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F8536E3
P 5200 3150
AR Path="/5F832F87/5F8536E3" Ref="#PWR?"  Part="1" 
AR Path="/5F8536E3" Ref="#PWR013"  Part="1" 
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
$Comp
L Display_Character:KCSC02-105 U?
U 1 1 5F8536EB
P 7450 4050
AR Path="/5F832F87/5F8536EB" Ref="U?"  Part="1" 
AR Path="/5F8536EB" Ref="U5"  Part="1" 
F 0 "U5" V 7400 4650 50  0000 R CNN
F 1 "KCSC02-105" V 7500 5050 50  0000 R CNN
F 2 "Display_7Segment:KCSC02-105" H 7450 3450 50  0001 C CNN
F 3 "http://www.kingbright.com/attachments/file/psearch/000/00/00/KCSC02-105(Ver.9A).pdf" H 6950 4525 50  0001 L CNN
	1    7450 4050
	0    -1   1    0   
$EndComp
$Comp
L Display_Character:KCSC02-105 U?
U 1 1 5F8536F1
P 9000 4050
AR Path="/5F832F87/5F8536F1" Ref="U?"  Part="1" 
AR Path="/5F8536F1" Ref="U6"  Part="1" 
F 0 "U6" V 8954 3520 50  0000 R CNN
F 1 "KCSC02-105" V 9045 3520 50  0000 R CNN
F 2 "Display_7Segment:KCSC02-105" H 9000 3450 50  0001 C CNN
F 3 "http://www.kingbright.com/attachments/file/psearch/000/00/00/KCSC02-105(Ver.9A).pdf" H 8500 4525 50  0001 L CNN
	1    9000 4050
	0    -1   1    0   
$EndComp
$Comp
L Display_Character:KCSC02-105 U?
U 1 1 5F8536F7
P 7450 2550
AR Path="/5F832F87/5F8536F7" Ref="U?"  Part="1" 
AR Path="/5F8536F7" Ref="U2"  Part="1" 
F 0 "U2" V 7496 2020 50  0000 R CNN
F 1 "KCSC02-105" V 7405 2020 50  0000 R CNN
F 2 "Display_7Segment:KCSC02-105" H 7450 1950 50  0001 C CNN
F 3 "http://www.kingbright.com/attachments/file/psearch/000/00/00/KCSC02-105(Ver.9A).pdf" H 6950 3025 50  0001 L CNN
	1    7450 2550
	0    -1   -1   0   
$EndComp
$Comp
L Display_Character:KCSC02-105 U?
U 1 1 5F8536FD
P 9000 2550
AR Path="/5F832F87/5F8536FD" Ref="U?"  Part="1" 
AR Path="/5F8536FD" Ref="U3"  Part="1" 
F 0 "U3" V 9046 2020 50  0000 R CNN
F 1 "KCSC02-105" V 8955 2020 50  0000 R CNN
F 2 "Display_7Segment:KCSC02-105" H 9000 1950 50  0001 C CNN
F 3 "http://www.kingbright.com/attachments/file/psearch/000/00/00/KCSC02-105(Ver.9A).pdf" H 8500 3025 50  0001 L CNN
	1    9000 2550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7250 3750 7250 3550
Wire Wire Line
	7350 2850 7350 3450
Wire Wire Line
	7450 2850 7450 3350
Wire Wire Line
	7550 3750 7550 3250
Wire Wire Line
	7650 2850 7650 3150
Wire Wire Line
	8700 2850 8700 3650
Wire Wire Line
	8800 3750 8800 3550
Wire Wire Line
	8900 2850 8900 3450
Wire Wire Line
	9000 2850 9000 3350
Wire Wire Line
	9100 3750 9100 3250
Wire Wire Line
	9200 2850 9200 3150
Wire Wire Line
	9300 2850 9300 3050
Wire Wire Line
	9400 3750 9400 2950
Wire Wire Line
	7750 2850 7750 3050
Wire Wire Line
	7850 3750 7850 2950
Wire Wire Line
	6300 3650 7150 3650
Connection ~ 7150 3650
Wire Wire Line
	7150 3650 7150 3750
Wire Wire Line
	7150 2850 7150 3650
Connection ~ 8700 3650
Wire Wire Line
	8700 3650 8700 3750
Connection ~ 8800 3550
Wire Wire Line
	8800 3550 8800 2850
Connection ~ 7250 3550
Wire Wire Line
	7250 3550 7250 2850
Wire Wire Line
	7250 3550 6300 3550
Wire Wire Line
	6300 3450 7350 3450
Connection ~ 7350 3450
Wire Wire Line
	7350 3450 7350 3750
Connection ~ 8900 3450
Wire Wire Line
	8900 3450 8900 3750
Wire Wire Line
	6300 3350 7450 3350
Connection ~ 7450 3350
Wire Wire Line
	7450 3350 7450 3750
Connection ~ 9000 3350
Wire Wire Line
	9000 3350 9000 3750
Connection ~ 9100 3250
Wire Wire Line
	9100 3250 9100 2850
Connection ~ 7550 3250
Wire Wire Line
	7550 3250 7550 2850
Wire Wire Line
	7550 3250 6300 3250
Wire Wire Line
	6300 3150 7650 3150
Connection ~ 7650 3150
Wire Wire Line
	7650 3150 7650 3750
Wire Wire Line
	7650 3150 9200 3150
Connection ~ 9200 3150
Wire Wire Line
	9200 3150 9200 3750
Wire Wire Line
	9300 3050 7750 3050
Connection ~ 9300 3050
Wire Wire Line
	9300 3050 9300 3750
Connection ~ 7750 3050
Wire Wire Line
	7750 3050 7750 3750
Wire Wire Line
	7750 3050 6300 3050
Wire Wire Line
	6300 2950 7850 2950
Connection ~ 7850 2950
Wire Wire Line
	7850 2950 7850 2850
Wire Wire Line
	7850 2950 9400 2950
Connection ~ 9400 2950
Wire Wire Line
	9400 2950 9400 2850
Wire Wire Line
	6400 2850 6300 2850
Wire Wire Line
	6400 2750 6300 2750
Wire Wire Line
	6400 2650 6300 2650
Wire Wire Line
	6400 2550 6300 2550
Text Label 6400 2850 0    50   ~ 0
DIG_0
Text Label 6400 2750 0    50   ~ 0
DIG_1
Text Label 6400 2650 0    50   ~ 0
DIG_2
Text Label 6400 2550 0    50   ~ 0
DIG_3
Wire Wire Line
	7750 2250 7750 2150
Wire Wire Line
	9300 2150 9300 2250
Text Label 7750 2150 0    50   ~ 0
DIG_0
Text Label 9300 2150 0    50   ~ 0
DIG_1
NoConn ~ 9400 2250
NoConn ~ 7850 2250
NoConn ~ 6300 2050
NoConn ~ 6300 2150
NoConn ~ 6300 2250
Wire Wire Line
	7150 3650 8700 3650
Wire Wire Line
	7450 3350 9000 3350
Wire Wire Line
	7550 3250 9100 3250
Wire Wire Line
	7250 3550 8800 3550
Wire Wire Line
	7350 3450 8900 3450
Wire Wire Line
	7750 4450 7750 4350
Wire Wire Line
	9300 4450 9300 4350
Text Label 7750 4450 0    50   ~ 0
DIG_2
Text Label 9300 4450 0    50   ~ 0
DIG_3
NoConn ~ 9400 4350
NoConn ~ 7850 4350
NoConn ~ 6300 2450
NoConn ~ 6300 2350
Wire Wire Line
	4400 3950 5050 3950
Wire Wire Line
	5050 3950 5050 4950
Wire Wire Line
	5050 4950 5300 4950
Wire Wire Line
	4400 4050 5000 4050
Wire Wire Line
	5000 4050 5000 5350
Wire Wire Line
	5000 5350 5300 5350
Wire Wire Line
	4400 4150 4950 4150
NoConn ~ 4400 4250
$Comp
L power:GND #PWR016
U 1 1 5F888A53
P 6000 5350
F 0 "#PWR016" H 6000 5100 50  0001 C CNN
F 1 "GND" H 6005 5177 50  0000 C CNN
F 2 "" H 6000 5350 50  0001 C CNN
F 3 "" H 6000 5350 50  0001 C CNN
	1    6000 5350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5700 4950 5850 4950
Wire Wire Line
	5850 5350 6000 5350
Wire Wire Line
	5700 5350 5850 5350
Connection ~ 5850 5350
Wire Wire Line
	5850 4950 5850 5350
Wire Wire Line
	5850 5750 5850 5350
Wire Wire Line
	5700 5750 5850 5750
Wire Wire Line
	4950 5750 5300 5750
Wire Wire Line
	4950 4150 4950 5750
$Comp
L Switch:SW_Push_Dual_x2 SW3
U 1 1 5F81D8BA
P 5500 5750
F 0 "SW3" H 5500 6035 50  0000 C CNN
F 1 "Submit" H 5500 5944 50  0000 C CNN
F 2 "" H 5500 5950 50  0001 C CNN
F 3 "~" H 5500 5950 50  0001 C CNN
	1    5500 5750
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D2
U 1 1 5F8A4D2E
P 6850 5400
F 0 "D2" H 6843 5145 50  0000 C CNN
F 1 "MHz_Backlight" H 6843 5236 50  0000 C CNN
F 2 "" H 6850 5400 50  0001 C CNN
F 3 "~" H 6850 5400 50  0001 C CNN
	1    6850 5400
	-1   0    0    1   
$EndComp
Wire Wire Line
	7000 5000 7200 5000
Wire Wire Line
	7200 5000 7200 5200
Wire Wire Line
	7000 5400 7200 5400
Wire Wire Line
	7200 5400 7200 5200
Connection ~ 7200 5200
Wire Wire Line
	7200 5200 7300 5200
$Comp
L power:GND #PWR015
U 1 1 5F8B0240
P 7300 5200
F 0 "#PWR015" H 7300 4950 50  0001 C CNN
F 1 "GND" H 7305 5027 50  0000 C CNN
F 2 "" H 7300 5200 50  0001 C CNN
F 3 "" H 7300 5200 50  0001 C CNN
	1    7300 5200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4400 2350 4950 2350
Wire Wire Line
	4950 2350 4950 3550
Wire Wire Line
	4950 3550 5300 3550
Wire Wire Line
	5300 3550 5300 4450
Wire Wire Line
	5300 4450 6550 4450
Wire Wire Line
	6550 4450 6550 5000
Wire Wire Line
	6550 5000 6700 5000
Wire Wire Line
	6700 5400 6450 5400
Wire Wire Line
	6450 5400 6450 4550
Wire Wire Line
	6450 4550 5200 4550
Wire Wire Line
	5200 4550 5200 3650
Wire Wire Line
	5200 3650 4850 3650
Wire Wire Line
	4850 3650 4850 2450
Wire Wire Line
	4850 2450 4400 2450
$Comp
L Device:Polyfuse F1
U 1 1 5F923195
P 2000 3050
F 0 "F1" V 1775 3050 50  0000 C CNN
F 1 "Polyfuse" V 1866 3050 50  0000 C CNN
F 2 "" H 2050 2850 50  0001 L CNN
F 3 "~" H 2000 3050 50  0001 C CNN
	1    2000 3050
	0    1    1    0   
$EndComp
Text Label 1400 3950 2    50   ~ 0
UVCC
Text Label 1400 3350 2    50   ~ 0
UVCC
Text Label 1750 3050 2    50   ~ 0
UVCC
Wire Wire Line
	1750 3050 1850 3050
Wire Wire Line
	2150 3050 2250 3050
$EndSCHEMATC
