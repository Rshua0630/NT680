/**
    @file       NVTReturn.h
    @ingroup    mINvtUser

    @brief      NVT User return values.\n
                Declare return values of User API


    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.
*/
#ifndef NVT_ERR_H
#define NVT_ERR_H

/**
    @addtogroup mINvtUser
*/

//@{


/**
    Return value data type
*/
typedef UINT32 NVTRET;

/**
    Return values
*/
enum { //NVTRET
	NVTRET_OK       = 0,                ///< operation success
	NVTRET_ERROR,                       ///< operation fail
	NVTRET_TOTAL_RET
};

//@}
#endif
