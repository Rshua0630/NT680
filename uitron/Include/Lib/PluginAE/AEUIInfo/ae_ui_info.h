#ifndef _AE_UI_INFOR_H_
#define _AE_UI_INFOR_H_

#include "Type.h"
#include "ipl_utility.h"

#define AE_UI_INFOR_ERR   0xFFFFFFFF

typedef struct _AE_UI_INFO_TAB {
	UINT32 value;
	UINT32 max_value;
} AE_UI_INFO_TAB;

/**
    AE informatin selection index(ae_set_ui_info() or ae_get_ui_info())
*/
typedef enum _AE_UI_INFO {
	AE_UI_SCENE,			        ///<Scene mode, data type #AE_SCENE		
	AE_UI_FLASH,              	    ///<flash mode, data type #AE_FLASH
	AE_UI_FREQUENCY,              	///<AE anti-frequency, data type #AE_FREQUENCY
	AE_UI_METER,                 	///<AE detect mode, data type #AE_METER
	AE_UI_EV,                   	///<AE ev compensation, data type #AE_EV
	AE_UI_ISO,               	    ///<AE iso index, data type #AE_ISO
	AE_UI_ANTISHAKE,              	///<Anti shake AE on/off, data type #AE_ANTISHAKE	
	AE_UI_CAP_LONGEXP,              ///<Capture long exposure on/off, data type #AE_CAP_LONGEXP
	AE_UI_CAP_LONGEXP_EXPT,         ///<Capture exposure time for long exposure mode
	AE_UI_CAP_LONGEXP_ISO,          ///<Capture ISO for long exposure mode	
	AE_UI_CAP_MFHDR_EV,             ///<EV gap for multi frame HDR capture function
	AE_UI_MAX_CNT,
	ENUM_DUMMY4WORD(AE_UI_INFO)
} AE_UI_INFO;

/**
    AE scene mode
*/
typedef enum _AE_SCENE {
    AE_SCENE_AUTO,             
    AE_SCENE_LANDSCAPE,            
    AE_SCENE_POTRAIT,              
    AE_SCENE_SPORT,                
    AE_SCENE_NIGHTSCENE,           
    AE_SCENE_AV,                   
    AE_SCENE_TV,                   
    AE_SCENE_MANUAL,               
    AE_SCENE_NIGHTPOTRAIT,         
    AE_SCENE_NIGHTLANDSCAPE,       
    AE_SCENE_PARTY,                
    AE_SCENE_SUNSET,               
    AE_SCENE_BACKLIGHT,            
    AE_SCENE_DOCUMENT,             
    AE_SCENE_FIREWORK,             
    AE_SCENE_BEACH,                
    AE_SCENE_BMODE,                
    AE_SCENE_PROGRAM,              
    AE_SCENE_HIGHISO,              
    AE_SCENE_MAX_CNT,
    ENUM_DUMMY4WORD(AE_SCENE)
} AE_SCENE;

/**
    AE flash mode
*/
typedef enum _AE_FLASH {
    AE_FLASH_AUTO,             ///< Auto
    AE_FLASH_FORCEOFF,         ///< Force off
    AE_FLASH_FORCEON,          ///< Force on
    AE_FLASH_AUTOREDEYE,       ///< Auto + red eye
    AE_FLASH_FORCEONREDEYE,    ///< Force on + red eye
    AE_FLASH_NIGHTSTROBE,      ///< Night strobe
    AE_FLASH_MAX_CNT,
    ENUM_DUMMY4WORD(AE_FLASH)
} AE_FLASH;

/**
    AE frequency
*/
typedef enum _AE_FREQUENCY {
    AE_FREQUENCY_50HZ,              ///< Anti-freq 50Hz
    AE_FREQUENCY_60HZ,              ///< Anti-freq 60Hz
    AE_FREQUENCY_AUTO,              ///< Auto detect current freq
    AE_FREQUENCY_NONE,
    AE_FREQUENCY_MAX_CNT,
    ENUM_DUMMY4WORD(AE_FREQUENCY)
} AE_FREQUENCY;

/**
    AE meter
*/

typedef enum _AE_METER {
    AE_METER_CENTERWEIGHTED,          ///< Center weighting
    AE_METER_SPOT,                    ///< Spot
    AE_METER_MATRIX,                  ///< Matic
    AE_METER_EVALUATIVE,              ///<
    AE_METER_FACEDETECTION,           ///< Face
    AE_METER_USERDEFINE,              ///< User define
    AE_METER_MAX_CNT,
    ENUM_DUMMY4WORD(AE_METER)
} AE_METER;

/**
    AE EV
*/
typedef enum _AE_EV {
    AE_EV_N4,                ///< EV -4
    AE_EV_N11_3,             ///< EV -11/3
    AE_EV_N7_2,              ///< EV -7/2
    AE_EV_N10_3,             ///< EV -10/3
    AE_EV_N3,                ///< EV -3
    AE_EV_N8_3,              ///< EV -8/3
    AE_EV_N5_2,              ///< EV -5/2
    AE_EV_N7_3,              ///< EV -7/3
    AE_EV_N2,                ///< EV -2
    AE_EV_N5_3,              ///< EV -5/3
    AE_EV_N3_2,              ///< EV -3/2
    AE_EV_N4_3,              ///< EV -4/3
    AE_EV_N1,                ///< EV -1
    AE_EV_N2_3,              ///< EV -2/3
    AE_EV_N1_2,              ///< EV -1/2
    AE_EV_N1_3,              ///< EV -1/3
    AE_EV_00,                ///< EV +0
    AE_EV_P1_3,              ///< EV +1/3
    AE_EV_P1_2,              ///< EV +1/2
    AE_EV_P2_3,              ///< EV +2/3
    AE_EV_P1,                ///< EV +1
    AE_EV_P4_3,              ///< EV +4/3
    AE_EV_P3_2,              ///< EV +3/2
    AE_EV_P5_3,              ///< EV +5/3
    AE_EV_P2,                ///< EV +2
    AE_EV_P7_3,              ///< EV +7/3
    AE_EV_P5_2,              ///< EV +5/2
    AE_EV_P8_3,              ///< EV +8/3
    AE_EV_P3,                ///< EV +3
    AE_EV_P10_3,             ///< EV +10/3
    AE_EV_P7_2,              ///< EV +7/2
    AE_EV_P11_3,             ///< EV +11/3
    AE_EV_P4,                ///< EV +4
    AE_EV_MAX_CNT,
    ENUM_DUMMY4WORD(AE_EV)
} AE_EV;

/**
    AE iso index
*/
typedef enum _AE_ISO {
	AE_ISO_AUTO =      0,   
	AE_ISO_50 =       50,     
	AE_ISO_80 =       80,    
	AE_ISO_100 =     100,   
	AE_ISO_200 =     200,    
	AE_ISO_400 =     400,    
	AE_ISO_800 =     800,    
	AE_ISO_1600 =   1600,  
	AE_ISO_3200 =   3200,  
	AE_ISO_6400 =   6400,  
	AE_ISO_12800 = 12800, 
	AE_ISO_25600 = 25600, 
	AE_ISO_MAX_CNT,
	ENUM_DUMMY4WORD(AE_ISO)
} AE_ISO;

/**
    AE anti shake on/off
*/
typedef enum _AE_ANTISHAKE {
	AE_ANTISHAKE_LV0 = 0,       ///< disable
	AE_ANTISHAKE_LV1,
	AE_ANTISHAKE_LV2,             
	AE_ANTISHAKE_MAX_CNT,
	ENUM_DUMMY4WORD(AE_ANTISHAKE)
} AE_ANTISHAKE;

/**
   AE capture long exposure time on/off
*/
typedef enum _AE_CAP_LONGEXP {
	AE_CAP_LONGEXP_OFF,          
	AE_CAP_LONGEXP_ON,                
	AE_CAP_LONGEXP_MAX_CNT,
	ENUM_DUMMY4WORD(AE_CAP_LONGEXP)
} AE_CAP_LONGEXP;

/**
   AE HDR multi frame capture EV gap
*/
typedef enum _AE_CAP_MFHDR_EV {
	AE_CAP_MFHDR_1EV = 0,
	AE_CAP_MFHDR_2EV,
	AE_CAP_MFHDR_MAX_CNT,
	ENUM_DUMMY4WORD(AE_CAP_MFHDR_EV)
} AE_CAP_MFHDR_EV;

extern ER ae_set_ui_info(IPL_PROC_ID id, AE_UI_INFO index, UINT32 value);
extern UINT32 ae_get_ui_info(IPL_PROC_ID id, AE_UI_INFO index);

#endif
