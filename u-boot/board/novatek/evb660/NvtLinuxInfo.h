#ifndef _NVTLINUXINFO_H
#define _NVTLINUXINFO_H

typedef struct _NVTLINUXINFO
{
    unsigned int InfoFourCC; ///< must be 'L' 'I' 'F' 'O'
    unsigned int InfoSize;   ///< this struct size
    char CmdLine[64];        ///< extend command line for startup
}NVTLINUXINFO, *PNVTLINUXINFO;

#endif