#ifndef _KEYDEF_H
#define _KEYDEF_H

////////////////////////////////////////////////////////////////////////////////
//GxInput                   Get Key
//Key Driver                Detect Key change

//-----------------------------------------------------------------------------
// KeyScan global definitions
//-----------------------------------------------------------------------------
// Flag pattern
// Key pressed
#define FLGKEY_UP                       0x00000001      // Key UP pressed
#define FLGKEY_DOWN                     0x00000002      // Key DOWN pressed
#define FLGKEY_LEFT                     0x00000004      // Key LEFT pressed
#define FLGKEY_RIGHT                    0x00000008      // Key RIGHT pressed

#define FLGKEY_ENTER                    0x00000010      // Key ENTER (SET) pressed
#define FLGKEY_MENU                     0x00000020      // Key MENU pressed
#define FLGKEY_DEL                      0x00000040      // Key DELETE (TRASH) pressed
#define FLGKEY_MODE                     0x00000080      // Key MODE pressed

#define FLGKEY_ZOOMOUT                  0x00000100      // Key ZOOM OUT pressed
#define FLGKEY_ZOOMIN                   0x00000200      // Key ZOOM IN pressed
#define FLGKEY_SHUTTER1                 0x00000400      // Key SHUTTER 1 (SHUTTER_HALF) pressed
#define FLGKEY_SHUTTER2                 0x00000800      // Key SHUTTER 2 (SHUTTER_FULL) pressed

#define FLGKEY_PLAYBACK                 0x00001000      // Key FACE DETECT pressed
#define FLGKEY_MOVIE                    0x00002000
#define FLGKEY_FACEDETECT               0x00004000
#define FLGKEY_I                        0x00008000
#define FLGKEY_CUSTOM1                  0x00010000

//special normal key
#define FLGKEY_KEY_POWER                0x80000000

#define FLGKEY_KEY_MASK                 0xFFFFFFFF      // Key flag mask

#define FLGKEY_KEY_MASK_DEFAULT         FLGKEY_KEY_MASK // Default key mask
#define FLGKEY_KEY_MASK_NULL            0x00000000

#define FLGKEY_KEY_REL_MASK_DEFAULT     FLGKEY_KEY_MASK      // Default key released mask
#define FLGKEY_KEY_CONT_MASK_DEFAULT    FLGKEY_KEY_MASK      // Default key continue mask
#define FLGKEY_KEY_UDLR                 FLGKEY_UP|FLGKEY_DOWN|FLGKEY_RIGHT|FLGKEY_LEFT
#define FLGKEY_KEY_OFFALL               0x00000000
#define FLGKEY_KEY_CONT_OFFALL          0x00000000
#define FLGKEY_KEY_CONT_MASK_OFFALL     0x00000000
#define FLGKEY_KEY_SOUND_MASK_DEFAULT   FLGKEY_KEY_MASK

#define STATUS_KEY_LVL_UNKNOWN          0
#define STATUS_KEY_LVL_1                0x00000001
#define STATUS_KEY_LVL_2                0x00000002
#define STATUS_KEY_LVL_3                0x00000004
#define STATUS_KEY_LVL_4                0x00000008
#define STATUS_KEY_LVL_5                0x00000010
#define STATUS_KEY_LVL_6                0x00000020
#define STATUS_KEY_LVL_7                0x00000040
#define STATUS_KEY_LVL_8                0x00000080
#define STATUS_KEY_LVL_9                0x00000100
#define STATUS_KEY_LVL_10               0x00000200
#define STATUS_KEY_LVL_11               0x00000400
#define STATUS_KEY_LVL_12               0x00000800
#define STATUS_KEY_LVL_13               0x00001000
#define STATUS_KEY_LVL_14               0x00002000
#define STATUS_KEY_LVL_15               0x00004000
#define STATUS_KEY_LVL_16               0x00008000
#define STATUS_KEY_LVL_17               0x00010000
#define STATUS_KEY_LVL_18               0x00020000
#define STATUS_KEY_LVL_19               0x00040000
#define STATUS_KEY_LVL_20               0x00080000


////////////////////////////////////////////////////////////////
////////Project level define, temporarily moved here////////////
////////////////////////////////////////////////////////////////
//#NT#2009/11/30#Photon Lin -begin
/*
#define VOLDET_MS_PHOTO_AUTO            STATUS_KEY_LVL_7//0x00000040
#define VOLDET_MS_PHOTO_MANUAL          STATUS_KEY_LVL_8//0x00000080
#define VOLDET_MS_PHOTO_PANORAMA        STATUS_KEY_LVL_1//0x00000001
#define VOLDET_MS_PHOTO_STAB            STATUS_KEY_LVL_2//0x00000002
#define VOLDET_MS_PHOTO_SCENE           STATUS_KEY_LVL_3//0x00000004
#define VOLDET_MS_MOVIE                 STATUS_KEY_LVL_4//0x00000008
#define VOLDET_MS_ASCENE                STATUS_KEY_LVL_5//0x00000010
#define VOLDET_MS_PHOTO_PORTRAIT        STATUS_KEY_LVL_6//0x00000020
*/
//#NT#2009/12/28#Lincy Lin -begin
#define VOLDET_MS_PHOTO_AUTO            STATUS_KEY_LVL_1//0x00000001
#define VOLDET_MS_PHOTO_MANUAL          STATUS_KEY_LVL_2//0x00000002
#define VOLDET_MS_ASCENE                STATUS_KEY_LVL_3//0x00000004
#define VOLDET_MS_PHOTO_PANORAMA        STATUS_KEY_LVL_4//0x00000008
#define VOLDET_MS_PHOTO_SCENE           STATUS_KEY_LVL_5//0x00000010
#define VOLDET_MS_PHOTO_STAB            STATUS_KEY_LVL_6//0x00000020
#define VOLDET_MS_MOVIE                 STATUS_KEY_LVL_7//0x00000040
#define VOLDET_MS_PHOTO_PORTRAIT        STATUS_KEY_LVL_8//0x00000080
//#NT#2009/12/28#Lincy Lin -end

//#NT#2009/11/30#Photon Lin -end
////////////////////////////////////////////////////////////////

#define TOUCH_PRESS                          0x00000001
#define TOUCH_RELEASE                        0x00000002
#define TOUCH_MOVE                           0x00000004
#define TOUCH_CLICK                          0x00000008
#define TOUCH_DOUBLECLICK                    0x00000010
#define TOUCH_SLIDE_LEFT                     0x00000020
#define TOUCH_SLIDE_RIGHT                    0x00000040
#define TOUCH_SLIDE_UP                       0x00000080
#define TOUCH_SLIDE_DOWN                     0x00000100
#define TOUCH_MASK_DEFAULT                   0x000001FF
#define TOUCH_MASK_NULL                      0x00000000

#endif //_KEYDEF_H
