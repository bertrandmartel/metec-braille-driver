EESchema Schematic File Version 4
LIBS:8pin-adapter-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "8 pin adapter"
Date "2018-07-31"
Rev "1.0"
Comp ""
Comment1 "1,27mm to 2,54mm adapter"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector:Conn_01x08_Female J2
U 1 1 5B60C776
P 6700 3800
F 0 "J2" H 6600 3300 50  0000 L CNN
F 1 "Conn_01x08_Female" H 6350 4250 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x08_P2.54mm_Vertical" H 6700 3800 50  0001 C CNN
F 3 "~" H 6700 3800 50  0001 C CNN
	1    6700 3800
	1    0    0    -1  
$EndComp
Text Label 6300 3500 0    50   ~ 0
PIN1
Text Label 6300 3600 0    50   ~ 0
PIN2
Text Label 6300 3700 0    50   ~ 0
PIN3
Text Label 6300 3800 0    50   ~ 0
PIN4
Text Label 6300 3900 0    50   ~ 0
PIN5
Text Label 6300 4000 0    50   ~ 0
PIN6
Text Label 6300 4100 0    50   ~ 0
PIN7
Text Label 6300 4200 0    50   ~ 0
PIN8
Wire Wire Line
	6500 3500 6300 3500
Wire Wire Line
	6500 3600 6300 3600
Wire Wire Line
	6500 3700 6300 3700
Wire Wire Line
	6500 3800 6300 3800
Wire Wire Line
	6500 3900 6300 3900
Wire Wire Line
	6500 4000 6300 4000
Wire Wire Line
	6500 4100 6300 4100
Wire Wire Line
	6500 4200 6300 4200
$Comp
L Connector:Conn_01x02_Male J3
U 1 1 5B60DE2D
P 7550 3600
F 0 "J3" H 7650 3700 50  0000 C CNN
F 1 "Conn_01x02_Male" H 7600 3350 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7550 3600 50  0001 C CNN
F 3 "~" H 7550 3600 50  0001 C CNN
	1    7550 3600
	-1   0    0    1   
$EndComp
Wire Wire Line
	7350 3500 7200 3500
Wire Wire Line
	7350 3600 7200 3600
Text Label 7200 3500 0    50   ~ 0
PIN6
Text Label 7200 3600 0    50   ~ 0
PIN1
Text Label 5900 4200 0    50   ~ 0
PIN1
Wire Wire Line
	5800 4200 5900 4200
Wire Wire Line
	5800 4100 5900 4100
Text Label 5900 4100 0    50   ~ 0
PIN2
Wire Wire Line
	5800 4000 5900 4000
Text Label 5900 4000 0    50   ~ 0
PIN3
Text Label 5900 3900 0    50   ~ 0
PIN4
Wire Wire Line
	5800 3900 5900 3900
Wire Wire Line
	5800 3800 5900 3800
Text Label 5900 3800 0    50   ~ 0
PIN5
Wire Wire Line
	5800 3700 5900 3700
Text Label 5900 3700 0    50   ~ 0
PIN6
Text Label 5900 3600 0    50   ~ 0
PIN7
Wire Wire Line
	5800 3600 5900 3600
Text Label 5900 3500 0    50   ~ 0
PIN8
Wire Wire Line
	5800 3500 5900 3500
$Comp
L Connector:Conn_01x08_Female J1
U 1 1 5B60C8C6
P 5600 3900
F 0 "J1" H 5500 4300 50  0000 C CNN
F 1 "Conn_01x08_Female" H 5494 3366 50  0000 C CNN
F 2 "Connector_PinSocket_2.00mm:PinSocket_1x08_P2.00mm_Vertical" H 5600 3900 50  0001 C CNN
F 3 "~" H 5600 3900 50  0001 C CNN
	1    5600 3900
	-1   0    0    1   
$EndComp
$EndSCHEMATC
