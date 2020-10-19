EESchema Schematic File Version 4
LIBS:EPaperDriver-cache
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
L Logic_LevelTranslator:TXS0108EPW U1
U 1 1 5F859D4F
P 5550 2000
F 0 "U1" H 5800 1350 50  0000 C CNN
F 1 "TXS0108EPW" H 5250 1350 50  0000 C CNN
F 2 "Package_SO:TSSOP-20_4.4x6.5mm_P0.65mm" H 5550 1250 50  0001 C CNN
F 3 "www.ti.com/lit/ds/symlink/txs0108e.pdf" H 5550 1900 50  0001 C CNN
	1    5550 2000
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:AP131-33 U2
U 1 1 5F85B684
P 2000 4400
F 0 "U2" H 2000 4742 50  0000 C CNN
F 1 "RT9193-33GB" H 2000 4651 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 2000 4725 50  0001 C CNN
F 3 "http://www.diodes.com/_files/datasheets/AP131.pdf" H 2000 4400 50  0001 C CNN
	1    2000 4400
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R1
U 1 1 5F85D0CA
P 9750 4450
F 0 "R1" H 9818 4496 50  0000 L CNN
F 1 "2R2" H 9818 4405 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 9790 4440 50  0001 C CNN
F 3 "~" H 9750 4450 50  0001 C CNN
	1    9750 4450
	1    0    0    -1  
$EndComp
$Comp
L Device:L L1
U 1 1 5F85DAFF
P 9500 5300
F 0 "L1" V 9600 5400 50  0000 C CNN
F 1 "47uH" V 9600 5250 50  0000 C CNN
F 2 "Inductor_SMD:L_12x12mm_H6mm" H 9500 5300 50  0001 C CNN
F 3 "~" H 9500 5300 50  0001 C CNN
	1    9500 5300
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C1
U 1 1 5F85E89D
P 9500 5500
F 0 "C1" V 9450 5400 50  0000 C CNN
F 1 "1uF" V 9450 5650 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 9538 5350 50  0001 C CNN
F 3 "~" H 9500 5500 50  0001 C CNN
	1    9500 5500
	0    1    1    0   
$EndComp
$Comp
L Diode:MBR0530 D3
U 1 1 5F85F53F
P 9750 5750
F 0 "D3" V 9796 5671 50  0000 R CNN
F 1 "MBR0530" V 9705 5671 50  0000 R CNN
F 2 "Diode_SMD:D_SOD-123" H 9750 5575 50  0001 C CNN
F 3 "http://www.mccsemi.com/up_pdf/MBR0520~MBR0580(SOD123).pdf" H 9750 5750 50  0001 C CNN
	1    9750 5750
	0    -1   -1   0   
$EndComp
$Comp
L Diode:MBR0530 D2
U 1 1 5F85FECF
P 9250 5750
F 0 "D2" V 9204 5829 50  0000 L CNN
F 1 "MBR0530" V 9295 5829 50  0000 L CNN
F 2 "Diode_SMD:D_SOD-123" H 9250 5575 50  0001 C CNN
F 3 "http://www.mccsemi.com/up_pdf/MBR0520~MBR0580(SOD123).pdf" H 9250 5750 50  0001 C CNN
	1    9250 5750
	0    1    1    0   
$EndComp
$Comp
L Diode:MBR0530 D1
U 1 1 5F86027F
P 9000 5750
F 0 "D1" V 9050 5900 50  0000 R CNN
F 1 "MBR0530" V 8900 6150 50  0000 R CNN
F 2 "Diode_SMD:D_SOD-123" H 9000 5575 50  0001 C CNN
F 3 "http://www.mccsemi.com/up_pdf/MBR0520~MBR0580(SOD123).pdf" H 9000 5750 50  0001 C CNN
	1    9000 5750
	0    -1   -1   0   
$EndComp
$Comp
L Jumper:Jumper_3_Bridged12 JP1
U 1 1 5F863B31
P 2450 2100
F 0 "JP1" H 2450 2304 50  0000 C CNN
F 1 "Jumper_3_Bridged12" H 2450 2213 50  0000 C CNN
F 2 "Jumper:SolderJumper-3_P1.3mm_Open_Pad1.0x1.5mm" H 2450 2100 50  0001 C CNN
F 3 "~" H 2450 2100 50  0001 C CNN
	1    2450 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 7100 2100 7100
Wire Wire Line
	2200 7000 2100 7000
Wire Wire Line
	2200 6900 2100 6900
Wire Wire Line
	2200 6800 2100 6800
Wire Wire Line
	2200 6700 2100 6700
Wire Wire Line
	2200 6600 2100 6600
Wire Wire Line
	2200 6300 2100 6300
Wire Wire Line
	2200 6200 2100 6200
Text Label 2200 6200 0    50   ~ 0
VCC
Text Label 2200 6300 0    50   ~ 0
GND
Text Label 2200 6600 0    50   ~ 0
MOSI
Text Label 2200 6700 0    50   ~ 0
SCLK
Text Label 2200 6800 0    50   ~ 0
E_CS
Text Label 2200 6900 0    50   ~ 0
DC
Text Label 2200 7000 0    50   ~ 0
RST
Text Label 2200 7100 0    50   ~ 0
BUSY
Wire Wire Line
	5550 2800 5550 2700
Text Label 5550 2800 0    50   ~ 0
GND
Text Label 5650 1200 0    50   ~ 0
VCC
Text Label 5450 1200 0    50   ~ 0
3V3
Wire Wire Line
	6050 2400 5950 2400
Wire Wire Line
	6050 2300 5950 2300
Wire Wire Line
	6050 2200 5950 2200
Wire Wire Line
	6050 2100 5950 2100
Wire Wire Line
	6050 2000 5950 2000
Wire Wire Line
	6050 1900 5950 1900
Text Label 6050 1900 0    50   ~ 0
MOSI
Text Label 6050 2000 0    50   ~ 0
SCLK
Text Label 6050 2100 0    50   ~ 0
E_CS
Text Label 6050 2200 0    50   ~ 0
DC
Text Label 6050 2300 0    50   ~ 0
RST
Text Label 6050 2400 0    50   ~ 0
BUSY
Wire Wire Line
	5650 1200 5650 1300
Wire Wire Line
	5450 1300 5450 1200
Wire Wire Line
	5050 2400 5150 2400
Wire Wire Line
	5050 2300 5150 2300
Wire Wire Line
	5050 2200 5150 2200
Wire Wire Line
	5050 2100 5150 2100
Wire Wire Line
	5050 2000 5150 2000
Wire Wire Line
	5050 1900 5150 1900
Text Label 5050 1900 2    50   ~ 0
MOSI'
Text Label 5050 2000 2    50   ~ 0
SCLK'
Text Label 5050 2100 2    50   ~ 0
E_CS'
Text Label 5050 2200 2    50   ~ 0
DC'
Text Label 5050 2300 2    50   ~ 0
RST'
Text Label 5050 2400 2    50   ~ 0
BUSY'
Text Label 5050 1600 2    50   ~ 0
3V3
Wire Wire Line
	5050 1600 5150 1600
Text Label 1400 4350 2    50   ~ 0
VCC
Text Label 2600 4300 0    50   ~ 0
3V3
$Comp
L Device:R_US R2
U 1 1 5F87732D
P 1500 4550
F 0 "R2" H 1568 4596 50  0000 L CNN
F 1 "100k" H 1568 4505 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 1540 4540 50  0001 C CNN
F 3 "~" H 1500 4550 50  0001 C CNN
	1    1500 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 4800 2000 4700
Wire Wire Line
	2300 4300 2450 4300
Wire Wire Line
	1700 4400 1500 4400
Wire Wire Line
	1500 4700 1500 4800
$Comp
L Device:C C3
U 1 1 5F87C53A
P 1500 4150
F 0 "C3" H 1615 4196 50  0000 L CNN
F 1 "1uF" H 1615 4105 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 1538 4000 50  0001 C CNN
F 3 "~" H 1500 4150 50  0001 C CNN
	1    1500 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 4300 1700 4300
$Comp
L Device:C C4
U 1 1 5F87D70B
P 2450 4150
F 0 "C4" H 2565 4196 50  0000 L CNN
F 1 "1uF" H 2565 4105 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 2488 4000 50  0001 C CNN
F 3 "~" H 2450 4150 50  0001 C CNN
	1    2450 4150
	1    0    0    -1  
$EndComp
Connection ~ 2450 4300
Wire Wire Line
	2450 4300 2600 4300
Text Label 2000 3800 0    50   ~ 0
GND
Wire Wire Line
	2450 3900 2450 4000
Wire Wire Line
	1500 3900 1500 4000
$Comp
L Device:C C8
U 1 1 5F87F39F
P 2450 4550
F 0 "C8" H 2565 4596 50  0000 L CNN
F 1 "22nF" H 2565 4505 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 2488 4400 50  0001 C CNN
F 3 "~" H 2450 4550 50  0001 C CNN
	1    2450 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 4400 2450 4400
Wire Wire Line
	2450 4800 2450 4700
Wire Wire Line
	1500 4800 2000 4800
Wire Wire Line
	2000 4800 2450 4800
Connection ~ 2000 4800
Wire Wire Line
	2000 4900 2000 4800
Text Label 2000 4900 0    50   ~ 0
GND
Wire Wire Line
	1500 3900 2000 3900
Wire Wire Line
	2000 3800 2000 3900
Connection ~ 2000 3900
Wire Wire Line
	2000 3900 2450 3900
Wire Wire Line
	1500 4300 1500 4350
Connection ~ 1500 4300
Connection ~ 1500 4400
Wire Wire Line
	1500 4350 1400 4350
Connection ~ 1500 4350
Wire Wire Line
	1500 4350 1500 4400
Text Label 2450 2350 0    50   ~ 0
BS
Wire Wire Line
	2450 2250 2450 2350
Text Label 2100 2100 2    50   ~ 0
GND
Text Label 2800 2100 0    50   ~ 0
3V3
Wire Wire Line
	2800 2100 2700 2100
Wire Wire Line
	2200 2100 2100 2100
Text Notes 2150 1650 0    50   ~ 0
GND = 4-line SPI
Text Notes 2150 1750 0    50   ~ 0
3V3 = 3-line SPI
$Comp
L Display_Graphic:MDE029A128296RBW FPC1
U 1 1 5F899E8A
P 10050 1050
F 0 "FPC1" H 10078 -254 50  0000 L CNN
F 1 "MDE029A128296RBW" H 10078 -345 50  0000 L CNN
F 2 "Connector_FFC-FPC:Hirose_FH12-24S-0.5SH_1x24-1MP_P0.50mm_Horizontal" H 10050 1050 50  0001 C CNN
F 3 "" H 10050 1050 50  0001 C CNN
	1    10050 1050
	1    0    0    -1  
$EndComp
NoConn ~ 9650 1850
NoConn ~ 9650 1750
Wire Wire Line
	9650 1950 9550 1950
Wire Wire Line
	9550 2050 9650 2050
Wire Wire Line
	9550 2150 9650 2150
Wire Wire Line
	9550 2250 9650 2250
Wire Wire Line
	9550 2350 9650 2350
Wire Wire Line
	9550 2450 9650 2450
Wire Wire Line
	9550 2550 9650 2550
Wire Wire Line
	9550 2650 9650 2650
Wire Wire Line
	9550 2750 9650 2750
Wire Wire Line
	9550 3250 9650 3250
Wire Wire Line
	9550 3450 9650 3450
Text Label 9550 1950 2    50   ~ 0
BS
Text Label 9550 2050 2    50   ~ 0
BUSY'
Text Label 9550 2150 2    50   ~ 0
RST'
Text Label 9550 2250 2    50   ~ 0
DC'
Text Label 9550 2350 2    50   ~ 0
E_CS'
Text Label 9550 2450 2    50   ~ 0
SCLK'
Text Label 9550 2550 2    50   ~ 0
MOSI'
Text Label 9550 2650 2    50   ~ 0
3V3
Text Label 9550 2750 2    50   ~ 0
3V3
Text Label 9550 3250 2    50   ~ 0
VGH
Text Label 9550 3450 2    50   ~ 0
VGL
Wire Wire Line
	9550 1350 9650 1350
Wire Wire Line
	9550 1450 9650 1450
Text Label 9550 1350 2    50   ~ 0
GDR
Text Label 9550 1450 2    50   ~ 0
RESE
$Comp
L Device:C C2
U 1 1 5F8ADE19
P 9150 1650
F 0 "C2" V 9200 1750 50  0000 C CNN
F 1 "1uF" V 9200 1500 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 9188 1500 50  0001 C CNN
F 3 "~" H 9150 1650 50  0001 C CNN
	1    9150 1650
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C5
U 1 1 5F8AF38B
P 9150 2950
F 0 "C5" V 9200 3050 50  0000 C CNN
F 1 "1uF" V 9200 2800 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 9188 2800 50  0001 C CNN
F 3 "~" H 9150 2950 50  0001 C CNN
	1    9150 2950
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C6
U 1 1 5F8B0874
P 9150 3050
F 0 "C6" V 9200 3150 50  0000 C CNN
F 1 "1uF" V 9200 2900 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 9188 2900 50  0001 C CNN
F 3 "~" H 9150 3050 50  0001 C CNN
	1    9150 3050
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C7
U 1 1 5F8B0A7C
P 9150 3150
F 0 "C7" V 9200 3250 50  0000 C CNN
F 1 "1uF" V 9200 3000 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 9188 3000 50  0001 C CNN
F 3 "~" H 9150 3150 50  0001 C CNN
	1    9150 3150
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C9
U 1 1 5F8B0EDA
P 9150 3350
F 0 "C9" V 9200 3450 50  0000 C CNN
F 1 "1uF" V 9200 3200 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 9188 3200 50  0001 C CNN
F 3 "~" H 9150 3350 50  0001 C CNN
	1    9150 3350
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C10
U 1 1 5F8B12FD
P 9150 3550
F 0 "C10" V 9200 3650 50  0000 C CNN
F 1 "0.47uF" V 9200 3350 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 9188 3400 50  0001 C CNN
F 3 "~" H 9150 3550 50  0001 C CNN
	1    9150 3550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9550 2850 9650 2850
Text Label 9550 2850 2    50   ~ 0
GND
Wire Wire Line
	9650 2950 9300 2950
Wire Wire Line
	9300 3050 9650 3050
Wire Wire Line
	9650 3150 9300 3150
Wire Wire Line
	9300 3350 9650 3350
Wire Wire Line
	9650 3550 9300 3550
Wire Wire Line
	9000 3550 8900 3550
Wire Wire Line
	8900 3550 8900 3350
Wire Wire Line
	8900 2950 9000 2950
Wire Wire Line
	9000 3050 8900 3050
Connection ~ 8900 3050
Wire Wire Line
	8900 3050 8900 2950
Wire Wire Line
	8900 3150 9000 3150
Connection ~ 8900 3150
Wire Wire Line
	8900 3150 8900 3050
Wire Wire Line
	9000 3350 8900 3350
Connection ~ 8900 3350
Wire Wire Line
	8900 3350 8900 3250
Text Label 8800 3250 2    50   ~ 0
GND
Wire Wire Line
	8800 3250 8900 3250
Connection ~ 8900 3250
Wire Wire Line
	8900 3250 8900 3150
Wire Wire Line
	9300 1650 9650 1650
Wire Wire Line
	9000 1650 8900 1650
Text Label 8900 1650 2    50   ~ 0
GND
Text Label 9750 4200 0    50   ~ 0
GND
Text Label 9850 4700 0    50   ~ 0
RESE
$Comp
L Transistor_FET:BSS214NW Q1
U 1 1 5F8CB2BF
P 9650 5000
F 0 "Q1" H 9856 4954 50  0000 L CNN
F 1 "Si1304BDL" H 9856 5045 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-323_SC-70" H 9850 4925 50  0001 L CIN
F 3 "https://www.infineon.com/dgdl/Infineon-BSS214NW-DS-v02_02-en.pdf?fileId=db3a30431b3e89eb011b695aebc01bde" H 9650 5000 50  0001 L CNN
	1    9650 5000
	1    0    0    1   
$EndComp
Text Label 9350 5000 2    50   ~ 0
GDR
Wire Wire Line
	9350 5000 9450 5000
Wire Wire Line
	9750 4600 9750 4700
Wire Wire Line
	9850 4700 9750 4700
Connection ~ 9750 4700
Wire Wire Line
	9750 4700 9750 4800
Wire Wire Line
	9750 4300 9750 4200
Text Label 9250 5300 2    50   ~ 0
3V3
Text Label 9000 6000 0    50   ~ 0
GND
Text Label 9750 6000 0    50   ~ 0
VGH
Text Label 9250 6000 0    50   ~ 0
VGL
Wire Wire Line
	9000 6000 9000 5900
Wire Wire Line
	9250 5900 9250 6000
Wire Wire Line
	9750 6000 9750 5900
Wire Wire Line
	9000 5600 9000 5500
Wire Wire Line
	9000 5500 9250 5500
Wire Wire Line
	9250 5500 9250 5600
Wire Wire Line
	9750 5600 9750 5500
Wire Wire Line
	9650 5300 9750 5300
Connection ~ 9750 5300
Wire Wire Line
	9750 5300 9750 5200
Wire Wire Line
	9350 5300 9250 5300
Wire Wire Line
	9350 5500 9250 5500
Connection ~ 9250 5500
Wire Wire Line
	9650 5500 9750 5500
Connection ~ 9750 5500
Wire Wire Line
	9750 5500 9750 5300
$Comp
L Memory_RAM:23LC1024 U3
U 1 1 5F90D53F
P 5300 6200
F 0 "U3" H 5250 6215 50  0000 C CNN
F 1 "23LC1024" H 5250 6124 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 5300 6200 50  0001 C CNN
F 3 "" H 5300 6200 50  0001 C CNN
	1    5300 6200
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x11 J2
U 1 1 5F91087B
P 1900 6700
F 0 "J2" H 1818 5975 50  0000 C CNN
F 1 "Conn_01x11" H 1818 6066 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x11_P2.54mm_Vertical" H 1900 6700 50  0001 C CNN
F 3 "~" H 1900 6700 50  0001 C CNN
	1    1900 6700
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2200 6400 2100 6400
Text Label 2200 6400 0    50   ~ 0
MISO
Wire Wire Line
	6050 1700 5950 1700
Text Label 6050 1700 0    50   ~ 0
MISO
Wire Wire Line
	5050 1700 5150 1700
Text Label 5050 1700 2    50   ~ 0
MISO'
Wire Wire Line
	2200 6500 2100 6500
Text Label 2200 6500 0    50   ~ 0
SD_CS
Wire Wire Line
	2200 7200 2100 7200
Text Label 2200 7200 0    50   ~ 0
RAM_CS
Wire Wire Line
	4750 6450 4850 6450
Text Label 4750 6450 2    50   ~ 0
RAM_CS
Wire Wire Line
	4750 6550 4850 6550
Text Label 4750 6550 2    50   ~ 0
MISO
Wire Wire Line
	5750 6650 5650 6650
Text Label 5750 6650 0    50   ~ 0
SCLK
Wire Wire Line
	5750 6750 5650 6750
Text Label 5750 6750 0    50   ~ 0
MOSI
$Comp
L Device:R_US R4
U 1 1 5F9797C4
P 4400 6500
F 0 "R4" H 4468 6546 50  0000 L CNN
F 1 "10k" H 4468 6455 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 4440 6490 50  0001 C CNN
F 3 "~" H 4400 6500 50  0001 C CNN
	1    4400 6500
	-1   0    0    1   
$EndComp
Wire Wire Line
	5700 5450 5700 5650
Text Label 5700 5450 1    50   ~ 0
VCC
Wire Wire Line
	4750 6750 4850 6750
Text Label 4750 6750 2    50   ~ 0
GND
Wire Wire Line
	5650 6450 5700 6450
Wire Wire Line
	5700 6450 5700 6250
Wire Wire Line
	5650 6550 5800 6550
Wire Wire Line
	5800 6250 5700 6250
Connection ~ 5700 6250
$Comp
L Device:R_US R3
U 1 1 5F97AD15
P 5800 6400
F 0 "R3" H 5868 6446 50  0000 L CNN
F 1 "10k" H 5868 6355 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 5840 6390 50  0001 C CNN
F 3 "~" H 5800 6400 50  0001 C CNN
	1    5800 6400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 6650 4850 6650
Wire Wire Line
	4400 6250 4400 6350
Text Label 4400 6250 1    50   ~ 0
VCC
$Comp
L Device:C C12
U 1 1 5F9AF6C1
P 5550 6050
F 0 "C12" V 5600 6150 50  0000 L CNN
F 1 "22nF" V 5500 6100 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 5588 5900 50  0001 C CNN
F 3 "~" H 5550 6050 50  0001 C CNN
	1    5550 6050
	0    -1   -1   0   
$EndComp
Connection ~ 5700 6050
Wire Wire Line
	5700 6050 5700 6250
$Comp
L Device:C C11
U 1 1 5F9B0B77
P 5550 5650
F 0 "C11" V 5600 5750 50  0000 L CNN
F 1 "22nF" V 5500 5700 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 5588 5500 50  0001 C CNN
F 3 "~" H 5550 5650 50  0001 C CNN
	1    5550 5650
	0    -1   -1   0   
$EndComp
Connection ~ 5700 5650
Wire Wire Line
	5700 5650 5700 6050
Wire Wire Line
	5150 5850 5250 5850
Text Label 5150 5850 2    50   ~ 0
GND
Wire Wire Line
	5400 5650 5250 5650
Wire Wire Line
	5250 5650 5250 5850
Wire Wire Line
	5400 6050 5250 6050
Wire Wire Line
	5250 6050 5250 5850
Connection ~ 5250 5850
$Comp
L Connector:Micro_SD_Card_Det J1
U 1 1 5F9D36E5
P 5850 4100
F 0 "J1" H 5800 4917 50  0000 C CNN
F 1 "Micro_SD_Card_Det" H 5800 4826 50  0000 C CNN
F 2 "Connector_Card:microSD_HC_Hirose_DM3D-SF" H 7900 4800 50  0001 C CNN
F 3 "https://www.hirose.com/product/en/download_file/key_name/DM3/category/Catalog/doc_file_id/49662/?file_category_id=4&item_id=195&is_series=1" H 5850 4200 50  0001 C CNN
	1    5850 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 1800 5950 1800
Text Label 6050 1800 0    50   ~ 0
SD_CS
Wire Wire Line
	5050 1800 5150 1800
Text Label 5050 1800 2    50   ~ 0
SD_CS'
Wire Wire Line
	4850 3800 4950 3800
Text Label 4850 3800 2    50   ~ 0
SD_CS'
NoConn ~ 4950 3700
Wire Wire Line
	4850 4200 4950 4200
Text Label 4850 4200 2    50   ~ 0
GND
Text Label 4850 4000 2    50   ~ 0
3V3
Wire Wire Line
	4950 4000 4850 4000
Wire Wire Line
	4850 3900 4950 3900
Text Label 4850 3900 2    50   ~ 0
MOSI'
Wire Wire Line
	4850 4100 4950 4100
Text Label 4850 4100 2    50   ~ 0
SCLK'
NoConn ~ 4950 4600
NoConn ~ 4950 4500
NoConn ~ 4950 4400
Wire Wire Line
	4850 4300 4950 4300
Text Label 4850 4300 2    50   ~ 0
MISO'
Wire Wire Line
	6750 4600 6650 4600
Text Label 6750 4600 0    50   ~ 0
GND
$EndSCHEMATC
