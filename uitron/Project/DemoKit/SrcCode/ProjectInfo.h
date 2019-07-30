/**
    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.

    @file       ProjectInfo.h
    @ingroup    mIPRJAPCfg

    @brief      Project source build time information
                Project source build time information

    @note       The project source build time info will be updated every time you build the image.

    @date       2007/11/06
*/

/** \addtogroup mIPRJAPCfg */
//@{

#ifndef _PROJECTINFO_H_
#define _PROJECTINFO_H_

#include "Type.h"

//-----------------------------------------------------------------------------
// User define info
//-----------------------------------------------------------------------------

extern char    *Prj_GetVersionString(void); //PrjCfg.h FW_VERSION_NUM
extern char    *Prj_GetModelInfo(void); //VersionInfo.s :: ProjectCode
extern char    *Prj_GetVerInfo(void); //VersionInfo.s :: VersionNumber
extern char    *Prj_GetReleaseDate(void); //VersionInfo.s :: ReleaseDate
extern char    *Prj_GetCheckinDate(void); //ProjectInfo.cfg
extern char    *Prj_GetGitRevision(void);


//-----------------------------------------------------------------------------
// Compiler-time info
//-----------------------------------------------------------------------------

extern CHAR *Prj_GetBuildDateStd(void);
extern CHAR *Prj_GetBuildDateUser(void);
extern CHAR *Prj_GetGitRevision(void);


#endif //_PROJECTINFO_H_
//@}
