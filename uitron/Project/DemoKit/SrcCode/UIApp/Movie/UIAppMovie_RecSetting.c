#include "SysCommon.h"
#include "UIMovieInfo.h"
#include "UIMovieMapping.h"
//#include "UIPhotoMapping.h"
#include "PrjCfg.h"
//#include "ImageApp_Photo.h"
#include "ImageApp_Movie.h"
#include "ImageApp_MovieMulti.h"

#define __MODULE__          UIMovieRecSetting
#define __DBGLVL__          1 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#define MOVIE_DUAL_RECORD_TEST          0////1

#if MOVIE_MAPPING_MULTIREC

MOVIE_RECODE_INFO gMovie_Rec_Info[SENSOR_MAX_NUM] = {

	{
		_CFG_REC_ID_1,			//MOVIE_CFG_REC_ID
		_CFG_VID_IN_1,			//MOVIE_CFG_VID_IN
		{1920, 1080},			//MOVIE_CFG_IMG_SIZE
		30,						//MOVIE_CFG_FRAME_RATE
		2 * 1024 * 1024,		//MOVIE_CFG_TARGET_RATE
		#if (defined(_CHIP_96683_) && (defined(_MODEL_WiFiSDV_) || defined(_MODEL_FIREFLY_)|| (defined(_MODEL_B5_))) )
		_CFG_CODEC_H265,		//MOVIE_CFG_CODEC
		#else
		_CFG_CODEC_H264,		//MOVIE_CFG_CODEC
		#endif
		_CFG_AUD_CODEC_AAC,		//MOVIE_CFG_AUD_CODEC
		_CFG_REC_TYPE_AV,		//MOVIE_CFG_REC_MODE
		_CFG_FILE_FORMAT_MP4,	//MOVIE_CFG_FILE_FORMAT
		{16, 9},				//MOVIE_CFG_IMG_RATIO
		{1920, 1080},			//MOVIE_CFG_RAWENC_SIZE
		0,						//MOVIE_CFG_IMAGEPIPE_FUNC (IMAGEPIPE_FUNC_SQUARE)
		ENABLE,					//MOVIE_CFG_DISP_ENABLE
		TRUE,					//ipl_set_enable,  FALSE: can not set sensor info, TRUE: can set sensor info
		_CFG_DAR_DEFAULT,		//MOVIE_CFG_DAR
		{0},					//MOVIE_CFG_AQ_INFO
		{0},					//MOVIE_CFG_CBR_INFO
		FALSE,					//Sensor rotate setting
		0,						//MOVIE_CFG_IMAGEPIPE_FUNC2
	},

	{
		_CFG_REC_ID_2,			//MOVIE_CFG_REC_ID
		_CFG_VID_IN_1,			//MOVIE_CFG_VID_IN
		{1920, 1080},			//MOVIE_CFG_IMG_SIZE
		30, 					//MOVIE_CFG_FRAME_RATE
		2 * 1024 * 1024,		//MOVIE_CFG_TARGET_RATE
		#if (defined(_CHIP_96683_) && (defined(_MODEL_WiFiSDV_) || defined(_MODEL_FIREFLY_)|| (defined(_MODEL_B5_))) )
		_CFG_CODEC_H265,		//MOVIE_CFG_CODEC
		#else
		_CFG_CODEC_H264,		//MOVIE_CFG_CODEC
		#endif
		_CFG_AUD_CODEC_AAC, 	//MOVIE_CFG_AUD_CODEC
		_CFG_REC_TYPE_AV,		//MOVIE_CFG_REC_MODE
		_CFG_FILE_FORMAT_MP4,	//MOVIE_CFG_FILE_FORMAT
		{16, 9},				//MOVIE_CFG_IMG_RATIO
		{1920, 1080},			//MOVIE_CFG_RAWENC_SIZE
		0,						//MOVIE_CFG_IMAGEPIPE_FUNC (IMAGEPIPE_FUNC_SQUARE)
		ENABLE, 				//MOVIE_CFG_DISP_ENABLE
		TRUE,					//ipl_set_enable,  FALSE: can not set sensor info, TRUE: can set sensor info
		_CFG_DAR_DEFAULT,		//MOVIE_CFG_DAR
		{0},					//MOVIE_CFG_AQ_INFO
		{0},					//MOVIE_CFG_CBR_INFO
		FALSE,					//Sensor rotate setting
		0,						//MOVIE_CFG_IMAGEPIPE_FUNC2
	},

	{
		_CFG_REC_ID_3,			//MOVIE_CFG_REC_ID
		_CFG_VID_IN_1,			//MOVIE_CFG_VID_IN
		{1920, 1080},			//MOVIE_CFG_IMG_SIZE
		30, 					//MOVIE_CFG_FRAME_RATE
		2 * 1024 * 1024,		//MOVIE_CFG_TARGET_RATE
		#if (defined(_CHIP_96683_) && (defined(_MODEL_WiFiSDV_) || defined(_MODEL_FIREFLY_)|| (defined(_MODEL_B5_))) )
		_CFG_CODEC_H265,		//MOVIE_CFG_CODEC
		#else
		_CFG_CODEC_H264,		//MOVIE_CFG_CODEC
		#endif
		_CFG_AUD_CODEC_AAC, 	//MOVIE_CFG_AUD_CODEC
		_CFG_REC_TYPE_AV,		//MOVIE_CFG_REC_MODE
		_CFG_FILE_FORMAT_MP4,	//MOVIE_CFG_FILE_FORMAT
		{16, 9},				//MOVIE_CFG_IMG_RATIO
		{1920, 1080},			//MOVIE_CFG_RAWENC_SIZE
		0,						//MOVIE_CFG_IMAGEPIPE_FUNC (IMAGEPIPE_FUNC_SQUARE)
		ENABLE, 				//MOVIE_CFG_DISP_ENABLE
		TRUE,					//ipl_set_enable,  FALSE: can not set sensor info, TRUE: can set sensor info
		_CFG_DAR_DEFAULT,		//MOVIE_CFG_DAR
		{0},					//MOVIE_CFG_AQ_INFO
		{0},					//MOVIE_CFG_CBR_INFO
		FALSE,					//Sensor rotate setting
		0,						//MOVIE_CFG_IMAGEPIPE_FUNC2
	},

	{
		_CFG_REC_ID_4,			//MOVIE_CFG_REC_ID
		_CFG_VID_IN_1,			//MOVIE_CFG_VID_IN
		{1920, 1080},			//MOVIE_CFG_IMG_SIZE
		30, 					//MOVIE_CFG_FRAME_RATE
		2 * 1024 * 1024,		//MOVIE_CFG_TARGET_RATE
		#if (defined(_CHIP_96683_) && (defined(_MODEL_WiFiSDV_) || defined(_MODEL_FIREFLY_)|| (defined(_MODEL_B5_))) )
		_CFG_CODEC_H265,		//MOVIE_CFG_CODEC
		#else
		_CFG_CODEC_H264,		//MOVIE_CFG_CODEC
		#endif
		_CFG_AUD_CODEC_AAC, 	//MOVIE_CFG_AUD_CODEC
		_CFG_REC_TYPE_AV,		//MOVIE_CFG_REC_MODE
		_CFG_FILE_FORMAT_MP4,	//MOVIE_CFG_FILE_FORMAT
		{16, 9},				//MOVIE_CFG_IMG_RATIO
		{1920, 1080},			//MOVIE_CFG_RAWENC_SIZE
		0,						//MOVIE_CFG_IMAGEPIPE_FUNC (IMAGEPIPE_FUNC_SQUARE)
		ENABLE, 				//MOVIE_CFG_DISP_ENABLE
		TRUE,					//ipl_set_enable,  FALSE: can not set sensor info, TRUE: can set sensor info
		_CFG_DAR_DEFAULT,		//MOVIE_CFG_DAR
		{0},					//MOVIE_CFG_AQ_INFO
		{0},					//MOVIE_CFG_CBR_INFO
		FALSE,					//Sensor rotate setting
		0,						//MOVIE_CFG_IMAGEPIPE_FUNC2
	},

};

MOVIE_RECODE_INFO gMovie_Clone_Info[SENSOR_MAX_NUM] = {

	{
		_CFG_CLONE_ID_1,		//MOVIE_CFG_REC_ID
		_CFG_VID_IN_1,			//MOVIE_CFG_VID_IN
		{848, 480},				//MOVIE_CFG_IMG_SIZE
		30,						//MOVIE_CFG_FRAME_RATE
		250 * 1024,				//MOVIE_CFG_TARGET_RATE
		#if (defined(_CHIP_96683_) && (defined(_MODEL_WiFiSDV_) || defined(_MODEL_FIREFLY_)|| (defined(_MODEL_B5_))) )
		_CFG_CODEC_H265,		//MOVIE_CFG_CODEC
		#else
		_CFG_CODEC_H264,		//MOVIE_CFG_CODEC
		#endif
		_CFG_AUD_CODEC_AAC,		//MOVIE_CFG_AUD_CODEC
		_CFG_REC_TYPE_AV,		//MOVIE_CFG_REC_MODE
		_CFG_FILE_FORMAT_MP4,	//MOVIE_CFG_FILE_FORMAT
		{16, 9},				//MOVIE_CFG_IMG_RATIO
		{848, 480},				//MOVIE_CFG_RAWENC_SIZE
		0,						//MOVIE_CFG_IMAGEPIPE_FUNC (IMAGEPIPE_FUNC_SQUARE)
		ENABLE,					//MOVIE_CFG_DISP_ENABLE
		TRUE,					//ipl_set_enable,  FALSE: can not set sensor info, TRUE: can set sensor info
		_CFG_DAR_DEFAULT,		//MOVIE_CFG_DAR
		{0},					//MOVIE_CFG_AQ_INFO
		{0},					//MOVIE_CFG_CBR_INFO
		FALSE,					//Sensor rotate setting
		0,						//MOVIE_CFG_IMAGEPIPE_FUNC2
	},

	{
		_CFG_CLONE_ID_2,		//MOVIE_CFG_REC_ID
		_CFG_VID_IN_1,			//MOVIE_CFG_VID_IN
		{848, 480},				//MOVIE_CFG_IMG_SIZE
		30, 					//MOVIE_CFG_FRAME_RATE
		250 * 1024, 			//MOVIE_CFG_TARGET_RATE
		#if (defined(_CHIP_96683_) && (defined(_MODEL_WiFiSDV_) || defined(_MODEL_FIREFLY_)|| (defined(_MODEL_B5_))) )
		_CFG_CODEC_H265,		//MOVIE_CFG_CODEC
		#else
		_CFG_CODEC_H264,		//MOVIE_CFG_CODEC
		#endif
		_CFG_AUD_CODEC_AAC, 	//MOVIE_CFG_AUD_CODEC
		_CFG_REC_TYPE_AV,		//MOVIE_CFG_REC_MODE
		_CFG_FILE_FORMAT_MP4,	//MOVIE_CFG_FILE_FORMAT
		{16, 9},				//MOVIE_CFG_IMG_RATIO
		{848, 480},				//MOVIE_CFG_RAWENC_SIZE
		0,						//MOVIE_CFG_IMAGEPIPE_FUNC (IMAGEPIPE_FUNC_SQUARE)
		ENABLE, 				//MOVIE_CFG_DISP_ENABLE
		TRUE,					//ipl_set_enable,  FALSE: can not set sensor info, TRUE: can set sensor info
		_CFG_DAR_DEFAULT,		//MOVIE_CFG_DAR
		{0},					//MOVIE_CFG_AQ_INFO
		{0},					//MOVIE_CFG_CBR_INFO
		FALSE,					//Sensor rotate setting
		0,						//MOVIE_CFG_IMAGEPIPE_FUNC2
	},

	{
		_CFG_CLONE_ID_3,		//MOVIE_CFG_REC_ID
		_CFG_VID_IN_1,			//MOVIE_CFG_VID_IN
		{848, 480},				//MOVIE_CFG_IMG_SIZE
		30, 					//MOVIE_CFG_FRAME_RATE
		250 * 1024, 			//MOVIE_CFG_TARGET_RATE
		#if (defined(_CHIP_96683_) && (defined(_MODEL_WiFiSDV_) || defined(_MODEL_FIREFLY_)|| (defined(_MODEL_B5_))) )
		_CFG_CODEC_H265,		//MOVIE_CFG_CODEC
		#else
		_CFG_CODEC_H264,		//MOVIE_CFG_CODEC
		#endif
		_CFG_AUD_CODEC_AAC, 	//MOVIE_CFG_AUD_CODEC
		_CFG_REC_TYPE_AV,		//MOVIE_CFG_REC_MODE
		_CFG_FILE_FORMAT_MP4,	//MOVIE_CFG_FILE_FORMAT
		{16, 9},				//MOVIE_CFG_IMG_RATIO
		{848, 480},				//MOVIE_CFG_RAWENC_SIZE
		0,						//MOVIE_CFG_IMAGEPIPE_FUNC (IMAGEPIPE_FUNC_SQUARE)
		ENABLE, 				//MOVIE_CFG_DISP_ENABLE
		TRUE,					//ipl_set_enable,  FALSE: can not set sensor info, TRUE: can set sensor info
		_CFG_DAR_DEFAULT,		//MOVIE_CFG_DAR
		{0},					//MOVIE_CFG_AQ_INFO
		{0},					//MOVIE_CFG_CBR_INFO
		FALSE,					//Sensor rotate setting
		0,						//MOVIE_CFG_IMAGEPIPE_FUNC2
	},

	{
		_CFG_CLONE_ID_4,		//MOVIE_CFG_REC_ID
		_CFG_VID_IN_1,			//MOVIE_CFG_VID_IN
		{848, 480},				//MOVIE_CFG_IMG_SIZE
		30, 					//MOVIE_CFG_FRAME_RATE
		250 * 1024, 			//MOVIE_CFG_TARGET_RATE
		#if (defined(_CHIP_96683_) && (defined(_MODEL_WiFiSDV_) || defined(_MODEL_FIREFLY_)|| (defined(_MODEL_B5_))) )
		_CFG_CODEC_H265,		//MOVIE_CFG_CODEC
		#else
		_CFG_CODEC_H264,		//MOVIE_CFG_CODEC
		#endif
		_CFG_AUD_CODEC_AAC, 	//MOVIE_CFG_AUD_CODEC
		_CFG_REC_TYPE_AV,		//MOVIE_CFG_REC_MODE
		_CFG_FILE_FORMAT_MP4,	//MOVIE_CFG_FILE_FORMAT
		{16, 9},				//MOVIE_CFG_IMG_RATIO
		{848, 480},				//MOVIE_CFG_RAWENC_SIZE
		0,						//MOVIE_CFG_IMAGEPIPE_FUNC (IMAGEPIPE_FUNC_SQUARE)
		ENABLE, 				//MOVIE_CFG_DISP_ENABLE
		TRUE,					//ipl_set_enable,  FALSE: can not set sensor info, TRUE: can set sensor info
		_CFG_DAR_DEFAULT,		//MOVIE_CFG_DAR
		{0},					//MOVIE_CFG_AQ_INFO
		{0},					//MOVIE_CFG_CBR_INFO
		FALSE,					//Sensor rotate setting
		0,						//MOVIE_CFG_IMAGEPIPE_FUNC2
	},

};

//#NT#2018/04/02#IsiahChang -begin
//#NT#To support Wi-Fi command 2025 BRC.
MOVIE_STRM_INFO gMovie_Strm_Info = {
	_CFG_STRM_ID_1,				//MOVIE_CFG_REC_ID
	_CFG_VID_IN_1,				//MOVIE_CFG_VID_IN
	{848,480},					//MOVIE_CFG_IMG_SIZE
	30,							//MOVIE_CFG_FRAME_RATE
	350 * 1024,					//MOVIE_CFG_TARGET_RATE
	_CFG_CODEC_H264,			//MOVIE_CFG_CODEC
	15,							//MOVIE_CFG_GOP_NUM
	_CFG_AUD_CODEC_AAC,			//MOVIE_CFG_AUD_CODEC
	TRUE,						//MOVIE_CFG_RTSP_STRM_OUT
	{0},						//MOVIE_CFG_AQ_INFO, Defined by g_MovieRecSizeTable[].
	{0},						// MOVIE_CFG_CBR_INFO, Defined by g_MovieRecSizeTable[].
	FALSE, 						// ipl_set_enable,  FALSE: can not set sensor info, TRUE: can set sensor info
	{16,9},						//MOVIE_CFG_IMG_RATIO
	#if(MOVIE_STRM_PATH_USERPROC == ENABLE)
	TRUE						//userproc_pull
	#else
	FALSE						//userproc_pull
	#endif
};
//#NT#2018/04/02#IsiahChang -end

MOVIE_RECODE_FILE_OPTION gMovie_Rec_Option = {
	_CFG_REC_ID_1,				//MOVIE_CFG_REC_ID
	180,						//MOVIE_CFG_FILE_SECOND seamless_sec
	FALSE,						//MOVIE_CFG_FILE_OPTION emr_on
	3,							//MOVIE_CFG_FILE_OPTION emr_sec (rollback sec)
	5,							//MOVIE_CFG_FILE_SECOND keep_sec
	0,							//MOVIE_CFG_FILE_SECOND overlap_sec (only 0 or 1 is valid)
	MOVREC_ENDTYPE_CUTOVERLAP,	//MOVIE_CFG_FILE_ENDTYPE  end_type
};

MOVIE_RECODE_FOLDER_NAMING gMovie_Folder_Naming[SENSOR_MAX_NUM] = {
	{
		_CFG_REC_ID_1,			//MOVIE_CFG_REC_ID
		"Movie_1",				//MOVIE_CFG_FOLDER_NAME   movie
		"EMR_1",				//MOVIE_CFG_FOLDER_NAME   emr
		"Movie_1",				//MOVIE_CFG_FOLDER_NAME   snapshot  modify for autotest
		TRUE,					//MOVIE_CFG_FILE_NAMING   2018/02/13
	},

	{
		_CFG_REC_ID_2,			//MOVIE_CFG_REC_ID
		"Movie_2",				//MOVIE_CFG_FOLDER_NAME   movie
		"EMR_2",				//MOVIE_CFG_FOLDER_NAME   emr
		"Movie_2",				//MOVIE_CFG_FOLDER_NAME   snapshot  modify for autotest
		TRUE,					//MOVIE_CFG_FILE_NAMING   2018/02/13
	},

	{
		_CFG_REC_ID_3,			//MOVIE_CFG_REC_ID
		"Movie_3",				//MOVIE_CFG_FOLDER_NAME   movie
		"EMR_3",				//MOVIE_CFG_FOLDER_NAME   emr
		"Movie_3",				//MOVIE_CFG_FOLDER_NAME   snapshot	modify for autotest
		TRUE,					//MOVIE_CFG_FILE_NAMING   2018/02/13
	},

	{
		_CFG_REC_ID_4,			//MOVIE_CFG_REC_ID
		"Movie_4",				//MOVIE_CFG_FOLDER_NAME   movie
		"EMR_4",				//MOVIE_CFG_FOLDER_NAME   emr
		"Movie_4",				//MOVIE_CFG_FOLDER_NAME   snapshot	modify for autotest
		TRUE,					//MOVIE_CFG_FILE_NAMING   2018/02/13
	},
};

MOVIE_RECODE_FOLDER_NAMING gMovie_Clone_Folder_Naming[SENSOR_MAX_NUM] = {
	{
		_CFG_CLONE_ID_1,		//MOVIE_CFG_REC_ID
		"Movie_1_S",			//MOVIE_CFG_FOLDER_NAME   movie
		"EMR_1_S",				//MOVIE_CFG_FOLDER_NAME   emr
		"Photo_1_S",			//MOVIE_CFG_FOLDER_NAME   snapshot  modify for autotest
		TRUE,					//MOVIE_CFG_FILE_NAMING   2018/02/13
	},

	{
		_CFG_CLONE_ID_2,		//MOVIE_CFG_REC_ID
		"Movie_2_S",			//MOVIE_CFG_FOLDER_NAME   movie
		"EMR_2_S",				//MOVIE_CFG_FOLDER_NAME   emr
		"Photo_2_S",			//MOVIE_CFG_FOLDER_NAME   snapshot  modify for autotest
		TRUE,					//MOVIE_CFG_FILE_NAMING   2018/02/13
	},

	{
		_CFG_CLONE_ID_3,		//MOVIE_CFG_REC_ID
		"Movie_3_S",			//MOVIE_CFG_FOLDER_NAME   movie
		"EMR_3_S",				//MOVIE_CFG_FOLDER_NAME   emr
		"Photo_3_S",			//MOVIE_CFG_FOLDER_NAME   snapshot	modify for autotest
		TRUE,					//MOVIE_CFG_FILE_NAMING   2018/02/13
	},

	{
		_CFG_CLONE_ID_4,		//MOVIE_CFG_REC_ID
		"Movie_4_S",			//MOVIE_CFG_FOLDER_NAME   movie
		"EMR_4_S",				//MOVIE_CFG_FOLDER_NAME   emr
		"Photo_4_S",			//MOVIE_CFG_FOLDER_NAME   snapshot	modify for autotest
		TRUE,					//MOVIE_CFG_FILE_NAMING   2018/02/13
	},
};

MOVIE_AUDIO_INFO   gMovie_Audio_Info = {
	32000,                     //MOVIE_CFG_AUD_RATE
	AUDIO_CH_STEREO,           //MOVIE_CFG_AUD_CH
};

MOVIE_CFG_DISP_INFO gMovie_Disp_Info = {
    VDO_ROTATE_DIR,
	#if MOVIE_DUAL_RECORD_TEST
    TRUE,
	#else
    FALSE,
	#endif
};

static UINT32 gSensorRecMask = 0xF; // which sensor is for movie recording; bit [0:3] means sensor 1~4

UINT32 Movie_GetSensorRecMask(void)
{
	return (gSensorRecMask & System_GetEnableSensor());
}

void Movie_SetSensorRecMask(UINT32 mask)
{
	gSensorRecMask = mask & System_GetEnableSensor();
}

// get main movie record mask, bit [0:3] means main movie 1~4
UINT32 Movie_GetMovieRecMask(void)
{
	UINT32 i;
	UINT32 rec_type;
	UINT32 movie_size_idx;
	UINT32 sensor_enable; // sensor enabled
	UINT32 sensor_record; // sensor for recording
	UINT32 movie_rec_mask = 0, mask = 1;
	UINT32 setting_count = 0;

	sensor_enable = System_GetEnableSensor();
	sensor_record = Movie_GetSensorRecMask();
	movie_size_idx = UI_GetData(FL_MOVIE_SIZE);
	rec_type = MovieMapping_GetRecType(movie_size_idx);

	for (i = 0; i < SENSOR_CAPS_COUNT; i++)	{

		if (sensor_enable & mask) {
			if (rec_type == MOVIE_REC_TYPE_FRONT || rec_type == MOVIE_REC_TYPE_CLONE) { // single recording
				if (sensor_record & mask) { // active sensor, for movie recording
					movie_rec_mask = mask;
					break;
				}
			} else if (rec_type == MOVIE_REC_TYPE_DUAL) { // dual recording
				if ((sensor_record & mask) && (setting_count < 2)) { // active sensor, for movie recording
					movie_rec_mask |= mask;
					setting_count++;
					if (setting_count == 2)
						break;
				}
			} else if (rec_type == MOVIE_REC_TYPE_TRI) { // tri recording
				if ((sensor_record & mask) && (setting_count < 3)) { // active sensor, for movie recording
					movie_rec_mask |= mask;
					setting_count++;
					if (setting_count == 3)
						break;
				}
			} else { // quad recording
				if ((sensor_record & mask) && (setting_count < 4)) { // active sensor, for movie recording
					movie_rec_mask |= mask;
					setting_count++;
					if (setting_count == 4)
						break;
				}
			}

		}

		mask <<= 1;
	}

	return movie_rec_mask;
}

// get clone movie record mask, bit [0:3] means clone movie 1~4
UINT32 Movie_GetCloneRecMask(void)
{
	UINT32 rec_type;
	UINT32 movie_size_idx;
	UINT32 clone_rec_mask = 0;

	movie_size_idx = UI_GetData(FL_MOVIE_SIZE);
	rec_type = MovieMapping_GetRecType(movie_size_idx);

	if (rec_type == MOVIE_REC_TYPE_CLONE) {
		clone_rec_mask = Movie_GetMovieRecMask();
	}

	return clone_rec_mask;
}

void Movie_SetResvSize(void)
{
	// set 10MB reserved size for normal recording
	ImageApp_MovieMulti_SetReservedSize('A', 10*1024*1024, FALSE);

	// set 100MB reserved size for loop recording
	// according to 4MB/s TBR, 10 sec deleting files time, 10 sec bs buffer, and some tolerance
	ImageApp_MovieMulti_SetReservedSize('A', 100*1024*1024, TRUE);
}

UINT32 Movie_GetFreeSec(void)
{
	UINT32 sec;
	UINT32 i, mask, rec_id;
	UINT32 movie_rec_mask, clone_rec_mask;
	UINT64 remain_size;
	static MOVIEMULTI_CALCSEC_SETTING2 movie_setting;

	memset(&movie_setting, 0, sizeof(MOVIEMULTI_CALCSEC_SETTING2));

	ImageApp_MovieMulti_GetRemainSize('A', &remain_size);
	if (remain_size == 0xFFFFFFFFFFFFFFFF) // get remaining size failed
		remain_size = 0;

	movie_rec_mask = Movie_GetMovieRecMask();
	clone_rec_mask = Movie_GetCloneRecMask();

	mask = 1;
	rec_id = 0;
	for (i = 0; i < SENSOR_CAPS_COUNT; i++) {
		if (movie_rec_mask & mask) {
			movie_setting.info[rec_id].vidfps = gMovie_Rec_Info[i].frame_rate;
			movie_setting.info[rec_id].vidTBR = gMovie_Rec_Info[i].target_bitrate;
			movie_setting.info[rec_id].audSampleRate = gMovie_Audio_Info.aud_rate;
			movie_setting.info[rec_id].audChs = gMovie_Audio_Info.aud_ch;
			movie_setting.info[rec_id].gpson = TRUE;
			movie_setting.info[rec_id].nidxon = TRUE;
			rec_id++;
		}

		if (clone_rec_mask & mask) {
			movie_setting.info[rec_id].vidfps = gMovie_Clone_Info[i].frame_rate;
			movie_setting.info[rec_id].vidTBR = gMovie_Clone_Info[i].target_bitrate;
			movie_setting.info[rec_id].audSampleRate = gMovie_Audio_Info.aud_rate;
			movie_setting.info[rec_id].audChs = gMovie_Audio_Info.aud_ch;
			movie_setting.info[rec_id].gpson = TRUE;
			movie_setting.info[rec_id].nidxon = TRUE;
			rec_id++;
		}

		mask <<= 1;
	}

	movie_setting.givenSpace = remain_size;
	sec = ImageApp_MovieMulti_GetFreeRec2(&movie_setting);

	return sec;
}

#endif
