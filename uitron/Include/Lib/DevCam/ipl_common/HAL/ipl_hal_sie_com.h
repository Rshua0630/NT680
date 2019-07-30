/**
    IPH module (SIE type define)

    @file       IPL_Hal_SIE.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_SIE_COM_H
#define _IPL_Hal_SIE_COM_H
#include "ipl_hal_sie_com_info.h"
/** \addtogroup mILibIPH */
//@{

/**
    query enum

    read from hw register
*/
typedef enum _IPH_SIE_QUE {
	SIE_REG_CARST           = 0x00000001,   ///< read CA result(R,G,B average)         Data type: SIE_CARST  eng: SIE1234578
	SIE_REG_LARST           = 0X00000002,   ///< read LA result(Luminacne)              Data type: SIE_VARST  eng: SIE1234578
	SIE_REG_LARST_MANUAL    = 0X00000003,   ///< read LA result(Luminacne)              Data type: SIE_VARST  eng: SIE1234578
	SIE_REG_VARST           = 0X00000004,   ///< read VA result()                       Data type: SIE_VARST  eng: SIE1,
	SIE_REG_VA_INDEP_RST    = 0X00000005,   ///< read VA IndepWin result()              Data type: SIE_VARST  eng: SIE1,
	SIE_REG_RAWENC          = 0x00000006,   ///< read rawEncode Bitstream size          Data type: UINT32     eng: SIE1234
	SIE_REG_Ch0CURRDYADDR   = 0x00000007,   ///< read sie current dma ready address     Data type: UINT32     eng: SIE12345678
	SIE_REG_OBDETRST        = 0x00000008,   ///< read sie ob detection rst              Data type: UINT32     eng: SIE12345678
	SIE_REG_Ch1CURRDYADDR   = 0x00000009,   ///< read sie current dma ready address CA  Data type: UINT32     eng: SIE12345678
	SIE_REG_Ch2CURRDYADDR   = 0x0000000A,   ///< read sie current dma ready address LA  Data type: UINT32     eng: SIE12345678
	SIE_REG_Ch3CURRDYADDR   = 0x0000000B,   ///< read sie current dma ready address VA  Data type: UINT32     eng: SIE1
	SIE_REG_Ch4CURRDYADDR   = 0x0000000C,   ///< read sie current dma ready address ETH Data type: UINT32     eng: SIE1
	SIE_REG_LASUM           = 0x0000000D,   ///< read sie LA sum.                       Data type: UINT32     eng: SIE12345678
	ENUM_DUMMY4WORD(IPH_SIE_QUE)
} IPH_SIE_QUE;


/**
     enum of SIE update selection
*/
typedef enum _SIE_UPDATE {
	SIE_INIT                =   0x0000000000000001LLU,   ///< init sie eng: SIE12345678
	SIE_OUTADDR_CH0         =   0x0000000000000002LLU,   ///< update ch0 crop/crop & scale/DVI Y image out addr eng: SIE12345678
	SIE_OUTADDR_CH1         =   0x0000000000000004LLU,   ///< update ch1 CA/DVI UV out addr eng: SIE12345678
	SIE_OUTADDR_CH2         =   0x0000000000000008LLU,   ///< update ch2 LA out addr eng: SIE12345678
	SIE_OUTADDR_CH3         =   0x0000000000000010LLU,   ///< update ch3 VA/crop & scale image/shdr out addr eng: SIE1234
	SIE_OUTADDR_CH4         =   0x0000000000000020LLU,   ///< update ch4 VA/eth out addr eng: SIE1
	SIE_OUTADDR_CH5         =   0x0000000000000040LLU,   ///< update ch5 raw enc out addr eng: SIE1234
	SIE_IOSIZE              =   0x0000000000000080LLU,   ///< update crop size/start pix/CFA,bs output size eng: SIE12345678
	SIE_OUTINFO_CH0         =   0x0000000000000100LLU,   ///< update ch0 dma output information eng: SIE12345678
	SIE_OUTINFO_CH1         =   0x0000000000000200LLU,   ///< update ch1 dma output information eng: SIE12345678
	SIE_OUTINFO_CH2         =   0x0000000000000400LLU,   ///< update ch2 dma output information eng: SIE12345678
	SIE_OUTINFO_CH3         =   0x0000000000000800LLU,   ///< update ch3 dma output information eng: SIE1234
	SIE_OUTINFO_CH4         =   0x0000000000001000LLU,   ///< update ch4 dma output information eng: SIE1
	SIE_OUTINFO_CH5         =   0x0000000000002000LLU,   ///< update ch5 dma output information eng: SIE1234
	SIE_SUBFEN              =   0x0000000000004000LLU,   ///< update func enable/disable eng: SIE12345678
	SIE_DPC_                =   0x0000000000008000LLU,   ///< update defect pixel concealment eng: SIE1234
	SIE_OBAVG_              =   0x0000000000010000LLU,   ///< update ob avg ( ob detection) eng: SIE12345678
	SIE_OBOFS_              =   0x0000000000020000LLU,   ///< update ob offset eng: SIE12345678
	SIE_ECS_                =   0x0000000000040000LLU,   ///< update ECS/including ECS table addr eng: SIE12345678
	SIE_DGAIN               =   0x0000000000080000LLU,   ///< update Digital gain eng: SIE12345678
	SIE_CGAIN               =   0x0000000000100000LLU,   ///< update color gain eng: SIE1234
	SIE_PATGEN              =   0x0000000000200000LLU,   ///< update patten gen eng: SIE12345678
	SIE_DVI_                =   0x0000000000400000LLU,   ///< update DVI information eng:SIE1345678
	SIE_BP_                 =   0x0000000000800000LLU,   ///< update break point eng:SIE12345678
	SIE_CH3_YOUT_SIZE       =   0x0000000001000000LLU,   ///< update ch3_yout window size eng:SIE1234
	SIE_CH3_YOUT_CGAIN      =   0x0000000002000000LLU,	 ///< update ch3_yout color gain eng:SIE1234
	SIE_GRIDLINE            =   0x0000000004000000LLU,	 ///< update sie gridline info eng:SIE1234
	//                      =   0x0000000008000000LLU
	//                      =   0x0000000010000000LLU,
	SIE_STATS_VIG           =   0x0000000020000000LLU,   ///< statisitc path VIG eng: SIE1234
	SIE_STATS_CA_SIZE       =   0x0000000040000000LLU,   ///< statistic CA window size eng: SIE12345678
	SIE_STATS_LA_SIZE       =   0x0000000080000000LLU,   ///< statistic LA window size eng: SIE12345678
	SIE_STATS_LA_CG         =   0x0000000100000000LLU,   ///< statistic LA color gain eng: SIE12345678
	SIE_STATS_LA_GAMMA      =   0x0000000200000000LLU,   ///< statistic LA gamma eng: SIE12345678
	SIE_STATS_LA_           =   0x0000000400000000LLU,   ///< statistic LA information SIE12345678
	//                      =   0x0000000800000000LLU
	//                      =   0x0000001000000000LLU
	//                      =   0x0000002000000000LLU
	SIE_STATS_CA_IRSUB      =   0x0000004000000000LLU,   ///< update  statisitc CA IRSUB eng: SIE12345678
	SIE_STATS_CA_           =   0x0000008000000000LLU,   ///< update statistic CA threshold eng: SIE12345678
	//                      =   0x0000010000000000LLU
	//                      =   0x0000020000000000LLU
	//                      =   0x0000040000000000LLU
	//                      =   0x0000080000000000LLU
	SIE_STATS_VA_ETH_CG     =   0x0000100000000000LLU,   ///< update statistic VA_ETH color gain eng: SIE1
	SIE_STATS_VA_ETH_SIZE   =   0x0000200000000000LLU,   ///< update statistic VA_ETH size eng: SIE1
	SIE_STATS_VA_ETH_GAMMA  =   0x0000400000000000LLU,   ///< update statistic VA_ETH gamma eng: SIE1
	SIE_STATS_VA_           =   0x0000800000000000LLU,   ///< update statistic VA information eng: SIE1
	SIE_STATS_ETH_          =   0x0001000000000000LLU,   ///< update statistic ETH information eng: SIE1
	//                      =   0x0002000000000000LLU
	//                      =   0x0004000000000000LLU
	//                      =   0x0008000000000000LLU
	//                      =   0x0010000000000000LLU
	//                      =   0x0020000000000000LLU
	//                      =   0x0040000000000000LLU
	//                      =   0x0080000000000000LLU
	//                      =   0x0100000000000000LLU
	//                      =   0x0200000000000000LLU
	//                      =   0x0400000000000000LLU
	//                      =   0x0800000000000000LLU
	//                      =   0x1000000000000000LLU
	//                      =   0x2000000000000000LLU
	//                      =   0x4000000000000000LLU
	//                      =   0x8000000000000000LLU
} SIE_UPDATE;


#define SIE1_UPDATE_ENABLE_BIT              (SIE_INIT                   |       \
		SIE_OUTADDR_CH0            |       \
		SIE_OUTADDR_CH1            |       \
		SIE_OUTADDR_CH2            |       \
		SIE_OUTADDR_CH3            |       \
		SIE_OUTADDR_CH4            |       \
		SIE_OUTADDR_CH5            |       \
		SIE_IOSIZE                 |       \
		SIE_OUTINFO_CH0            |       \
		SIE_OUTINFO_CH1            |       \
		SIE_OUTINFO_CH2            |       \
		SIE_OUTINFO_CH3            |       \
		SIE_OUTINFO_CH4            |       \
		SIE_OUTINFO_CH5            |       \
		SIE_SUBFEN                 |       \
		SIE_DPC_                   |       \
		SIE_OBAVG_                 |       \
		SIE_OBOFS_                 |       \
		SIE_ECS_                   |       \
		SIE_DGAIN                  |       \
		SIE_CGAIN                  |       \
		SIE_PATGEN                 |       \
		SIE_DVI_                   |       \
		SIE_BP_                    |       \
		SIE_STATS_VIG              |       \
		SIE_STATS_CA_SIZE          |       \
		SIE_STATS_LA_SIZE          |       \
		SIE_STATS_LA_CG            |       \
		SIE_STATS_LA_GAMMA         |       \
		SIE_STATS_LA_              |       \
		SIE_STATS_CA_IRSUB         |       \
		SIE_STATS_CA_              |       \
		SIE_STATS_VA_ETH_CG        |       \
		SIE_STATS_VA_ETH_SIZE      |       \
		SIE_STATS_VA_ETH_GAMMA     |       \
		SIE_STATS_VA_              |       \
		SIE_STATS_ETH_             |       \
		SIE_CH3_YOUT_SIZE          |       \
		SIE_CH3_YOUT_CGAIN         |       \
		SIE_GRIDLINE			   |	   \
		0);

#define SIE2_UPDATE_ENABLE_BIT              (SIE_INIT                   |       \
		SIE_OUTADDR_CH0            |       \
		SIE_OUTADDR_CH1            |       \
		SIE_OUTADDR_CH2            |       \
		SIE_OUTADDR_CH3            |       \
		SIE_OUTADDR_CH5            |       \
		SIE_IOSIZE                 |       \
		SIE_OUTINFO_CH0            |       \
		SIE_OUTINFO_CH1            |       \
		SIE_OUTINFO_CH2            |       \
		SIE_OUTINFO_CH3            |       \
		SIE_OUTINFO_CH5            |       \
		SIE_SUBFEN                 |       \
		SIE_DPC_                   |       \
		SIE_OBAVG_                 |       \
		SIE_OBOFS_                 |       \
		SIE_ECS_                   |       \
		SIE_DGAIN                  |       \
		SIE_CGAIN                  |       \
		SIE_PATGEN                 |       \
		SIE_BP_                    |       \
		SIE_STATS_VIG              |       \
		SIE_STATS_CA_SIZE          |       \
		SIE_STATS_LA_SIZE          |       \
		SIE_STATS_LA_CG            |       \
		SIE_STATS_LA_GAMMA         |       \
		SIE_STATS_LA_              |       \
		SIE_STATS_CA_IRSUB         |       \
		SIE_STATS_CA_              |       \
		SIE_CH3_YOUT_SIZE          |       \
		SIE_CH3_YOUT_CGAIN         |       \
		SIE_GRIDLINE			   |	   \
		0);

#define SIE3_UPDATE_ENABLE_BIT              (SIE_INIT                   |       \
		SIE_OUTADDR_CH0            |       \
		SIE_OUTADDR_CH1            |       \
		SIE_OUTADDR_CH2            |       \
		SIE_OUTADDR_CH3            |       \
		SIE_OUTADDR_CH5            |       \
		SIE_IOSIZE                 |       \
		SIE_OUTINFO_CH0            |       \
		SIE_OUTINFO_CH1            |       \
		SIE_OUTINFO_CH2            |       \
		SIE_OUTINFO_CH3            |       \
		SIE_OUTINFO_CH5            |       \
		SIE_SUBFEN                 |       \
		SIE_DPC_                   |       \
		SIE_OBAVG_                 |       \
		SIE_OBOFS_                 |       \
		SIE_ECS_                   |       \
		SIE_DGAIN                  |       \
		SIE_CGAIN                  |       \
		SIE_PATGEN                 |       \
		SIE_DVI_                   |       \
		SIE_BP_                    |       \
		SIE_STATS_VIG              |       \
		SIE_STATS_CA_SIZE          |       \
		SIE_STATS_LA_SIZE          |       \
		SIE_STATS_LA_CG            |       \
		SIE_STATS_LA_GAMMA         |       \
		SIE_STATS_LA_              |       \
		SIE_STATS_CA_IRSUB         |       \
		SIE_STATS_CA_              |       \
		SIE_CH3_YOUT_SIZE          |       \
		SIE_CH3_YOUT_CGAIN         |       \
		SIE_GRIDLINE			   |	   \
		0);

#define SIE4_UPDATE_ENABLE_BIT              (SIE_INIT                   |       \
		SIE_OUTADDR_CH0            |       \
		SIE_OUTADDR_CH1            |       \
		SIE_OUTADDR_CH2            |       \
		SIE_OUTADDR_CH3            |       \
		SIE_OUTADDR_CH5            |       \
		SIE_IOSIZE                 |       \
		SIE_OUTINFO_CH0            |       \
		SIE_OUTINFO_CH1            |       \
		SIE_OUTINFO_CH2            |       \
		SIE_OUTINFO_CH3            |       \
		SIE_OUTINFO_CH5            |       \
		SIE_SUBFEN                 |       \
		SIE_DPC_                   |       \
		SIE_OBAVG_                 |       \
		SIE_OBOFS_                 |       \
		SIE_ECS_                   |       \
		SIE_DGAIN                  |       \
		SIE_CGAIN                  |       \
		SIE_PATGEN                 |       \
		SIE_DVI_                   |       \
		SIE_BP_                    |       \
		SIE_STATS_VIG              |       \
		SIE_STATS_CA_SIZE          |       \
		SIE_STATS_LA_SIZE          |       \
		SIE_STATS_LA_CG            |       \
		SIE_STATS_LA_GAMMA         |       \
		SIE_STATS_LA_              |       \
		SIE_STATS_CA_IRSUB         |       \
		SIE_STATS_CA_              |       \
		SIE_CH3_YOUT_SIZE          |       \
		SIE_CH3_YOUT_CGAIN         |       \
		SIE_GRIDLINE			   |	   \
		0);

#define SIE5_UPDATE_ENABLE_BIT              (SIE_INIT                   |       \
		SIE_OUTADDR_CH0            |       \
		SIE_OUTADDR_CH1            |       \
		SIE_OUTADDR_CH2            |       \
		SIE_IOSIZE                 |       \
		SIE_OUTINFO_CH0            |       \
		SIE_OUTINFO_CH1            |       \
		SIE_OUTINFO_CH2            |       \
		SIE_SUBFEN                 |       \
		SIE_OBAVG_                 |       \
		SIE_OBOFS_                 |       \
		SIE_ECS_                   |       \
		SIE_DGAIN                  |       \
		SIE_PATGEN                 |       \
		SIE_DVI_                   |       \
		SIE_BP_                    |       \
		SIE_STATS_CA_SIZE          |       \
		SIE_STATS_LA_SIZE          |       \
		SIE_STATS_LA_CG            |       \
		SIE_STATS_LA_GAMMA         |       \
		SIE_STATS_LA_              |       \
		SIE_STATS_CA_IRSUB         |       \
		SIE_STATS_CA_              |       \
 		0)

#define SIE6_UPDATE_ENABLE_BIT              (SIE_INIT                   |       \
		SIE_OUTADDR_CH0            |       \
		SIE_OUTADDR_CH1            |       \
		SIE_OUTADDR_CH2            |       \
		SIE_IOSIZE                 |       \
		SIE_OUTINFO_CH0            |       \
		SIE_OUTINFO_CH1            |       \
		SIE_OUTINFO_CH2            |       \
		SIE_SUBFEN                 |       \
		SIE_OBAVG_                 |       \
		SIE_OBOFS_                 |       \
		SIE_ECS_                   |       \
		SIE_DGAIN                  |       \
		SIE_PATGEN                 |       \
		SIE_DVI_                   |       \
		SIE_BP_                    |       \
		SIE_STATS_CA_SIZE          |       \
		SIE_STATS_LA_SIZE          |       \
		SIE_STATS_LA_CG            |       \
		SIE_STATS_LA_GAMMA         |       \
		SIE_STATS_LA_              |       \
		SIE_STATS_CA_IRSUB         |       \
		SIE_STATS_CA_              |       \
 		0)

#define SIE7_UPDATE_ENABLE_BIT              (SIE_INIT                   |       \
		SIE_OUTADDR_CH0            |       \
		SIE_OUTADDR_CH1            |       \
		SIE_OUTADDR_CH2            |       \
		SIE_IOSIZE                 |       \
		SIE_OUTINFO_CH0            |       \
		SIE_OUTINFO_CH1            |       \
		SIE_OUTINFO_CH2            |       \
		SIE_SUBFEN                 |       \
		SIE_OBAVG_                 |       \
		SIE_OBOFS_                 |       \
		SIE_ECS_                   |       \
		SIE_DGAIN                  |       \
		SIE_PATGEN                 |       \
		SIE_DVI_                   |       \
		SIE_BP_                    |       \
		SIE_STATS_CA_SIZE          |       \
		SIE_STATS_LA_SIZE          |       \
		SIE_STATS_LA_CG            |       \
		SIE_STATS_LA_GAMMA         |       \
		SIE_STATS_LA_              |       \
		SIE_STATS_CA_IRSUB         |       \
		SIE_STATS_CA_              |       \
 		0)

#define SIE8_UPDATE_ENABLE_BIT              (SIE_INIT                   |       \
		SIE_OUTADDR_CH0            |       \
		SIE_OUTADDR_CH1            |       \
		SIE_OUTADDR_CH2            |       \
		SIE_IOSIZE                 |       \
		SIE_OUTINFO_CH0            |       \
		SIE_OUTINFO_CH1            |       \
		SIE_OUTINFO_CH2            |       \
		SIE_SUBFEN                 |       \
		SIE_OBAVG_                 |       \
		SIE_OBOFS_                 |       \
		SIE_ECS_                   |       \
		SIE_DGAIN                  |       \
		SIE_PATGEN                 |       \
		SIE_DVI_                   |       \
		SIE_BP_                    |       \
		SIE_STATS_CA_SIZE          |       \
		SIE_STATS_LA_SIZE          |       \
		SIE_STATS_LA_CG            |       \
		SIE_STATS_LA_GAMMA         |       \
		SIE_STATS_LA_              |       \
		SIE_STATS_CA_IRSUB         |       \
		SIE_STATS_CA_              |       \
 		0)

/**
     enum of SIE read selection

     read from inner table, not from hw register.
*/
typedef enum _SIE_READ {
	SIE_R_PHASE         = 0x00000001,   ///< read signal                                        Data type: IPP_SIE_SIGNAL       eng: SIE12345678
	SIE_R_CH0_INFO      = 0x00000002,   ///< read sie ch0 dma addr/size/lineofs/bit             Data type: SIE                  eng: SIE12345678
	SIE_R_CH1_INFO      = 0x00000003,   ///< read sie ch0 dma addr/size/lineofs/bit             Data type: SIE                  eng: SIE12345678
	SIE_R_CH2_INFO      = 0x00000004,   ///< read sie ch0 dma addr/size/lineofs/bit             Data type: SIE                  eng: SIE12345678
	SIE_R_CH3_INFO      = 0x00000005,   ///< read sie ch0 dma addr/size/lineofs/bit             Data type: SIE                  eng: SIE1234
	SIE_R_CH4_INFO      = 0x00000006,   ///< read sie ch0 dma addr/size/lineofs/bit             Data type: pSubFunc             eng: SIE1
	SIE_R_CH5_INFO      = 0x00000007,   ///< read sie ch0 dma addr/size/lineofs/bit             Data type: pSubFunc             eng: SIE1234
	SIE_R_BS_OUT        = 0x00000008,   ///< read sie bayer scale out size                      Data type: SIE                  eng: SIE1, SIE2
	SIE_R_INSIZE        = 0x00000009,   ///< read act window size,crop window info              Data type: SIE_WINDOW           eng: SIE1234
	SIE_R_SUBFEN        = 0x0000000a,   ///< read func enable/disable,                          Data type: SieFuncEn            eng: SIE12345678
	SIE_R_DPC_          = 0X0000000b,   ///< read defect pixel concealment,                     Data type: SIE_DPC              eng: SIE12345678
	SIE_R_OBAVG_        = 0x0000000c,   ///< read ob avg                                        Data type: SIE_OBAVG            eng: SIE12345678
	SIE_R_OBOFS_        = 0X0000000d,   ///< read ob offset                                     Data type: SIE_OBSUB            eng: SIE12345678
	SIE_R_ECS_          = 0X00000010,   ///< read ECS                                           Data type: SIE_EmbeddedCS       eng: SIE12345678
	SIE_R_DGAIN_        = 0x00000011,   ///< read Digital gain                                  Data type: UINT32               eng: SIE12345678
	SIE_R_VIG_          = 0x00000012,   ///< read VIG  para. set                                Data type: SIE_CA_VIG           eng: SIE12345678
	SIE_R_CA_           = 0X00000013,   ///< read CACC para. set,not include addr               Data type: SIE_CACC             eng: SIE12345678
	SIE_R_LA_           = 0X00000014,   ///< read LACC para. set,not include addr               Data type: SIE_LACC             eng: SIE12345678
	SIE_R_VA_           = 0X00000015,   ///< read VACC para. set                                Data type: SIE_VACC             eng: SIE1
	SIE_R_VACROP_       = 0X00000016,   ///< read VA Crop para. set                             Data type: VACROP               eng: SIE1
	SIE_R_RAWEN_        = 0x0000001a,   ///< read Raw encode                                    Data type: SIE_RAWENC           eng: SIE1234
	SIE_R_INTEEN_       = 0x0000001b,   ///< read sie interrupt enable bit                      Data type: SIE_INTE_EN          eng: SIE12345678
	SIE_R_CA_PARAM_     = 0x0000001c,   ///< read ch2 infomration                               Data type: SIE_CAPARAM          eng: SIE12345678
	SIE_R_LA_PARAM_     = 0x0000001d,   ///< read ch1 infomration                               Data type: SIE_LAPARAM          eng: SIE12345678
	SIE_R_ETH_PARAM_    = 0x0000001e,   ///< read edge threshold output,addr,lineofs,V only     Data type: SIE_ETH_DramRst          eng: SIE1
	SIE_R_CGAIN_        = 0x0000001f,   ///< read Digital gain                                  Data type: UINT32               eng: SIE1234
	SIE_R_YOUT_         = 0x00000020,   ///< read YOUT information                              Data type: SIE_HAL_YOUT_INFO    eng: SIE1234
	SIE_R_YOUT_PARAM_   = 0x00000021,   ///< read YOUT Parameters                               Data type: SIE_YOUT_PARAM       eng: SIE1234
	ENUM_DUMMY4WORD(SIE_READ)
} SIE_READ;


/**
     SIE configuration

     all SIE configuration including all SIE sub function
*/
typedef struct _SIE_Manager {
	IPP_SIE_SIGNAL  Sie_Signal; ///< sensor information                         @SIE1/SIE2/SIE3/SIE4
	SIE_ACT_MODE ActMode;       ///< sie source selection                       @SIE1/SIE2
	SIE_WINDOW SensorIn_Win;    ///< act/crop wiondow                           @SIE1/SIE2/SIE3/SIE4
	SIE_WIN_INFO BSOutImg;      ///< Sie bs output size
	BAYER_TYPE Bayer_Type;      ///< bayer format

	struct {
		SIE_Dma_In In;              ///< bayer in for ActMode = SIE_IN_DRAM     @SIE1
		SIE_WIN_INFO DramIn_Win;    ///< Sie input source image size.           @SIE1/SIE2/SIE3/SIE4
	} RAW_IN;

	struct {
		SIE_Dma_Out Out;        ///< -                                          @SIE1/SIE2
		CH0_SRC     Src;        ///< output selection                           @SIE1/SIE2
		SIE_PACKBUS Bit;        ///< pack bit
	} CH0;                      ///< sie channel_0 output

	struct {
		SIE_Dma_Out Out;        ///< -                                          @SIE1/SIE2/SIE3/SIE4
	} CH1;

	struct {
		SIE_Dma_Out Out;        ///< -                                          @SIE1/SIE2/SIE3/SIE4
	} CH2;

	struct {
		SIE_Dma_Out Out;        ///< -                                          @SIE1
		CH3_SRC     Src;        ///< -                                          @SIE1
		SIE_PACKBUS Bit;        ///< pack bit
	} CH3;

	struct {
		SIE_Dma_Out Out;        ///< -                                          @SIE1
		CH4_SRC     Src;        ///< -                                          @SIE1
	} CH4;

	struct {
		SIE_Dma_Out Out;        ///< -                                          @SIE1
	} CH5;

	SIE_BURSTLENGTH_SET BurstLength;    ///< sie burst length                   @SIE1/SIE2/SIE3/SIE4
	SIE_SUBFUNC *pSubFunc;      ///< -                                          @SIE1/SIE2/SIE3/SIE4

	//reverse
	//UINT32 rev;                     //reserve for IPH paraset idx
} SIE_Manager, *pSIE_Manager;

/**
     SIE configuration
*/
typedef SIE_Manager SIE;

//@}
#endif
