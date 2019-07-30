/**
    General Purpose I/O controller header file

    General Purpose I/O controller header file

    @file       gpio.h
    @ingroup    mIDrvIO_GPIO
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/

#ifndef _GPIO_H
#define _GPIO_H

#include "Type.h"
#include "Driver.h"

/**
    @addtogroup mIDrvIO_GPIO
*/
//@{

/**
    GPIO direction

    GPIO direction definition for gpio_setDir() and gpio_getDir()
*/
typedef enum {
	GPIO_DIR_INPUT =            0,      ///< GPIO is input direction
	GPIO_DIR_OUTPUT =           1,      ///< GPIO is output direction

	ENUM_DUMMY4WORD(GPIO_DIR)
} GPIO_DIR;

/**
    GPIO interrupt type

    GPIO interrupt type definition for type argument of gpio_setIntTypePol()
*/
typedef enum {
	GPIO_INTTYPE_EDGE =         0,      ///< GPIO interrupt is edge trigger
	GPIO_INTTYPE_LEVEL =        1,      ///< GPIO interrupt is level trigger

	ENUM_DUMMY4WORD(GPIO_INTTYPE)
} GPIO_INTTYPE;

/**
    GPIO interrupt polarity

    GPIO interrupt polarity definition for pol argument of gpio_setIntTypePol()
*/
typedef enum {
	GPIO_INTPOL_POSHIGH =       0,      ///< GPIO interrupt polarity is \n
	///< - @b positvie edge for edge trigger
	///< - @b high level for level trigger
	GPIO_INTPOL_NEGLOW =        1,      ///< GPIO interrupt polarity is \n
	///< - @b negative edge for edge trigger
	///< - @b low level for level trigger
	GPIO_INTPOL_BOTHEDGE =      2,      ///< GPIO interrupt polarity is \n
	///< - @b both edge for edge trigger
	ENUM_DUMMY4WORD(GPIO_INTPOL)
} GPIO_INTPOL;



/**
    @name   GPIO pins ID

    GPIO pins ID definition

    For detail GPIO pin out, please refer to NT96650 data sheet.
*/
//@{
/*Storage GPIO - CGPIO*/
#define C_GPIO_0    0                       ///< C_GPIO[0]
#define C_GPIO_1    1                       ///< C_GPIO[1]
#define C_GPIO_2    2                       ///< C_GPIO[2]
#define C_GPIO_3    3                       ///< C_GPIO[3]
#define C_GPIO_4    4                       ///< C_GPIO[4]
#define C_GPIO_5    5                       ///< C_GPIO[5]
#define C_GPIO_6    6                       ///< C_GPIO[6]
#define C_GPIO_7    7                       ///< C_GPIO[7]
#define C_GPIO_8    8                       ///< C_GPIO[8]
#define C_GPIO_9    9                       ///< C_GPIO[9]
#define C_GPIO_10   10                      ///< C_GPIO[10]
#define C_GPIO_11   11                      ///< C_GPIO[11]
#define C_GPIO_12   12                      ///< C_GPIO[12]
#define C_GPIO_13   13                      ///< C_GPIO[13]
#define C_GPIO_14   14                      ///< C_GPIO[14]
#define C_GPIO_15   15                      ///< C_GPIO[15]
#define C_GPIO_16   16                      ///< C_GPIO[16]
#define C_GPIO_17   17                      ///< C_GPIO[17]
#define C_GPIO_18   18                      ///< C_GPIO[18]
#define C_GPIO_19   19                      ///< C_GPIO[19]
#define C_GPIO_20   20                      ///< C_GPIO[20]
#define C_GPIO_21   21                      ///< C_GPIO[21]
#define C_GPIO_22   22                      ///< C_GPIO[22]
#define C_GPIO_23   23                      ///< C_GPIO[23]
#define C_GPIO_24   24                      ///< C_GPIO[24]
#define C_GPIO_25   25                      ///< C_GPIO[25]
#define C_GPIO_26   26                      ///< C_GPIO[26]
#define C_GPIO_27   27                      ///< C_GPIO[27]
#define C_GPIO_28   28                      ///< C_GPIO[28]
#define C_GPIO_29   29                      ///< C_GPIO[29]
#define C_GPIO_30   30                      ///< C_GPIO[30]
#define C_GPIO_31   31                      ///< C_GPIO[31]
#define C_GPIO_32   32                      ///< C_GPIO[32]
#define C_GPIO_33   33                      ///< C_GPIO[33]


/*Peripheral GPIO - PGPIO*/
#define P_GPIO_SHIFT_BASE 64
#define P_GPIO_0    (0 +P_GPIO_SHIFT_BASE)  ///< P_GPIO[0]
#define P_GPIO_1    (1 +P_GPIO_SHIFT_BASE)  ///< P_GPIO[1]
#define P_GPIO_2    (2 +P_GPIO_SHIFT_BASE)  ///< P_GPIO[2]
#define P_GPIO_3    (3 +P_GPIO_SHIFT_BASE)  ///< P_GPIO[3]
#define P_GPIO_4    (4 +P_GPIO_SHIFT_BASE)  ///< P_GPIO[4]
#define P_GPIO_5    (5 +P_GPIO_SHIFT_BASE)  ///< P_GPIO[5]
#define P_GPIO_6    (6 +P_GPIO_SHIFT_BASE)  ///< P_GPIO[6]
#define P_GPIO_7    (7 +P_GPIO_SHIFT_BASE)  ///< P_GPIO[7]
#define P_GPIO_8    (8 +P_GPIO_SHIFT_BASE)  ///< P_GPIO[8]
#define P_GPIO_9    (9 +P_GPIO_SHIFT_BASE)  ///< P_GPIO[9]
#define P_GPIO_10   (10+P_GPIO_SHIFT_BASE)  ///< P_GPIO[10]
#define P_GPIO_11   (11+P_GPIO_SHIFT_BASE)  ///< P_GPIO[11]
#define P_GPIO_12   (12+P_GPIO_SHIFT_BASE)  ///< P_GPIO[12]
#define P_GPIO_13   (13+P_GPIO_SHIFT_BASE)  ///< P_GPIO[13]
#define P_GPIO_14   (14+P_GPIO_SHIFT_BASE)  ///< P_GPIO[14]
#define P_GPIO_15   (15+P_GPIO_SHIFT_BASE)  ///< P_GPIO[15]
#define P_GPIO_16   (16+P_GPIO_SHIFT_BASE)  ///< P_GPIO[16]
#define P_GPIO_17   (17+P_GPIO_SHIFT_BASE)  ///< P_GPIO[17]
#define P_GPIO_18   (18+P_GPIO_SHIFT_BASE)  ///< P_GPIO[18]
#define P_GPIO_19   (19+P_GPIO_SHIFT_BASE)  ///< P_GPIO[19]
#define P_GPIO_20   (20+P_GPIO_SHIFT_BASE)  ///< P_GPIO[20]
#define P_GPIO_21   (21+P_GPIO_SHIFT_BASE)  ///< P_GPIO[21]
#define P_GPIO_22   (22+P_GPIO_SHIFT_BASE)  ///< P_GPIO[22]
#define P_GPIO_23   (23+P_GPIO_SHIFT_BASE)  ///< P_GPIO[23]
#define P_GPIO_24   (24+P_GPIO_SHIFT_BASE)  ///< P_GPIO[24]
#define P_GPIO_25   (25+P_GPIO_SHIFT_BASE)  ///< P_GPIO[25]
#define P_GPIO_26   (26+P_GPIO_SHIFT_BASE)  ///< P_GPIO[26]
#define P_GPIO_27   (27+P_GPIO_SHIFT_BASE)  ///< P_GPIO[27]
#define P_GPIO_28   (28+P_GPIO_SHIFT_BASE)  ///< P_GPIO[28]
#define P_GPIO_29   (29+P_GPIO_SHIFT_BASE)  ///< P_GPIO[29]
#define P_GPIO_30   (30+P_GPIO_SHIFT_BASE)  ///< P_GPIO[30]
#define P_GPIO_31   (31+P_GPIO_SHIFT_BASE)  ///< P_GPIO[31]

#define P_GPIO_32   (32+P_GPIO_SHIFT_BASE)  ///< P_GPIO[32]
#define P_GPIO_33   (33+P_GPIO_SHIFT_BASE)  ///< P_GPIO[33]
#define P_GPIO_34   (34+P_GPIO_SHIFT_BASE)  ///< P_GPIO[34]
#define P_GPIO_35   (35+P_GPIO_SHIFT_BASE)  ///< P_GPIO[35]
#define P_GPIO_36   (36+P_GPIO_SHIFT_BASE)  ///< P_GPIO[36]
#define P_GPIO_37   (37+P_GPIO_SHIFT_BASE)  ///< P_GPIO[37]
#define P_GPIO_38   (38+P_GPIO_SHIFT_BASE)  ///< P_GPIO[38]
#define P_GPIO_39   (39+P_GPIO_SHIFT_BASE)  ///< P_GPIO[39]
#define P_GPIO_40   (40+P_GPIO_SHIFT_BASE)  ///< P_GPIO[40]
#define P_GPIO_41   (41+P_GPIO_SHIFT_BASE)  ///< P_GPIO[41]
#define P_GPIO_42   (42+P_GPIO_SHIFT_BASE)  ///< P_GPIO[42]
#define P_GPIO_43   (43+P_GPIO_SHIFT_BASE)  ///< P_GPIO[43]
#define P_GPIO_44   (44+P_GPIO_SHIFT_BASE)  ///< P_GPIO[44]
#define P_GPIO_45   (45+P_GPIO_SHIFT_BASE)  ///< P_GPIO[45]
#define P_GPIO_46   (46+P_GPIO_SHIFT_BASE)  ///< P_GPIO[46]
#define P_GPIO_47   (47+P_GPIO_SHIFT_BASE)  ///< P_GPIO[47]

#if 0
#define P_GPIO_48   (48+P_GPIO_SHIFT_BASE)  ///< P_GPIO[48]
#define P_GPIO_49   (49+P_GPIO_SHIFT_BASE)  ///< P_GPIO[49]
#define P_GPIO_50   (50+P_GPIO_SHIFT_BASE)  ///< P_GPIO[50]
#define P_GPIO_51   (51+P_GPIO_SHIFT_BASE)  ///< P_GPIO[51]
#define P_GPIO_52   (52+P_GPIO_SHIFT_BASE)  ///< P_GPIO[52]
#define P_GPIO_53   (53+P_GPIO_SHIFT_BASE)  ///< P_GPIO[53]
#define P_GPIO_54   (54+P_GPIO_SHIFT_BASE)  ///< P_GPIO[54]
#define P_GPIO_55   (55+P_GPIO_SHIFT_BASE)  ///< P_GPIO[55]
#define P_GPIO_56   (56+P_GPIO_SHIFT_BASE)  ///< P_GPIO[56]
#define P_GPIO_57   (57+P_GPIO_SHIFT_BASE)  ///< P_GPIO[57]
#define P_GPIO_58   (58+P_GPIO_SHIFT_BASE)  ///< P_GPIO[58]
#define P_GPIO_59   (59+P_GPIO_SHIFT_BASE)  ///< P_GPIO[59]
#define P_GPIO_60   (60+P_GPIO_SHIFT_BASE)  ///< P_GPIO[60]
#define P_GPIO_61   (61+P_GPIO_SHIFT_BASE)  ///< P_GPIO[61]
#endif

/*Sensor GPIO - SGPIO*/
#define S_GPIO_SHIFT_BASE 128
#define S_GPIO_0    (0 +S_GPIO_SHIFT_BASE)  ///< S_GPI[0]
#define S_GPIO_1    (1 +S_GPIO_SHIFT_BASE)  ///< S_GPI[1]
#define S_GPIO_2    (2 +S_GPIO_SHIFT_BASE)  ///< S_GPI[2]
#define S_GPIO_3    (3 +S_GPIO_SHIFT_BASE)  ///< S_GPI[3]
#define S_GPIO_4    (4 +S_GPIO_SHIFT_BASE)  ///< S_GPI[4]
#define S_GPIO_5    (5 +S_GPIO_SHIFT_BASE)  ///< S_GPI[5]
#define S_GPIO_6    (6 +S_GPIO_SHIFT_BASE)  ///< S_GPI[6]
#define S_GPIO_7    (7 +S_GPIO_SHIFT_BASE)  ///< S_GPI[7]
#define S_GPIO_8    (8 +S_GPIO_SHIFT_BASE)  ///< S_GPI[8]
#define S_GPIO_9    (9 +S_GPIO_SHIFT_BASE)  ///< S_GPI[9]
#define S_GPIO_10   (10+S_GPIO_SHIFT_BASE)  ///< S_GPI[10]
#define S_GPIO_11   (11+S_GPIO_SHIFT_BASE)  ///< S_GPI[11]


/*LCD GPIO - LGPIO*/
#define L_GPIO_SHIFT_BASE 160
#define L_GPIO_0    (0 +L_GPIO_SHIFT_BASE)  ///< L_GPIO[0]
#define L_GPIO_1    (1 +L_GPIO_SHIFT_BASE)  ///< L_GPIO[1]
#define L_GPIO_2    (2 +L_GPIO_SHIFT_BASE)  ///< L_GPIO[2]
#define L_GPIO_3    (3 +L_GPIO_SHIFT_BASE)  ///< L_GPIO[3]
#define L_GPIO_4    (4 +L_GPIO_SHIFT_BASE)  ///< L_GPIO[4]
#define L_GPIO_5    (5 +L_GPIO_SHIFT_BASE)  ///< L_GPIO[5]
#define L_GPIO_6    (6 +L_GPIO_SHIFT_BASE)  ///< L_GPIO[6]
#define L_GPIO_7    (7 +L_GPIO_SHIFT_BASE)  ///< L_GPIO[7]
#define L_GPIO_8    (8 +L_GPIO_SHIFT_BASE)  ///< L_GPIO[8]
#define L_GPIO_9    (9 +L_GPIO_SHIFT_BASE)  ///< L_GPIO[9]
#define L_GPIO_10   (10+L_GPIO_SHIFT_BASE)  ///< L_GPIO[10]
#define L_GPIO_11   (11+L_GPIO_SHIFT_BASE)  ///< L_GPIO[11]
#define L_GPIO_12   (12+L_GPIO_SHIFT_BASE)  ///< L_GPIO[12]
#define L_GPIO_13   (13+L_GPIO_SHIFT_BASE)  ///< L_GPIO[13]
#define L_GPIO_14   (14+L_GPIO_SHIFT_BASE)  ///< L_GPIO[14]
#define L_GPIO_15   (15+L_GPIO_SHIFT_BASE)  ///< L_GPIO[15]
#define L_GPIO_16   (16+L_GPIO_SHIFT_BASE)  ///< L_GPIO[16]
#define L_GPIO_17   (17+L_GPIO_SHIFT_BASE)  ///< L_GPIO[17]
#define L_GPIO_18   (18+L_GPIO_SHIFT_BASE)  ///< L_GPIO[18]
#define L_GPIO_19   (19+L_GPIO_SHIFT_BASE)  ///< L_GPIO[19]
#define L_GPIO_20   (20+L_GPIO_SHIFT_BASE)  ///< L_GPIO[20]
#define L_GPIO_21   (21+L_GPIO_SHIFT_BASE)  ///< L_GPIO[21]
#define L_GPIO_22   (22+L_GPIO_SHIFT_BASE)  ///< L_GPIO[22]
#define L_GPIO_23   (23+L_GPIO_SHIFT_BASE)  ///< L_GPIO[23]
#define L_GPIO_24   (24+L_GPIO_SHIFT_BASE)  ///< L_GPIO[24]
#define L_GPIO_25   (25+L_GPIO_SHIFT_BASE)  ///< L_GPIO[25]
#define L_GPIO_26   (26+L_GPIO_SHIFT_BASE)  ///< L_GPIO[26]
#define L_GPIO_27   (27+L_GPIO_SHIFT_BASE)  ///< L_GPIO[27]
#define L_GPIO_28   (28+L_GPIO_SHIFT_BASE)  ///< L_GPIO[28]
#define L_GPIO_29   (29+L_GPIO_SHIFT_BASE)  ///< L_GPIO[29]
#define L_GPIO_30   (30+L_GPIO_SHIFT_BASE)  ///< L_GPIO[30]
#define L_GPIO_31   (31+L_GPIO_SHIFT_BASE)  ///< L_GPIO[31]
#define L_GPIO_32   (32+L_GPIO_SHIFT_BASE)  ///< L_GPIO[32]

/*Dedicated GPIO - DGPIO*/
// In order to backward comaptible, DGPIO is used as " GPIO_IS_DGPIO | D_GPIO_* "
#define D_GPIO_SHIFT_BASE 224
#define D_GPIO_0    (0 +D_GPIO_SHIFT_BASE)  ///< DGPIO[0]
#define D_GPIO_1    (1 +D_GPIO_SHIFT_BASE)  ///< DGPIO[1]
#define D_GPIO_2    (2 +D_GPIO_SHIFT_BASE)  ///< DGPIO[2]
#define D_GPIO_3    (3 +D_GPIO_SHIFT_BASE)  ///< DGPIO[3]
#define D_GPIO_4    (4 +D_GPIO_SHIFT_BASE)  ///< DGPIO[4]
#define D_GPIO_5    (5 +D_GPIO_SHIFT_BASE)  ///< DGPIO[5]
#define D_GPIO_6    (6 +D_GPIO_SHIFT_BASE)  ///< DGPIO[6]
#define D_GPIO_7    (7 +D_GPIO_SHIFT_BASE)  ///< DGPIO[7]
#define D_GPIO_8    (8 +D_GPIO_SHIFT_BASE)  ///< DGPIO[8]
#define D_GPIO_9    (9 +D_GPIO_SHIFT_BASE)  ///< DGPIO[9]
#define D_GPIO_10   (10+D_GPIO_SHIFT_BASE)  ///< DGPIO[10]
#define D_GPIO_11   (11+D_GPIO_SHIFT_BASE)  ///< DGPIO[11]
#define D_GPIO_12   (12+D_GPIO_SHIFT_BASE)  ///< DGPIO[12]

#define D_GPIO_16   (16 +D_GPIO_SHIFT_BASE) ///< D_GPIO[16] (USB Wakeup) (No pad instance)
#define D_GPIO_17   (17 +D_GPIO_SHIFT_BASE) ///< D_GPIO[16] (USB Port power 1) (No pad instance)
#define D_GPIO_18   (18 +D_GPIO_SHIFT_BASE) ///< D_GPIO[18] (USB Port power 2) (No pad instance)
#define D_GPIO_19   (19 +D_GPIO_SHIFT_BASE) ///< D_GPIO[19] (Ethernet MII) (No pad instance)
#define D_GPIO_20   (20 +D_GPIO_SHIFT_BASE) ///< D_GPIO[19] (USB PHY) (No pad instance)

/*GPI HSI Data register(High speed interface)*/
#define HSI_GPI_SHIFT_BASE 256
#define HSI_GPIO_0    (0 +HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[0]
#define HSI_GPIO_1    (1 +HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[1]
#define HSI_GPIO_2    (2 +HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[2]
#define HSI_GPIO_3    (3 +HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[3]
#define HSI_GPIO_4    (4 +HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[4]
#define HSI_GPIO_5    (5 +HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[5]
#define HSI_GPIO_6    (6 +HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[6]
#define HSI_GPIO_7    (7 +HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[7]
#define HSI_GPIO_8    (8 +HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[8]
#define HSI_GPIO_9    (9 +HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[9]
#define HSI_GPIO_10   (10+HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[10]
#define HSI_GPIO_11   (11+HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[11]
#define HSI_GPIO_12   (12+HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[12]
#define HSI_GPIO_13   (13+HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[13]
#define HSI_GPIO_14   (14+HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[14]
#define HSI_GPIO_15   (15+HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[15]
#define HSI_GPIO_16   (16+HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[16]
#define HSI_GPIO_17   (17+HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[17]
#define HSI_GPIO_18   (18+HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[18]
#define HSI_GPIO_19   (19+HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[19]
#define HSI_GPIO_20   (20+HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[20]
#define HSI_GPIO_21   (21+HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[21]
#define HSI_GPIO_22   (22+HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[22]
#define HSI_GPIO_23   (23+HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[23]
#define HSI_GPIO_24   (24+HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[24]
#define HSI_GPIO_25   (25+HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[25]
#define HSI_GPIO_26   (26+HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[26]
#define HSI_GPIO_27   (27+HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[27]
#define HSI_GPIO_28   (28+HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[28]
#define HSI_GPIO_29   (29+HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[29]
#define HSI_GPIO_30   (30+HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[30]
#define HSI_GPIO_31   (31+HSI_GPI_SHIFT_BASE)  ///< HSI_GPI[31]

//@}






/**
    @name   GPIO Interrupt ID

    GPIO interrupt ID definition

    GPIO interrupt ID for interrupt related APIs.
*/
//@{
#define GPIO_INT_00                 0       ///< GPIO INT[0]:   C_GPIO[3]
#define GPIO_INT_01                 1       ///< GPIO INT[1]:   C_GPIO[9]
#define GPIO_INT_02                 2       ///< GPIO INT[2]:   C_GPIO[15]
#define GPIO_INT_03                 3       ///< GPIO INT[3]:   C_GPIO[21]
#define GPIO_INT_04                 4       ///< GPIO INT[4]:   C_GPIO[26]
#define GPIO_INT_05                 5       ///< GPIO INT[5]:   C_GPIO[27]
#define GPIO_INT_06                 6       ///< GPIO INT[6]:   C_GPIO[28]
#define GPIO_INT_07                 7       ///< GPIO INT[7]:   C_GPIO[33]
#define GPIO_INT_08                 8       ///< GPIO INT[8]:   P_GPIO[3]
#define GPIO_INT_09                 9       ///< GPIO INT[9]:   P_GPIO[6]
#define GPIO_INT_10                 10      ///< GPIO INT[10]:  P_GPIO[11]
#define GPIO_INT_11                 11      ///< GPIO INT[11]:  P_GPIO[15]
#define GPIO_INT_12                 12      ///< GPIO INT[12]:  P_GPIO[19]
#define GPIO_INT_13                 13      ///< GPIO INT[13]:  P_GPIO[29]
#define GPIO_INT_14                 14      ///< GPIO INT[14]:  P_GPIO[31]
#define GPIO_INT_15                 15      ///< GPIO INT[15]:  P_GPIO[35]
#define GPIO_INT_16                 16      ///< GPIO INT[16]:  P_GPIO[36]
#define GPIO_INT_17                 17      ///< GPIO INT[17]:  P_GPIO[37]
#define GPIO_INT_18                 18      ///< GPIO INT[18]:  P_GPIO[41]
#define GPIO_INT_19                 19      ///< GPIO INT[19]:  P_GPIO[20]
#define GPIO_INT_20                 20      ///< GPIO INT[20]:  P_GPIO[21]
#define GPIO_INT_21                 21      ///< GPIO INT[21]:  P_GPIO[43]
#define GPIO_INT_22                 22      ///< GPIO INT[22]:  C_GPIO[25]
#define GPIO_INT_23                 23      ///< GPIO INT[23]:  C_GPIO[31]
#define GPIO_INT_24                 24      ///< GPIO INT[24]:  L_GPIO[14]
#define GPIO_INT_25                 25      ///< GPIO INT[25]:  L_GPIO[15]
#define GPIO_INT_26                 26      ///< GPIO INT[26]:  L_GPIO[20]
#define GPIO_INT_27                 27      ///< GPIO INT[27]:  L_GPIO[21]
#define GPIO_INT_28                 28      ///< GPIO INT[28]:  L_GPIO[22]
#define GPIO_INT_29                 29      ///< GPIO INT[29]:  L_GPIO[26]
#define GPIO_INT_30                 30      ///< GPIO INT[30]:  P_GPIO[44]
#define GPIO_INT_31                 31      ///< GPIO INT[31]:  P_GPIO[47]
#define GPIO_INT_32                 32      ///< DGPIO INT[0]:  D_GPIO[0]
#define GPIO_INT_33                 33      ///< DGPIO INT[1]:  D_GPIO[1]
#define GPIO_INT_34                 34      ///< DGPIO INT[2]:  D_GPIO[2]
#define GPIO_INT_35                 35      ///< DGPIO INT[3]:  D_GPIO[3]
#define GPIO_INT_36                 36      ///< DGPIO INT[4]:  D_GPIO[4]
#define GPIO_INT_37                 37      ///< DGPIO INT[5]:  D_GPIO[5]
#define GPIO_INT_38                 38      ///< DGPIO INT[6]:  D_GPIO[6]
#define GPIO_INT_39                 39      ///< DGPIO INT[7]:  D_GPIO[7]
#define GPIO_INT_40                 40      ///< DGPIO INT[8]:  D_GPIO[8]
#define GPIO_INT_41                 41      ///< DGPIO INT[9]:  D_GPIO[9]
#define GPIO_INT_42                 42      ///< DGPIO INT[10]: D_GPIO[10]
#define GPIO_INT_43                 43      ///< DGPIO INT[11]: D_GPIO[11]
#define GPIO_INT_44                 44      ///< DGPIO INT[12]: D_GPIO[12]

#define GPIO_INT_48                 48      ///< DGPIO INT[16]: USB wakeup
#define GPIO_INT_49                 49      ///< DGPIO INT[17]: USB port power 1
#define GPIO_INT_50                 50      ///< DGPIO INT[18]: USB port power 2
#define GPIO_INT_51                 51      ///< DGPIO INT[19]: ETHERNET MII
#define GPIO_INT_52                 52      ///< DGPIO INT[20]: USB PHY

#define GPIO_INT_USBWAKEUP          (GPIO_INT_48)   ///< DGPIO INT[16]: USB wakeup
#define GPIO_INT_USBPOWER1          (GPIO_INT_49)   ///< DGPIO INT[17]: USB port power 1
#define GPIO_INT_USBPOWER2          (GPIO_INT_50)   ///< DGPIO INT[18]: USB port power 2
#define GPIO_INT_ETHMII             (GPIO_INT_51)   ///< DGPIO INT[19]: ETHERNET MII
#define GPIO_INT_USBPHY             (GPIO_INT_52)   ///< DGPIO INT[20]: USB PHY

#define GPIO_INT_EACOCP             0               ///Dummy

#define GPIO_INT_USBPLUGIN          (GPIO_INT_43)   ///< DGPIO INT[11]: USB VBUSI
#define GPIO_INT_USB2PLUGIN         (GPIO_INT_44)   ///< DGPIO INT[12]: USB2 VBUSI
//@}

// In order to backward comaptible, DGPIO is used as " GPIO_IS_DGPIO | D_GPIO_* " or "D_GPIO_*"
#define GPIO_IS_DGPIO               (224)


//
// The general api for the GPIO device driver
//
extern ER       gpio_open(void);
extern ER       gpio_close(void);
extern BOOL     gpio_isOpened(void);

extern ER       gpio_waitIntFlag(UINT32 gpioIntID);

// ---- GPIO ----
extern void     gpio_setDir(UINT32 pin, GPIO_DIR dir);
extern GPIO_DIR gpio_getDir(UINT32 pin);
extern void     gpio_setPin(UINT32 pin);
extern void     gpio_clearPin(UINT32 pin);
extern UINT32   gpio_getPin(UINT32 pin);
extern void     gpio_pullSet(UINT32 pin);
extern void     gpio_pullClear(UINT32 pin);
extern UINT32   gpio_readData(UINT32 dataidx);
extern void     gpio_writeData(UINT32 dataidx, UINT32 value);

// ---- DGPIO ----
extern UINT32   dgpio_readData(void);
extern void     dgpio_writeData(UINT32 value);

// ---- Interrupt Control ----
extern void     gpio_enableInt(UINT32 ipin);
extern void     gpio_disableInt(UINT32 ipin);
extern UINT32   gpio_getIntEnable(UINT32 ipin);
extern void     gpio_clearIntStatus(UINT32 ipin);
extern UINT32   gpio_getIntStatus(UINT32 ipin);
extern void     gpio_setIntTypePol(UINT32 ipin, GPIO_INTTYPE type, GPIO_INTPOL pol);
extern void     gpio_setIntIsr(UINT32 ipin, DRV_CB pHdl);
extern void     gpio_enableDestination(UINT32 ipin);
extern void     gpio_disableDestination(UINT32 ipin);

//@}
#endif

