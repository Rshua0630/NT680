/**
    Definition of Exif SPEC.

    @file       ExifDef.h
    @ingroup    mIEXIF
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _EXIFDEF_H
#define _EXIFDEF_H

/**
    @addtogroup mIEXIF
*/
//@{

/**
     @name The basic definition of Exif SPEC
*/
//@{
#define JPEG_MARKER_SOI    0xD8FF
#define JPEG_MARKER_APP0   0xE0FF
#define JPEG_MARKER_APP1   0xE1FF
#define JPEG_MARKER_APP2   0xE2FF

#define BYTE_ORDER_LITTLE_ENDIAN    0x4949
#define BYTE_ORDER_BIG_ENDIAN       0x4D4D

#define TIFF_IDENTIFY             42
//@}

//-----------------------------------------
//      TIFF IFD Tag ID
//-----------------------------------------
/**
     @name Tags in 0th IFD
*/
//@{
//----- 0th IFD -----------------------
#define TAG_ID_IMAGE_WIDTH                     0x0100   ///< ImageWidth
#define TAG_ID_IMAGE_LENGTH                    0x0101   ///< ImageLength
#define TAG_ID_BITS_PER_SAMPLE                 0x0102   ///< BitsPerSample
#define TAG_ID_COMPRESSION                     0x0103   ///< Compression
#define TAG_ID_PHOTOMETRIC_INTERPRETATION      0x0106   ///< PhotometricInterpretation
#define TAG_ID_IMAGE_DESCRIPTION               0x010E   ///< ImageDescription
#define TAG_ID_MAKE                            0x010F   ///< Make
#define TAG_ID_MODEL                           0x0110   ///< Model
#define TAG_ID_STRIP_OFFSET                    0x0111   ///< StripOffsets
#define TAG_ID_ORIENTATION                     0x0112   ///< Orientation
#define TAG_ID_SAMPLES_PER_PIXEL               0x0115   ///< SamplesPerPixel
#define TAG_ID_ROWS_PER_STRIP                  0x0116   ///< RowsPerStrip
#define TAG_ID_STRIP_BYTE_COUNTS               0x0117   ///< StripByteCounts
#define TAG_ID_X_RESOLUTION                    0x011A   ///< XResolution
#define TAG_ID_Y_RESOLUTION                    0x011B   ///< YResolution
#define TAG_ID_RESOLUTION_UNIT                 0x0128   ///< ResolutionUnit
#define TAG_ID_SOFTWARE                        0x0131   ///< Software
#define TAG_ID_DATE_TIME                       0x0132   ///< DateTime
#define TAG_ID_Y_CB_CR_POSITIONING             0x0213   ///< YCbCrPositioning
#define TAG_ID_COPYRIGHT                       0x8298   ///< Copyright
//@}
/**
     @name Tags in Exif IFD
*/
//@{
//----- Exif IFD -----------------------
#define TAG_ID_EXPOSURE_TIME                   0x829A   ///< ExposureTime
#define TAG_ID_F_NUMBER                        0x829D   ///< FNumber
#define TAG_ID_EXPOSURE_PROGRAM                0x8822   ///< ExposurePogram
#define TAG_ID_ISO_SPEED_RATING                0x8827   ///< ISOSpeedRatings
#define TAG_ID_EXIF_VERSION                    0x9000   ///< ExifVersion
#define TAG_ID_DATE_TIME_ORIGINAL              0x9003   ///< DateTimeOriginal
#define TAG_ID_DATA_TIME_DIGITIZED             0x9004   ///< DateTimeDigitized
#define TAG_ID_COMPONENT_CONFIGURATION         0x9101   ///< ComponentsConfiguration
#define TAG_ID_COMPRESSED_BITS_PER_PIXEL       0x9102   ///< CompressedBitsPerPixel
#define TAG_ID_SHUTTER_SPEED_VALUE             0x9201   ///< ShutterSpeedValue
#define TAG_ID_APERTURE_VALUE                  0x9202   ///< ApertureValue
#define TAG_ID_BRIGHTNESS                      0x9203   ///< BrightnessValue
#define TAG_ID_EXPOSURE_BIAS_VALUE             0x9204   ///< ExposureBiasValue
#define TAG_ID_MAX_APERTURE_RATIO_VALUE        0x9205   ///< MaxApertureValue
#define TAG_ID_METERING_MODE                   0x9207   ///< MeteringMode
#define TAG_ID_LIGHT_SOURCE                    0x9208   ///< LightSource
#define TAG_ID_FLASH                           0x9209   ///< Flash
#define TAG_ID_FOCAL_LENGTH                    0x920A   ///< FocalLength
#define TAG_ID_MAKER_NOTE                      0x927C   ///< MakerNote
#define TAG_ID_USER_COMMENT                    0x9286   ///< UserComment
#define TAG_ID_FLASH_PIX_VERSION               0xA000   ///< FlashpixVersion
#define TAG_ID_COLOR_SPACE                     0xA001   ///< ColorSpace
#define TAG_ID_PIXEL_X_DIMENSION               0xA002   ///< PixelXDimension
#define TAG_ID_PIXEL_Y_DIMENSION               0xA003   ///< PixelYDimension
#define TAG_ID_RELATED_SOUND_FILE              0xA004   ///< RelatedSoundFile
#define TAG_ID_EXPOSURE_INDEX                  0xA215   ///< ExposureIndex
#define TAG_ID_SENSING_METHOD                  0xA217   ///< SensingMethod
#define TAG_ID_FILE_SOURCE                     0xA300   ///< FileSource
#define TAG_ID_SCENE_TYPE                      0xA301   ///< SceneType
#define TAG_ID_CUSTOM_RENDERD                  0xA401   ///< CustomRendered
#define TAG_ID_EXPOSURE_MODE                   0xA402   ///< ExposureMode
#define TAG_ID_WHITE_BALANCE                   0xA403   ///< WhiteBalance
#define TAG_ID_DZOOM_RATIO                     0xA404   ///< Digital zoom ratio
#define TAG_ID_SCENE_CAPTURE_TYPE              0xA406   ///< SceneCaptureType
#define TAG_ID_SHARPNESS                       0xA40A   ///< Sharpness

//@}
/**
     @name Tags in GPS IFD
*/
//@{
//----- GPS IFD  -----
#define TAG_ID_GPS_VERSIONID                   0x0000   ///< GPSVersionID
#define TAG_ID_GPS_LATITUDEREF                 0x0001   ///< GPSLatitudeRef
#define TAG_ID_GPS_LATITUDE                    0x0002   ///< GPSLatitude
#define TAG_ID_GPS_LONGITUDEREF                0x0003   ///< GPSLongitudeRef
#define TAG_ID_GPS_LONGITUDE                   0x0004   ///< GPSLongitude
#define TAG_ID_GPS_ALTITUDEREF                 0x0005   ///< GPSAltitudeRef
#define TAG_ID_GPS_ALTITUDE                    0x0006   ///< GPSAltitude
#define TAG_ID_GPS_TIMESTAMP                   0x0007   ///< GPSTimeStamp
#define TAG_ID_GPS_SATELLITES                  0x0008   ///< GPSSatellites
#define TAG_ID_GPS_STATUS                      0x0009   ///< GPSStatus
#define TAG_ID_GPS_MEASUREMODE                 0x000A   ///< GPSMeasureMode
#define TAG_ID_GPS_DOP                         0x000B   ///< GPSDOP
#define TAG_ID_GPS_SPEEDREF                    0x000C   ///< GPSSpeedRef
#define TAG_ID_GPS_SPEED                       0x000D   ///< GPSSpeed
#define TAG_ID_GPS_TRACKREF                    0x000E   ///< GPSTrackRef
#define TAG_ID_GPS_TRACK                       0x000F   ///< GPSTrack
#define TAG_ID_GPS_IMGDIRECTIONREF             0x0010   ///< GPSImgDirectionRef
#define TAG_ID_GPS_IMGDIRECTION                0x0011   ///< GPSImgDirection
#define TAG_ID_GPS_MAPDATUM                    0x0012   ///< GPSMapDatum
#define TAG_ID_GPS_DESTLATITUDEREF             0x0013   ///< GPSDestLatitudeRef
#define TAG_ID_GPS_DESTLATITUDE                0x0014   ///< GPSDestLatitude
#define TAG_ID_GPS_DESTLONGITUDEREF            0x0015   ///< GPSDestLongitudeRef
#define TAG_ID_GPS_DESTLONGITUDE               0x0016   ///< GPSDestLongitude
#define TAG_ID_GPS_DESTBEARINGREF              0x0017   ///< GPSDestBearingRef
#define TAG_ID_GPS_DESTBEARING                 0x0018   ///< GPSDestBearing
#define TAG_ID_GPS_DESTDISTANCEREF             0x0019   ///< GPSDestDistanceRef
#define TAG_ID_GPS_DESTDISTANCE                0x001A   ///< GPSDestDistance
#define TAG_ID_GPS_PROCESSINGMETHOD            0x001B   ///< GPSProcessingMethod
#define TAG_ID_GPS_AREAINFORMATION             0x001C   ///< GPSAreaInformation
#define TAG_ID_GPS_DATESTAMP                   0x001D   ///< GPSDateStamp
#define TAG_ID_GPS_DIFFERENTIAL                0x001E   ///< GPSDifferential

//@}
/**
     @name Tags in InterOperability IFD
*/
//@{
//----- InterOperability IFD ----------
#define TAG_ID_INTER_OPERABILITY_INDEX         0x0001   ///< InteroperabilityIndex
#define TAG_ID_EXIF_98_VERSION                 0x0002   ///< Exif98Version
//@}
/**
     @name Tags in 1st IFD
*/
//@{
//----- 1st IFD -------------------------------
#define TAG_ID_X_RESOLUTION                    0x011A   ///< XResolution
#define TAG_ID_Y_RESOLUTION                    0x011B   ///< YResolution
#define TAG_ID_RESOLUTION_UNIT                 0x0128   ///< ResolutionUnit
#define TAG_ID_INTERCHANGE_FORMAT              0x0201   ///< JPEGInterchangeFormat
#define TAG_ID_INTERCHANGE_FORMAT_LENGTH       0x0202   ///< JPEGInterchangeFormatLength


/**
     @name Tags in MP Index IFD
*/
//@{
//----- MP Index IFD----------------------------
#define TAG_ID_MPF_VERSION                     0xB000   ///< MPFVersion
#define TAG_ID_NUMBER_OF_IMAGES                0xB001   ///< NumberOfImages
#define TAG_ID_MP_ENTRY                        0xB002   ///< MPEntry
//@}
/**
     @name Tags in MP Attribute IFD
*/
//@{
//----- MP Attribute IFD----------------------------
#define TAG_ID_MP_INDIVIDUAL_NUM               0xB101   ///< MPIndividualNum
#define TAG_ID_BASE_VIEWPOINT_NUM              0xB204   ///< BaseViewpointNum
#define TAG_ID_CONVERGENCE_ANGLE               0xB205   ///< ConvergenceAngle
#define TAG_ID_BASELINE_LENGTH                 0xB206   ///< BaselineLength
//@}

/**
     @name Tag Type
*/
//@{
//-----------------------------------------
//      TIFF IFD Type
//-----------------------------------------
#define TAG_TYPE_BYTE                      1    ///< An 8-bit unsigned integer.
#define TAG_TYPE_ASCII                     2    ///< An 8-bit byte containing one 7-bit ASCII code.The final byte is terminated with NULL.
#define TAG_TYPE_SHORT                     3    ///< A 16-bit (2-byte) unsigned integer.
#define TAG_TYPE_LONG                      4    ///< A 32-bit (4-byte) unsigned integer.
#define TAG_TYPE_RATIONAL                  5    ///< Two  LONGs. The first LONG is the numerator and the second LONG expresses the denominator.
#define TAG_TYPE_SBYTE                     6    ///< An 8-bit signed integer.
#define TAG_TYPE_UNDEFINED                 7    ///< An 8-bit byte that may take any value depending on the field definition.
#define TAG_TYPE_SSHORT                    8    ///< A 16-bit (2-byte) signed integer.
#define TAG_TYPE_SLONG                     9    ///< A 32-bit (4-byte) signed integer (2's complement notation).
#define TAG_TYPE_SRATIONAL                 10   ///< Two SLONGs. The first SLONG is the numerator and the second SLONG is the denominator.
//@}


/**
    The value of Exif-Orientation Tag
*/
typedef enum _JPEG_EXIF_ORI {
	JPEG_EXIF_ORI_DEFAULT         = 1,  ///<1, Default orientation
	JPEG_EXIF_ORI_FLIP_HORIZONTAL = 2,  ///<2, flip horizontal
	JPEG_EXIF_ORI_ROTATE_180      = 3,  ///<3, rotate 180 deg
	JPEG_EXIF_ORI_FLIP_VERTICAL   = 4,  ///<4, flip vertical
	JPEG_EXIF_ORI_TRANSPOSE       = 5,  ///<5, transpose
	JPEG_EXIF_ORI_ROTATE_90       = 6,  ///<6, rotate 90 deg
	JPEG_EXIF_ORI_TRANSVERSE      = 7,  ///<7, transverse
	JPEG_EXIF_ORI_ROTATE_270      = 8   ///<8, rotate 270 deg
} JPEG_EXIF_ORI;

//@}
#endif  //_EXIFDEF_H
