/*
    The APIs to get version and build date of Novatek's library

    The APIs to get version and build date of Novatek's library.

    @file       NvtVerInfo.h
    @ingroup
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/

#ifndef _NVTVERINFO_H
#define _NVTVERINFO_H

#include "Type.h"

/*
    @addtogroup
*/
//@{

/*
    Dump version information of Nvt's library(.a)

    Display all library version information. This function will list library's
\n  module_name     version_code    build_date & build_time

    Example:
    @code
    {
           Startup   1.00.000  Sep 11 2013, 14:05:12
        AppControl   1.00.002  Sep 11 2013, 14:11:06
           Capture   1.00.005  Sep 11 2013, 14:11:20
            DevMan   1.00.002  Sep 11 2013, 14:11:22
          LviewNvt   1.00.006  Sep 11 2013, 14:11:24
             Debug   1.00.012  Sep 11 2013, 14:04:55
            Kernel   1.01.006  Sep 11 2013, 14:04:59
            ...
            ...
    }
    @endcode
*/
extern void Nvtver_DumpVerInfo(void);

// Assign a defined variable to a specific section
#ifndef CYGBLD_ATTRIB_SECTION
#define CYGBLD_ATTRIB_SECTION(__sect__) __attribute__((section (__sect__)))
#endif

#define __string(_x) #_x
#define __xstring(_x) __string(_x)


#ifdef _PROJECT_CODE_
#ifndef VERSION_INFO_ID4
#define VERSION_INFO_ID4(_name, _major, _minor, _bugfix, _ext) \
	CYGBLD_ATTRIB_SECTION(".version.info." __xstring(_major) "_" __xstring(_minor) "_" __xstring(_bugfix) "_" __xstring(_PROJECT_CODE_) "_" __xstring(_ext) "." __xstring(_name))
#endif


#define NVTVER_VERSION_ENTRY(_l, _major, _minor, _bugfix, _ext)  \
	const CHAR __module_name__ [] VERSION_INFO_ID4(__section_name__, _major, _minor, _bugfix, _ext)  = __xstring(__section_name__) "#" __xstring(_major) "." __xstring(_minor) "." __xstring(_bugfix) "." __xstring(_PROJECT_CODE_) "." __xstring(_ext) "_SHA.00000000";

#else

#ifndef VERSION_INFO_ID4
#define VERSION_INFO_ID4(_name, _major, _minor, _bugfix, _ext) \
	CYGBLD_ATTRIB_SECTION(".version.info." __xstring(_major) "_" __xstring(_minor) "_" __xstring(_bugfix) "." __xstring(_name))
#endif



#define NVTVER_VERSION_ENTRY(_l, _major, _minor, _bugfix, _ext)  \
	const CHAR __module_name__ [] VERSION_INFO_ID4(__section_name__, _major, _minor, _bugfix, _ext)  = __xstring(__section_name__) "#" __xstring(_major) "." __xstring(_minor) "." __xstring(_bugfix) "_SHA.00000000";


#endif

//@}

#endif
