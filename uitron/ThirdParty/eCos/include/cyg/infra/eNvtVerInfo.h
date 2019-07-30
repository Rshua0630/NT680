/**
    Macro to generate version and build date information for eCos platform

    Macro to generate version and build date information for eCos platform.

    @file       eNvtVersion.h
    @ingroup
    @note       Usage: #include <cyg/infra/eNvtVerInfo.h>

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/

#ifndef _ENVTVERINFO_H
#define _ENVTVERINFO_H

typedef char    CHAR;

// Please don't reference these macro -begin
// Assign a defined variable to a specific section
#ifndef NVTVER_ATTRIB_SECTION
    #define NVTVER_ATTRIB_SECTION(__sect__) __attribute__((section (__sect__)))
#endif

#define __string(_x) #_x
#define __xstring(_x) __string(_x)

//
// Project leader should add this macro _PROJECT_CODE_ when project is branched.
//
//#define _PROJECT_CODE_  A0D1

#ifdef _PROJECT_CODE_
  #ifndef NVTVER_ID3
    #define NVTVER_ID3( _name, _major, _minor, _bugfix, _ext ) \
        NVTVER_ATTRIB_SECTION(".version.info." __xstring(_major) "_" __xstring(_minor) "_" __xstring(_bugfix) "_" __xstring(_PROJECT_CODE_) "_" __xstring(_ext) "." __xstring(_name))
  #endif

    // Macro to generate version and build date information
    #define NVTVER_VERSION_ENTRY(_l, _major, _minor, _bugfix, _ext)  \
    const CHAR _l##_LIBRARY_VERSION_INFO[]     NVTVER_ID3(_l,_major,_minor,_bugfix, _ext)   = __xstring(_l) "#" __xstring(_major) "." __xstring(_minor) "." __xstring(_bugfix) "." __xstring(_PROJECT_CODE_) "." __xstring(_ext) "_SHA.00000000";
#else
  #ifndef NVTVER_ID3
    #define NVTVER_ID3( _name, _major, _minor, _bugfix, _ext ) \
        NVTVER_ATTRIB_SECTION(".version.info." __xstring(_major) "_" __xstring(_minor) "_" __xstring(_bugfix) "." __xstring(_name))
  #endif

    // Macro to generate version and build date information
    #define NVTVER_VERSION_ENTRY(_l, _major, _minor, _bugfix, _ext)  \
    const CHAR _l##_LIBRARY_VERSION_INFO[]     NVTVER_ID3(_l,_major,_minor,_bugfix, _ext)   = __xstring(_l) "#" __xstring(_major) "." __xstring(_minor) "." __xstring(_bugfix) "_SHA.00000000";
#endif

#ifndef NVTVER_ID4
    #define NVTVER_ID4( _name, _milestone, _major, _minor, _bugfix ) \
        NVTVER_ATTRIB_SECTION(".version.info." __xstring(_milestone) "_" __xstring(_major) "_" __xstring(_minor) "_" __xstring(_bugfix) "." __xstring(_name))
#endif
// Please don't reference these macro -end

#endif
