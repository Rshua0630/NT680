/**
    Header file of media fileformat reading library

    Exported header file of media fileformat reading library.

    @file       MediaReadLib.h
    @ingroup    mIMEDIAREAD
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _MEDIAREADLIB_H
#define _MEDIAREADLIB_H

#include "Type.h"
//#NT#2012/07/17#Calvin Chang#Porting New FST System -begin
#include "FileSysTsk.h"
//#NT#2012/07/17#Calvin Chang -end
#include "media_def.h"

/**
    @addtogroup mIMEDIAREAD
*/
//@{


/**
    @name filetype check ID

    Filetype check ID.
*/
//@{
#define MOVREADLIB_CHECKID  0xBC191380 ///< mdat base time ID
#define AVIREADLIB_CHECKID  0x72696666 ///< fourcc riff ID
//@}


/**
    @name Set information to file reading object library

    Setting information type to file reading library.
*/
//@{
#define MEDIAREADLIB_SETINFO_FILESIZE    1 ///< param1:total filesize(in)
#define MEDIAREADLIB_SETINFO_VIDENTRYBUF 2 ///< param1:address(in), param2:size(in)
#define MEDIAREADLIB_SETINFO_AUDENTRYBUF 3 ///< param1:address(in), param2:size(in)
#define MEDIAREADLIB_SETINFO_CLUSTERSIZE 4 ///< param1:cluster size(in)
#define MEDIAREADLIB_SETINFO_TSDEMUXBUF   5 ///< param1:ts demux buf(in)
#define MEDIAREADLIB_SETINFO_MEMSTARTADDR 6 ///< param1:ts demux memory start addr(in)
#define MEDIAREADLIB_SETINFO_TSENDINFO   7 ///< param1:address(in), param2:size(in)
#define MEDIAREADLIB_SETINFO_PREPARESEEK   8 ///< param1:seek target pts(in), param2:vidIdx(in), param3:audIdx(in)
#define MEDIAREADLIB_SETINFO_TSTHUMBINFO   9 ///< param1:src address(in), param2:size(in), param3:demuxed dst addr(in)
//@}

/**
    @name Get information from file reading object library

    Getting information type from file reading object library.
*/
//@{
#define MEDIAREADLIB_GETINFO_GETVIDEOPOSSIZE    0x01///< p1:video frame num(in), p2:pos(out), p3:size(out)
#define MEDIAREADLIB_GETINFO_GETAUDIOPOSSIZE    0x02///< p1:audio frame num(in), p2:pos(out), p3:size(out)
#define MEDIAREADLIB_GETINFO_VFRAMENUM          0x03///< p1:total video frame num(out)
#define MEDIAREADLIB_GETINFO_VIDEOTYPE          0x04///< p1:video type, MEDIAPLAY_VIDEO_MJPG or other(out)
#define MEDIAREADLIB_GETINFO_VIDEOFR            0x05///< p1:video frame rate(out)
#define MEDIAREADLIB_GETINFO_USERDATA           0x06///< p1:user data pos(out), p2:size(out)
#define MEDIAREADLIB_GETINFO_FRE1DATA           0x07///< p1:fre1 data pos(out), p2:size(out)
#define MEDIAREADLIB_GETINFO_H264DESC           0x08///< p1:h264 description addr(out), p2:len(out)
#define MEDIAREADLIB_GETINFO_VIDEO_W_H          0x09///< p1:video width(out), p2:video height(out)
#define MEDIAREADLIB_GETINFO_NEXTIFRAMENUM      0x0A///< p1:now video frame num(in), p2:next i framenumber(out)
#define MEDIAREADLIB_GETINFO_PREVIFRAMENUM      0x0B///< p1:now video frame num(in), p2:prev i framenumber(out)
//#NT#2012/08/21#Calvin Chang#Add for User Data & Thumbnail -begin
#define MEDIAREADLIB_GETINFO_VIDEOTHUMB         0x0C///< p1:video thumbnail pos(out), p2:size(out)
//#NT#2012/08/21#Calvin Chang -end
#define MEDIAREADLIB_GETINFO_AUDIOFR            0x0D///< p1:audio frame rate(out)//2012/11/21 Meg
//#NT#2013/10/18#Calvin Chang#Get track weight and height of TKHD atom for sample aspect ratio -begin
#define MEDIAREADLIB_GETINFO_VIDEO_TK_W_H          0x0E///< p1:video width of track(out), p2:video height of track(out)
//#NT#2013/10/18#Calvin Chang -end
//#NT#2013/12/17#Calvin Chang#Add for Screennail -begin
#define MEDIAREADLIB_GETINFO_VIDEOSCRA          0x0F///< p1:video screennail pos(out), p2:size(out)
//#NT#2013/12/17#Calvin Chang -end
#define MEDIAREADLIB_GETINFO_GETGOP             0x10///< p1: GOP number(out)
#define MEDIAREADLIB_GETINFO_TOTALSEC           0x11///< p1: Total sec in file(out)
#define MEDIAREADLIB_GETINFO_SEEKRESULT         0x12///< p1, p2: distance to target pts(out), p3:seek result(out)
#define MEDIAREADLIB_GETINFO_FIRSTPTS           0x13///< p1, p2: first pts in file(out)
#define MEDIAREADLIB_GETINFO_TOTALVIDFRAME      0x14///< p1: Total video frames in file(out)
#define MEDIAREADLIB_GETINFO_IFRAMEDISTANCE     0x15///< p1: I-frame distance in bytes(out)
//#NT#2017/02/14#Adam Su -begin
//#NT#Support H.265 codec type
#define MEDIAREADLIB_GETINFO_H265DESC           0x16///< p1:h265 description addr(out), p2:len(out)
//#NT#2017/02/14#Adam Su -end

#define MOVREADLIB_GETINFO_MAXIFRAMESIZE        0x81///< p1:max iframe size(out)
#define MOVREADLIB_GETINFO_IFRAMETOTALCOUNT     0x82///< p1:iframe total count(out)
#define MOVREADLIB_GETINFO_SECONDS2FILEPOS      0x83///< p1:sec(in), p2:pos(out)
#define MOVREADLIB_GETINFO_SHOWALLSIZE          0x84///< no parameters, show size and pos of all frames
#define MOVREADLIB_GETINFO_GETVIDEOENTRY        0x85///< p1:video frame num(in), p2:(MOV_Ientry *)(out)
#define MOVREADLIB_GETINFO_GETAUDIOENTRY        0x86///< p1:audio frame num(in), p2:(MOV_Ientry *)(out)
#define MOVREADLIB_GETINFO_TOTALDURATION        0x87///< p1:total sec(out)
#define MOVREADLIB_GETINFO_NEXTPFRAME           0x8C///< p1:now video frame num(in), p2:next i or p framenumber(out)
#define MOVREADLIB_GETINFO_H264_IPB             0x8D///< p1:has B frames or not(out)
#define MOVREADLIB_GETINFO_VID_ENTRY_ADDR       0x8E///< p1:video entry starting address, gp_movReadEntry(out)
#define MOVREADLIB_GETINFO_AUD_ENTRY_ADDR       0x8F///< p1:audio entry starting address, gp_movReadAudioEntry(out)
//#NT#2013/06/11#Calvin Chang#Support Create/Modified Data inMov/Mp4 File format -begin
#define MOVREADLIB_GETINFO_CMDATE               0x90///< p1:create time(in), p2:modification time(in)
//#NT#2013/06/11#Calvin Chang -end

#define AVIREADLIB_GETINFO_GETVIDEOENTRY        0x91///< p1:video entry
#define AVIREADLIB_GETINFO_GETAUDIOENTRY        0x92///< p1:audio entry
//#NT#2012/06/27#Calvin Chang#Parse AVI Header Media Infomation - begin
#define AVIREADLIB_GETINFO_HEADER_MEDIA         0x93///< p1:AVI Header (MEDIA_AVIHEADER)(in)
//#NT#2012/06/27#Calvin Chang -end
//@}


#define AVIREAD_INDEXSIZE 0x100000   ///< minimum avi index size

//#NT#2012/04/06#Calvin Chang -begin
//Search Index table solution in Parse 1st video & Pre-decode frames for new H264 driver
/**
    @name Video pre-decode frame number definition

    Definition of video pre-decode frame number.
*/
//@{
#define MEDIAREADLIB_PREDEC_FRMNUM  3
//@}
//#NT#2012/04/06#Calvin Chang -end

/**
    @name Error Code from AVI Header Parser

    Returned error code from AVI header parser API.
*/
//@{
#define AVIREADLIB_PRSERR_OK           0      ///< Parser OK
#define AVIREADLIB_PRSERR_NOTRIFF      (-5)   ///< Not four-character code id: FOURCC_RIFF
#define AVIREADLIB_PRSERR_NOTAVI       (-6)   ///< Not four-character code id: FOURCC_AVI
#define AVIREADLIB_PRSERR_NOLIST       (-7)   ///< No LIST chunk exist
#define AVIREADLIB_PRSERR_NOxxrl       (-8)   ///< No four-character code id: FOURCC_HDRL or FOURCC_STRL
#define AVIREADLIB_PRSERR_NOstrf       (-9)   ///< Not four-character code id: FOURCC_STRF
#define AVIREADLIB_PRSERR_NOTavih      (-10)  ///< Not four-character code id: FOURCC_avih
#define AVIREADLIB_PRSERR_NOTstrh      (-11)  ///< Not four-character code id: FOURCC_strh
#define AVIREADLIB_PRSERR_NOTxxds      (-12)  ///< Not four-character code id: FOURCC_VIDS or FOURCC_AUDS
#define AVIREADLIB_PRSERR_vids         (-13)  ///< Parse four-character code FOURCC_VIDS error
#define AVIREADLIB_PRSERR_auds         (-14)  ///< Parse four-character code FOURCC_AUDS error
#define AVIREADLIB_PRSERR_NOmovi       (-15)  ///< No movi chunk
#define AVIREADLIB_PRSERR_NOidx1       (-16)  ///< No idx1 chunk
//@}

/**
    Type defintion struture of file container object

    Type defintion struture of file container object functions.
*/
typedef struct {
	ER(*Probe)(UINT32 addr, UINT32 size);                                           ///< Probe file type
	ER(*Initialize)(void);                                                          ///< Initailize file container object
	ER(*SetMemBuf)(UINT32 startAddr, UINT32 size);                                  ///< Set memeory buffer to file container object
	ER(*ParseHeader)(UINT32 hdrAddr, UINT32 hdrSize, void *pobj);                   ///< Parse header
	ER(*GetInfo)(UINT32 type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3);   ///< Get information from container
	ER(*SetInfo)(UINT32 type, UINT32 param1, UINT32 param2, UINT32 param3);         ///< Get information from container
	ER(*CustomizeFunc)(UINT32 type, void *pobj);                                    ///< customize function, if needed
	ER(*cbReadFile)(UINT8 *type, UINT64 pos, UINT32 size, UINT32 addr);             ///< file read callback function
	void (*cbShowErrMsg)(char *fmtstr, ...);                                        ///< show error message callback function
	UINT32 checkID;                                                                 ///< file check ID
} CONTAINERPARSER;

/**
    Type defintion struture of media header

    Type defintion struture of media header.

    @note used for ParseHeader() of file container
*/
typedef struct {
	UINT32  checkID;
	UINT32  uiFileStartAddr;        ///< AVI file start address
	UINT32  uiFirstVideoOffset;     ///< first video frame offset
	UINT32  uiFirstAudioOffset;     ///< first audio frame offset
	UINT32  uiIndexChunkOffset;     ///< index chunk offset
	UINT32  uiTotalFileSize;        ///< total file size
	UINT32  uiTotalVideoFrames;     ///< total frames, video
	UINT32  uiTotalAudioFrames;     ///< total frames, audio
	UINT32  uiTotalSecs;            ///< total seconds
	UINT32  uiVidWidth;             ///< video width
	UINT32  uiVidHeight;            ///< video height
	UINT32  uiVidScale;             ///< video scale     (66666)
	UINT32  uiVidRate;              ///< video rate    (1000000)
	UINT32  uiVidType;              ///< MEDIAPLAY_VIDEO_MJPG or other
	UINT32  uiAudSampleRate;        ///< audio sample rate
	UINT32  uiAudBytesPerSec;       ///< audio bytes per second
	UINT32  uiAudWavTotalSamples;   ///< audio all samples
	UINT32  uiAudType;              ///< audio type, MEDIAAUDIO_CODEC_SOWT or others
	UINT8   ucAudBitsPerSample;     ///< audio bits per sample
	UINT8   ucAudChannels;          ///< audio channels
	BOOL    bAudioSupport;          ///< with audio data
	BOOL    bVideoSupport;          ///< with video data
} MEDIA_HEADINFO, *PMEDIA_HEADINFO;

/**
    Type defintion struture of File information

    Type defintion struture of File detail information.
*/
typedef struct {
	MEDIA_HEADINFO headinfo; ///< header information
	UINT32 totalsize_high;   ///< totoal file size
	UINT32 totalsize_low;    ///< totoal file size
	UINT32 nowpos_high;      ///< file position
	UINT32 nowpos_low;       ///< file position
	UINT16 speed;            ///< file play speed
	UINT16 direct;           ///< file play direction
	UINT16 status;           ///< file play status
	UINT16 rev;              ///< reserved
} MEDIA_FILE_INFO;

/**
    Type defintion struture of media infomation

    Parse media information from header and 1st video frame.

    @note used for ParseHeader of file container (1st video frame)
*/
typedef struct {
	INT32  err;     ///< [out] parse header error code
	UINT32 pos;     ///< [out] first video frame position
	UINT32 size;    ///< [out] first video frame size
	UINT16 wid;     ///< [out] video width
	UINT16 hei;     ///< [out] video height
	UINT32 type;    ///< [out] container format after probed, MEDIA_FILEFORMAT_MOV or others
	UINT64 udta_pos;///< [out] filepos of user data
	UINT32 udta_size;///<[out] size of user data
	UINT32 dur;     ///< [out] total duration (in ms)
	UINT32 audSR;   ///< [out] audio sample rates
	UINT32 audChs;  ///< [out] audio channels
	UINT32 vidFR;   ///< [out] video frameRate
	UINT32 audFR;   ///< [out] audio frameRate
	UINT32 vidtype; ///< [out] video frame type, MEDIAPLAY_VIDEO_H264 or others
	UINT32 audtype; ///< [out] audio type, MEDIAAUDIO_CODEC_SOWT or others
	UINT64 fre1_pos;///< [out] file position of user data
	UINT32 fre1_size;///<[out] size of user data
	UINT32 avgBPS;  ///< [out] average bytes per second
	//#NT#2012/04/06#Calvin Chang -begin
	//Search Index table solution in Parse 1st video & pre-decode video frames for new H264 driver
	UINT32 vidfrm_pos[MEDIAREADLIB_PREDEC_FRMNUM];  ///< [out] video frames position
	UINT32 vidfrm_size[MEDIAREADLIB_PREDEC_FRMNUM]; ///< [out] video frames size
	//#NT#2012/04/06#Calvin Chang -end
	//#NT#2013/04/17#Calvin Chang#Support Rotation information in Mov/Mp4 File format -begin
	UINT32 vidrotate; ///< [out] video roation information
	//#NT#2013/04/17#Calvin Chang -end
	//#NT#2013/10/18#Calvin Chang#Get track width and height for sample aspect ratio -begin
	UINT32 tkwid;    ///< [out] video track width
	UINT32 tkhei;    ///< [out] video track height
	//#NT#2013/10/18#Calvin Chang -end
} MEDIA_FIRST_INFO;

/**
    Type defintion struture of file container parser

    Type defintion struture of file container parser.
*/
typedef struct {
	UINT32  ParserIndex;        ///< file type parser index
	CONTAINERPARSER *parser;    ///< file container object pointer
	char *ParserName;           ///< file container name
} FORMATPARSER_OBJ;


//#NT#2012/06/27#Calvin Chang#Parse AVI Header Media Infomation - begin
/**
     Type defintion struture of main AVI header information

     Type defintion struture of main AVI header information.
*/
typedef struct {
	UINT32 FOURCC_ID;                 ///< Four-Character Code ID
	UINT32 uiSize;                    ///< cb Size
	UINT32 uiMicroSecPerFrame;        ///< The duration of one video frame in microseconds
	UINT32 uiMaxBytePerSec;           ///< Highest occuring data rate in second
	UINT32 uiPaddingGranularity;      ///< File is padded to a multiple of this
	UINT32 uiFlags;                   ///< Value : AVIF_HASINDEX | AVIF_WASCAPTUREFILE
	UINT32 uiTotalFrames;             ///< The total number of video frames
	UINT32 uiInitialFrames;           ///< Value : 0x00000000
	UINT32 uiStreams;                 ///< Number of streams in the file
	UINT32 uiSuggestedBufferSize;     ///< Size of buffer required to hold chunks of the file
	UINT32 uiWidth;                   ///< Width of video stream
	UINT32 uiHeight;                  ///< Height of video stream
} MEDIA_MAINAVIHEADER;


/**
     Type defintion struture of AVI stream header information

     Type defintion struture of AVI stream header information.
*/
typedef struct {
	UINT32 FOURCC_ID;                 ///< Four-Character Code ID
	UINT32 FOURCC_Type;               ///< Can be 'vids' - video and 'auds' - audio
	UINT32 FOURCC_Handler;            ///< FourCC of codec to be used
	UINT32 uiSize;                    ///< Header Size
	UINT32 uiFlags;                   ///< Value : 0x00000000
	UINT16 uiPriority;                ///< Value : 0x00000000
	UINT16 uiLanguage;                ///< Value : 0x00000000
	UINT32 uiInitialFrames;           ///< Value : 0x00000000
	UINT32 uiScale;                   ///< uiRate / uiScale = samples / second (audio) or frames / second (video)
	UINT32 uiRate;                    ///< uiRate / uiScale = samples / second (audio) or frames / second (video)
	UINT32 uiStart;                   ///< Start time of stream
	UINT32 uiLength;                  ///< Size of stream in units as dened in dwRate and dwScale
	UINT32 uiSuggestedBufferSize;     ///< Size of buffer necessary to store blocks of that stream
	UINT32 uiQuality;                 ///< The quality of the stream
	UINT32 uiSampleSize;              ///< Number of bytes of one stream atom
	UINT32 uiRec_UpLeft;              ///< The Region of Play Window (Up/Left)
	UINT32 uiRec_DownRight;           ///< The Region of Play Window (Down/Right)
} MEDIA_AVISTREAMHEADER;

/**
     Type defintion struture of video image data information

     Type defintion struture of video image data information (BITMAPINFO).
*/
typedef struct {
	UINT32 FOURCC_ID;                 ///< Four-Character Code ID
	UINT32 uiSize;                    ///< Info Size
	UINT32 uiWidth;                   ///< Video Width
	UINT32 uiHeight;                  ///< Video Height
	UINT16 uiPlanes;                  ///< The number of planes
	UINT16 uiBitCount;                ///< The number of bits-per-pixel
	UINT32 uiCompression;             ///< The type of compression
	UINT32 uiSizeImage;               ///< The size of the frame (in bytes)
	UINT32 uiXPelsPerMeter;           ///< Value : 0x00000000
	UINT32 uiYPelsPerMeter;           ///< Value : 0x00000000
	UINT32 uiClrUsed;                 ///< Value : 0x00000000
	UINT32 uiClrImportant;            ///< Value : 0x00000000
} MEDIA_BITMAPINFO;

/**
     Type defintion struture of audio wave data information

     Type defintion struture of audio wave data information.
*/
typedef struct {
	UINT32 FOURCC_ID;                 ///< Four-Character Code ID
	UINT32 uiSize;                    ///< Info Size
	UINT16 uiFormatTag;               ///< Waveform-audio format type (0x00000001)
	UINT16 uiChannels;                ///< Number of channels
	UINT32 uiSamplesPerSec;           ///< Sample rate in samples per second (Hz)
	UINT32 uiAvgBytesPerSec;          ///< Average of bytes per second
	UINT16 uiBlockAlign;              ///< Block alignment in bytes
	UINT16 uiBitsPerSample;           ///< Bits per sample
	UINT16 uicbSize;                  ///< cb Size
} MEDIA_WAVEFORMATEXINFO;

/**
     Type defintion struture of AVI Header

     Type defintion struture of AVI Header
*/
typedef struct {
	MEDIA_MAINAVIHEADER    MainAviHeader;    ///< Main AVI Header
	MEDIA_AVISTREAMHEADER  VidStrHeader;     ///< AVI Video Stream Header
	MEDIA_BITMAPINFO       VidStrFmt;        ///< Video Image data information
	MEDIA_AVISTREAMHEADER  AudStrHeader;     ///< AVI Audio Stream Header
	MEDIA_WAVEFORMATEXINFO AudStrFmt;        ///< Audio Wave Data information
} MEDIA_AVIHEADER, *PMEDIA_AVIHEADER;
//#NT#2012/06/27#Calvin Chang -end

//@}

//*****[Legacy Structure/API Definition ]******
typedef struct {
	UINT32 fileStartAddr;//in: file address
	UINT32 freeSize;     //in: avaliable size
	UINT32 yAddr;        //in: y starting addr
	UINT32 imageWidth;   //out: image width
	UINT32 imageHeight;  //out: image height
	UINT32 cbAddr;       //out: cb addr
	UINT32 crAddr;       //out: cr addr
	UINT8  filetype;     //out: MEDIA_FILEFORMAT_MOV
	UINT8  videoCodec;   //out: MEDIAPLAY_VIDEO_MJPG
	UINT8  jpgType;      //out: JPG_FMT_YUV420
	UINT8  rev;
	//#NT#2012/07/17#Calvin Chang#Porting New FST System -begin
	FST_FILE hfilehdl;   //out: Opened Active Media File Handler
	//#NT#2012/07/17#Calvin Chang -end
} MEDIA_PLAY_1ST_INFO;

#endif //_MEDIAREADLIB_H
