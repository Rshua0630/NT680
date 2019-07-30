/*SFILE***************************************************************
 * Copyright (c) 2013 Samsung Electronics Co.Ltd
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 *
 * @file SpecialItemEnum_Base.h
 *
 * @brief This is header file for special item enum definations.
 *
 * @author SRI DELHI
 *
 *	 History	 :	 2013/8/13	 (initially created)
 *			 :	 2013/10/17  (modified on basis of FOXP ATSE F8000)
 **************************************************************EFILE*/

#ifndef _SPECIALITEM_ENUM_BASE_H_
#define _SPECIALITEM_ENUM_BASE_H_

/********************************************************************
    MACRO CONSTANT DEFINITIONS
********************************************************************/	
typedef enum
{
						IS_SPECIAL_ID_START = 0,
/*Option Byte*/
						ID_OPTION = 1,
/*ADC*/
						ID_ADC = 2,
/*ADC Target*/
						ID_ADC_TARGET = 3,
/*ADC Result*/
						ID_ADC_RESULT = 4,
/*White Balance*/
						ID_WB = 5,
/*W/B Movie*/
						ID_WBMOVIE = 6,
/*EPA Standard*/						
						ID_EPA_STANDARD = 7,
/*Advanced*/				
						ID_ADVANCED = 8,
/*Video/Scaler*/
						ID_VIDEOSCALER = 9,
/*Enhancement*/
						ID_ENHANCEMENT = 10,
/*SOUND*/
						ID_SOUND = 11,
/*LNA+*/
						ID_LNA_PLUS = 12,
/*Hotel Option*/
						ID_HOTEL_OPTION = 13,
/*EDID*/
						ID_EDID = 14,	
/*CHECKSUM*/
						ID_CHECKSUM = 15,
/*Factory Reset*/
						ID_FACTORY_RESET = 16,
/*Type*/
						ID_PANEL_TYPE = 17,
/*Model*/
						ID_MODEL = 18,
/*Country*/
						ID_COUNTRY = 19,
/*Watchdog Enable*/						
						ID_WATCHDOG = 20,
/*Dimming Selection*/		
						ID_DIMMING_SEL = 21,
/*RS-232 Jack*/
						ID_RS232_JACK = 22,
/*Gamma*/
						ID_OPTION_GAMMA = 23,
/*Panel Display  or Lamp Time*/
						ID_PANELTIME = 24,
/*Mute Time*/			
						ID_RF_MUTE_TIME = 25,
/*Shop Mode*/						
						ID_SHOP_MODE = 26,
/*7.5 IRE*/
						ID_75_IRE = 27,
/*7.5 IRE Offset*/
						ID_75_IRE_OFFSET = 28,
/*HDMI Hot Plug*/				
						ID_HDMI_HOT_PLUG = 29,
/*HDMI Delay Time*/						
						ID_HDMI_DELAY_TIME = 30,
/*HDMI FLT CNT SIG*/
						ID_HDMI_FLT_CNT_SIG = 31,
/*HDMI FLT CNT LOS*/
						ID_HDMI_FLT_CNT_LOS = 32,
/*HDMI EQ*/
						ID_HDMI_EQ = 33,
/*UNSTABLE BAN CNT*/
						ID_UNSTABLE_BAN_CNT = 34,
/*HDMI Err Cnt*/
						ID_HDMI_ERR_CNT = 35,
/*PC Mode Ident*/
						ID_PC_MODE_IDENT = 36,
/*Watchdog Count*/
						ID_WATCHDOG_CNT = 37,
/*SUB MICOM DOWN*/					
						ID_SUBMICOM_DOWN = 38,
/*IIC BUS STOP*/					
						ID_IIC_BUSSTOP = 39,
/*Wall Mount - Wall Mount OSD Menu 지원여부에 대한 Special ID (동작 지원에 대해서는 TDiSystem Support Option으로 별도 처리 )*/				
						ID_WALL_MOUNT_SUPPORT = 40,
/*Language (PnP Language) */
						ID_LANGUAGE = 41,
/*TTX - 제품별로 TTX Supoort 하지 않는 경우 있음.*/						
						ID_TTX_SUPPORT = 42,
/*TTX List*/						
						ID_TTXLIST_MODE = 43,
/*TTX Group*/			
						ID_TTX_GROUP = 44,
////////////////////////// For only DVB End/////////////////////////////////////////						
/*Visual Test*/			
						ID_VISUAL_TEST = 45,
/*TUNER*/			
						ID_TUNER = 46,
/*Expert Adj*/			
						ID_EXPERT_ADJ = 47,
/*N/D Adj*/
						ID_ND_ADJ = 48,
/*WB Reset*/			
						ID_WB_RESET = 49,
/*EER Reset*/		
						ID_EER_RESET = 50,
////////////////////////// For Calibration /////////////////////////////////////////												
/*AV Calibration*/						
						ID_AV_CALIBRATION = 51,
/*COMP Calibration*/						
						ID_COMP_CALIBRATION = 52,
/*PC Calibration*/						
						ID_PC_CALIBRATION = 53,
/*HDMI Calibration*/						
						ID_HDMI_CALIBRATION = 54,
/*Calibration Target 1st AV Y Low*/
						ID_1ST_AV_YL = 55,
/*Calibration Target 1st AV Y High*/						
						ID_1ST_AV_YH = 56,	
/*Calibration Target 1st AV Delta*/						
						ID_1ST_AV_D = 57,	
/*Calibration Target 1st COMP Y Low*/						
						ID_1ST_COMP_YL = 58,
/*Calibration Target 1st COMP Y High*/
						ID_1ST_COMP_YH = 59,
/*Calibration Target 1st COMP Cb Low*/						
						ID_1ST_COMP_CbL = 60,
/*Calibration Target 1st COMP Cb High*/						
						ID_1ST_COMP_CbH = 61,				
/*Calibration Target 1st COMP Cr Low*/						
						ID_1ST_COMP_CrL = 62,
/*Calibration Target 1st COMP Cr High*/						
						ID_1ST_COMP_CrH = 63,						
/*Calibration Target 1st COMP Delta*/						
						ID_1ST_COMP_D = 64,
/*Calibration Target 1st PC Delta*/						
						ID_1ST_PC_D = 65,
/*	ADC Result	- Value	*/	
/*1st_Y_GH*/		
						ID_1st_Y_GH = 66,
/*1st_Y_GL*/		
						ID_1st_Y_GL = 67,
/*1st_Cb_BH*/		
						ID_1st_Cb_BH = 68,
/*1st_Cb_BL*/		
						ID_1st_Cb_BL = 69,
/*1st_Cr_RH*/		
						ID_1st_Cr_RH = 70,
/*1st_Cr_RL*/		
						ID_1st_Cr_RL = 71,
/*2nd_R_L*/		
						ID_2nd_R_L = 72,
/*2nd_G_L*/		
						ID_2nd_G_L = 73,
/*2nd_B_L*/		
						ID_2nd_B_L = 74,	
/*2nd_R_H*/		
						ID_2nd_R_H = 75,
/*2nd_G_H*/		
						ID_2nd_G_H = 76,
/*2nd_B_H*/		
						ID_2nd_B_H = 77,
////////////////////////// For Calibration End/////////////////////////////////////////	
/*Sub Brightness*/	
						ID_WB_SUB_BRIGHTNESS = 78,
/*R-Offset*/						
						ID_WB_ROFFSET = 79,
/*G-Offset*/						
						ID_WB_GOFFSET = 80,
/*B-Offset*/						
						ID_WB_BOFFSET = 81,						
/*Sub Contrast*/	
						ID_WB_SUB_CONTRAST = 82,
/*R-Gain*/						
						ID_WB_RGAIN = 83,
/*G-Gain*/						
						ID_WB_GGAIN = 84,
/*B-Gain*/						
						ID_WB_BGAIN = 85,		
/*Movie R-Offset*/	
						ID_WB_MOVIE_ROFFSET = 86,
/*Movie B-Offset*/	
						ID_WB_MOVIE_BOFFSET = 87,
/*Movie R-Gain*/	
						ID_WB_MOVIE_RGAIN = 88,
/*Movie B-Gain*/	
						ID_WB_MOVIE_BGAIN = 89,
/*W/B Movie*/		
						ID_WB_MOVIE_ONOFF = 90,
/*Mode*/		
						ID_WB_MOVIE_MODE = 91,
/*Color Tone*/						
						ID_WB_MOVIE_COLORTONE = 92,
/*MSub Brightness*/						
						ID_WB_MOVIE_SUBBRIGHTNESS = 93,
/*MSub Contrast*/
						ID_WB_MOVIE_SUBCONTRAST = 94,
/*W2_RGain*/						
						ID_WB_MOVIE_W2_RGAIN = 95,	
/*W2_BGain*/						
						ID_WB_MOVIE_W2_BGAIN = 96,	
/*W2_ROffset*/						
						ID_WB_MOVIE_W2_ROFFSET = 97,
/*W2_BOffset*/						
						ID_WB_MOVIE_W2_BOFFSET = 98,
/*Movie Gamma*/
						ID_WB_MOVIE_GAMMA = 99,							
/*Movie Contrast*/
						ID_WB_MOVIE_CONTRAST = 100,							
/*Movie Brightness*/
						ID_WB_MOVIE_BRIGHTNESS = 101,							
/*Movie Color*/
						ID_WB_MOVIE_COLOR = 102,							
/*Movie Sharpness*/
						ID_WB_MOVIE_SHARPNESS = 103,							
/*Movie Tint*/
						ID_WB_MOVIE_TINT = 104,							
/*Movie Backlight*/
						ID_WB_MOVIE_BACKLIGHT = 105,		
/*Standard Contrast*/
						ID_EPA_STANDARD_CONTRAST = 106,							
/*Standard Brightness*/
						ID_EPA_STANDARD_BRIGHTNESS = 107,							
/*Standard Color*/
						ID_EPA_STANDARD_COLOR = 108,							
/*Standard Sharpness*/
						ID_EPA_STANDARD_SHARPNESS = 109,							
/*Standard Tint*/
						ID_EPA_STANDARD_TINT = 110,							
/*Standard Backlight*/
						ID_EPA_STANDARD_BACKLIGHT = 111,
/* (HotelTv + RetailHotel) Hospitality Option Mode - value ? TDiSystem::TDHotelMode_k (TPCTv::TV_OPTION_HOTEL_MODE) */
						ID_HOTEL_MODE = 112,
/* (HotelTv + RetailHotel) Power On Channel */
						ID_HOTEL_POWER_ON_CHANNEL = 113,
/* (HotelTv + RetailHotel) Power On Volume*/
						ID_HOTEL_POWER_ON_VOLUME = 114,
/* (HotelTv + RetailHotel) Power On Source*/
						ID_HOTEL_POWER_ON_SOURCE = 115,
/* (HotelTv + RetailHotel) Panel Button Lock*/
						ID_HOTEL_PANELBTN_LOCK = 116,
/* (HotelTv + RetailHotel) Min Volume*/
						ID_HOTEL_MIN_VOLUME = 117,
/* (HotelTv + RetailHotel) Max Volume*/
						ID_HOTEL_MAX_VOLUME = 118,
/* (HotelTv + RetailHotel) Picture Menu Lock*/
						ID_HOTEL_PICTURE_MENU_LOCK = 119,
/*Sub AMP Volume*/
						ID_HOTEL_SUB_AMP_VOLUME = 120,
/*Sub AMP Mode*/
						ID_HOTEL_SUB_AMP_MODE = 121,
/* (HotelTv) Local Time - Auto/ Manual/ TTX */
						ID_HOTEL_LOCAL_TIME = 122,
/*EDID ON/OFF*/
						ID_EDID_ONOFF = 123,
/*ALL EDID*/
						ID_EDID_ALL = 124,
/*PC EDID*/
						ID_PC_EDID = 125,
/*HDMI1 EDID*/
						ID_HDMI1_EDID = 126,
/*HDMI2 EDID*/
						ID_HDMI2_EDID = 127,
/*HDMI3 EDID*/
						ID_HDMI3_EDID = 128,
/*HDMI4 EDID*/
						ID_HDMI4_EDID = 129,						
/*EDID HDMI1 Ver*/						
						ID_EDID_VER = 130,
/*Patt-Sel*/
						ID_PATTSEL = 131,
/*DVI/HDMI Sound*/
						ID_DVI_HDMI_SOUND = 132,
/*Select Log Type*/
						ID_LOG_TYPE = 133,
/*View Log*/						
						ID_VIEW_LOG = 134,
/*Delete Log*/						
						ID_DELETE_LOG = 135,			
/*Num of ATV*/			
						ID_NUM_OF_ATV = 136,
/*Num of DTV*/			
						ID_NUM_OF_DTV = 137,
/*Num of AV*/			
						ID_NUM_OF_AV = 138,
/*Num of SVIDEO*/	
						ID_NUM_OF_SVIDEO = 139,
/*Num of COMP*/	
						ID_NUM_OF_COMP = 140,
/*Num of HDMI*/	
						ID_NUM_OF_HDMI = 141,		
/*Num of PC*/			
						ID_NUM_OF_PC = 142,
/*Num of SCART*/		
						ID_NUM_OF_SCART = 143,
/*Num of DVI*/		
						ID_NUM_OF_DVI = 144,
/*Num of OPTICAL Link*/	
						ID_NUM_OF_OPTICAL_LINK = 145,
/*Num of MEDIA*/	
						ID_NUM_OF_MEDIA = 146,
/*Num of PANEL KEY*/					
						ID_NUM_OF_PANELKEY = 147,
/*Num of USB Port*/						
						ID_NUM_OF_USBPORT = 148,
/*LNA Support*/						
						ID_LNA_SUPPORT = 149,	
/*Info Link Type*/						
						ID_INFOLINK_TYPE = 150,
/*SELECT LCD/PDP : match with TDPanelType_k */
						ID_SELECT_LCD_PDP = 151,
/*MFT Offset*/
						ID_MFT_OFFSET = 152,
/*Light Effect*/
						ID_STANDBY_LED = 153,
/*Pixel Shift Test*/
						ID_PDP_PIXELSHIFT_TEST = 154,
/*LVDS Format */
						ID_LVDS_FORMAT = 155,
/*Source*/				
						ID_ND_SOURCE = 156,
/*DVI/HDMI Port*/
						ID_DVI_HDMI_PORT = 157,
/*Spread Spectrum*/						
						ID_SPREAD_SPECTRUM_ONOFF = 158,
/*Spread Spectrum MRR*/						
						ID_SPREAD_SPECTRUM_MRR = 159,
/*Spread Spectrum MFR*/						
						ID_SPREAD_SPECTRUM_MFR = 160,
/*Spread Spectrum QLC*/						
						ID_SPREAD_SPECTRUM_QLC = 161,
/*Sharpness*/
						ID_SHARPNESS = 162,
/*PDP > Pattern Select*/
						ID_PDP_PATTSEL = 163,
/*Dynamic Contrast*/	
						ID_DYNAMIC_CONTRAST = 164,
/*Region*/				
						ID_REGION = 165,
/*InfoLink Server Type*/
						ID_INFOLINK_SERVER_TYPE = 166,						
/*C_RGain*/						
						ID_WB_MOVIE_N_RGAIN = 167,	
/*C_BGain*/						
						ID_WB_MOVIE_N_BGAIN = 168,	
/*C_ROffset*/						
						ID_WB_MOVIE_N_ROFFSET = 169,
/*C_BOffset*/						
						ID_WB_MOVIE_N_BOFFSET = 170,						
/*W3_RGain*/						
						ID_WB_MOVIE_W3_RGAIN = 171,	
/*W3_BGain*/						
						ID_WB_MOVIE_W3_BGAIN = 172,	
/*W3_ROffset*/						
						ID_WB_MOVIE_W3_ROFFSET = 173,
/*W3_BOffset*/						
						ID_WB_MOVIE_W3_BOFFSET = 174,
/*Service Parameter*/
						ID_SERVICE_PARAMETER = 175,
/*Dynamic Dimming*/	
						ID_DYNAMIC_DIMMING = 176,
/*Indicator LED*/
						ID_INDICATOR_LED = 177,
/*MediaPlay Video Enable*/
						ID_MEDIAPLAY_VIDEO = 178,
/*MediaPlay DLNA Enable*/
						ID_MEDIAPLAY_DLNA = 179,
/*ARB Language (LanguageSet) */				
						ID_ARABIC_LANGUAGE = 180,
//3Will be DELETED..
						ID_OTA_TEST_ONOFF = 181,
//3Will be DELETED..
						ID_OTA_DELETE_ALTERNATIVE_VERSION = 182,
/*Support Sub Woofer*/
						ID_SUPPORT_SUBWOOFER = 183,
/*EDID All Write*/
						ID_HDMI_ALL_EDID = 184,
/*EDID 1.2 Port - 내년도에 명칭 변경 예정*/
						ID_EDID_1_2_PORT = 185,			
//3Will be DELETED.. replace to ProductInfo
						ID_NETWORK = 186,
/*LocalSet - China HD*/
						ID_CHINA_HD = 187,
/*LocalSet - NT Conversion*/
						ID_NT_CONVERSION = 188,
/*LocalSet - Volume Table*/
						ID_VOLUME_CURVE = 189,
/*LocalSet - High Devi*/
						ID_HIGH_DEVI = 190,
/*LocalSet - Sepco 120Hz*/
						ID_SEPCO_120HZ = 191,
/*LocalSet - Unbalance*/
						ID_UNBALANCE = 192,
/*PDP Group*/
						ID_PDP_GROUP = 193,
/*Local Set*/
						ID_LOCALSET = 194,
//3Will be DELETED..
						ID_OTN_RESET = 195,
//3Will be DELETED..
						ID_OTN_DURATION = 196,
//3Will be DELETED..
						ID_OTN_FAIL_TEST = 197,
/*PDP만을 위한 메뉴는 PDP에서만 활성화 하도록*/
						ID_PDP_OPTION = 198,	
/*Exhibition mode*/
						ID_EXHIBITION_MODE = 199,
/*Service Parameter*/
						ID_SERVICE_PARAMETER1 = 200,

						ID_SERVICE_PARAMETER2 = 201,

						ID_SERVICE_PARAMETER3 = 202,

						ID_SERVICE_PARAMETER4 = 203,
/*Tools Support*/
						ID_TOOLS_SUPPORT = 204,
/*OTN Support (TPCFactory::FACTORY_OPTION_OTN_ONOFF, TPCFactory::FACTORY_CTRL_OTN_ONOFF) */
						ID_OTN_ONOFF = 205,
/*Carrier Mute*/
						ID_CARRIER_MUTE = 206,
/*Brazil Demodulator*/
						ID_DEMODULATOR = 207,
/*Num Of Display*/
						ID_NUM_OF_DISPLAY = 208,	
/*Cal Day/Cal Nigth Support*/
						ID_SUPPORT_ND_ADJ = 209,
/*MediaPlay DB*/
						ID_MEDIAPLAY_DB = 210, 
/*PDP 24Px4 Support*/
						ID_SUPPORT_24PX4 = 211,
/*HDMI Callback*/
						ID_HDMI_CALL_BACK = 212,
/*Support Power Indicator*/
						ID_SUPPORT_POWER_INDICATOR = 213,
/*OTN Testserver*/
						ID_OTN_TESTSERVER = 214,
/*HP Mode Support*/
						ID_SUPPORT_PICTURE_MODE_HP = 215,
/*Ch Map Table*/
						ID_CHMAP_TABLE = 216,
/*Support RF Remocon*/
						ID_SUPPORT_RF_REMOCON = 217,
/*SAPC Timer*/
						ID_PDP_APC_TIMER = 218,
/*OTN Server Type*/
						ID_OTN_SERVER_TYPE = 219,
/*Pattern select - 1*/
						ID_PATTSEL_1 = 220,
/*Pattern select - 2*/
						ID_PATTSEL_2 = 221,
/*HDMI Black Level*/
						ID_HDMI_BLACK_LEVEL = 222,	
/*Pattern Select Menu - Factory 해제시에 모든 하위 항목들을 0으로 셋팅*/
						ID_PATTSEL_MENU = 223,
/*PDP APC Speed*/
						ID_PDP_APC_SPEED = 224,
/*Auto Panel Setting*/
						ID_AUTO_PANEL_SETTING = 225,
/*Tuner Status*/
						ID_TUNER_STATUS = 226,
/*Logic Download*/
						ID_LOGIC_DOWNLOAD = 227,
/*Data Service*/
						ID_SUPPORT_DATA_SERVICE = 228,

						ID_SCC_ONOFF = 229,

						ID_SCC_IN_HX = 230,

						ID_SCC_IN_HY = 231,

						ID_SCC_IN_LX = 232,

						ID_SCC_IN_LY = 233,

						ID_SCC_SOURCE_DATA = 234,

						ID_SCC_SWAP = 235,

						ID_PANEL_SCC_CONST_HX = 236,

						ID_PANEL_SCC_CONST_HY = 237,

						ID_PANEL_SCC_CONST_LX = 238,

						ID_PANEL_SCC_CONST_LY = 239,

						ID_PBA_SCC_CONST_HX = 240,

						ID_PBA_SCC_CONST_HY = 241,

						ID_PBA_SCC_CONST_LX = 242,

						ID_PBA_SCC_CONST_LY = 243,

						ID_INFOLINK_COUNTRY = 244,

						ID_TCON_DOWNLOAD = 245,

						ID_2ND_ENCODER_WATCHDOG_COUNT = 246,
						
						ID_DVI_EDID = 247,
						
						ID_HOTEL_SI_VENDOR = 248,
/* (HotelTv + RetailHotel) Music Mode AV */						
						ID_HOTEL_MUSIC_MODE_AV = 249,
/* (HotelTv + RetailHotel) Music Mode PC */
						ID_HOTEL_MUSIC_MODE_PC = 250,
/* (HotelTv + RetailHotel) Music Mode Comp */
						ID_HOTEL_MUSIC_MODE_COMP = 251,
/* (HotelTv + RetailHotel) Music Mode Backlight */
						ID_HOTEL_MUSIC_MODE_BK_LGHT = 252,

						ID_HOTEL_RJP_PRIORITY_AV = 253,

						ID_HOTEL_RJP_PRIORITY_PC = 254,

						ID_HOTEL_RJP_PRIORITY_HDMI = 255,

						ID_HOTEL_RJP_AV_OPTION = 256,

						ID_HOTEL_RJP_HDMI_OPTION = 257,

						ID_HOTEL_TIME_FORMAT = 258,

						ID_HOTEL_PMO_LED_TEST = 259,

						ID_HOTEL_PMO_NORMAL_DIM = 260,

						ID_HOTEL_PMO_STDBY_DIM = 261,

						ID_HOTEL_AUDIO_LOOP_IN = 262,
/* (HotelTv + RetailHotel) Menu Display */
						ID_HOTEL_MENU_DSPLAY = 263,
/* Hospitality Logo */
						ID_HOTEL_HTL_LOGO = 264,
/* Hospitality Logo Download */
						ID_HOTEL_HTL_LOGO_DWLD = 265,

						ID_HOTEL_LOGO_DISP_TIME = 266,
/* (HotelTv + RetailHotel) Power On Option */
						ID_HOTEL_PWR_ON_OPT = 267,
/* (HotelTv + RetailHotel) Auto Source */
						ID_HOTEL_AUTO_PC = 268,
/* (HotelTv + RetailHotel) Energy Saving - value ? TPCTv::EEnergySaving */
						ID_HOTEL_ENERGY_SAVING = 269,
/* (HotelTv + RetailHotel) Clone TV to USB */
						ID_HOTEL_CLONE_TV2USB = 270,
/* (HotelTv + RetailHotel) Clone USB to TV */
						ID_HOTEL_CLONE_USB2TV = 271,

						ID_HOTEL_PAN_EURO_MHEG = 272,
/* (HotelTv + RetailHotel) Setting Auto Initialization */
						ID_HOTEL_DATA_RESET = 273,

						ID_HOTEL_WELCOME_MSG = 274,

						ID_HOTEL_EDIT_WEL_MSG = 275,

						ID_HOTEL_ENERGY_SAV_TIMER = 276,

						ID_HOTEL_TV_VERSION = 277,

						ID_HOTEL_CAPTION = 278,
/* (Hotel TV) Channel Editor */
						ID_HOTEL_CHANNEL_EDITOR = 279,
/*to dump emergency log data to USB*/						
						ID_EMERGENCY_LOG = 280,

						ID_NUM_OF_TUNER = 281,

						ID_NUM_OF_DECODER = 282,	
/* the number of FM Radio Source */
						ID_NUM_OF_FM_RADIO = 283,				
/* to get the support information of the FM transmitter */
						ID_FM_TRANSMITTER_SUPPORT = 284,

						ID_SUPPORT_COLORSPACE = 285,

						ID_SCC_MOVIE_MODE = 286,

						ID_MHP_SUPPORT = 287,
/* Type of panel key - match with TDiSystem::TDFunctionKeyLevel_k (HORIZONTAL, VERTICAL, PDP_VERTICAL, PDP_HORIZONTAL) */
						ID_FUNCTION_KEY = 288,
/*MediaPlay PlayList*/
						ID_MEDIAPLAY_PLAYLIST = 289,
/* It represents number of IPTV source  */
						ID_NUM_OF_IPTV = 290,
/* This is used to get the support information of the Gemstar EPG */		
						ID_GEMSTAR_EPG_SUPPORT = 291,

						ID_SUPPORT_ATV_WSS = 292,

						ID_CABLE_MODULATION = 293,
/* (HotelTV) IPTV enable/disable to TPCFactory/TCFactory::EOption	*/
						ID_IPTV_ENABLE = 294,
/* (HotelTv + RetailHotel) CHANNEL TYPE */
						ID_HOTEL_POWER_ON_CHANNEL_TYPE = 295,
/* Factory control for Server IP Setting and Network Set-up in IPTV configuration in 2010	(modified to 'HotelSmartTV' Configuration in 2011) */
						ID_IPTV_SERVER_IP_SETTING = 296,
						
						ID_IPTV_NETWORK_SETUP = 297,
/* Update FactoryPart2.dat. same used [TPCFactory::FACTORY_CTRL_HOTEL_CLONE_USB_TO_TV] with [ID_HOTEL_CLONE_USB2TV] */
						ID_COPY_FAC_PART2 = 298,		

						ID_PEAK_DIMMING_ONOFF = 299,

						ID_HDMI_EDID_WRITE_TYPE = 300,

						ID_SUPPORT_PVR = 301,

						ID_HOTEL_DYNAMIC_SI = 302,

						ID_PICTURE = 303,
/*Standard Contrast - match with [ID_EPA_STANDARD_CONTRAST] */						
						ID_3D_CONTRAST = 304,
/*Standard Brightness - match with [ID_EPA_STANDARD_BRIGHTNESS]	*/
						ID_3D_BRIGHTNESS = 305,

						ID_HOTEL_DC_POWER = 306,
/* (HotelTV) Mixed Channel Map for DVB. 1 is On, 0 is Of*/
						ID_HOTEL_MIXED_CHMAP = 307,

						ID_FRONT_COLOR = 308,
/* (Hospital Model) Channel Auto Store (match with TPCFactory::FACTORY_CTRL_HOTEL_CHANNEL_AUTO_STORE) */
						ID_HOTEL_CHANNEL_AUTO_STORE = 309,
/* HDMI VSI Filtering Count 조정 */			
						ID_HDMI_3D_DET = 310,

						ID_FM_TRANSMITTER_CARRIER = 311,

						ID_MONO_LAST_MEMORY = 312,
/* IPTV: for VOD Bandwidth Menu */
						ID_VOD_BANDWIDTH = 313,

						ID_OTP_LOCK = 314,
/* (Hospital TV) Channel Bank Editor	*/
						ID_HOTEL_CHANNEL_BANK_EDITOR = 315,
/* Hospital TV Enable (On/Off) :  only for HotelTV (not RetailHotel) */
						ID_HOSPITAL_MODE = 316,
/* (Hospital TV) Pillow Speaker Mode On/Off */
						ID_PILLOW_SPEAKER_MODE = 317,
/* (Hospital TV) Pillow speaker Type: Analog(1Key)/Zenith/Philips/Sec */
						ID_PILLOW_SPEAKER_TYPE = 318,
/* (Hospital TV) Channel Bank service level */						
						ID_HOSPITAL_CHANNEL_BANK_LEVEL = 319,
/* (Hospital TV) For US Hospitals is needed to be added in standalone and interactive menu. (value: Min/Normal/Min) */
						ID_PHILLOW_AMP_GAIN = 320,
/* Auto store type : '0' - ("Nerver")Trident/X4 방식, '1' - ("Fast")Valencia 방식, 0 - 외부demodulator, 1 - 내부demodulator*/						
						ID_AUTO_STORE_TYPE = 321,				
/* (HotelTv + RetailHotel) POWER ON VOLUME ENABLE - User Defined / Last Saved */
						ID_POWER_ON_VOLUME_EN = 322,
/* (HotelTv + RetailHotel) POWER ON CHANNEL ENABLE - User Defined / Last Saved */
						ID_POWER_ON_CHANNEL_EN = 323,
/* LFD Calibration with user settig by Menu Operation(USER AUTO COLOR), Set : 1 Reset : 0 */ 
						ID_USER_AUTO_COLOR = 324,
/* Cconcurrent Record Num */
						ID_CONCURRENT_RECORD_NUM = 325,
/* LFD Video System : 값에 따라  TD_STORE_TYPE_35 의 Nvram Index가 할당된다  */
						ID_VIDEO_SYSTEM = 326,
/* (LFD) Retry SyncProcess */
						ID_RETRY_SYNCPROCESS = 327,
/* Num of Display Port */			
						ID_NUM_OF_DISPLAY_PORT = 328,
/* WB Movie_3D - match with [ID_WBMOVIE]	*/
						ID_3D_WB_MOVIE = 329,
/* W/B MOVIE ON/OFF - match with [ID_WB_MOVIE_ONOFF]	*/
						ID_3D_WB_MOVIE_ON_OFF = 330,
/* MODE - match with [ID_WB_MOVIE_MODE]	*/
						ID_3D_WB_MOVIE_MODE = 331,
/* Color Tone - match with [ID_WB_MOVIE_COLORTONE]	*/
						ID_3D_COLOR_TONE = 332,
/* 3D_Sub Contrast - match with [ID_WB_SUB_CONTRAST]	*/
						ID_3D_SUB_CONTRAST = 333,
/* 3D_Sub Brightnes - match with [ID_WB_SUB_BRIGHTNESS]	*/
						ID_3D_SUB_BRIGHTNESS = 334,
/* 3D_MSub Brightness - match with [ID_WB_MOVIE_SUBBRIGHTNESS]		*/
						ID_3D_M_SUB_BRIGHTNESS = 335,
/* 3D_MSub Contrast - match with [ID_WB_MOVIE_SUBCONTRAST]	*/
						ID_3D_M_SUB_CONTRAST = 336,
/* 3D_C_Rgain */
						ID_3D_C_RGAIN = 337,
/* 3D_C_Bgain */
						ID_3D_C_BGAIN = 338,
/* 3D_C_Roffset */
						ID_3D_C_ROFFSET = 339,
/* 3D_C_Boffset */
						ID_3D_C_BOFFSET = 340,
/* 3D_N_Rgain - match with [ID_WB_MOVIE_N_RGAIN]	*/
						ID_3D_N_RGAIN = 341,
/* 3D_N_Bgain - match with [ID_WB_MOVIE_N_BGAIN]	*/
						ID_3D_N_BGAIN = 342,
/* 3D_N_Roffset - match with [ID_WB_MOVIE_N_ROFFSET]	*/
						ID_3D_N_ROFFSET = 343,
/* 3D_N_Boffset - match with [ID_WB_MOVIE_N_BOFFSET]	*/
						ID_3D_N_BOFFSET = 344,
/* 3D_W2_Rgain - match with [ID_WB_MOVIE_W2_RGAIN]	*/
						ID_3D_W2_RGAIN = 345,
/* 3D_W2_Bgain - match with [ID_WB_MOVIE_W2_BGAIN]	*/
						ID_3D_W2_BGAIN = 346,
/* 3D_W2_Roffset - match with [ID_WB_MOVIE_W2_ROFFSET]	*/
						ID_3D_W2_ROFFSET = 347,
/* 3D_W2_Boffset - match with [ID_WB_MOVIE_W2_BOFFSET]	*/
						ID_3D_W2_BOFFSET = 348,
/* 3D_Movie Contrast - match with [ID_WB_MOVIE_CONTRAST]	*/
						ID_3D_MOVIE_CONTRAST = 349,
/* 3D_Movie Bright - match with [ID_WB_MOVIE_BRIGHTNESS]	*/
						ID_3D_MOVIE_BRIGHT = 350,
/* 3D_Movie Color - match with [ID_WB_MOVIE_COLOR]		*/
						ID_3D_MOVIE_COLOR = 351,
/* 3D_Movie Sharpness - match with [ID_WB_MOVIE_SHARPNESS]		*/
						ID_3D_MOVIE_SHARPNESS = 352,
/* 3D_Movie Tint - match with [ID_WB_MOVIE_TINT]	*/
						ID_3D_MOVIE_TINT = 353,
/* 3D_Movie Backlight - match with [ID_WB_MOVIE_BACKLIGHT]	*/
						ID_3D_MOVIE_BACKLIGHT = 354,
/* 3D_Movie Gamma - match with [ID_WB_MOVIE_GAMMA]	*/
						ID_3D_MOVIE_GAMMA = 355,
/* 3D_M_Sub_Gamma */
						ID_3D_M_SUB_GAMMA = 356,
/* HDMI_Black_level - match with [ID_HDMI_BLACK_LEVEL]	*/
						ID_3D_HDMI_BLACK_LEVEL = 357,
/* EPA_3D - match with [ID_EPA_STANDARD]		*/
						ID_3D_EPA_STANDARD = 358,
/* 3D_Standard Sharpness - match with [ID_EPA_STANDARD_SHARPNESS]*/
						ID_3D_STANDARD_SHARPNESS = 359,
/* 3D_Standard Color - match with [ID_EPA_STANDARD_COLOR]	*/
						ID_3D_STANDARD_COLOR = 360,
/* 3D_Standard Tint - match with [ID_EPA_STANDARD_TINT]	*/	
						ID_3D_STANDARD_TINT = 361,
/* 3D_Standard Backlight - match with [ID_EPA_STANDARD_BACKLIGHT]	*/
						ID_3D_STANDARD_BACKLIGHT = 362,	
/* Gamma - match with [ID_OPTION_GAMMA]	*/
						ID_OPTION_GAMMA_3D = 363,						
/* Natural_Gamma 3D - match with [ID_OPTION_NATURAL_GAMMA] */
						ID_OPTION_NATURAL_GAMMA_3D = 364,	
/* Natural_Gamma	 */
						ID_OPTION_NATURAL_GAMMA = 365,
/* the number of RVU source */						
						ID_NUM_OF_RVU = 366,
/* (Projector) Panel Display or Lamp Time Clear*/
						ID_PANELTIME_CLEAR = 367,
/* (Projector) Projector Time*/
						ID_PROJECTORTIME = 368,
/* (Projector) Projector Time Clear*/
						ID_PROJECTORTIME_CLEAR = 369,
/* (Projector) Color Wheel Delay */
						ID_COLORWHEEL_INDEX_DELAY = 370,
/* (Projector) Main to DMD */
						ID_MAIN_TO_DMD = 371,																	
/* 3D Picture */						
						ID_PICTURE_3D = 372,
/* (HotelTV) To get the information which hotel widget mode is enable */						
						ID_HOTEL_WIDGET_MODE = 373,
/* (3D Projector) Protection Mode*/
						ID_PROTECTION_MODE = 374,
/* (3D Projector) DDP Test Pattern */
						ID_DDP_PATTSEL = 375,
/* SUBMICOM USB UPGRADE */
						ID_SUBMICOM_USB_UPGRADE = 376,
/* Calibration Target 1st PC Low */
						ID_1ST_PC_L = 377,
/* Calibration Target 1st PC High */
						ID_1ST_PC_H = 378,
/* Calibration Target 2nd ACH Low */
						ID_2ND_ACH_L = 379,
/* Calibration Target 2nd ACH High */
						ID_2ND_ACH_H = 380,
/* Calibration Target 2nd PC Low */
						ID_2ND_PC_L = 381,
/* Calibration Target 2nd PC High */
						ID_2ND_PC_H = 382,
/* Calibration Target 2nd Delta */						
						ID_2ND_D = 383,
/* (Hotel) External Source banner - value? On/Off	*/
						ID_EXT_SOURCE_BANNER = 384,
/* Napoli Pre Test Pattern*/
						ID_NAPOLI_PRE_PATTSEL = 385,
/* Napoli Post Test Pattern*/
						ID_NAPOLI_POST_PATTSEL = 386,
/* (Hotel) Manual SIRCH */
						ID_SIRCH_MANUAL = 387,
/* (Hotel) SIRCH Channel */
						ID_SIRCH_CHANNEL = 388,
/* 3D Enhancement - match with [ID_ENHANCEMENT]	*/
						ID_3D_ENHANCEMENT = 389,
/* 3D Sharpness - match with [ID_SHARPNESS]	*/
						ID_3D_SHARPNESS = 390,
/* WIFI - iperf server stop/start */
						ID_WIFI_IPERF = 391,
/* Control AMP EQ and AudioProcessor SRS */
						ID_AMP_SPK_EQ = 392,
/* (Hotel) Hotel ID (cf. Edit Welcome Message) */
						ID_HOTEL_ID = 393,
/* (Hotel) Widget Download */						
						ID_WIDGET_DOWNLOAD = 394,
/* (3D Projector) Temperature Protection*/
						ID_TEMP_PROTECTION = 395,						
/* (Hotel - EU Models only) Ch rescan msg - ON/OFF */
						ID_CH_RESCAN_MSG_ON_OFF = 396,
/* (Hospital) Speaker Select - MAIN/EXTERNAL */
						ID_SPEAKER_SELECT = 397,
/* OTA Support - OFF, GENERAL */
						ID_NORMAL_OTA_SUPPORT = 398,
/* (HotelTV) OTA Support - OFF(OnlyManual)/ 1hour/ 2hour/ 12:00am/ 02:00am/ 12:00pm/ 02:00pm  */
						ID_HOTEL_OTA_SUPPORT = 399,
//3 Will be DELETED..
						ID_HOTEL_MODE_OPTION = 400,						
/* (BDPlayer) Internal HDD Support option - value ? on/off (match with TPCFactory::FACTORY_OPTION_INTERNAL_HDD_SUPPORT) */
						ID_INTERNAL_HDD_SUPPORT = 401,		
/* Key Sensitivity */						
						ID_KEY_SENSITIVITY = 402,
/* (Hotel Product - US config) HDMI Music Mode - on/off */
						ID_HOTEL_HDMI_MUSIC_MODE = 403,
/* (Hotel Product) PI AES DATA */
						ID_HOTEL_PI_AES_DATA = 404,
/* (Hotel) Channel Setup */						
						ID_HOTEL_CHANNEL_SETUP = 405, 
/* Wifi Region */
						ID_WIFI_REGION = 406,
/* (Hote Productl) this feature will be used where a Jack is there which outputs the current source of TV to another TV on AV with restrictions. will be cloned  and also it is needed in SIRCH PC Applications*/
						ID_HOTEL_MONITOR_OUT_CVBS = 407,
/* To download a key for FKP. (FKP : Field Key Provision) */						
						ID_FKP_DOWN = 408,
/* HDMI Filter count current signal - count when resolution is changed. */
						ID_HDMI_FLT_CNT_CURR_SIG = 409,
/* HDMI Filter count current los - count before scaler unmute.  */
						ID_HDMI_FLT_CNT_CURR_LOS = 410,						
/* Bluetooth Upgrade */				
						ID_BLUETOOTH_UPGRADE = 411,
/* SVC Reset */						
						ID_SERVICE_RESET = 412,
/* (Hotel) Lodgenet Remote Control Option - value? 0: display string will be LRC3101, 1: display string will be LRC3200*/
						ID_LODGENET_REMOTE = 413,
/* HDMI WB Pattern - value? 1: on, 0: off ( (ARC) 0x4F-FAC_WB_PATTERN : 1 Pattern On, 0 Off) */
						ID_HDMI_WB_PATT = 414,
//3Will be DELETED..
						ID_FUNCTION_KEY_LOCK = 415,
/* Add factory option for POP default setting - value? on/off (match with TPCFactory::FACTORY_OPTION_POP_DEFAULT) */
						ID_POP_DEFAULT = 416,
/* (Hotel) Hotel Network Status (match with TPCFactory::FACTORY_CTRL_NETWORK_STATUS) */						
						ID_NETWORK_STATUS = 417,
/* HDMI UNSTABLE BAN CNT - HDMI입력 시, 해상도 Unstable 상태에서 banner 뜨기까지의 시간 */
						ID_HDMI_UNSTABLE_BAN_CNT = 418,
/* (Hotel) Sound Bar Out - value? On/Off (see : AP HDMI CEC Team, match with TPCFactory::FACTORY_OPTION_HOTEL_SOUND_BAR_OUT) */						
						ID_HOTEL_SOUND_BAR_OUT = 419,
/* FUNCTIONKEY USB UPGRADE */
						ID_FUNCTIONKEY_USB_UPGRADE = 420,
/* Bluetooth Audio - value? On/Off (see: AP Bluetooth module, match with TPCFactory::FACTORY_OPTION_BLUETOOTH_AUDIO_ON_OFF) */
						ID_BT_AUDIO_ON_OFF = 421,
/* (LFD) Number of MagicInfo source */
						ID_NUM_OF_MAGIC_INFO = 422,
/* Auto Power - value? AlwaysOn/ AlwaysOff/ LastPower (defined SpecialId for setting to micom when Factory-init(booting) - generally using UniqueID) */
						ID_AUTO_POWER = 423,
/* Key Proximity (defined SpecialId for setting to micom when Factory-init(booting) - generally using UniqueID) */
						ID_KEY_PROXIMITY = 424,
/* (Hospital TV) USB Media Mode - WiseLink excutes when usb device is detected. add SpecialID to SIRCH App */
						ID_USB_MEDIA_MODE = 425,
/* (HotelTV) SW Clock - On/Off (it is needed in SIRCH PC App) */
						ID_HOTEL_SW_CLOCK_ON_OFF = 426,
/* (HotelTV) Genre TV (My Channel En) - On/Off (it is needed in SIRCH PC App) */
						ID_HOTEL_GENRE_TV_ON_OFF = 427,
/* (HotelTV) Sound Bar - On/Off (it is needed in SIRCH PC App) */
						ID_HOTEL_SOUND_BAR_ON_OFF = 428,
/* (HotelTV) MDC ID Setting (it is needed in SIRCH PC App) */
						ID_HOTEL_MDC_ID_SETTING = 429,
/* IPTV CIP - CI+ Loop Back (match with TPCFactory::FACTORY_OPTION_NUM_OF_IPTV_CIP) */						
						ID_NUM_OF_IPTV_CIP = 430,
/* [Operator spec] GetString (TPCFactory::FACTORY_CTRL_OPERATOR_SPEC) */
						ID_OPERATOR_SPEC = 431,
/* (B10 NDF) leeum watchdog count (see : PowerControl::SetPower() - TPCTv::POWER_TYPE_REBOOT) */
						ID_AR_CNT = 432,
/* (HotelTV) ChannelSetup category(user menu) - on/off (match with TPCFactory::FACTORY_OPTION_HOTEL_CHANNEL_SETUP_ONOFF) */						
						ID_HOTEL_CHANNEL_SETUP_ONOFF = 433,
/* Display Model */
						ID_DISPLAY_MODEL = 434,
/* Num of CI (match with TPCFactory::FACTORY_OPTION_CI_SUPPORT) */			
						ID_NUM_OF_CI = 435,
/* Smart Hub Apps reset (match with TPCFactory::FACTORY_CTRL_SMART_HUB_APPS_RESET) */
						ID_SMART_HUB_APPS_RESET = 436,
/* (HotelTV) Anynet + Return Source - It will function as when we disconnect the Anynet then which source it will return to after disconnect */
						ID_HOTEL_ANYNET_P_RETURN_SOURCE = 437,
/* (HotelTV) Self Diagnosis - It will work similar to user menu for Retail TV. It will be moved to Hsopitality Menu and functionality wise*/
						ID_HOTEL_SELF_DIAGNOSIS = 438,
/* (HotelTV) SW Upgarde - It is similar to SW upgarde in user menu and will also be replicated in Hospitality Menu*/
						ID_HOTEL_SW_UPGRADE = 439,
/* (HotelTV) Factory Reset for Hotel - It is similar to Factory Reset in Factory and it will also be added in Hospitality Menu.*/
						ID_HOTEL_FACTORY_RESET = 440,
/* (HotelTV) SIRCH Group ID - It will have values from 0 to 512 and so that can be controlled by Factory.dat*/
						ID_HOTEL_SIRCH_GROUP_ID = 441,
/* (HotelTV) Server URL Setting (TPCFactory::FACTORY_CTRL_HOTEL_SERVER_URL_SETTING)*/
						ID_HOTEL_SERVER_URL_SETTING = 442,
/* (HotelTV) Standby Booting - TV에서 Standby Power Booting 을 지원할 지 말지를 결정하는 항목. ON/OFF (default : OFF) (TPCFactory::FACTORY_OPTION_WIDGET_POWER_ON_SUPPORT)*/
						ID_HOTEL_SUPPORT_WIDGET_POWER_ON = 443,
/* (HotelTV) SAMSUNG CH List - TV 의 Channel List OSD 를 보여줄지 말지를 결정하는 항목. ON/OFF (default : ON) (TPCFactory::FACTORY_OPTION_HOTEL_CH_LIST_OSD_ONOFF)*/
						ID_HOTEL_SUPPORT_SAMSUNG_CH_LIST = 444,
/* MultiACC_N_Rgain - match with [ID_WB_MOVIE_N_RGAIN]	*/
						ID_WB_MOVIE_M_N_RGAIN = 445,
/* MultiACC_N_Bgain - match with [ID_WB_MOVIE_N_BGAIN]	*/
						ID_WB_MOVIE_M_N_BGAIN = 446,
/* MultiACC_N_Roffset - match with [ID_WB_MOVIE_N_ROFFSET]	*/
						ID_WB_MOVIE_M_N_ROFFSET = 447,
/* MultiACC_N_Boffset - match with [ID_WB_MOVIE_N_BOFFSET]	*/
						ID_WB_MOVIE_M_N_BOFFSET = 448,
/* MultiACC_W2_Rgain - match with [ID_WB_MOVIE_W2_RGAIN]	*/
						ID_WB_MOVIE_M_W2_RGAIN = 449,
/* MultiACC_W2_Bgain - match with [ID_WB_MOVIE_W2_BGAIN]	*/
						ID_WB_MOVIE_M_W2_BGAIN = 450,
/* MultiACC_W2_Roffset - match with [ID_WB_MOVIE_W2_ROFFSET]	*/
						ID_WB_MOVIE_M_W2_ROFFSET = 451,
/* MultiACC_W2_Boffset - match with [ID_WB_MOVIE_W2_BOFFSET]	*/
						ID_WB_MOVIE_M_W2_BOFFSET = 452,
/* TX(MOIP Module) Upgrade (TPCFactory::FACTORY_CTRL_MOIP_CAMERA_DEVICE_UPGRADE)*/
						ID_MOIP_CAMERA_DEVICE_UPGRADE = 453,
/* (HotelTV) Solution Type - value ? CMS/SSWA (TPCFactory::FACTORY_OPTION_HOTEL_SOLUTION_TYPE, TPCFactory::EHotelSolutionType) */
						ID_HOTEL_SOLUTION_TYPE = 454,
/* (HotelTV) Off 인 경우에는 Source 키 입력시 Source 브라우저를 실행시켜 주는것이 아니라 바로 다음 소스로 전환 (TPCFactory::FACTORY_OPTION_HOTEL_EXT_SOURCE_BROWSER_ONOFF) */
						ID_HOTEL_EXT_SOURCE_BROWSER_ON_OFF = 455,
/* W/B MOVIE(MultiACC) ON/OFF - match with [ID_WB_MOVIE_ONOFF]	*/
						ID_WB_MOVIE_M_ONOFF = 456,
/* (HotelTv) SubOption item for Sub menu */						
						ID_HOTEL_SUBOPT_POWER_ON = 457,
						ID_HOTEL_SUBOPT_CHANNEL_SERVICE_MENU = 458,
						ID_HOTEL_SUBOPT_MY_CHANNEL = 459,
						ID_HOTEL_SUBOPT_MENU_OSD = 460,
						ID_HOTEL_SUBOPT_OPERATION = 461,
						ID_HOTEL_SUBOPT_CLOCK = 462,
						ID_HOTEL_SUBOPT_MUSIC_MODE = 463,
						ID_HOTEL_SUBOPT_EXT_DEVICE = 464,
						ID_HOTEL_SUBOPT_EXT_SOURCE = 465,
						ID_HOTEL_SUBOPT_BATHROOM_SPEAKER = 466,
						ID_HOTEL_SUBOPT_PILLOW_SPEAKER = 467,
						ID_HOTEL_SUBOPT_ECO_SOLUTION = 468,
						ID_HOTEL_SUBOPT_LOGO_MSG = 469,
						ID_HOTEL_SUBOPT_CLONING = 470,
						ID_HOTEL_SUBOPT_SIRCH = 471,
						ID_HOTEL_SUBOPT_NETWORK = 472,
						ID_HOTEL_SUBOPT_WIDGET = 473,
						ID_HOTEL_SUBOPT_SI_CONFIG = 474,
						ID_HOTEL_SUBOPT_SERVICE = 475,
/* (HotelTv) Mute On CC On/Off - 음소거하면 자동으로 Closed caption 을 ON (TPCFactory::FACTORY_OPTION_HOTEL_MUTE_ON_CC) */
						ID_HOTEL_MUTE_ON_CC = 476,
/* (HotelTv) SIRCH Update Immediate */
						ID_HOTEL_SIRCH_UPDATE_IMMEDIATE = 477,
/* (HotelTv) STORY */
						ID_HOTEL_STORY = 478,
/* (HotelTv) Test Pattern */
						ID_HOTEL_TEST_PATTERN = 479,
/* LUTnet에서 P_Bypass를 설정하기 위해 추가. */
						ID_P_BYPASS = 480,
/* For Dolby Certification (TPCTv::TV_OPTION_TEMP_PRIVATE_RANGE_USE) */
						ID_TEMP_PRIVATE_RANGE_USE = 481,
/* CI 1.3 Support */
						ID_CI_1_3_SUPPORT = 482,
/* (HotelTv) Subtitle auto on/off (TPCFactory::FACTORY_OPTION_HOTEL_SUBTITLE_AUTO_ONOFF) */
						ID_HOTEL_SUBTITLE_AUTO_ONOFF = 483,
/* the number of RUI source (TPCFactory::FACTORY_OPTION_NUM_OF_RUI) */				
						ID_NUM_OF_RUI = 484,
/* SWU Fail Test( 0 ~ 100 ) : 0 Default (TPCFactory::FACTORY_OPTION_SWU_FAIL_TEST ) */
						ID_SWU_FAIL_TEST = 485,
/* SWU Duration ( Off / On ) : Off Default (TPCFactory::FACTORY_OPTION_SWU_DURATION) */
						ID_SWU_DURATION = 486,
/* SWU Reset ( Fail / OK ) (TPCFactory::FACTORY_CTRL_SWU_RESET) */
						ID_SWU_RESET = 487,
/* SWU Diag Code - It shows SWU error code (TPCFactory::FACTORY_CTRL_SWU_DIAG_CODE) */
						ID_SWU_DIAG_CODE = 488,
/* Num of DIIVA */
						ID_NUM_OF_DIIVA = 489,
/* TX(MOIP Module) Upgrade (TPCFactory::FACTORY_CTRL_MOIP_AUDIO_DEVICE_UPGRADE)*/
						ID_MOIP_AUDIO_DEVICE_UPGRADE = 490,
/* SAMEX VOCs (TPCFactory:: FACTORY_CTRL_SVC_VIEW) */
						ID_SVC_VIEW = 491,
/* (HotelTv) ATV Cable AGC Gain - value? 0 / 6 / 7 / 8 / 10 / 11 / 12 / 13 / 14 / 15 */
						ID_HOTEL_ATV_CABLE_AGC_GAIN = 492,
/* (HotelTv) DTV OpenCable AGC Gain - value ? 0 / 6 / 7 / 8 / 10 / 11 / 12 / 13 / 14 / 15 */
						ID_HOTEL_DTV_OPEN_CABLE_AGC_GAIN = 493,
/* (HotelTv) REACH On/Off - This item is for downloadable UI. It is related to STORY */
						ID_HOTEL_REACH_ONOFF = 494,
/* (HotelTv) IPG Room Type - value ? Default/Public/Roomtype3. This is for Guesttek SI and its functinality is that to keep IPG setting according to the selected item in Factory */
						ID_HOTEL_IPG_ROOM_TYPE = 495,
/* (HotelTv) PI AES Log View - which were tuned recently in channel up/down or direct tune and their coresponding PI AES Data will reflect */
						ID_HOTEL_PI_AES_LOG = 496,
/* CAL DAY/NIGHT USB Backup (TPCFactory::FACTORY_CTRL_CAL_DAY_NIGHT_USB_BACKUP) */
						ID_CAL_DAY_NIGHT_USB_BACKUP = 497,
/* CAL DAY/NIGHT USB Restore (TPCFactory::FACTORY_CTRL_CAL_DAY_NIGHT_USB_RESTORE) */
						ID_CAL_DAY_NIGHT_USB_RESTORE = 498,
/* FRC 3D SubMicom USB Upgrade */
						ID_FRC3D_MICOM_USB_UPGRADE = 499,
//3Will be DELETED.. replace to ID_USB_RS232C */
						ID_FANET_ONOFF = 500,
/* Num of Network Stream */
						ID_NUM_OF_NETWORK_STREAM = 501,
/* WB Movie Normal G-Offset */	
						ID_WB_MOVIE_N_GOFFSET = 502,						
/* WB Movie Normal G-Gain */	
						ID_WB_MOVIE_N_GGAIN = 503,
/* (HotelTv) Genre Bank Editor (TPCFactory::FACTORY_CTRL_HOTEL_GENRE_BANK_EDITOR) */
						ID_HOTEL_GENRE_BANK_EDITOR = 504,
/* SVC */
						ID_SVC = 505,
/* Fast USB Booting On/Off - On : USB 활성화 시단 단축 Sequence 적용, OFF : 정상적인 Boot Sequence 적용 */
						ID_FAST_USB_BOOTING_ONOFF = 506,
/* (HotelTv) CAS Support - OFF: Pro:idiom, ON: SCAS */
						ID_HOTEL_CAS_SUPPORT = 507,
/* (LFD) Num of OPS */
						ID_NUM_OF_OPS = 508,
/* (HotelTv) PI AES Log On/Off */
						ID_HOTEL_PI_AES_LOG_ONOFF = 509,
/* CDD Mode - value ? broadcase, test*/
						ID_CDD_MODE = 510,
/* (HotelTv) Certificate Solution - HAL EXECUTE SKIP ITEM (eCtrlType : TD_CTRL_TYPE_EXECUTE_BY_APP) */
						ID_HOTEL_CERTIFICATE_SOLUTION = 511,
/* (HotelTv) TV ID to USB */
						ID_HOTEL_TVID_TO_USB = 512,
/* WB Movie M SUB GAMMA */
						ID_WB_MOVIE_M_SUB_GAMMA = 513,
/* Matching with the UID_TESTPATTERN_WB_PATTERN_FLIP */
						ID_WB_PATTERN_FLIP = 514,
/* Erase the Serial Number */
						ID_ERASE_SERIAL_NUMBER = 515,
/* (HotelTv) Security Mode - HAL EXECUTE SKIP ITEM (eCtrlType : TD_CTRL_TYPE_EXECUTE_BY_APP) */
						ID_HOTEL_SECURITY = 516,
/* RM - Server Type */
						ID_RM_SERVER_TYPE = 517,
/* RM - RTS Mode */
						ID_RTS_MODE = 518,
/* EarlyWarning(Self test) Bluetooth */
						ID_EARLY_WARNING_BT = 519,
/* EarlyWarning(Self test) MOIP */
						ID_EARLY_WARNING_MOIP = 520,
/* EarlyWarning(Self test) Eco Sensor */
						ID_EARLY_WARNING_ECO_SENSOR = 521,
/* EarlyWarning(Self test) Voltage Monitorning */
						ID_EARLY_WARNING_VOLTAGE_MONITORING = 522,
/* Smart Inspection Device Self Test */
						ID_SMART_INSPCETION_DEVICE = 523,
/* Smart Inspection Application Self Test */
						ID_SMART_INSPCETION_APP = 524,
/* Num Of ISP (Internet Service Provider - Network base settop) */
						ID_NUM_OF_ISP = 525,
/* Smart Inspection AV Audio Loopback Test */
						ID_SVC_LOOPBACK_AUDIO_START = 526,
/* Smart Inspection DVI Audio Loopback Test */
						ID_DVI_AUDIO_LOOPBACK = 527,
/* Smart Inspection USB Hub Test */
						ID_USB_HUB_TEST = 528,
/* Smart Inspection CPU Test */
						ID_CPU_TEST = 529,
/* USB Port for Uart Communication On/Off */
						ID_USB_RS232C = 530,
/* WB N ROFFSET */
						ID_WB_N_ROFFSET = 531,
/* WB N BOFFSET */
						ID_WB_N_BOFFSET = 532,
/* WB N RGAIN */
						ID_WB_N_RGAIN = 533,
/* WB N BGAIN */
						ID_WB_N_BGAIN = 534,
/* MGA Pattern Register - Red */
						ID_MGA_R1_GAIN = 535,
						ID_MGA_R2_GAIN = 536,
						ID_MGA_R3_GAIN = 537,
						ID_MGA_R4_GAIN = 538,
						ID_MGA_R5_GAIN = 539,
						ID_MGA_R6_GAIN = 540,
						ID_MGA_R7_GAIN = 541,
						ID_MGA_R8_GAIN = 542,
						ID_MGA_R9_GAIN = 543,
						ID_MGA_R10_GAIN = 544,
/* MGA Pattern Register - Green */						
						ID_MGA_G1_GAIN = 545,
						ID_MGA_G2_GAIN = 546,
						ID_MGA_G3_GAIN = 547,
						ID_MGA_G4_GAIN = 548,
						ID_MGA_G5_GAIN = 549,
						ID_MGA_G6_GAIN = 550,
						ID_MGA_G7_GAIN = 551,
						ID_MGA_G8_GAIN = 552,
						ID_MGA_G9_GAIN = 553,
						ID_MGA_G10_GAIN = 554,
/* MGA Pattern Register - Blue */						
						ID_MGA_B1_GAIN = 555,
						ID_MGA_B2_GAIN = 556,
						ID_MGA_B3_GAIN = 557,
						ID_MGA_B4_GAIN = 558,
						ID_MGA_B5_GAIN = 559,
						ID_MGA_B6_GAIN = 560,
						ID_MGA_B7_GAIN = 561,
						ID_MGA_B8_GAIN = 562,
						ID_MGA_B9_GAIN = 563,
						ID_MGA_B10_GAIN = 564,
/* This is used to get whether channel map is updated by PSI only. value ? On/Off */
						ID_USING_PSI_UPDATE = 565,
/* Voice Recognition debug */
						ID_VOICE_RECOGNITION_DEBUG =  566,
/* Smart Inspection Video AV1 Loopback Test */
						ID_VIDEO_AV1_LOOPBACK = 567,
/* Smart Inspection Video Comp1 Loopback Test */
						ID_VIDEO_COMP1_LOOPBACK = 568,
/* Smart Inspection LAN loopback Test */
						ID_LAN_LOOPBACK = 569,
/* Smart Inspection SPDIF Loopback Test */
						ID_AUDIO_SPDIF_LOOPBACK = 570,
/* Smart Inspection EXT Sound Self Test */
						ID_EXT_SOUND_TEST = 571,
/* Smart Inspection Woofer Sound Self Test */
						ID_WOOFER_SOUND_TEST = 572,
/* Smart Inspection AV2 Audio Loopback Test */
						ID_AV2_AUDIO_LOOPBACK = 573,
/* Smart Inspection Video AV2 Loopback Test */
						ID_VIDEO_AV2_LOOPBACK = 574,
/* Smart Inspection ATV Channel Test */
						ID_CHANNEL_ATV_TEST = 575,
/* Smart Inspection DTV Channel Test */
						ID_CHANNEL_DTV_TEST = 576,
/* Smart Inspection Satellite Channel Test */
						ID_CHANNEL_SATELLITE_TEST = 577,
/* Smart Inspection USB Loopback Test */
						ID_USB_LOOPBACK = 578,
/* Smart Inspection FOX-DP CRC Check */
						ID_DP_CRC_TEST = 579,			
/* Smart Inspection HDMI Loopback Test */
						ID_HDMI_LOOPBACK = 580,						
/* (HotelTv) Door Eye */
						ID_HOTEL_SUBOPT_DOOR_EYE = 581,						
						ID_HOTEL_DOOR_EYE_MODE = 582,
						ID_HOTEL_DOOR_EYE_SOURCE = 583,
						ID_HOTEL_DOOR_EYE_PROMPT_OSD = 584,
						ID_HOTEL_DOOR_EYE_SOURCE_RET_TIME = 585,
/* (HotelTv) HD Self Test */						
						ID_HOTEL_HD_SELF_TEST = 586,

/* the number of RUIS source (TPCFactory::FACTORY_OPTION_NUM_OF_RUI_CLIENTS) */
						ID_NUM_OF_RUI_CLIENTS = 587,
/* Update MAXQ610 chip for uart communicate with IR Blaster */
						ID_UART_IR_BLASTER_UPDATE = 588,
/* Smart Inspection SCART Audio Loopback Test */
						ID_LOOPBACK_SCART_AUDIO = 589,
/* Smart Inspection SCART CVBS Loopback Test */
						ID_LOOPBACK_SCART_CVBS = 590,
/* Smart Inspection SCART RGB Loopback Test */
						ID_LOOPBACK_SCART_RGB = 591,
/* (HotelTv) Ticker on/off */
						ID_HOTEL_TICKER_ONOFF = 592,
/* MGA - Parent item for MGA menu */
						ID_MGA = 593,
/* MGA On/Off */
						ID_MGA_ON_OFF = 594,
/* (HotelTv) REACH Volume - On(default)/Off */
						ID_HOTEL_REACH_VOLUME_ONOFF = 595,
/* (HotelTv) Timer Type - Wakeup type(default)/OnOff type */
						ID_HOTEL_CLOCK_TIMER_TYPE = 596,
/* End Of Speech - on/off */
						ID_EOS_CLICK_ON_OFF = 597,
/* Smart Inspection Video Comp1 Loopback Test2 */
						ID_VIDEO_COMP2_LOOPBACK = 598,
/* Enhanced EPG Enable - on/off */
						ID_EEPG_ENABLE = 599,
/* CEShow Mode - on/off */
						ID_CES_MODE = 600,
/* (LFD) Smart Inspection - Loopback */ 
						ID_SVC_LOOPBACK_LAN = 601,
						ID_SVC_LOOPBACK_DP_INOUT = 602,
						ID_SVC_LOOPBACK_PIM = 603,
						ID_SVC_LOOPBACK_RS232OUT = 604,
						ID_SVC_LOOPBACK_IR_INOUT = 605,
						ID_SVC_LOOPBACK_EXT_SENSOR = 606,
/* (LFD) Smart Inspection - Monitoring */
						ID_SVC_MONITORING_PIM_POWER = 607,
						ID_SVC_MONITORING_DP_LOOPOUT = 608,
/* (LFD) Smart Inspection - Self Test */
						ID_SVC_PHY_IC = 609,
						ID_SVC_SOUND_AMP = 610,
						ID_SVC_INTERNAL_VIDEO_PATH = 611,
						ID_SVC_LVDS = 612,
						ID_SVC_TCON_FRC = 613,
						ID_SVC_EDID_IC = 614,
						ID_SVC_FAN = 615,
						ID_SVC_FUNCTION = 616,
						ID_SVC_RTC = 617,
						ID_SVC_IO_EXPANDER = 618,
						ID_SVC_UART_CONVERTER = 619,
						ID_SVC_EEPROM = 620,
/* (LFD) PIM Interface */
						ID_SVC_LOOPBACK_PIM_TMDS = 621,
						ID_SVC_LOOPBACK_PIM_AUDIO_LR = 622,
						ID_SVC_LOOPBACK_PIM_DDC_EEPROM = 623,
						ID_SVC_LOOPBACK_PIM_UART = 624,
						ID_SVC_LOOPBACK_PIM_HPD_PS_ONOFF = 625,
						ID_SVC_LOOPBACK_PIM_DETECT = 626,
						ID_SVC_LOOPBACK_PIM_PWR_STATUS = 627,
/* (LFD) MDC WhiteBalance Data */
						ID_WB_APP_MOVIE_N_RGAIN = 628,						
						ID_WB_APP_MOVIE_N_GGAIN = 629, 
						ID_WB_APP_MOVIE_N_BGAIN = 630,
						ID_WB_APP_MOVIE_N_ROFFSET = 631,
						ID_WB_APP_MOVIE_N_GOFFSET = 632,
						ID_WB_APP_MOVIE_N_BOFFSET = 633,
/* (HotelTv) PC Audio Test */		
						ID_SVC_PC_AUDIO_TEST = 634,
/* (HotelTv) PC Self Test */
						ID_SVC_PC_LOOPBACK = 635,
/* (HotelTv) Self Diagnosis */
						ID_HOTEL_SELF_DIAGNOSTIC_PI = 636,			
						ID_HOTEL_SELF_DIAGNOSTIC_RTC = 637,
						ID_HOTEL_SELF_DIAGNOSTIC_SUBAMP = 638,
						ID_HOTEL_SELF_DIAGNOSTIC_SI_VENDOR = 639,
/* Check WIFI Module */						
						ID_WIFI_MODULE_TEST = 640,
/* (HotelTv) Security Mode */						
						ID_HOTEL_SUBOPT_SECURITY_MODE = 641,
/* (HotelTv) Sub menu of Security Mode - Password Input is required to use below Menu */
						ID_HOTEL_SECURITY_MODE_PWD_INPUT = 642,
/* (HotelTv) Sub menu of Security Mode - Pop-up screen : New Password / Confirm Password */
						ID_HOTEL_SECURITY_MODE_PWD_SETTING = 643,
/* (HotelTv) Sub menu of Security Mode - Restore Password to 00000000 */
						ID_HOTEL_SECURITY_MODE_PWD_RESET = 644, 
/* (HotelTv) Sub menu of Security Mode - Below Menu Items are disabled */
						ID_HOTEL_SECURITY_MODE_ONOFF = 645,
/* (HotelTv) Sub menu of Security Mode - USB Source is not shown from Source List */
						ID_HOTEL_SECURITY_MODE_USB_ENABLE = 646,
/* (HotelTv) Sub menu of Security Mode - HDMI Source is not shown from Source List */
						ID_HOTEL_SECURITY_MODE_HDMI_ENABLE = 647,
/* (HotelTv) Sub menu of Security Mode - When TTX button is pressed, None of TTX function appeared */
						ID_HOTEL_SECURITY_MODE_TTX_ENABLE = 648,
/* (HotelTv) Room number */
						ID_HOTEL_ROOM_NUM_SETTING = 649,
/* Source Direct On/Off */						
						ID_SOURCE_DIRECT_ONOFF = 650,
/* Color Mapping, Picture Mode : All, Color space : Auto */
						ID_WB_RED_R = 651,
						ID_WB_RED_G = 652,
						ID_WB_RED_B = 653,
						ID_WB_GREEN_R = 654,
						ID_WB_GREEN_G = 655,
						ID_WB_GREEN_B = 656,
						ID_WB_BLUE_R = 657,
						ID_WB_BLUE_G = 658,
						ID_WB_BLUE_B = 659,
						ID_WB_YELLOW_R = 660,
						ID_WB_YELLOW_G = 661,
						ID_WB_YELLOW_B = 662,
						ID_WB_CYAN_R = 663,
						ID_WB_CYAN_G = 664,
						ID_WB_CYAN_B = 665,
						ID_WB_MAGENTA_R = 666,
						ID_WB_MAGENTA_G = 667,
						ID_WB_MAGENTA_B = 668,
/* (UHD) MAIN USB path */
						ID_MAIN_USB_PATH = 669,
/* (UHD) JackP USB path */
						ID_JACKP_USB_PATH = 670,
/* (UHD) Upgrade UHD OSD Test */
						ID_UPGRADE_UHD_OSD_TEST = 671,						
/* FRC2 Pre Test Pattern */
						ID_NAPOLI2_PRE_PATTSEL = 672,
/* FRC2 POST Test Pattern */
						ID_NAPOLI2_POST_PATTSEL = 673,
/* (NTV) Relax Sharpness */
						ID_WB_RELAX_SHARPNESS = 674,
/* Color Mapping, Picture Mode : Dynamic/Natural/Relax/Movie/Cal-Day/Cal-Night, Color space : Native */
						ID_WB_N_RED_R = 675,
						ID_WB_N_RED_G = 676,
						ID_WB_N_RED_B = 677,
						ID_WB_N_GREEN_R = 678,
						ID_WB_N_GREEN_G = 679,
						ID_WB_N_GREEN_B = 680,
						ID_WB_N_BLUE_R = 681,
						ID_WB_N_BLUE_G = 682,
						ID_WB_N_BLUE_B = 683,
						ID_WB_N_YELLOW_R = 684,
						ID_WB_N_YELLOW_G = 685,
						ID_WB_N_YELLOW_B = 686,
						ID_WB_N_CYAN_R = 687,
						ID_WB_N_CYAN_G = 688,
						ID_WB_N_CYAN_B = 689,
						ID_WB_N_MAGENTA_R = 690,
						ID_WB_N_MAGENTA_G = 691,
						ID_WB_N_MAGENTA_B = 692,
/* MIC Program update (firmware) */
						ID_MIC_PROGRAM_UPDATE = 693,
/* Smart hub apps update */
						ID_SMART_HUB_APPS_UPDATE = 694,
/* Last screen (Suspended branch) */
						ID_LAST_SCREEN = 695,
/* Log analyzer print On/ Off */
						ID_LOG_ANALYZER = 696,
/* (LFD) Launcher Power on Support */
						ID_LFD_LAUNCHER_POWER_ON_SUPPORT = 697,						
/* (LFD) Magic Info lite/S */
						ID_LFD_MAGIC_INFO_LITE_S = 698,
/* Serial log on/off */
						ID_SERIAL_LOG = 699,	
/* BP PMS module reset */
						ID_BP_PMS_RESET = 700,	


/* Following lists exist in menu "SVC - Restriction". */						
/* (SVC - Restriction) EER Reset */
						ID_SVC_EER_RESET = 701,
/* (SVC - Restriction) Factory Reset */
						ID_SVC_FACTORY_RESET = 702,
/* (SVC - Restriction) Numer of DTV */
						ID_SVC_NUM_OF_DTV = 703,
/* (SVC - Restriction) Numer of HDMI */
						ID_SVC_NUM_OF_HDMI = 704,
/* (SVC - Restriction) Numer of COMP */
						ID_SVC_NUM_OF_COMP = 705,						
/* (SVC - Restriction) Numer of AV */
						ID_SVC_NUM_OF_AV = 706,
/* (SVC - Restriction) Num of USB Port*/						
						ID_SVC_NUM_OF_USBPORT = 707,						
/* (SVC - Restriction) (Hotel) When turn on Tv, this concludes booting source */
						ID_SVC_HOTEL_POWER_ON_SOURCE = 708,		
//////////////////////////////////////////////////////////////

/* Color Mapping 3D, Color space : Auto*/
						ID_COLORMAPPING_3D_A_RED_R = 709,
						ID_COLORMAPPING_3D_A_RED_G = 710,
						ID_COLORMAPPING_3D_A_RED_B = 711,
						ID_COLORMAPPING_3D_A_GREEN_R = 712,
						ID_COLORMAPPING_3D_A_GREEN_G = 713,
						ID_COLORMAPPING_3D_A_GREEN_B = 714,
						ID_COLORMAPPING_3D_A_BLUE_R = 715,
						ID_COLORMAPPING_3D_A_BLUE_G = 716,
						ID_COLORMAPPING_3D_A_BLUE_B = 717,
						ID_COLORMAPPING_3D_A_YELLOW_R = 718,
						ID_COLORMAPPING_3D_A_YELLOW_G = 719,
						ID_COLORMAPPING_3D_A_YELLOW_B = 720,
						ID_COLORMAPPING_3D_A_CYAN_R = 721,
						ID_COLORMAPPING_3D_A_CYAN_G = 722,
						ID_COLORMAPPING_3D_A_CYAN_B = 723,
						ID_COLORMAPPING_3D_A_MAGENTA_R = 724,
						ID_COLORMAPPING_3D_A_MAGENTA_G = 725,
						ID_COLORMAPPING_3D_A_MAGENTA_B = 726,
/* Color Mapping, Picture Mode : Standard/BD-Wise/BD Wise Web, Color space : Native */
						ID_WB_S_RED_R = 727,
						ID_WB_S_RED_G = 728,
						ID_WB_S_RED_B = 729,
						ID_WB_S_GREEN_R = 730,
						ID_WB_S_GREEN_G = 731,
						ID_WB_S_GREEN_B = 732,
						ID_WB_S_BLUE_R = 733,
						ID_WB_S_BLUE_G = 734,
						ID_WB_S_BLUE_B = 735,
						ID_WB_S_YELLOW_R = 736,
						ID_WB_S_YELLOW_G = 737,
						ID_WB_S_YELLOW_B = 738,
						ID_WB_S_CYAN_R = 739,
						ID_WB_S_CYAN_G = 740,
						ID_WB_S_CYAN_B = 741,
						ID_WB_S_MAGENTA_R = 742,
						ID_WB_S_MAGENTA_G = 743,
						ID_WB_S_MAGENTA_B = 744,
/* Error count which is related to FRC3D (panel) */
						ID_FRC3D_ER_COUNT = 745,
/* (Hotel) Home menu display */
						ID_HOTEL_HOME_MENU_DISPLAY = 746,						
/* (Hotel) Home menu editor */
						ID_HOTEL_HOME_MENU_EDITOR = 747,
/* (Hotel) Home menu auto start */
						ID_HOTEL_HOME_MENU_AUTO_START = 748,
/* (Hotel) Applite on off */
						ID_HOTEL_APPLITE_ON_OFF = 749,
/* (Hotel) Applite room number setting */
						ID_HOTEL_APPLITE_ROOM_NUM_SETTING = 750,
/* The number of FAnet thread */
						ID_NUM_OF_FANET_THREAD = 751,						
/* (Hotel) Time channel type */
						ID_HOTEL_TIME_CHANNEL_TYPE = 752, 
/* (Hotel) time channel number */
						ID_HOTEL_TIME_CHANNEL_NUM = 753,
/* (Hotel) DRM support on off */
						ID_HOTEL_SUBOPT_DRM = 754,
/* (LFD) POWER ON VOLUME ENABLE - Enable/Disable */
						ID_LFD_POWER_ON_VOLUME_EN = 755,
/* (LFD) HPower On Volume */
						ID_LFD_POWER_ON_VOLUME = 756,
/* (LFD) MDC Broadcast Ack on off */
						ID_LFD_MDC_BROADCAST_ACK = 757,	
/* User fastboot default value */
						ID_USER_FASTBOOT_DEFAULT_VALUE = 758,
/* (LFD) Internal/External Same Volume */
						ID_LFD_INT_EXT_SAME_VOLUME = 759,						
/* SVC -Info (sub menu) */
						ID_SVC_INFO = 760,
/* for preventing to change localset when TV is imported */
						ID_PREVENTING_IMPORT = 761,
/* Flash Test */
						ID_SVC_FLASH_START = 762,
/* DDR Test */
						ID_SVC_DDR_START = 763,
/* Power failure error count */
						ID_POWER_FAIL_ERROR_COUNT = 764,
/* Aging line test */
						ID_AGING_LINE_TEST = 765,
/* Ginga NCL support on off */
						ID_GINGA_NCL_SUPPORT = 766,						
/* (LFD) WB Movie picture data */
						ID_WB_MOVIE_N_CONTRAST = 767,
/* (UHD9000) FAN support on off */
						ID_SUPPORT_FAN = 768,							
/* (LFD) WB Movie picture data */						
						ID_WB_MOVIE_W2_CONTRAST = 769,
						ID_WB_MOVIE_W2_GGAIN = 770,
						ID_WB_MOVIE_C_CONTRAST = 771,
						ID_WB_MOVIE_C_RGAIN = 772,
						ID_WB_MOVIE_C_GGAIN = 773,
						ID_WB_MOVIE_C_BGAIN = 774,
/* (Self test) Twitter sound inspection */						
						ID_TWITTER_SOUND_TEST = 775,		
/* (LFD) Lamp Luminunce : Backlight control */						
						ID_LAMP_LUMINUNCE = 776,
/* ADK Firmware update of BT Module */						
						ID_SC_ADK_UPGRADE = 777,
/* MBR Firmware update of BT Module */						
						ID_SC_MBR_UPGRADE = 778,
/* (LFD) Console support on off */
						ID_LFD_CONSOLE_SUPPORT = 779,
/* (LFD) Console port number */
						ID_LFD_CONSOLE_PORT_NUMBER = 780,
/* Certification mode on off */
						ID_CERTIFICATION_ON_OFF = 781,
/* (LFD) Color temperature */
						ID_COLOR_TEMPERATURE = 782,
/* The number of Hybrid source */
						ID_NUM_OF_HYBRID_TV = 783,						
/* RM log transmission on off */
						ID_RM_LOG_TRANSMISSION = 784,
/* Uart enable on off */
						ID_UART_ENABLE = 785,
/* JAVA Data service support */
						ID_SUPPORT_DATA_SERVICE_JAVA = 786,	
/* AP Module Error count */
						ID_WIFI_NO_DETECTION_COUNT = 787,
						ID_WIFI_DETACHMENT_COUNT = 788,
						ID_BT_NO_DETECTION_COUNT = 789,
						ID_BT_DETACHMENT_COUNT = 790,
						ID_BT_MGT_OPEN_FAIL_COUNT = 791,
						ID_BT_MGT_DISCONNECT_COUNT = 792,
/* (LFD) SD Card test */						
						ID_SD_CARD_TEST = 793,
/* Fan error count */						
						ID_FAN_ERROR_COUNT = 794,
/* ACM Menu On Off */						
						ID_ACM_MENU = 795,
/* Factory entry count */						
						ID_FACTORY_ENTRY_COUNT = 796,	
/* To display factory history that was executed by customer until now. */						
						ID_FACTORY_EXECUTION_HISTORY = 797,
/* To display factory reset info that was executed by customer until now. */						
						ID_FACTORY_RESET_HISTORY = 798,
/* Mini browser support. */						
						ID_SUPPORT_MINIBROWSER = 799,	
/* IR blaster update delay time */						
						ID_IRB_DELAY_TIME = 800,
/* (Hotel) To execute Contact Samsung menu */						
						ID_HOTEL_CONTACT_SAMSUNG = 801,
/* (Hotel) User fastboot default value */					
						ID_HOTEL_USER_FASTBOOT_DEFAULT_VALUE = 802,						
/* (Jackpack) Data sync status */						
						ID_DATA_SYNC_STATUS = 803,	
/* In case of FRC 3D error, it concludes whether TV is rebooted or not */						
						ID_FRC3D_REBOOTING_ONOFF = 804,	
/* (Jackpack) Do migration Scenario 1 */						
						ID_DATA_SYNC_INIT = 805,
/* (Jackpack) Serdes error count */						
						ID_SERDES_ERROR_COUNT = 806,	
/* (Jackpack) Serdes reset count */												
						ID_SERDES_RESET_COUNT = 807,
/* (Jackpack) Serdes Watchdog on off */												
						ID_SERDES_WATCHDOG_ON_OFF = 808,						
/* (Hotel) TTX mode */												
						ID_HOTEL_TTX_MODE = 809,
/* LVDS self test */												
						ID_SVC_LVDS_TEST = 810,
/* (UHD9 Frameless) PC Dimming */																
						ID_PC_DIMMING_SUPPORT = 811,
/* For checking channel missing issue */
						ID_UNIQUE_TRIPLET = 812,	


/* Part 1 */
						ID_MRT_OPTION,
						ID_MRT_ENGINEERING_OPTION,

/* Option - MRT Option */
						ID_MRT_LEDMOTIONPLUS_SUPPORT,
						ID_MRT_NATURAL_MODE_SUPPORT,
						ID_MRT_RELAX_MODE_SUPPORT,
						ID_MRT_HV_FLIP,
						ID_MRT_CAMERA_SUPPORT,
						ID_MRT_NETWORK_SUPPORT,
						ID_MRT_ECOSENSOR_SUPPORT,
						ID_MRT_3D_SUPPORT,
						ID_MRT_BT_SUPPORT,
						ID_MRT_BT_ADDRESS,
						ID_MRT_HP_LINE,
						ID_MRT_RESOLUTION_SEL,
						ID_MRT_MULTIVIEW_SUPPORT,
						ID_MRT_LOCAL_DIMMING_PANEL,
						ID_MRT_WIFI_VENDOR,
						ID_MRT_VOICE_RECOG,
						ID_MRT_MOTION_RECOG,
						ID_TTV_SUPPORT_ON_OFF,

/* Option - Engineer Option */

						ID_ENGINEER_5_WAY_FUNCTION_KEY,
						ID_ENGINEER_CONTENTS_BAR,
						ID_ENGINEER_STANDBY_LED_ON_OFF,
						ID_ENGINEER_RECOGNITION_SUPPORT,
						ID_ENGINEER_IF_AGC,
						ID_ENGINEER_D_AGC,
						ID_ENGINEER_PH_BW,
						ID_ENGINEER_FQ_BW,
						ID_ENGINEER_PH_RATE,
						ID_ENGINEER_PD_EN,
						ID_ENGINEER_PEQ_INX,
						ID_ENGINEER_WF_SCALE,
						ID_ENGINEER_WF_TYPE,
						ID_ENGINEER_DP_V_SIZE,
						ID_ENGINEER_BACKEND_DEVICE,
						ID_ENGINEER_CONFIG_AV_PATH,						
						ID_ENGINEER_V_HDMI_IDENT_TYPE,
						ID_ENGINEER_V_HDMI_PATH_TYPE,
						ID_ENGINEER_V_EDID_TYPE,
						ID_ENGINEER_V_ATV,
						ID_ENGINEER_V_AV1,
						ID_ENGINEER_V_AV2,
						ID_ENGINEER_V_COMP1,
						ID_ENGINEER_V_COMP2,
						ID_ENGINEER_V_PC,
						ID_ENGINEER_V_SCART1_CVBS,
						ID_ENGINEER_V_SCART1_RGB,
						ID_ENGINEER_V_SCART2_CVBS,
						ID_ENGINEER_V_SCART2_RGB,
						ID_ENGINEER_A_ATV,
						ID_ENGINEER_A_DTV,
						ID_ENGINEER_A_AV1,
						ID_ENGINEER_A_AV2,
						ID_ENGINEER_A_COMP1,
						ID_ENGINEER_A_COMP2,
						ID_ENGINEER_A_PC,
						ID_ENGINEER_A_SCART1,
						ID_ENGINEER_A_SCART2,
						ID_ENGINEER_A_DVI,
						ID_ENGINEER_A_HDMI,
						ID_ENGINEER_A_MEDIA,
						ID_ENGINEER_PANEL_INIT_TIME,
						ID_ENGINEER_TCON_INIT_TIME,
						ID_ENGINEER_WRITE_MAC_ADDRESS,

/* Control - Suboption */
						ID_SUB_FRC_MONITORING,
						ID_SUB_ECO_IC_TYPE,
						ID_SUB_LVDS_OUTPUT,
						ID_SUB_DPMS_SUPPORT,
						ID_SUB_T_CON_DEVICE,
						ID_SUB_BOARD_CONTROL,
						ID_SUB_THRESHOLD_LV1,
						ID_SUB_LMK_THRESHOLD = ID_SUB_THRESHOLD_LV1,
						ID_SUB_THRESHOLD_LV2,
						ID_SUB_LOW_THRESHOLD = ID_SUB_THRESHOLD_LV2,
						ID_SUB_HIGH_THRESHOLD,
						ID_SUB_THRESHOLD_LV3 = ID_SUB_HIGH_THRESHOLD,
						ID_SUB_CLB,
						ID_SUB_APP_RESUME,
						ID_SUB_HOTKEY_LIST,

/* Control - Asia Option */
						ID_ASIA_AF_LEVEL_ADJUST,
						ID_ASIA_TX_POWER_LEVEL,
						ID_ASIA_H_SHAKING,

/* Control - Shop Option */
						ID_SHOP_3D_EMIT_ON,
						ID_SHOP_3D_EMITSHOWMODE,
						ID_SHOP_3D_GLASS_PULSE_S,
						ID_SHOP_3D_GLASS_PULSE_H,
						ID_SHOP_3D_CUBE,
						ID_SHOP_PEAKMODE,

/* Control - Sound Option */
						ID_SOUND_FM_M_PRESCALE,
						ID_SOUND_FM_PRESCALE,
						ID_SOUND_AM_PRESCALE,
						ID_SOUND_NICAM_PRESCALE,
						ID_SOUND_PILOT_LEVEL_HIGH_THLD,
						ID_SOUND_PILOT_LEVEL_LOW_THLD,
						ID_SOUND_CHATTERING_CNT,
						ID_SOUND_CARRIER2_AMP_HIGH_THLD,
						ID_SOUND_CARRIER2_AMP_LOW_THLD,
						ID_SOUND_CARRIER2_SNR_HIGH_THR,
						ID_SOUND_CARRIER2_SNR_LOW_THR,
						ID_SOUND_A2_PILOT_SENSITIVITY,
						ID_SOUND_AMP_VOLUME,
						ID_SOUND_AMP_SCALE,
						ID_SOUND_AMP_CHECK_SUM,
						ID_SOUND_AMP_LOCAL_CHECK_SUM,
						ID_SOUND_WOOFER_TYPE,
						ID_SOUND_WOOFER_VOLUME,
						ID_SOUND_WOOFER_SCALE,
						ID_SOUND_WOOFER_CHECK_SUM,
						ID_SOUND_WOOFER_LOCAL_CHECKSUM,
						ID_SOUND_LOCAL_SPEAKER_EQ,						
						ID_SOUND_PEQ_TEST,
						ID_SOUND_AMP_MODEL,
						ID_SOUND_BOTTOM_CHECK_SUM,
						ID_SOUND_BOTTOM_LOCAL_CHECK_SUM,
						ID_SOUND_WALL_FILTER_TYPE,
						ID_SOUND_SRS_TUNING_PARM,
						ID_SOUND_SPDIF_PCM_GAIN,
						ID_SOUND_AUDIODOCK_BT_DELAY,
						ID_SOUND_3D_GLASS_BT_DELAY,
						ID_SOUND_SPEAKER_CUT_OFF_FREQ,
						ID_SOUND_AUDIO_IP_TEST,
						ID_SOUND_TRUBASS_CHECKSUM,
						ID_SOUND_MIC_SCALE,
						ID_SOUND_INDIA_SOUND,
						ID_SOUND_SPEAKER_DELAY_NORMAL,
						ID_SOUND_CU_DELAY,
						ID_SOUND_LIPSYNC_INX,
						ID_SOUND_LIPSYNC_CHECKSUM,
						ID_SOUND_LIPSYNC_USB_TEST,
						ID_SOUND_LIPSYNC_BT_CHECKSUM,
						ID_SOUND_TP_VOLUME,
						ID_SOUND_TP_SCALE,
						ID_SOUND_TP_EQ_CHECKSUM,

/* DEBUG - Spread Spectrum */
						ID_SPREAD_SPECTRUM_LVDS_SPREAD,
						ID_SPREAD_SPECTRUM_DDR_SPREAD,
						ID_SPREAD_SPECTRUM_LVDS_PERIOD,
						ID_SPREAD_SPECTRUM_AMPLITUDE,
						ID_SPREAD_SPECTRUM_HD_SSC_ON_OFF,
						ID_SPREAD_SPECTRUM_HD_SSC_VALUE,
						ID_SPREAD_SPECTRUM_LVDS_SSC_VALUE,
						ID_SPREAD_SPECTRUM_DUMMY,
						ID_SPREAD_SPECTRUM_US_DDR_SSC_ON_OFF,
						ID_SPREAD_SPECTRUM_US_DDR_VALUE,
						ID_SPREAD_SPECTRUM_US_LVDS_ON_OFF,
						ID_SPREAD_SPECTRUM_US_LVDS_VALUE,
						ID_SPREAD_SPECTRUM_MP_DDR_SSC_ON_OFF,
						ID_SPREAD_SPECTRUM_MP_DDR_VALUE,
						ID_SPREAD_SPECTRUM_AP_DDR_SSC_ON_OFF,
						ID_SPREAD_SPECTRUM_AP_DDR_VALUE,
						ID_SPREAD_SPECTRUM_FRC_VX1_SSC_ON_OFF,
						ID_SPREAD_SPECTRUM_FRC_VX1_SSC_PERIOD,
						ID_SPREAD_SPECTRUM_FRC_VX1_SSC_MODULATION,
						ID_SPREAD_SPECTRUM_FRC_LVDS_SSC_ON_OFF,
						ID_SPREAD_SPECTRUM_FRC_LVDS_SSC_MFR,
						ID_SPREAD_SPECTRUM_FRC_LVDS_SSC_MRR,
						ID_SPREAD_SPECTRUM_FRC_DDR_SSC_ON_OFF,
						ID_SPREAD_SPECTRUM_FRC_DDR_SSC_MFR,
						ID_SPREAD_SPECTRUM_FRC_DDR_SSC_MRR,
						ID_SPREAD_SPECTRUM_FRC_DDR_SSC_PERIOD,
						ID_SPREAD_SPECTRUM_FRC_DDR_SSC_MODULATION,
						ID_SPREAD_SPECTRUM_DDR_SSC_ON_OFF,
						ID_SPREAD_SPECTRUM_DDR_SSC_VALUE,
						ID_SPREAD_SPECTRUM_MP1_LVDS_SSC_ON_OFF,
						ID_SPREAD_SPECTRUM_MP1_LVDS_SSC_VALUE,
						ID_SPREAD_SPECTRUM_MP2_LVDS_SSC_ON_OFF,
						ID_SPREAD_SPECTRUM_MP2_LVDS_SSC_VALUE,
						ID_SPREAD_SPECTRUM_ADV7619_DATA_STRENGTH,
						ID_SPREAD_SPECTRUM_ADV7619_CLOCK_STRENGTH,
						ID_SPREAD_SPECTRUM_ADV7619_H_V_DE_STRENGTH,
/* DEBUG - Spread Spectrum */
						ID_DDR_MARGIN_A_CTRL_OFFSET_0_3,
						ID_DDR_MARGIN_A_CTRL_OFFSET_D,
						ID_DDR_MARGIN_B_CTRL_OFFSET_0_3,
						ID_DDR_MARGIN_B_CTRL_OFFSET_D,

/* DEBUG */
						ID_DEBUG_BT_ON_OFF,
						ID_DEBUG_TUNER_MARGIN,
						ID_DEBUG_MPEG_MARGIN,
						ID_DEBUG_H_264_MARGIN,
						ID_DEBUG_CAM_WAIT_TIME,
						ID_DEBUG_TCON_TEMP_READ,
						ID_DEBUG_TEMP_LAST,
						ID_DEBUG_DCC_VERSION,
						ID_DEBUG_DCC_CHK_SEL,
						ID_DEBUG_DCC_CHECK_LOCAL,
						ID_DEBUG_DCC_CHECK_TOTAL,
						ID_DEBUG_MULTIACC_CHECKSUM,
						ID_DEBUG_IIC_BUS_STOP,
						ID_DEBUG_VOICE_DEBUG,
						ID_DEBUG_CERT_OPTION,
						ID_DEBUG_HHP_OPTION=ID_DEBUG_CERT_OPTION,
						ID_DEBUG_RM_BIST_DTV,
						ID_DEBUG_RM_BIST_ATV,
						ID_DEBUG_RM_BIST_CABLE,
						ID_DEBUG_STRESS_MODE,
						ID_DEBUG_LOG_ANALYZER,

/* DEBUG - FRC */
						ID_FRC_FDISPLAY_ON_OFF,
						ID_FRC_3D_FDISPLAY_ON_OFF,
						ID_FRC_PC_MODE_ON_OFF,
						ID_FRC_HOME_PANEL_FRC,
						ID_FRC_DDR_TEST,

/* DEBUG - Power Management */
						ID_POWER_MANAGEMENT_AVS,
						ID_POWER_MANAGEMENT_DVFS,					

						ID_ND_ADJ_SUPPORT,
						ID_MICOM_POWER_OFF,
						ID_RM_BIST_DTV,
						ID_RM_BIST_ATV,

/* SVC - Self Test */
						ID_SVC_EEPROM_START,
						ID_SVC_X_TAL_START,
						ID_SVC_TUNER_START,
						ID_SVC_HDMI_SWITCH_IC_START,
						ID_SVC_LVDS_START,
						ID_SVC_T_CON_FRC_START,
						ID_SVC_T_CON_FRC2_START,						
						ID_SVC_PCBTEST_START,
						ID_SVC_TP_SOUND_INSPECTION,
						ID_SVC_TP_DP_CRC_RESULT,
						ID_SVC_VOLTAGE_RESULT,

/* SVC - Info */
						ID_SVC_INFO_WIFI_ER_COUNT,
						ID_SVC_INFO_BT_ER_COUNT,
						ID_SVC_INFO_SUDDEN_POWER_OFF_COUNT,

/* SVC - Upgrade */
						ID_UPGRADE_TCON_CHECKSUM,
						ID_UPGRADE_TCON2_DOWNLOAD,
						ID_UPGRADE_TCON2_CHECKSUM,
						ID_UPGRADE_PANEL_EEPROM_UPDATE,
						ID_UPGRADE_PANEL_FLASH_UPDATE,
						ID_UPGRADE_BT_FREEPAIRING,
						ID_SVC_FREEPAIRING=ID_UPGRADE_BT_FREEPAIRING,
						ID_UPGRADE_FRC3D_LD_UPGRADE,
						ID_UPGRADE_FRC2_3D_FW_UPGRADE,
						ID_UPGRADE_CPLD_USB_DOWNLOAD,
						ID_UPGRADE_CPLD_DOWNLOAD,
						ID_UPGRADE_JUMP_UPGRADE,
						ID_UPGRADE_IR_BLASTER_DELAY_TIME,
						ID_UPGRADE_LD_EEPROM_UPDATE,
						ID_UPGRADE_UD_LDC_PROFILE_UPGRADE,
						ID_UPGRADE_PIC_DATA_USB_UPDATE,
						ID_UPGRADE_AUDIO_DATA_USB_UPDATE,
						ID_UPGRADE_ECO_DATA_USB_UPDATE,

/* SVC - OPTION_HDMI */
						ID_OPTION_HDMI_HOTPLUG,
						ID_OPTION_HDMI_HOTPLUG_SWITCHING,
						ID_OPTION_HDMI_HOT_PLUG_DURATION,
						ID_OPTION_HDMI_CLK_TERM_DURATION,
						ID_OPTION_HDMI_ROBIN,
						ID_OPTION_HDMI_CTS_THLD,
						ID_OPTION_HDMI_CTS_CNT1,
						ID_OPTION_HDMI_EQ,
						ID_OPTION_HDMI_WRITE_TYPE,
						ID_OPTION_HDMI_SWITCH,
						ID_OPTION_HDMI_DVI_SET_TIME,
						ID_OPTION_HDMI_H_WRITE,
						ID_OPTION_HDMI_SYNC,
						ID_OPTION_HDMI_MUTE_TIME,
						ID_OPTION_HDMI_REPEA_AUDIO_PKT,
						ID_OPTION_HDMI_HDCP_EN,
						ID_OPTION_HDMI_HDCP_EN_FLAG,
						ID_OPTION_HDMI_POWER_ON_FLT_CNT_LOS,
						ID_OPTION_HDMI_UPDATE_SPI,
						ID_HDCP_SPI_VERSION,

/* SVC - Test Pattern */
						ID_TEST_PATTERN_FRC2_PRE_TEST_PATTERN,
						ID_TEST_PATTERN_FRC2_POST_TEST_PATTERN,
						ID_TEST_PATTERN_FRC_OSD_PRE_TEST_PATTERN,
						ID_TEST_PATTERN_FRC_OSD_POST_TEST_PATTERN,
						ID_TEST_PATTERN_SOC_TCON_TEST_PATTERN,
						ID_TEST_PATTERN_SOC_TCON_PATTERN_LEVEL,
						ID_TEST_PATTERN_SOC_TCON_FRC_PATTERN,
						ID_TEST_PATTERN_SOC_TCON2_TEST_PATTERN,
						ID_TEST_PATTERN_SOC_TCON2_PATTERN_LEVEL,
						ID_TEST_PATTERN_SOC_TCON2_FRC_PATTERN,						
						ID_TEST_PATTERN_US_PRE_TEST_PATTERN,
						ID_TEST_PATTERN_US_POST_TEST_PATTERN,


/* SVC - Other Setting */
						ID_OTHER_MICOM_POWER_OFF,
						ID_OTHER_ATV_IF_AGC_SPEED,

/* SVC - Restriction */
						ID_RESTRICTION_CAMERA_SUPPORT,
						ID_RESTRICTION_NETWORK_SUPPORT,
						ID_RESTRICTION_BT_SUPPORT,
						ID_RESTRICTION_RECOGNITION_SUPPORT,
						ID_RESTRICTION_SOURCE_SUPPORT,

/* Part2 */
/* WBMovie */
						ID_WBMOVIE_DYNAMIC_GAMMA,
						ID_WBMOVIE_STANDARD_GAMMA,
						ID_WBMOVIE_N_SUB_GAMMA,
						ID_WBMOVIE_S_SUB_GAMMA,
/* Picture 2D */
						ID_PICTURE_2D_WCE,
						ID_PICTURE_2D_COLORMAPPING,
						ID_PICTURE_2D_NR,
						ID_PICTURE_2D_YC_DELAY,
						ID_PICTURE_2D_PC_CONTRAST,

/* Picture 2D - SubSetting*/
						ID_SUBSETTING_PWM_MAX,
						ID_SUBSETTING_PWM_MAX_2,
						ID_SUBSETTING_PWM_INI_2D,
						ID_SUBSETTING_PWM_INI_3D,
						ID_SUBSETTING_PWM_MIN,
						ID_SUBSETTING_PWM_MID,						
						ID_SUBSETTING_DITHER_BYPASS,
						ID_SUBSETTING_LCM_MAX,
						ID_SUBSETTING_3D_POWER_LIMIT_MODE,
						ID_SUBSETTING_2D_DIMPOSI1_60,
						ID_SUBSETTING_2D_DIMPOSI2_60,
						ID_SUBSETTING_2D_DIMPOSI3_60,
						ID_SUBSETTING_2D_DIMPOSI4_60,
						ID_SUBSETTING_2D_DIMPOSI1_50,
						ID_SUBSETTING_2D_DIMPOSI2_50,
						ID_SUBSETTING_2D_DIMPOSI3_50,
						ID_SUBSETTING_2D_DIMPOSI4_50,
						ID_SUBSETTING_2D_LCMPOSI1_60,
						ID_SUBSETTING_2D_LCMPOSI2_60,
						ID_SUBSETTING_2D_LCMPOSI3_60,
						ID_SUBSETTING_2D_LCMPOSI4_60,
						ID_SUBSETTING_2D_LCMPOSI1_50,
						ID_SUBSETTING_2D_LCMPOSI2_50,
						ID_SUBSETTING_2D_LCMPOSI3_50,
						ID_SUBSETTING_2D_LCMPOSI4_50,
						ID_SUBSETTING_2D_LCMPOSI1_24,
						ID_SUBSETTING_2D_LCMPOSI2_24,
						ID_SUBSETTING_2D_LCMPOSI3_24,
						ID_SUBSETTING_2D_LCMPOSI4_24,

/* Picture 2D - Enhance*/						
						ID_ENHANCE_BM_START,
						ID_ENHANCE_BM_START_MAX,
						ID_ENHANCE_H_LFUNC_GAIN,
						ID_ENHANCE_H_HFUNC_GAIN,
						ID_ENHANCE_H_LCE_GAIN,
						ID_ENHANCE_M_LFUNC_GAIN,
						ID_ENHANCE_M_HFUNC_GAIN,
						ID_ENHANCE_M_LCE_GAIN,
						ID_ENHANCE_L_LFUNC_GAIN,
						ID_ENHANCE_L_HFUNC_GAIN,
						ID_ENHANCE_L_LCE_GAIN,
						ID_ENHANCE_OFF_LFUNC_GAIN,
						ID_ENHANCE_OFF_HFUNC_GAIN,
						ID_ENHANCE_OFF_LCE_GAIN,
						ID_ENHANCE_D_SKIN_UV,
						ID_ENHANCE_D_SUB_COLOR,
						ID_ENHANCE_S_SKIN_UV,
						ID_ENHANCE_S_SUB_COLOR,
						ID_ENHANCE_M_SKIN_UV,
						ID_ENHANCE_M_SUB_COLOR,
						ID_ENHANCE_N_SKIN_UV,
						ID_ENHANCE_D_BASE_COLOR,
						ID_ENHANCE_M_BASE_COLOR,
						ID_ENHANCE_D_SUB_TINT,
						ID_ENHANCE_S_SUB_TINT,
						ID_ENHANCE_M_SUB_TINT,
						ID_ENHANCE_TEMP_DEST,
						
/* Picture 2D - WCE*/
						ID_WCE_MCC2_U,
						ID_WCE_MCC2_V,
						ID_WCE_MCC3_U,
						ID_WCE_MCC3_V,
						ID_WCE_MCC4_U,
						ID_WCE_MCC4_V,
						ID_WCE_MCC5_U,
						ID_WCE_MCC5_V,
						ID_WCE_PPHC_G,
						ID_WCE_PPHC_C,
						ID_WCE_PPHC_M,
						ID_WCE_PPHC_Y,
						ID_WCE_PPSC_GAIN2_R,
						ID_WCE_PPSC_GAIN2_G,
						ID_WCE_PPSC_GAIN2_B,
						ID_WCE_PPSC_GAIN2_C,
						ID_WCE_PPSC_GAIN2_M,
						ID_WCE_PPSC_GAIN2_Y,

/* Picture 2D - COLORMAPPING*/
						ID_COLORMAPPING_A_RED_R,
						ID_COLORMAPPING_A_RED_G,
						ID_COLORMAPPING_A_RED_B,
						ID_COLORMAPPING_A_GREEN_R,
						ID_COLORMAPPING_A_GREEN_G,
						ID_COLORMAPPING_A_GREEN_B,
						ID_COLORMAPPING_A_BLUE_R,
						ID_COLORMAPPING_A_BLUE_G,
						ID_COLORMAPPING_A_BLUE_B,
						ID_COLORMAPPING_A_YELLOW_R,
						ID_COLORMAPPING_A_YELLOW_G,
						ID_COLORMAPPING_A_YELLOW_B,
						ID_COLORMAPPING_A_CYAN_R,
						ID_COLORMAPPING_A_CYAN_G,
						ID_COLORMAPPING_A_CYAN_B,
						ID_COLORMAPPING_A_MAGENTA_R,
						ID_COLORMAPPING_A_MAGENTA_G,
						ID_COLORMAPPING_A_MAGENTA_B,
						ID_COLORMAPPING_N_RED_R,
						ID_COLORMAPPING_N_RED_G,
						ID_COLORMAPPING_N_RED_B,
						ID_COLORMAPPING_N_GREEN_R,
						ID_COLORMAPPING_N_GREEN_G,
						ID_COLORMAPPING_N_GREEN_B,
						ID_COLORMAPPING_N_BLUE_R,
						ID_COLORMAPPING_N_BLUE_G,
						ID_COLORMAPPING_N_BLUE_B,
						ID_COLORMAPPING_N_YELLOW_R,
						ID_COLORMAPPING_N_YELLOW_G,
						ID_COLORMAPPING_N_YELLOW_B,
						ID_COLORMAPPING_N_CYAN_R,
						ID_COLORMAPPING_N_CYAN_G,
						ID_COLORMAPPING_N_CYAN_B,
						ID_COLORMAPPING_N_MAGENTA_R,
						ID_COLORMAPPING_N_MAGENTA_G,
						ID_COLORMAPPING_N_MAGENTA_B,
						ID_COLORMAPPING_S_RED_R,
						ID_COLORMAPPING_S_RED_G,
						ID_COLORMAPPING_S_RED_B,
						ID_COLORMAPPING_S_GREEN_R,
						ID_COLORMAPPING_S_GREEN_G,
						ID_COLORMAPPING_S_GREEN_B,
						ID_COLORMAPPING_S_BLUE_R,
						ID_COLORMAPPING_S_BLUE_G,
						ID_COLORMAPPING_S_BLUE_B,
						ID_COLORMAPPING_S_YELLOW_R,
						ID_COLORMAPPING_S_YELLOW_G,
						ID_COLORMAPPING_S_YELLOW_B,
						ID_COLORMAPPING_S_CYAN_R,
						ID_COLORMAPPING_S_CYAN_G,
						ID_COLORMAPPING_S_CYAN_B,
						ID_COLORMAPPING_S_MAGENTA_R,
						ID_COLORMAPPING_S_MAGENTA_G,
						ID_COLORMAPPING_S_MAGENTA_B,
/* Picture 2D - NR */
						ID_NR_DYN_OFF_NR_EN,
						ID_NR_MOV_OFF_NR_EN,
/* Picture 2D - VDEC */
						ID_VDEC_PHASE,
						ID_VDEC_SOGBW,
						ID_VDEC_SSCPC,
						ID_VDEC_RGB_DLY,
						ID_VDEC_VS_SLICE_LEVEL,
						ID_VDEC_HS_SLICE_LEVEL,
						ID_VDEC_RGB_DELAY_ADJ,
						ID_VDEC_RGB_LPF,


/* Picture 2D - YCDELAY */
						ID_YCDELAY_PAL_BG,
						ID_YCDELAY_PAL_DK,
						ID_YCDELAY_PAL_I,
						ID_YCDELAY_PAL_M,
						ID_YCDELAY_PAL_N,
						ID_YCDELAY_SECAM_BG,
						ID_YCDELAY_SECAM_DK,
						ID_YCDELAY_SECAM_L,
						ID_YCDELAY_NTSC_358,
						ID_YCDELAY_NTSC_443,
						ID_YCDELAY_AV_SCART_PAL,
						ID_YCDELAY_AV_SCART_PAL_M,
						ID_YCDELAY_AV_SCART_PAL_N,
						ID_YCDELAY_AV_SCART_SECAM,
						ID_YCDELAY_AV_SCART_NT_358,
						ID_YCDELAY_AV_SCART_NT_443,
						ID_YCDELAY_AV_SCART_PAL_60,
						ID_YCDELAY_SCART_RGB_PAL,
						ID_YCDELAY_SCART_RGB_PAL_M,
						ID_YCDELAY_SCART_RGB_PAL_N,
						ID_YCDELAY_SCART_RGB_SECAM,
						ID_YCDELAY_SCART_RGB_NT_358,
						ID_YCDELAY_SCART_RGB_NT_443,
						ID_YCDELAY_SCART_RGB_PAL_60,

/* Picture 2D - SHARPNESS */

						ID_SHARPNESS_PRE_GAINH1,
						ID_SHARPNESS_PRE_GAINH2,
						ID_SHARPNESS_PRE_GAINH3,
						ID_SHARPNESS_PRE_GAINV1,
						ID_SHARPNESS_PRE_GAINV2,
						ID_SHARPNESS_PRE_GAINV3,
						ID_SHARPNESS_POST_GAINH1,
						ID_SHARPNESS_POST_GAINH2,
						ID_SHARPNESS_POST_GAINH3,
						ID_SHARPNESS_POST_GAINV1,
						ID_SHARPNESS_POST_GAINV2,
						ID_SHARPNESS_POST_GAINV3,
						ID_SHARPNESS_RGBPK_GAIN,
						ID_SHARPNESS_D_PRE_TOT,
						ID_SHARPNESS_D_POST_TOT,
						ID_SHARPNESS_S_PRE_TOT,
						ID_SHARPNESS_S_POST_TOT,
						ID_SHARPNESS_US_POST_GAINH1,
						ID_SHARPNESS_US_POST_GAINH2,
						ID_SHARPNESS_US_POST_GAINH3,
						ID_SHARPNESS_US_POST_GAINV1,
						ID_SHARPNESS_US_POST_GAINV2,
						ID_SHARPNESS_US_POST_GAINV3,
						ID_SHARPNESS_US_POST_TOT,
						ID_SHARPNESS_NT_GAINH1,
						ID_SHARPNESS_NT_GAINH2,
						ID_SHARPNESS_NT_GAINH3,
						ID_SHARPNESS_NT_GAINV1,
						ID_SHARPNESS_NT_GAINV2,
						ID_SHARPNESS_NT_GAINV3,

/* Picture 2D - LNA PLUS */

						ID_LNAPLUS_SYNCTIP_NOISE,
						ID_LNAPLUS_DB0,
						ID_LNAPLUS_DB1,
						ID_LNAPLUS_DB2,
						ID_LNAPLUS_DB3,
						ID_LNAPLUS_DB4,
						ID_LNAPLUS_DB5,
						ID_LNAPLUS_DB6,
						ID_LNAPLUS_DB7,
						ID_LNA_PLUS_YFILTER,

/* Picture 2D - PC Contrast */
						ID_PC_CONTRAST_COOL,
						ID_PC_CONTRAST_NORMAL,
						ID_PC_CONTRAST_WARM1,
						ID_PC_CONTRAST_WARM2,

/* EPA */
						ID_EPA_UNDER_DRIVE,
						ID_EPA_HBES,
						ID_EPA_MOTIONLIGHTINGUP,
						ID_EPA_MOTIONLIGHTINGDOWN,

/* adding ID */
						ID_TEST_PATTERN_HDMI_PATTERN_SEL,
						ID_HDCP_UPDATE_SPI,
						ID_SVC_INFO_DEBUG_LOG_DOWN,
						ID_SVC_INFO_CAMERA_ER_COUNT,
						ID_ENGINEER_FAST_LOGO_DELAY,
						ID_MRT_CI_SUPPORT,
						ID_ADC_WB,
						ID_SUBSETTING,
						ID_SVC_PANEL,
						ID_SVC_DVB_CI,
						ID_DVB_CI_TS_CLOCK_DELAY_TC,
						ID_DVB_CI_TS_CLOCK_DELAY_S,
						ID_TIZEN_EXPORT,
						ID_TIZEN_IMPORT,
						ID_TIZEN_VERIFY,
						ID_TIZEN_AUTO_IMPORT,

						ID_SUB_OPTION,
						ID_EEPROM_RESET,
						ID_OPTION_SWU,
						ID_OPTION_NUM,
						ID_RM,
						
						ID_DUTY_DELAY,
						ID_DUTY_PDUTY240_BON,
						ID_DUTY_PDUTY240_BOFF,
						ID_DUTY_STARTPOSI_BON,
						ID_DUTY_DIMPOSI2_BON,
						ID_DUTY_DIMPOSI3_BON,
						ID_DUTY_DIMPOSI4_BON,
						ID_DUTY_STARTPOSI_BOFF,
						ID_DUTY_DIMPOSI2_BOFF,
						ID_DUTY_DIMPOSI3_BOFF,
						ID_DUTY_DIMPOSI4_BOFF,

						ID_WBMOVIE_3D_DYNAMIC_GAMMA,
						ID_WBMOVIE_3D_STANDARD_GAMMA,
						ID_WBMOVIE_3D_N_SUB_GAMMA,
						ID_WBMOVIE_3D_S_SUB_GAMMA,
					
						ID_EPA_3D_3D_STANDARD_BACKLGHT,
						ID_EPA_3D_3D_STANDARD_CONTRAST,
						ID_EPA_3D_3D_STANDARD_BRIGHTNESS,
						ID_EPA_3D_3D_STANDARD_SHARPNESS,
						ID_EPA_3D_3D_STANDARD_COLOR,
						ID_EPA_3D_3D_STANDARD_TINT,
						
						ID_SHARPNESS_3D_PRE_GAINH1,
						ID_SHARPNESS_3D_PRE_GAINH2,
						ID_SHARPNESS_3D_PRE_GAINH3,
						ID_SHARPNESS_3D_PRE_GAINV1,
						ID_SHARPNESS_3D_PRE_GAINV2,
						ID_SHARPNESS_3D_PRE_GAINV3,
						ID_SHARPNESS_3D_POST_GAINH1,
						ID_SHARPNESS_3D_POST_GAINH2,
						ID_SHARPNESS_3D_POST_GAINH3,
						ID_SHARPNESS_3D_POST_GAINV1,
						ID_SHARPNESS_3D_POST_GAINV2,
						ID_SHARPNESS_3D_POST_GAINV3,
						ID_SHARPNESS_3D_D_PRE_TOT,
						ID_SHARPNESS_3D_D_POST_TOT,
						ID_SHARPNESS_3D_S_PRE_TOT,
						ID_SHARPNESS_3D_S_POST_TOT,
						ID_SHARPNESS_3D_US_POST_GAINH1,
						ID_SHARPNESS_3D_US_POST_GAINH2,
						ID_SHARPNESS_3D_US_POST_GAINH3,
						ID_SHARPNESS_3D_US_POST_GAINV1,
						ID_SHARPNESS_3D_US_POST_GAINV2,
						ID_SHARPNESS_3D_US_POST_GAINV3,
						ID_SHARPNESS_3D_US_POST_TOT,
						ID_SHARPNESS_3D_NT_GAINH1,
						ID_SHARPNESS_3D_NT_GAINH2,
						ID_SHARPNESS_3D_NT_GAINH3,
						ID_SHARPNESS_3D_NT_GAINV1,
						ID_SHARPNESS_3D_NT_GAINV2,
						ID_SHARPNESS_3D_NT_GAINV3,
						
						ID_BT_IR_BTPAIRDIS_HO,
						ID_BT_IR_BTPAIRDIS_SH,
						ID_BT_IR_BTTRANSDIS,
						ID_BT_IR_BTTXDEL50_BON,
						ID_BT_IR_BTTXDEL50_BOFF,
						ID_BT_IR_BTTXDEL60_BON,
						ID_BT_IR_BTTXDEL60_BOFF,
						ID_BT_IR_BTGLSDUTY,
						ID_BT_IR_SLAVDELAY48,
						ID_BT_IR_SLAVDELAY60,
						ID_BT_IR_SLAVDELAY48_BON,
						ID_BT_IR_SLAVDELAY60_BON,
						
						ID_PDP_BT_IR_BTPAIRDIS_HO,
						ID_PDP_BT_IR_BTPAIRDIS_SH,
						ID_PDP_BT_IR_BTTRANSDIS,
						ID_PDP_BT_IR_BTEMIDEL_48_D,
						ID_PDP_BT_IR_BTEMIDEL_50_D,
						ID_PDP_BT_IR_BTEMIDEL_60_D,
						ID_PDP_BT_IR_BTGLSDUTY_D,
						ID_PDP_BT_IR_BTEMIDEL_48_S,
						ID_PDP_BT_IR_BTEMIDEL_50_S,
						ID_PDP_BT_IR_BTEMIDEL_60_S,
						ID_PDP_BT_IR_BTGLSDUTY_S,
						ID_PDP_BT_IR_BTEMIDEL_48_R,
						ID_PDP_BT_IR_BTEMIDEL_50_R,
						ID_PDP_BT_IR_BTEMIDEL_60_R,
						ID_PDP_BT_IR_BTGLSDUTY_R,
						ID_PDP_BT_IR_BTEMIDEL_48_M,
						ID_PDP_BT_IR_BTEMIDEL_50_M,
						ID_PDP_BT_IR_BTEMIDEL_60_M,
						ID_PDP_BT_IR_BTGLSDUTY_M,
						ID_PDP_BT_IR_IREMIMASK,
						ID_PDP_BT_IR_IRMASKPRD,
						ID_PDP_BT_IR_IREMINUM,
						ID_PDP_BT_IR_SLAVDELAY48,
						ID_PDP_BT_IR_SLAVDELAY50,
						ID_PDP_BT_IR_SLAVDELAY60,
						
						ID_GLIMIT_LBT0,
						ID_GLIMIT_LBT1,
						ID_GLIMIT_LBT2,
						ID_GLIMIT_LBT3,
						ID_GLIMIT_LBT4,
						ID_GLIMIT_LBT5,
						ID_GLIMIT_LBT6,
						ID_GLIMIT_LBT7,
						ID_GLIMIT_LBT8,
						ID_GLIMIT_LBT9,
						ID_GLIMIT_LBT10,
						ID_GLIMIT_LBT11,
						ID_GLIMIT_LBT12,
						ID_GLIMIT_LBT13,
						ID_GLIMIT_LBT14,
						ID_GLIMIT_LBT15,
						ID_GLIMIT_LBT16,
						ID_GLIMIT_LBT17,
						ID_GLIMIT_LBT18,
						ID_GLIMIT_LBT19,
						ID_GLIMIT_LBT20,
						
						ID_LOCALBLOCK_DCC_X1,
						ID_LOCALBLOCK_DCC_X2,
						ID_LOCALBLOCK_DCC_X3,
						ID_LOCALBLOCK_DCC_X4,
						ID_LOCALBLOCK_DCC_X5,
						ID_LOCALBLOCK_DCC_X6,
						ID_LOCALBLOCK_DCC_X7,
						ID_LOCALBLOCK_DCC_Y1,
						ID_LOCALBLOCK_DCC_Y2,
						ID_LOCALBLOCK_DCC_Y3,
						ID_LOCALBLOCK_DCC_Y4,
						ID_LOCALBLOCK_DCC_Y5,
						ID_LOCALBLOCK_DCC_H1,
						ID_LOCALBLOCK_DCC_H2,
						ID_LOCALBLOCK_DCC_H3,
						ID_LOCALBLOCK_DCC_H4,
						ID_LOCALBLOCK_DCC_H5,
						ID_LOCALBLOCK_DCC_H6,
						ID_LOCALBLOCK_DCC_H7,
						ID_LOCALBLOCK_DCC_V1,
						ID_LOCALBLOCK_DCC_V2,
						ID_LOCALBLOCK_DCC_V3,
						ID_LOCALBLOCK_DCC_V4,
						ID_LOCALBLOCK_DCC_V5,
						
						ID_DCC_BOOSTON_TEMP_DELTA0_BON,
						ID_DCC_BOOSTON_TEMP_DELTA1_BON,
						ID_DCC_BOOSTON_TEMP_DELTA2_BON,
						ID_DCC_BOOSTON_TEMP_DELTA3_BON,
						ID_DCC_BOOSTON_TEMP_DELTA4_BON,
						ID_DCC_BOOSTON_TEMP_DELTA5_BON,
						ID_DCC_BOOSTON_TEMP_DELTA6_BON,
						ID_DCC_BOOSTON_TEMP_DELTA7_BON,
						ID_DCC_BOOSTON_DCC_SEL0_BON,
						ID_DCC_BOOSTON_DCC_SEL1_BON,
						ID_DCC_BOOSTON_DCC_SEL2_BON,
						ID_DCC_BOOSTON_DCC_SEL3_BON,
						ID_DCC_BOOSTON_DCC_SEL4_BON,
						ID_DCC_BOOSTON_DCC_SEL5_BON,
						ID_DCC_BOOSTON_DCC_SEL6_BON,
						ID_DCC_BOOSTON_DCC_SEL7_BON,
						ID_DCC_BOOSTON_DCC_SEL8_BON,
						ID_DCC_BOOSTON_DCC_SEL9_BON,
						ID_DCC_BOOSTON_DCC_SEL10_BON,
						ID_DCC_BOOSTON_DCC_SEL11_BON,
						ID_DCC_BOOSTON_DCC_SEL12_BON,
						ID_DCC_BOOSTON_DCC_SEL13_BON,
						ID_DCC_BOOSTON_DCC_SEL14_BON,
						ID_DCC_BOOSTON_DCC_SEL15_BON,
						ID_DCC_BOOSTON_DCC_SEL16_BON,
						ID_DCC_BOOSTON_DCC_SEL17_BON,
						ID_DCC_BOOSTON_DCC_SEL18_BON,
						ID_DCC_BOOSTON_DCC_SEL19_BON,
						ID_DCC_BOOSTON_DCC_SEL20_BON,
						ID_DCC_BOOSTON_DCC_SEL21_BON,
						ID_DCC_BOOSTON_DCC_SEL22_BON,
						ID_DCC_BOOSTON_DCC_SEL23_BON,
						ID_DCC_BOOSTON_DCC_SEL24_BON,
						ID_DCC_BOOSTON_DCC_SEL25_BON,
						ID_DCC_BOOSTON_DCC_SEL26_BON,
						ID_DCC_BOOSTON_DCC_SEL27_BON,
						ID_DCC_BOOSTON_DCC_SEL28_BON,
						ID_DCC_BOOSTON_DCC_SEL29_BON,
						ID_DCC_BOOSTON_DCC_SEL30_BON,
						ID_DCC_BOOSTON_DCC_SEL31_BON,
						ID_DCC_BOOSTON_DCC_SEL32_BON,
						ID_DCC_BOOSTON_DCC_SEL33_BON,
						ID_DCC_BOOSTON_DCC_SEL34_BON,
						ID_DCC_BOOSTON_DCC_SEL35_BON,
						ID_DCC_BOOSTON_DCC_SEL36_BON,
						ID_DCC_BOOSTON_DCC_SEL37_BON,
						ID_DCC_BOOSTON_DCC_SEL38_BON,
						ID_DCC_BOOSTON_DCC_SEL39_BON,
						ID_DCC_BOOSTON_DCC_SEL40_BON,
						ID_DCC_BOOSTON_DCC_SEL41_BON,
						ID_DCC_BOOSTON_DCC_SEL42_BON,
						ID_DCC_BOOSTON_DCC_SEL43_BON,
						ID_DCC_BOOSTON_DCC_SEL44_BON,
						ID_DCC_BOOSTON_DCC_SEL45_BON,
						ID_DCC_BOOSTON_DCC_SEL46_BON,
						ID_DCC_BOOSTON_DCC_SEL47_BON,
						ID_DCC_BOOSTON_DEL_STEP_200_BON,
						ID_DCC_BOOSTON_TIME_COLD_BON,
						
						ID_DCC_BOOSTOFF_TEMP_DELTA0_BOFF,
						ID_DCC_BOOSTOFF_TEMP_DELTA1_BOFF,
						ID_DCC_BOOSTOFF_TEMP_DELTA2_BOFF,
						ID_DCC_BOOSTOFF_TEMP_DELTA3_BOFF,
						ID_DCC_BOOSTOFF_TEMP_DELTA4_BOFF,
						ID_DCC_BOOSTOFF_TEMP_DELTA5_BOFF,
						ID_DCC_BOOSTOFF_TEMP_DELTA6_BOFF,
						ID_DCC_BOOSTOFF_TEMP_DELTA7_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL0_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL1_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL2_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL3_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL4_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL5_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL6_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL7_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL8_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL9_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL10_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL11_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL12_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL13_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL14_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL15_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL16_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL17_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL18_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL19_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL20_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL21_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL22_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL23_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL24_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL25_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL26_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL27_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL28_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL29_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL30_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL31_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL32_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL33_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL34_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL35_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL36_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL37_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL38_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL39_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL40_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL41_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL42_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL43_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL44_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL45_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL46_BOFF,
						ID_DCC_BOOSTOFF_DCC_SEL47_BOFF,
						ID_DCC_BOOSTOFF_DEL_STEP_200_BOFF,
						ID_DCC_BOOSTOFF_TIME_COLD_BOFF,
						
						ID_BCTR_R_CTR_DYN_ON,
						ID_BCTR_R_CTR_MOV_ON,
						ID_BCTR_R_IIR_OPTION,
						ID_BCTR_R_IIR_VEL,
						ID_BCTR_R_SLPFAC,
						ID_BCTR_R_IIR_LTH,
						ID_BCTR_R_IIR_LCOEF,
						ID_BCTR_R_IIR_HCOEF,
						ID_BCTR_R_UMAX_DYN_VAL,
						ID_BCTR_R_UMIN_DYN_VAL,
						ID_BCTR_R_UMIN_MOV_VAL,
						ID_BCTR_R_MAX_DYN_DROP,
						ID_BCTR_R_MIN_DYN_DROP,
						ID_BCTR_CTR_ON_GLIMIT,
						ID_BCTR_R_MAXDOWN_TH1,
						ID_BCTR_R_MAXDOWN_TH2,
						ID_BCTR_R_UMAXH_TH,
						ID_BCTR_R_UMINH_TH,
						ID_BCTR_R_HISTO_DYN_BIN,
						ID_BCTR_UMAX_MOV_VAL,
						ID_BCTR_UMAXH_MOV_TH,
						ID_BCTR_UMINH_MOV_TH,
						ID_BCTR_R_MAX_MOV_DROP,
						ID_BCTR_R_MIN_MOV_DROP,
						ID_BCTR_R_HISTO_MOV_BIN,
						
						ID_DEBUG_DEBUG_ON_OFF,
						ID_DEBUG_DEBUG_USB,
						ID_DEBUG_DCCSELE0_0,
						ID_DEBUG_DCCSELE0_1,
						ID_DEBUG_DCCSELE0_2,
						ID_DEBUG_DCCSELE0_3,
						ID_DEBUG_DCCSELE0_4,
						ID_DEBUG_DCCSELE0_5,
						ID_DEBUG_DCCSELE0_6,
						ID_DEBUG_DCCSELE0_7,
						
						ID_BYPASS_3DP,
						ID_BYPASS_2DP,
					
						IS_SPECIAL_ID_LAST,
						ID_FACTORY_OPTION_DOWNLOADABLE_RRT,
						ID_FACTORY_OPTION_VCHIP_ONOFF,
						ID_FACTORY_OPTION_YAHOO_SUPPORT,
						ID_FACTORY_OPTION_AUTO_DEGAUSSING,
						ID_FACTORY_OPTION_COLOR_GAMUT_WIDE,
						ID_FACTORY_OPTION_XPOS_OF_DDP,
						ID_FACTORY_OPTION_YPOS_OF_DDP,
						ID_FACTORY_OPTION_WIDTH_OF_DDP,
						ID_FACTORY_OPTION_HEIGHT_OF_DDP,
						ID_FACTORY_OPTION_SIZE_4_3_CRT,
						ID_FACTORY_OPTION_MAKER_ID,
						ID_FACTORY_OPTION_MODEL_ID,
						ID_FACTORY_OPTION_DVI_I_SUPPORT,
						ID_FACTORY_OPTION_MELFAS_FUNCTION_SUPPORT,
						ID_FACTORY_OPTION_COLOR_SYSTEM_VALUE,
						ID_FACTORY_OPTION_SOUND_SYSTEM_VALUE,
						ID_FACTORY_OPTION_EXTERNAL_AUDIO_SUPPORT,
						ID_FACTORY_OPTION_OPERATOR_SPEC_SUPPORT_FREEVIEW_HD,
						ID_FACTORY_OPTION_OPERATOR_SPEC_SUPPORT_MHEG_DVR,
						ID_FACTORY_OPTION_OPERATOR_SPEC_SUPPORT_NUMERICABLE,
						ID_FACTORY_OPTION_OPERATOR_SPEC_SUPPORT_UPC,
						ID_FACTORY_OPTION_SYSTEM_WATCHDOG_COUNT,
						ID_FACTORY_OPTION_SMART_HUB_BLOCK,
						IS_SPECIAL_IDEOPTION_LAST,
						/*Version String ID*/
						ID_VERSION_FULL,
						ID_VERSION_SWVERSION,
						ID_VERSION_MICOM,
						ID_VERSION_SDAL,
						ID_VERSION_DATE,
						ID_VERSION_SWENGINE,
						ID_VERSION_HOTEL_SI_VENDOR,
						ID_VERSION_MODEL_NAME,
						ID_VERSION_HOTEL_VCP,
						ID_VERSION_SERIAL_NUMBER,
						ID_VERSION_CAMERA,
						ID_VERSION_MIC,
						ID_VERSION_IR_BLASTER_MICOM,
						ID_VERSION_MODEL,
						ID_VERSION_PANEL,
						IS_SPECIAL_IDVERSION_LAST,	
}FACTORY_SPECIAL_ID;
#endif
