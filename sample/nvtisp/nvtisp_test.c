#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "nvtisp.h"


#define DBG_WRN(fmtstr, args...) printf("\033[33mWRN:%s(): \033[0m" fmtstr,__func__, ##args)
#define DBG_ERR(fmtstr, args...) printf("\033[31mERR:%s(): \033[0m" fmtstr,__func__, ##args)

#if 0
#define DBG_IND(fmtstr, args...)
#define DBG_MSG(fmtstr, args...)
#else
#define DBG_IND(fmtstr, args...) printf(fmtstr, ##args)//printf("%s(): " fmtstr, __func__, ##args)
#define DBG_MSG(fmtstr, args...) printf(fmtstr, ##args)
#endif

#define DBG_DUMP(fmtstr, args...) printf(fmtstr, ##args)

#define TEST_DELAY_SHORT 1
#define TEST_DELAY_LONG  3

typedef enum{


    ISP_TEST_IMAGE_SETTING,
    ISP_TEST_AE_SETTING,
    ISP_TEST_AE_STATUS,
    ISP_TEST_WB_SETTING,
    ISP_TEST_WB_STATUS,
    ISP_TEST_WDR_SETTING,
    ISP_TEST_HDR_SETTING,
    ISP_TEST_NR_SETTING,
    ISP_DIS_SETTING,
    ISP_GET_RAW_DATA,
    ISP_CROP_SETTING,
    ISP_LDC_SETTING,
    ISP_LSC_SETTING,
    ISP_DEFOG_SETTING,
    ISP_IMESCALE_SETTING, 
    ISP_TEST_MAX


}ISPTestItem;


static void show_help(void)
{
    printf("usage:\n"
           "  nvtisp_test [video input channel]\n");
}

static void test_image_setting(UINT8 uiViChannelID)
{
    ISPImageSetting sImageSetting={0};
    unsigned int selectItem=0;

    NvtISP_GetImageSetting(uiViChannelID, &sImageSetting);

    printf("Current Image Setting\n");
    printf("==================================================\n");
    printf("Brightness    = %u \n", sImageSetting.uiBrightness);
    printf("Contrast      = %u \n", sImageSetting.uiContrast);
    printf("Saturation    = %u \n", sImageSetting.uiSaturation);
    printf("Hue           = %u \n", sImageSetting.uiHue);
    printf("Sharpness     = %u \n", sImageSetting.uiSharpness);
    printf("Color         = %s \n", sImageSetting.eColor == ISP_COLOR_TYPE_NORMAL ? "Color" : "Gray");
    printf("Mirror        = %s \n", sImageSetting.bMirror == NVT_TRUE ? "True" : "False");
    printf("Flip          = %s \n", sImageSetting.bFlip == NVT_TRUE ? "True" : "False");
    printf("bDewarp       = %ld \n", sImageSetting.bDewarp);
    printf("IRCut Mode    = ");
    switch (sImageSetting.eIRCutMode)
    {
        case ISP_IRCUT_TYPE_NIGHT:
            printf("Night \n");
            break;
        case ISP_IRCUT_TYPE_DAY:
            printf("Day \n");
            break;
        case ISP_IRCUT_TYPE_AUTO:
        default:
            printf("Auto \n");
            break;
    }
    printf("==================================================\n");

    printf("Test Item Option\n"
           " 0) Brightness\n"
           " 1) Contrast\n"
           " 2) Saturation\n"
           " 3) Hue\n"
           " 4) Sharpness\n"
           " 5) Color\n"
           " 6) Mirror\n"
           " 7) Flip\n"
           " 8) IRCut Mode\n"
           " 9) Dewarp\n"
           "Please enter your select: ");
    scanf("%u", &selectItem);
    switch (selectItem)
    {
        case 0:
            printf("Enter Item Value : ");
            scanf("%lu", &(sImageSetting.uiBrightness));
            break;
        case 1:
            printf("Enter Item Value : ");
            scanf("%lu", &(sImageSetting.uiContrast));
            break;
        case 2:
            printf("Enter Item Value : ");
            scanf("%lu", &(sImageSetting.uiSaturation));
            break;
        case 3:
            printf("Enter Item Value : ");
            scanf("%lu", &(sImageSetting.uiHue));
            break;
        case 4:
            printf("Enter Item Value : ");
            scanf("%lu", &(sImageSetting.uiSharpness));
            break;
        case 5:
            printf("Enter Item Value(%d:Color, %d:Gray) : ", ISP_COLOR_TYPE_NORMAL, ISP_COLOR_TYPE_GRAY);
            scanf("%u", &(sImageSetting.eColor));
            break;
        case 6:
            printf("Enter Item Value(%d:True, %d:False) : ", NVT_TRUE, NVT_FALSE);
            scanf("%d", &(sImageSetting.bMirror));
            break;
        case 7:
            printf("Enter Item Value(%d:True, %d:False) : ", NVT_TRUE, NVT_FALSE);
            scanf("%d", &(sImageSetting.bFlip));
            break;
        case 8:
            printf("Enter Item Value(%d:Night, %d:Day, %d:Auto) : ", ISP_IRCUT_TYPE_NIGHT, ISP_IRCUT_TYPE_DAY, ISP_IRCUT_TYPE_AUTO);
            scanf("%u", &(sImageSetting.eIRCutMode));
            break;
        case 9:
           printf("Enter Item value(0:Disable, 1:Enable) : ");
           scanf("%d", &(sImageSetting.bDewarp));
        default:
            printf("Invalid option(%u) !\n", selectItem);
            break;
    }

    NvtISP_SetImageSetting(uiViChannelID, &sImageSetting);
}

static void test_ae_setting(UINT8 uiViChannelID)
{
    ISPAESetting sAESetting={0};
    unsigned int selectItem=0, selectSubItem=0;
    unsigned int row=0, col=0, rowOffset=0, rowStart=0, rowEnd=0, colStart=0, colEnd=0;

    NvtISP_GetAESetting(uiViChannelID, &sAESetting);

    printf("Current Auto Exposure Setting\n");
    printf("==================================================\n");
    printf("AE Mode           = %s\n", sAESetting.eAEMode == ISP_CTRL_MODE_AUTO ? "Auto" : "Manual");
    printf("Shutter Max       = %u us\n", sAESetting.uiShutterSpeedMax);
    printf("Shutter Min       = %u us\n", sAESetting.uiShutterSpeedMin);
    printf("Gain Max          = %u\n", sAESetting.uiGainMax);
    printf("Gain Min          = %u\n", sAESetting.uiGainMin);
    printf("Convergence Speed = %u\n", sAESetting.uiConvergenceSpeed);
    // sAESetting.uiConvergenceStep
    printf("Manual Shutter    = %u us\n", sAESetting.uiManualShutterSpeed);
    printf("Manual Gain       = %u\n", sAESetting.uiManualGain);
    switch (sAESetting.eAntiFlicker)
    {
        case ISP_ANTIFLICKER_50HZ:
            printf("Anti-Flicker      = 50Hz\n");
            break;
        case ISP_ANTIFLICKER_60HZ:
            printf("Anti-Flicker      = 60Hz\n");
            break;
        case ISP_ANTIFLICKER_AUTO:
            printf("Anti-Flicker      = Auto\n");
            break;
        case ISP_ANTIFLICKER_NONE:
        default:
            printf("Anti-Flicker      = None\n");
            break;
    }
    // sAESetting.sIrisSetting
    printf("EV                = %u \n", sAESetting.uiEV);
    printf("Tolerance         = %u \n", sAESetting.uiAETolerance);
    printf("Delay Frame       = %u \n", sAESetting.uiAEDelay);
//    printf("Strategy          = %s \n", sAESetting.eAEStrategy == ISP_AE_STRATEGY_HIGHLIGHT ? "Highlight" : "Low");
    printf("Area Weight       = \n");
    for(row = 0; row < AE_EXP_ROW_NUM; row++)
    {
        printf("  ");
        if (row != 0)
            rowOffset += AE_EXP_COLUMN_NUM;
        for (col = 0; col < AE_EXP_COLUMN_NUM; col++)
            printf("%4u", sAESetting.uiExpAreaWeight[rowOffset+col]);
        printf("\n");
    }
    printf("==================================================\n");

    printf("Test Item Option\n"
           " 0) AE Mode\n"
           " 1) Shutter Max\n"
           " 2) Shutter Min\n"
           " 3) Gain Max\n"
           " 4) Gain Min\n"
           " 5) Convergence Speed\n"
           " 6) Manual Shutter\n"
           " 7) Manual Gain\n"
           " 8) Anti Flicket\n"
           " 9) EV\n"
           "10) Tolerance\n"
           "11) Delay Frame\n"
 //          "12) Strategy\n"
           "12) Area Weight\n"
           "Please enter your select: ");
    scanf("%u", &selectItem);

    switch (selectItem)
    {
        case 0:
            printf("Enter Item Value(%d:Auto, %d:Manual) : ", ISP_CTRL_MODE_AUTO, ISP_CTRL_MODE_MANUAL);
            scanf("%u", &(sAESetting.eAEMode));
            break;
        case 1:
            printf("Enter Item Value : ");
            scanf("%lu", &(sAESetting.uiShutterSpeedMax));
            break;
        case 2:
            printf("Enter Item Value : ");
            scanf("%lu", &(sAESetting.uiShutterSpeedMin));
            break;
        case 3:
            printf("Enter Item Value : ");
            scanf("%lu", &(sAESetting.uiGainMax));
            break;
        case 4:
            printf("Enter Item Value : ");
            scanf("%lu", &(sAESetting.uiGainMin));
            break;
        case 5:
            printf("Enter Item Value : ");
            scanf("%lu", &(sAESetting.uiConvergenceSpeed));
            break;
        case 6:
            printf("Enter Item Value : ");
            scanf("%lu", &(sAESetting.uiManualShutterSpeed));
            break;
        case 7:
            printf("Enter Item Value : ");
            scanf("%lu", &(sAESetting.uiManualGain));
            break;
        case 8:
            printf("Enter Item Value(%d:50Hz, %d:60Hz, %d:Auto, %d:None) : ",
                   ISP_ANTIFLICKER_50HZ,
                   ISP_ANTIFLICKER_60HZ,
                   ISP_ANTIFLICKER_AUTO,
                   ISP_ANTIFLICKER_NONE);
            scanf("%u", &(sAESetting.eAntiFlicker));
            break;
        case 9:
            printf("Enter Item Value : ");
            scanf("%lu", &(sAESetting.uiEV));
            break;
        case 10:
            printf("Enter Item Value : ");
            scanf("%lu", &(sAESetting.uiAETolerance));
            break;
        case 11:
            printf("Enter Item Value : ");
            scanf("%lu", &(sAESetting.uiAEDelay));
            break;
     //   case 12:
     //       printf("Enter Item Value(%d:Highlight, %d:Lowlight) : ", ISP_AE_STRATEGY_HIGHLIGHT, ISP_AE_STRATEGY_LOWLIGHT);
     //       scanf("%u", &(sAESetting.eAEStrategy));
     //       break;
        case 12:
            printf("Enter Item Value(0:Average, 1:Center, 2:Left, 3:Right, 4:Top, 5:Bottom) : ");
            scanf("%u", &selectSubItem);
            memset(sAESetting.uiExpAreaWeight, 1, sizeof(UINT8) * AE_EXP_ROW_NUM * AE_EXP_COLUMN_NUM);
            switch (selectSubItem)
            {
                case 1:
                    rowStart = AE_EXP_ROW_NUM / 4;
                    rowEnd = (rowStart * 3) - 1;
                    colStart = AE_EXP_COLUMN_NUM / 4;
                    colEnd = (colStart * 3) - 1;
                    break;
                case 2:
                    rowStart = 0;
                    rowEnd = AE_EXP_ROW_NUM - 1;
                    colStart = 0;
                    colEnd = (AE_EXP_COLUMN_NUM / 2) - 1;
                    break;
                case 3:
                    rowStart = 0;
                    rowEnd = AE_EXP_ROW_NUM - 1;
                    colStart = AE_EXP_COLUMN_NUM / 2;
                    colEnd = AE_EXP_COLUMN_NUM - 1;
                    break;
                case 4:
                    rowStart = 0;
                    rowEnd = (AE_EXP_ROW_NUM / 2) - 1;
                    colStart = 0;
                    colEnd = AE_EXP_COLUMN_NUM - 1;
                    break;
                case 5:
                    rowStart = AE_EXP_ROW_NUM / 2;
                    rowEnd = AE_EXP_ROW_NUM - 1;
                    colStart = 0;
                    colEnd = AE_EXP_COLUMN_NUM - 1;
                    break;
                default:
                    break;
            }

            for (row = rowStart; row <= rowEnd; row++)
            {
                rowOffset = AE_EXP_COLUMN_NUM * row;
                memset(sAESetting.uiExpAreaWeight + rowOffset + colStart, 4,
                       colEnd - colStart + 1);
            }
            break;
        default:
            printf("Invalid option(%u) !\n", selectItem);
            break;
    }

    NvtISP_SetAESetting(uiViChannelID, &sAESetting);
}

/* This variable is too large to put in stack memory, so
   put it in global memory */
ISPAEStatus sAEStatus={0};

static void test_ae_status(UINT8 uiViChannelID)
{
    UINT8 rowNum, columnNum;
    UINT32 row=0, col=0, rowOffset=0;

    printf("Please enter luma row number(4~32): ");
    scanf("%hhu", &rowNum);

    printf("Please enter luma column number(4~32): ");
    scanf("%hhu", &columnNum);

    NvtISP_GetAEStatus(uiViChannelID, &sAEStatus, rowNum, columnNum);

    printf("Auto Exposure Status\n");
    printf("==================================================\n");
    printf("Shutter        = %u us\n", sAEStatus.uiShutter);
    printf("Gain           = %u\n", sAEStatus.uiGain);
    printf("Average Luma   = %u\n", sAEStatus.uiLumaAverage);
    printf("Luma Value     =\n");
    for (col = 0; col < columnNum; col++)
    {
        printf("  ");
        if (col != 0)
            rowOffset += rowNum;
        for (row = 0; row < rowNum; row++)
            printf("%5u", sAEStatus.uiLumaValue[rowOffset+row]);
        printf("\n");
    }
    printf("Luma Histogram =");
    for (col = 0; col < AE_LUMA_HIST_NUM; col++)
    {
        if ((col % 8) == 0)
            printf("\n%3u: ", col);

        printf("%7u", sAEStatus.uiLumaHistogram[col]);
    }
    printf("\n");
    printf("==================================================\n");
}

static void test_wb_setting(UINT8 uiViChannelID)
{
    ISPWBSetting sWBSetting={0};
    unsigned int selectItem=0;

    NvtISP_GetWBSetting(uiViChannelID, &sWBSetting);

    printf("Current White Balance Test Setting\n");
    printf("==================================================\n");
    printf("WB Mode                = %s\n", sWBSetting.eWBMode == ISP_CTRL_MODE_AUTO ? "Auto" : "Manual");
    printf("Convergence Skip Frame = %u\n", sWBSetting.uiConvergenceSkipFrame);
    printf("Convergence Speed      = %u\n", sWBSetting.uiConvergenceSpeed);
    printf("R ratio                = %u\n", sWBSetting.uiRratio);
    printf("B ratio                = %u\n", sWBSetting.uiBratio);    
    printf("Manual RGain           = %u\n", sWBSetting.uiManualRGain);
    printf("Manual GGain           = %u\n", sWBSetting.uiManualGGain);
    printf("Manual BGain           = %u\n", sWBSetting.uiManualBGain);
    printf("==================================================\n");

    printf("Test Item Option\n"
           " 0) WB Mode\n"
           " 1) Convergence Skip Frame\n"
           " 2) Convergence Speed\n"
           " 3) R ratio\n"
           " 4) B ratio\n"           
           " 5) Manual RGain\n"
           " 6) Manual BGain\n"
           "Please enter your select: ");
    scanf("%u", &selectItem);

    switch (selectItem)
    {
        case 0:
            printf("Enter Item Value(%d:Auto, %d:Manual) : ", ISP_CTRL_MODE_AUTO, ISP_CTRL_MODE_MANUAL);
            scanf("%u", &(sWBSetting.eWBMode));
            break;
        case 1:
            printf("Enter Item Value : ");
            scanf("%lu", &(sWBSetting.uiConvergenceSkipFrame));
            break;
        case 2:
            printf("Enter Item Value : ");
            scanf("%lu", &(sWBSetting.uiConvergenceSpeed));
            break;
        case 3:
            printf("Enter Item Value : ");
            scanf("%lu", &(sWBSetting.uiRratio));
            break;
        case 4:
            printf("Enter Item Value : ");
            scanf("%lu", &(sWBSetting.uiBratio));
            break;            
        case 5:
            printf("Enter Item Value : ");
            scanf("%lu", &(sWBSetting.uiManualRGain));
            break;
        case 6:
            printf("Enter Item Value : ");
            scanf("%lu", &(sWBSetting.uiManualBGain));
            break;
        default:
            printf("Invalid option(%u) !\n", selectItem);
            break;
    }

    NvtISP_SetWBSetting(uiViChannelID, &sWBSetting);
}

static void test_wb_status(UINT8 uiViChannelID)
{
    ISPWBStatus sWBStatus={0};

    NvtISP_GetWBStatus(uiViChannelID, &sWBStatus);

    printf("Current Auto Exposure Status\n");
    printf("==================================================\n");
    printf("RGain             = %u\n", sWBStatus.uiRGain);
    printf("GGain             = %u\n", sWBStatus.uiGGain);
    printf("BGain             = %u\n", sWBStatus.uiBGain);
    printf("R ratio           = %u\n", sWBStatus.uiRratio);
    printf("B ratio           = %u\n", sWBStatus.uiBratio);    
    printf("Color Temperature = %u\n", sWBStatus.uiColorTemperature);
    printf("==================================================\n");
}

static void test_wdr_setting(UINT8 uiViChannelID)
{
    ISPWDRSetting sWDRSetting={0};
    unsigned int selectItem=0;

    NvtISP_GetWDRSetting(uiViChannelID, &sWDRSetting);

    printf("Current WDR Setting\n");
    printf("==================================================\n");
    printf("Mode           = %d  (%d--> off mode)\n", sWDRSetting.eWDRMode, ISP_CTRL_MODE_OFF);
    printf("Strength       = %u\n", sWDRSetting.uiWDRStrength);
    printf("==================================================\n");

    printf("Test Item Option\n"
           " 0) WDR Mode\n"
           " 1) WDR Strength\n"
           "Please enter your select: ");
    scanf("%u", &selectItem);
    switch (selectItem)
    {
        case 0:
            printf("Enter Item Value(%d:Auto, %d:Manual, %d:Off) : ", ISP_CTRL_MODE_AUTO, ISP_CTRL_MODE_MANUAL,ISP_CTRL_MODE_OFF );
            scanf("%u", &(sWDRSetting.eWDRMode));
            break;
        case 1:
            printf("Enter Item Value (0~128): ");
            scanf("%lu", &(sWDRSetting.uiWDRStrength));
            break;
        default:
            printf("Invalid option(%u) !\n", selectItem);
            break;
    }

    NvtISP_SetWDRSetting(uiViChannelID, &sWDRSetting);
}

static void test_dis_setting(UINT8 uiViChannelID)
{
    ISPDISSetting sDISSetting={0};
    unsigned int selectItem=0;

    NvtISP_GetDISSetting(uiViChannelID, &sDISSetting);

    printf("Current DIS Setting\n");
    printf("==================================================\n");
    printf("Enable         = %s\n", sDISSetting.bDISEnable== NVT_TRUE ? "On" : "Off");
    printf("==================================================\n");

    printf("Test Item Option\n"
           " 0) DIS Enable\n"
           "Please enter your select: ");
    scanf("%u", &selectItem);
    switch (selectItem)
    {
        case 0:
            printf("Enter Item Value(%d:Off, %d:On) : ", NVT_FALSE, NVT_TRUE);
            scanf("%d", &(sDISSetting.bDISEnable));
            break;
        default:
            printf("Invalid option(%u) !\n", selectItem);
            break;
    }

    NvtISP_SetDISSetting(uiViChannelID, &sDISSetting);
}

static void test_nr_setting(UINT8 uiViChannelID)
{
    ISPNRSetting sNRSetting={0};
    unsigned int selectItem=0;

    NvtISP_GetNRSetting(uiViChannelID, &sNRSetting);

    printf("Current IME NR Setting\n");
    printf("==================================================\n");
    printf("2DNR Strength         = %u\n", sNRSetting.uiNR2DStrength);
    printf("3DNR Enable         = %u\n", sNRSetting.uiNR3DStrength);
    printf("==================================================\n");

    printf("Test Item Option \n"
           " 0) 2DNR Strength\n"
           " 1) 3DNR Enable\n"
           "Please enter your select: ");
    scanf("%u", &selectItem);
    switch (selectItem)
    {
        case 0:
            printf("Enter Item Value (0~100): ");
            scanf("%lu", &(sNRSetting.uiNR2DStrength));
            break;
        case 1:
            printf("Enter Item Value (0:disable 1:enable): ");
            scanf("%lu", &(sNRSetting.uiNR3DStrength));
            break;
        default:
            printf("Invalid option(%u) !\n", selectItem);
            break;
   }

    NvtISP_SetNRSetting(uiViChannelID, &sNRSetting);
}
#if 1
static void test_ldc_setting(UINT8 uiViChannelID)
{
    ISPLDCSetting sLDCSetting={0};
    unsigned int selectItem=0;

    NvtISP_GetLDCSetting(uiViChannelID, &sLDCSetting);

    printf("Current LDC Setting\n");
    printf("==================================================\n");
    printf("Enable         = %s\n", sLDCSetting.bLDCEnable == NVT_TRUE ? "On" : "Off");
    printf("Ratio          = %u\n", sLDCSetting.uiRatio);
    printf("==================================================\n");

    printf("Test Item Option\n"
           " 0) LDC Enable\n"
           " 1) LDC Ratio\n"
           "Please enter your select: ");
    scanf("%u", &selectItem);
    switch (selectItem)
    {
        case 0:
            printf("Enter Item Value(%d:Off, %d:On) : ", NVT_FALSE, NVT_TRUE);
            scanf("%d", &(sLDCSetting.bLDCEnable));
            break;
        case 1:
            printf("Enter Item Value : ");
            scanf("%lu", &(sLDCSetting.uiRatio));
            break;
        default:
            printf("Invalid option(%u) !\n", selectItem);
            break;
    }

    NvtISP_SetLDCSetting(uiViChannelID, &sLDCSetting);
}
#endif
#if 1
static void test_lsc_setting(UINT8 uiViChannelID)
{
    ISPLSCSetting sLSCSetting={0};
    unsigned int selectItem=0;

    NvtISP_GetLSCSetting(uiViChannelID, &sLSCSetting);

    printf("Current LSC Setting\n");
    printf("==================================================\n");
    printf("Enable         = %s\n", sLSCSetting.bLSCEnable == NVT_TRUE ? "On" : "Off");
    printf("Ratio          = %u\n", sLSCSetting.uiRatio);
    printf("==================================================\n");

    printf("Test Item Option\n"
           " 0) LSC Enable\n"
           " 1) LSC Ratio\n"
           "Please enter your select: ");
    scanf("%u", &selectItem);
    switch (selectItem)
    {
        case 0:
            printf("Enter Item Value(%d:Off, %d:On) : ", NVT_FALSE, NVT_TRUE);
            scanf("%d", &(sLSCSetting.bLSCEnable));
            break;
        case 1:
            printf("Enter Item Value (0~100): ");
            scanf("%lu", &(sLSCSetting.uiRatio));
            break;
        default:
            printf("Invalid option(%u) !\n", selectItem);
            break;
    }

    NvtISP_SetLSCSetting(uiViChannelID, &sLSCSetting);
}
#endif
#if 0
static void user_define_gamma(unsigned int userIdx, UINT16 *uiUserGammaTable)
{
    unsigned int i, tPoint;

    if (userIdx == 0)
    {
        for (i = 0; i < GAMMA_NODE_NUM; i++)
        {
            if (i == (GAMMA_NODE_NUM - 1))
                uiUserGammaTable[i] = 1023;
            else
                uiUserGammaTable[i] = 8 * i;

            if (uiUserGammaTable[i] > 1023)
                uiUserGammaTable[i]= 1023;
        }
    }
    else
    {
        tPoint = (GAMMA_NODE_NUM / 3) * 2;
        for (i = 0; i < tPoint; i++)
            uiUserGammaTable[i] = 10 * i;

        for (i = tPoint; i < GAMMA_NODE_NUM; i++)
            uiUserGammaTable[i] = uiUserGammaTable[tPoint-1] + (2 * i);

        if (uiUserGammaTable[i] > 1023)
            uiUserGammaTable[i]= 1023;
    }

    return;
}
static void test_gamma_setting(UINT8 uiViChannelID)
{
    ISPGammaSetting sGammaSetting={0};
    unsigned int selectItem=0, selectSubItem=0;
    unsigned int i;

    NvtISP_GetGammaSetting(uiViChannelID, &sGammaSetting);

    printf("Current Gamma Setting\n");
    printf("==================================================\n");
    printf("Gamma Curve Mode   = ");
    switch (sGammaSetting.eGammaCurveMode)
    {
        case ISP_GAMMA_CURVE_MODE_LINEAR:
            printf("Linear\n");
            break;
        case ISP_GAMMA_CURVE_MODE_USER:
            printf("User Define\n");
            break;
        case ISP_GAMMA_CURVE_MODE_16:
            printf("1.6\n");
            break;
        case ISP_GAMMA_CURVE_MODE_18:
            printf("1.8\n");
            break;
        case ISP_GAMMA_CURVE_MODE_20:
            printf("2.0\n");
            break;
        case ISP_GAMMA_CURVE_MODE_22:
            printf("2.2\n");
            break;
        case ISP_GAMMA_CURVE_MODE_24:
            printf("2.4\n");
            break;
        case ISP_GAMMA_CURVE_MODE_DEFAULT:
        default:
            printf("Default\n");
            break;
    }
    printf("Gamma Table        = \n");
    for (i = 0; i < GAMMA_NODE_NUM; i++)
        printf("%5u", sGammaSetting.uiUserGammaTable[i]);
    printf("\n");
    printf("==================================================\n");

    printf("Test Item Option\n"
           " 0) Gamma Curve Mode\n"
           " 1) Gamma Table\n"
           "Please enter your select: ");
    scanf("%u", &selectItem);
    switch (selectItem)
    {
        case 0:
            printf("%2u) Linear\n", ISP_GAMMA_CURVE_MODE_LINEAR);
            printf("%2u) Default\n", ISP_GAMMA_CURVE_MODE_DEFAULT);
            printf("%2u) User Define\n", ISP_GAMMA_CURVE_MODE_USER);
            printf("%2u) 1.6\n", ISP_GAMMA_CURVE_MODE_16);
            printf("%2u) 1.8\n", ISP_GAMMA_CURVE_MODE_18);
            printf("%2u) 2.0\n", ISP_GAMMA_CURVE_MODE_20);
            printf("%2u) 2.2\n", ISP_GAMMA_CURVE_MODE_22);
            printf("%2u) 2.4\n", ISP_GAMMA_CURVE_MODE_24);
            printf("Please enter your select: ");
            scanf("%u", &(sGammaSetting.eGammaCurveMode));
            break;
        case 1:
            printf("Enter Item Option(0: User1, 1: User2) : ");
            scanf("%u", &selectSubItem);
            user_define_gamma(selectSubItem, sGammaSetting.uiUserGammaTable);
            break;
        default:
            printf("Invalid option(%u) !\n", selectItem);
            break;
    }

    NvtISP_SetGammaSetting(uiViChannelID, &sGammaSetting);
}

static void user_define_ccm(unsigned int profileIdx, ISPCCMSetting *psCCMSetting)
{
    if (psCCMSetting == NULL)
    {
        DBG_ERR("psCCMSetting is NULL!\n");
        return;
    }

    if (profileIdx == 0)
    {
        psCCMSetting->uiLowCCM[0] = 256;
        psCCMSetting->uiLowCCM[1] = 0;
        psCCMSetting->uiLowCCM[2] = 0;
        psCCMSetting->uiLowCCM[3] = 0;
        psCCMSetting->uiLowCCM[4] = 256;
        psCCMSetting->uiLowCCM[5] = 0;
        psCCMSetting->uiLowCCM[6] = 0;
        psCCMSetting->uiLowCCM[7] = 0;
        psCCMSetting->uiLowCCM[8] = 256;
        psCCMSetting->uiMiddleCCM[0] = 542;
        psCCMSetting->uiMiddleCCM[1] = 1697;
        psCCMSetting->uiMiddleCCM[2] = 65;
        psCCMSetting->uiMiddleCCM[3] = 1905;
        psCCMSetting->uiMiddleCCM[4] = 487;
        psCCMSetting->uiMiddleCCM[5] = 1961;
        psCCMSetting->uiMiddleCCM[6] = 2037;
        psCCMSetting->uiMiddleCCM[7] = 1780;
        psCCMSetting->uiMiddleCCM[8] = 535;
        psCCMSetting->uiHighCCM[0] = 0;
        psCCMSetting->uiHighCCM[1] = 256;
        psCCMSetting->uiHighCCM[2] = 0;
        psCCMSetting->uiHighCCM[3] = 0;
        psCCMSetting->uiHighCCM[4] = 256;
        psCCMSetting->uiHighCCM[5] = 0;
        psCCMSetting->uiHighCCM[6] = 0;
        psCCMSetting->uiHighCCM[7] = 256;
        psCCMSetting->uiHighCCM[8] = 0;
    }
    else
    {
        psCCMSetting->uiLowCCM[0] = 256;
        psCCMSetting->uiLowCCM[1] = 1200;
        psCCMSetting->uiLowCCM[2] = 0;
        psCCMSetting->uiLowCCM[3] = 0;
        psCCMSetting->uiLowCCM[4] = 256;
        psCCMSetting->uiLowCCM[5] = 1200;
        psCCMSetting->uiLowCCM[6] = 0;
        psCCMSetting->uiLowCCM[7] = 0;
        psCCMSetting->uiLowCCM[8] = 256;
        psCCMSetting->uiMiddleCCM[0] = 542;
        psCCMSetting->uiMiddleCCM[1] = 1897;
        psCCMSetting->uiMiddleCCM[2] = 65;
        psCCMSetting->uiMiddleCCM[3] = 1205;
        psCCMSetting->uiMiddleCCM[4] = 487;
        psCCMSetting->uiMiddleCCM[5] = 1161;
        psCCMSetting->uiMiddleCCM[6] = 2037;
        psCCMSetting->uiMiddleCCM[7] = 780;
        psCCMSetting->uiMiddleCCM[8] = 535;
        psCCMSetting->uiHighCCM[0] = 0;
        psCCMSetting->uiHighCCM[1] = 256;
        psCCMSetting->uiHighCCM[2] = 1200;
        psCCMSetting->uiHighCCM[3] = 0;
        psCCMSetting->uiHighCCM[4] = 256;
        psCCMSetting->uiHighCCM[5] = 0;
        psCCMSetting->uiHighCCM[6] = 1200;
        psCCMSetting->uiHighCCM[7] = 256;
        psCCMSetting->uiHighCCM[8] = 0;
    }
}

static void test_ccm_setting(UINT8 uiViChannelID)
{
    ISPCCMSetting sCCMSetting={0};
    unsigned int selectItem=0, selectSubItem=0;
    unsigned int i;

    NvtISP_GetCCMSetting(uiViChannelID, &sCCMSetting);

    printf("Current CCM Setting\n");
    printf("==================================================\n");
    printf("CCM Mode                     = %s\n", sCCMSetting.eCCMMode == ISP_CTRL_MODE_AUTO ? "Auto" : "Manual");
    printf("Low Color Temperature CCM    = \n");
    for (i = 0; i < 9; i++)
    {
        printf("%5u", sCCMSetting.uiLowCCM[i]);
        if (((i + 1) % 3) == 0)
            printf("\n");
    }

    printf("\nMiddle Color Temperature CCM = \n");
    for (i = 0; i < 9; i++)
    {
        printf("%5u", sCCMSetting.uiMiddleCCM[i]);
        if (((i + 1) % 3) == 0)
            printf("\n");
    }

    printf("\nHigh Color Temperature CCM   = \n");
    for (i = 0; i < 9; i++)
    {
        printf("%5u", sCCMSetting.uiHighCCM[i]);
        if (((i + 1) % 3) == 0)
            printf("\n");
    }
    printf("==================================================\n");

    printf("Test Item Option\n"
           " 0) CCM Mode\n"
           " 1) CCM Table\n"
           "Please enter your select: ");
    scanf("%u", &selectItem);
    switch (selectItem)
    {
        case 0:
            printf("Enter Item Option(0: Auto, 1: Manual) : ");
            scanf("%u", &(sCCMSetting.eCCMMode));
            break;
        case 1:
            printf("Enter Item Option(0: Profile1, 1: Profile2) : ");
            scanf("%u", &selectSubItem);
            user_define_ccm(selectSubItem, &sCCMSetting);
            break;
        default:
            printf("Invalid option(%u) !\n", selectItem);
            break;
    }

    NvtISP_SetCCMSetting(uiViChannelID, &sCCMSetting);
}
#endif

#if 0
static void test_pip_setting(UINT8 uiViChannelID){

    printf("not support!\r\n");
    ISPPIPSetting sPIPSetting={0};
    unsigned int selectItem=0;
    NvtISP_GetPIPSetting(uiViChannelID, &sPIPSetting);
    printf("Current PIP Setting\n");
    printf("==================================================\n");
    printf("Enable	= %s\n", sPIPSetting.bEnable == NVT_TRUE ? "on" : "off");
    printf("==================================================\n");

    printf("Test Item Option\n");
    printf(" 0) PIP Enable\n");
    printf("Please enter your select: ");
    scanf("%u", &selectItem);
    switch(selectItem){
        
        case 0:
            printf("Enter Item Value(%d:Off, %d:On) : ", NVT_FALSE, NVT_TRUE);
            scanf("%d", &(sPIPSetting.bEnable));
            NvtISP_SetPIPSetting(uiViChannelID, &sPIPSetting);
            
            break;
        default:
            printf("Invalid option(%u) !\n", selectItem);
            break;



    }
}
#endif
#if 0
static void test_sbs_setting(UINT8 uiViChannelID){



    ISPSBSSetting sSBSSetting={0};
    unsigned int selectItem=0;
    NvtISP_GetSBSSetting(uiViChannelID, &sSBSSetting);
    printf("Current SBS Setting\n");
    printf("==================================================\n");
    printf("Mode      = %d\n", sSBSSetting.eSBSMode);
    printf("==================================================\n");

    printf("Test Item Option\n");
    printf(" 0) SBS Mode\n");
    printf("Please enter your select: ");
    scanf("%u", &selectItem);
    switch(selectItem){

        case 0:
            printf("Enter Item Value(0: OFF, 1: left right, 2: up down : ");
            scanf("%u", &(sSBSSetting.eSBSMode));
            NvtISP_SetSBSSetting(uiViChannelID, &sSBSSetting);

            break;
        default:
            printf("Invalid option(%u) !\n", selectItem);
            break;



    }
}
#endif
#if 1
static void test_defog_setting(UINT8 uiViChannelID)
{
    ISPDeFogSetting sDeFogSetting={0};
    unsigned int selectItem=0;

    NvtISP_GetDeFogSetting(uiViChannelID, &sDeFogSetting);

    printf("Current DeFog Setting\n");
    printf("==================================================\n");
    printf("Mode           = %s\n", sDeFogSetting.eDeFogMode == ISP_DEFOG_ON ? "On" : "Off");
    printf("Strength       = %u\n", sDeFogSetting.uiDeFogStrength);
    printf("==================================================\n");

    printf("Test Item Option\n"
           " 0) DeFog Mode\n"
           " 1) DeFog Strength\n"
           "Please enter your select: ");
    scanf("%u", &selectItem);
    switch (selectItem)
    {
        case 0:
            printf("Enter Item Value(%d:Off, %d:On) : ", ISP_DEFOG_OFF, ISP_DEFOG_ON);
            scanf("%u", &(sDeFogSetting.eDeFogMode));
            break;
        case 1:
            printf("Enter Item Value(0~255) : ");
            scanf("%lu", &(sDeFogSetting.uiDeFogStrength));
            break;
        default:
            printf("Invalid option(%u) !\n", selectItem);
            break;
    }

    NvtISP_SetDeFogSetting(uiViChannelID, &sDeFogSetting);
}

static void test_imescale_setting(UINT8 uiViChannelID){

    ISPIMEScaleSetting sIMEScaleSetting={0};
    NvtISP_GetIMEScaleSetting(uiViChannelID, &sIMEScaleSetting);
   
    unsigned int selectItem=0;
 
    printf("Current IME Scale Type Setting\n");
    printf("==================================================\n");
    printf("Thredhold       = 0x%x\n",sIMEScaleSetting.uiRatioThreshold);
    printf("eMathodLarge    = %u\n",sIMEScaleSetting.eMathodLarge);
    printf("eMathodSmall    = %u\n",sIMEScaleSetting.eMathodSmall);
    printf("==================================================\n");

    printf("Test Item Option\n"
           "0) Thredhold\n"
           "1) Large Method\n"
           "2) Small Method\n"
           "Please enter your select: ");
    scanf(" %u", &selectItem);

    switch (selectItem){
        case 0:
            printf("Enter Item Value : ");
            scanf("%lu", &(sIMEScaleSetting.uiRatioThreshold));
            break;
        case 1:
            printf("Enter Item Option\n"
                   "0) ISP_IME_BICUBIC\n"
                   "1) ISP_IME_BILINEAR\n"
                   "2) ISP_IME_NEAREST\n"
                   "3) ISP_IME_INTEGRATION\n"
                   "4) ISP_IME_AUTO\n"
                   "Please enter your select: ");
            scanf("%u", &(sIMEScaleSetting.eMathodLarge));
            break;
        case 2:
            printf("Enter Item Option\n"
                   "0) ISP_IME_BICUBIC\n"
                   "1) ISP_IME_BILINEAR\n"
                   "2) ISP_IME_NEAREST\n"
                   "3) ISP_IME_INTEGRATION\n"
                   "4) ISP_IME_AUTO\n"
                   "Please enter your select: ");
            scanf("%u", &(sIMEScaleSetting.eMathodSmall));

            break;
    }

    NvtISP_SetIMEScaleSetting(uiViChannelID, &sIMEScaleSetting); 
}



#endif
static void test_raw_data(UINT8 uiViChannelID){

    char raw_buf[1024*1024*5]={0};
    ISPTuningRawInfo sRawInfo ={0};
    sRawInfo.uiAddr = (UINT32)raw_buf;
    sRawInfo.uiRawsize = sizeof(raw_buf);

    NvtISP_GetRawInfo(uiViChannelID, &sRawInfo);

    FILE *pf =NULL;
    pf = fopen("/mnt/sd/raw0.raw","wb");
    fwrite(raw_buf, 1, sRawInfo.uiRawsize, pf);
    fclose(pf);
    printf("finish save raw data\r\n");
    printf("RAW_info====================\r\n");
    printf("width\t\t\t:%d\n",sRawInfo.uiWidth);
    printf("uiHeight\t\t:%d\n",sRawInfo.uiHeight);
    printf("uiBits\t\t\t:%d\n",sRawInfo.uiBits);
    printf("uiStartpix\t\t:%d\n",sRawInfo.uiStartpix);
    printf("uiB2strp\t\t:%d\n",sRawInfo.uiB2strp);
    printf("uiLineoffset[0]\t\t:%d\n",sRawInfo.uiLineoffset[0]);
    printf("uiLineoffset[1]\t\t:%d\n",sRawInfo.uiLineoffset[1]);
    printf("uiOverlap\t\t:%d\n",sRawInfo.uiOverlap);
    printf("uiRawsize\t\t:%d\n",sRawInfo.uiRawsize);

}

static void test_hdr_setting(UINT8 uiViChannelID){

    ISPHDRSetting sHDRSetting={0};

    NvtISP_GetHDRSetting(uiViChannelID, &sHDRSetting);

    printf("Current HDR Setting\n");
    printf("==================================================\n");
    printf("Enable         = %s\n", sHDRSetting.bHDREnable == NVT_TRUE ? "On" : "Off");
    printf("Frame Number   = %d\n", sHDRSetting.uiFrameNum);
    printf("==================================================\n");


    printf("Enter Enable Value(0:Off, 1:On)\n");
    scanf(" %d", &(sHDRSetting.bHDREnable));

    if(sHDRSetting.bHDREnable > 1){
        sHDRSetting.bHDREnable = 1;
    }

    printf("Enter Frame Number Value (1~4)\n");
    printf("IMX290 support  2~3\n");
    scanf(" %lu", &sHDRSetting.uiFrameNum);

    NvtISP_SetHDRSetting(uiViChannelID, &sHDRSetting);

}



static void test_crop_setting(UINT8 uiViChannelID){


    unsigned int selectItem=0;
    unsigned int IMEIndex =0;
    ISPImageCropSetting CropSetting = {0};


    printf("Please Select IME Value:\r\n");
    printf(" 0) IME output1\r\n");
    printf(" 1) IME output2\r\n");
    printf(" 2) IME output3\r\n");
    printf(" 3) IME output4\r\n");
    printf(" 4) IME output5\r\n");

    scanf(" %u",&IMEIndex);
   
    NvtISP_GetCropSetting(uiViChannelID, IMEIndex, &CropSetting);

    printf("\nCurrent Sensor %d IME %d Setting\r\n", uiViChannelID,IMEIndex);
    printf("========================\r\n");
    printf("Enable                              =    %u\r\n",CropSetting.bEnable);
    printf("Sensor Input Width (read only)      =    %u\r\n",CropSetting.uiSensorWidth);
    printf("Sensor Input Height (read only)     =    %u\r\n",CropSetting.uiSensorHeight);
    printf("x(reference sensor input size)      =    %u\r\n",CropSetting.sCropInfo.x);
    printf("y(reference sensor input size)      =    %u\r\n",CropSetting.sCropInfo.y);
    printf("width(reference sensor input size)  =    %u\r\n",CropSetting.sCropInfo.w);
    printf("hieght(reference sensor input size) =    %u\r\n",CropSetting.sCropInfo.h);
    printf("Scale Width                         =    %u\r\n",CropSetting.uiScaleWidth);
    printf("Scale Height                        =    %u\r\n",CropSetting.uiScaleHeight);
    printf("========================\r\n");

    printf("Please Enter Enable Value: \r\n");
    printf(" 0) Disable\r\n");
    printf(" 1) Enable\r\n");
    scanf(" %u",&selectItem);
    CropSetting.bEnable = selectItem;
    CropSetting.uiSensor = (UINT32)uiViChannelID;
    CropSetting.uiIME = IMEIndex;
    if( CropSetting.bEnable == 0){

        CropSetting.sCropInfo.x = 0;
        CropSetting.sCropInfo.y = 0;
        CropSetting.sCropInfo.w = 0;
        CropSetting.sCropInfo.h = 0;
        CropSetting.uiScaleWidth = 0;
        CropSetting.uiScaleHeight = 0;

    }
    else if(CropSetting.bEnable == 1){

        printf("Please Enter Scaling Width value:\r\n");
        scanf(" %u",&selectItem);
        CropSetting.uiScaleWidth = selectItem ;

        printf("Please Enter Scaling Height value:\r\n");
        scanf(" %u",&selectItem);
        CropSetting.uiScaleHeight = selectItem ;

        printf("Please Enter Crop x value:\r\n");
        scanf(" %u",&selectItem);
        CropSetting.sCropInfo.x = selectItem ;
        
        printf("Please Enter Crop y value:\r\n");
        scanf(" %u",&selectItem);
        CropSetting.sCropInfo.y = selectItem ;

        printf("Please Enter Crop width value:\r\n");
        scanf(" %u",&selectItem);
        CropSetting.sCropInfo.w = selectItem ;

        printf("Please Enter Crop height value:\r\n");
        scanf(" %u",&selectItem);
        CropSetting.sCropInfo.h = selectItem ;

    }
    else{
        return;

    }

    NvtISP_SetCropSetting(uiViChannelID, &CropSetting);

}

int main(int argc, char *argv[])
{
    UINT8 ViChannelID;
    unsigned int selectItem=0;
    BOOL flagTestStop=NVT_FALSE;

    if (argc != 2)
    {
        show_help();
        return 0;
    }

    ViChannelID = atoi(argv[1]);

    do {
        printf("ISP category:\n");
        printf(" %d) Image Setting\n",ISP_TEST_IMAGE_SETTING);
        printf(" %d) Auto Exposure Setting\n",ISP_TEST_AE_SETTING);
        printf(" %d) Auto Exposure Status\n",ISP_TEST_AE_STATUS);
        printf(" %d) White Balance Setting\n",ISP_TEST_WB_SETTING);
        printf(" %d) White Balance Status\n",ISP_TEST_WB_STATUS);
        printf(" %d) WDR Setting\n",ISP_TEST_WDR_SETTING);
        printf(" %d) HDR Setting\n",ISP_TEST_HDR_SETTING);
        printf(" %d) IME NR Setting\n",ISP_TEST_NR_SETTING);
        printf(" %d) DIS Setting\n",ISP_DIS_SETTING);
        printf(" %d) Get RAW Data\n",ISP_GET_RAW_DATA);
        printf(" %d) Crop Setting\n",ISP_CROP_SETTING);
        printf(" %d) LDC Setting\n",ISP_LDC_SETTING);
        printf(" %d) LSC Setting\n",ISP_LSC_SETTING);
        printf(" %d) DeFog Setting\n",ISP_DEFOG_SETTING);
        printf(" %d) IME Scale Type Setting\n",ISP_IMESCALE_SETTING);
#if 0
        printf("12) PIP Setting\n");
        printf("13) SBS (side by side) Setting\n");
#endif
        printf("%d) *** Test End ***\n",ISP_TEST_MAX);
        printf("Please enter your select: ");
        scanf("%u", &selectItem);
        switch (selectItem)
        {
            case ISP_TEST_IMAGE_SETTING:
                test_image_setting(ViChannelID);
                break;
            case ISP_TEST_AE_SETTING:
                test_ae_setting(ViChannelID);
                break;
            case ISP_TEST_AE_STATUS:
                test_ae_status(ViChannelID);
                break;
            case ISP_TEST_WB_SETTING:
                test_wb_setting(ViChannelID);
                break;
            case ISP_TEST_WB_STATUS:
                test_wb_status(ViChannelID);
                break;
            case ISP_TEST_WDR_SETTING:
                test_wdr_setting(ViChannelID);
                break;
            case ISP_TEST_HDR_SETTING:
                test_hdr_setting(ViChannelID);
                break;
            case ISP_TEST_NR_SETTING:
                test_nr_setting(ViChannelID);
                break;
            case ISP_DIS_SETTING:
                test_dis_setting(ViChannelID);
                break;
            case ISP_LDC_SETTING:
                test_ldc_setting(ViChannelID);
                break;
            case ISP_LSC_SETTING:
                test_lsc_setting(ViChannelID);
                break;
            case ISP_DEFOG_SETTING:
                test_defog_setting(ViChannelID);
                break;
            case ISP_IMESCALE_SETTING:
                test_imescale_setting(ViChannelID);
                break;
#if 0
            case 12:
                test_pip_setting(ViChannelID);
                break;
            case 13:
                test_sbs_setting(ViChannelID);
               break;
#endif
            case ISP_GET_RAW_DATA:
                test_raw_data(ViChannelID);
                break;
            case ISP_CROP_SETTING:
                test_crop_setting(ViChannelID);
                break;
            default:
                flagTestStop = NVT_TRUE;
                break;
        }
    } while(flagTestStop != NVT_TRUE);

    return 0;
}




