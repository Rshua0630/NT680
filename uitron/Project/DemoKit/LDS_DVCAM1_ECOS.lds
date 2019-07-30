/* Linker script for malta
 *
 * Version: Sourcery G++ Lite 4.3-221
 * Support: https://support.codesourcery.com/GNUToolchain/
 *
 * Copyright (c) 2007, 2008, 2009 CodeSourcery, Inc.
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions.  No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply.
 * */

/* Rule:
 * 1. to use .o in project layer just type the .o name with file extension. (UiFlow.o)
 * 2. to use .o in library layer have to add prefix module name. (DscSystem/SysTask.o)
 * */

OUTPUT_ARCH(arm)
ENTRY(reset_vector)
SEARCH_DIR(.)
GROUP(libtarget.a libgcc.a)
SECTIONS
{
    .SPECIAL_RO $BOARD_UITRON_ADDR:
    {
        _section_01_addr = ABSOLUTE(.);
		KEEP(../../ThirdParty/eCos/lib/vectors.o(.vectors*));
    }
    .BIN_INFO $BOARD_UITRON_ADDR + 0x100:
    {
        KEEP(BinInfo.o(.data*));
    }
	.CODE_INFO $BOARD_UITRON_ADDR + 0x2F0:
    {
    	_code_info_start_base = ABSOLUTE(.);
        KEEP(CodeInfo.o(.data*));
        _code_info_end_base = ABSOLUTE(.);
    }
    .START_UP $BOARD_UITRON_ADDR + 0x400:
    {
        _load_entry_start_base = ABSOLUTE(.);
        KEEP(../../ThirdParty/eCos/lib/vectors.o(.text.start*));
        _load_entry_end_base = ABSOLUTE(.);
    }
    .MODELEXT_INFO $BOARD_UITRON_ADDR + 0x1000:
    {
    	_section_modelext_info_addr = ABSOLUTE(.);
    	KEEP($MODELEXT/modelext_info.o (modelext_header.modelext_info));
    	KEEP(*(modelext_data.modelext_info));
    	KEEP($MODELEXT/bin_info.o (modelext_header.bin_info));
    	KEEP(*(modelext_data.bin_info));
    	KEEP($MODELEXT/pinmux_cfg.o (modelext_header.pinmux_cfg));
    	KEEP(*(modelext_data.pinmux_cfg));
    	KEEP($MODELEXT/intdir_cfg.o (modelext_header.intdir_cfg));
    	KEEP(*(modelext_data.intdir_cfg));
    	KEEP($MODELEXT/emb_partition_info.o (modelext_header.emb_partition_info));
    	KEEP(*(modelext_data.emb_partition_info));
    	KEEP($MODELEXT/gpio_info.o (modelext_header.gpio_info));
    	KEEP(*(modelext_data.gpio_info));
    	KEEP($MODELEXT/dram_partition_info.o (modelext_header.dram_partition_info));
    	KEEP(*(modelext_data.dram_partition_info));
    	KEEP($MODELEXT/model_cfg.o (modelext_header.model_cfg));
    	KEEP(*(modelext_data.model_cfg));
    	_section_modelext_info_size = ABSOLUTE(.) - _section_modelext_info_addr;
    }

    .DOCHG_DMA_CLK 0xF07F0000:
    AT (LOADADDR(.MODELEXT_INFO) + SIZEOF(.MODELEXT_INFO))
    {
		_ram_change_dma_clock_start = LOADADDR(.MODELEXT_INFO) + SIZEOF(.MODELEXT_INFO);
		_load_change_dma_clock_start = ABSOLUTE(.);
        KEEP(Driver_Sys/clock_api.o(.text* .data*))
		KEEP(Driver_Sys/power_down_isr.o(.text* .data*))
		KEEP(Driver_Sys/power_off_flow.o(.text* .data*))
		_load_change_dma_clock_base = ABSOLUTE(.);
		*(.chgdma_data*) /* collect variables used in change DMA flow */
    }
	/*
	_stext = ABSOLUTE(.);
	*/

	_stext = LOADADDR(.MODELEXT_INFO) + SIZEOF(.MODELEXT_INFO) + SIZEOF(.DOCHG_DMA_CLK);

    /* kernel common */
    . = ALIGN(4);
    .SECTION_01 LOADADDR(.MODELEXT_INFO) + SIZEOF(.MODELEXT_INFO) + SIZEOF(.DOCHG_DMA_CLK):
	AT (LOADADDR(.MODELEXT_INFO) + SIZEOF(.MODELEXT_INFO) + SIZEOF(.DOCHG_DMA_CLK))
    {
    	. = .; __CTOR_LIST__ = .; PROVIDE (__CTOR_LIST__ = .); KEEP (*(SORT(.ctors.*))) KEEP (*(.ctors*)) __CTOR_END__ = .; PROVIDE (__CTOR_END__ = .);
    	. = .; __DTOR_LIST__ = .; PROVIDE (__DTOR_LIST__ = .); KEEP (*(SORT(.dtors.*))) KEEP (*(.dtors*)) __DTOR_END__ = .; PROVIDE (__DTOR_END__ = .);
		__init_array_start__ = ABSOLUTE (.);
		KEEP (*(SORT (.init_array.*)))
		KEEP (*(SORT (.init_array)))
		__init_array_end__ = ABSOLUTE (.);
    	. = .; KEEP(*( SORT (.ecos.table.*)))
		vectors.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        extras.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        libextras.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        libtarget.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
    	eCos_main.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        /*============================ before os start ===============================  */
        *(.kercfg_data*) /*collect install ID process of all modules*/
        *(.kercfg_text*) /*collect install ID process of all modules*/
        _main.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        SysCfg.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        GxGfx/DC.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        LogFile.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        /*============================ os & common ===============================  */
        Kernel.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        OS.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        Debug.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        Utility.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        DbgUt.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        LibC.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        libgcc.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        GxSystem.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        GxImage.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        Ker_dsp.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        Ker_pkg.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        /*============================ drv ===============================  */
        Dx.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        DxSys.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        $CHIPCFG.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        Driver_Sys.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        Driver_IPP/grph.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        Driver_Codec/jpeg.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        Driver_Codec/jpeg_int.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        /*============================ drv ext ===============================  */
        $GSENSOREXT.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        $MODELEXT.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        /*============================ after os start ===============================  */
        main.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        SysMain.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        SysMain_State.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        SysMain_Flow_Init.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        SysMain_Exe.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ProjectInfo.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        AppDisp_View.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        /*============================ timer ===============================  */
        GxTimer/GxTimer_p1.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        /*============================ i/o ===============================  */
        GxInput.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        GxOutput.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        /*============================ ipc ===============================  */
        SysIpc_Exe.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        GxDSP.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        NvtIPC.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        /*============================ power ===============================  */
        GxPower.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        SysPower_Exe.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        GxUSB.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        SysUsb_Exe.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        /*============================ embmem ===============================  */
        SpiFlash.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        Ramdisk.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        FwSrv.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        PartLoad.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        SysStrg_Exe.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        SysStrg_CB.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        /*============================ user ===============================  */
        NvtUser.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        MsdcNvtCb.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        HwClock.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        UMSD.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
    }

    /* device common */
    . = ALIGN(4);
    .SECTION_02 :
    {
    	_section_01_size = ABSOLUTE(.) - _section_01_addr;
        _section_02_addr = ABSOLUTE(.);
        libnvtimeth96680.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
	libtarget2.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        /*============================ i/o ===============================  */
        SysOutput_Exe.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        /*============================ storage ===============================  */
        GxStrg/GxStrg_p1.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        GxStrg.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        FileSys.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        FsUitron.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        EXIF.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        DCF.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        FileDB.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ExifVendor.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        /*============================ sensor ===============================  */
        SysSensor_Exe.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        SysSensor_CB.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        Driver_IPP.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        Sensor.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        ipl_common.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        RSCTask.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
	AEAlg.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        $RSC.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        $SENSOR_1.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        $SENSOR_2.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        $IPL_1.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        $IPL_2.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        /*============================ lens ===============================  */
        /* $LENSDRV.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*) */
        /*============================ display ===============================  */
        SysVideo_Exe.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        SysVideo_CB.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        $LCD.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        Disp_PANELCOMMON.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        DispObj.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        GxVideo.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        GxDisplay.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        vdodisp.a  (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        /*============================ show logo ===============================  */
        /*BG_Opening.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)*/
        GxImage.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        GxImageFile.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
        dal_jpeg.a (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
    }

    /* device photo */
    . = ALIGN(4);
    .SECTION_03 :
    {
    	_section_02_size = ABSOLUTE(.) - _section_02_addr;
        _section_03_addr = ABSOLUTE(.);
    }

    /* device movie */
    . = ALIGN(4);
    .SECTION_04 :
    {
    	_section_03_size = ABSOLUTE(.) - _section_03_addr;
        _section_04_addr = ABSOLUTE(.);
    }

    /* device playback */
    . = ALIGN(4);
    .SECTION_05 :
    {
    	_section_04_size = ABSOLUTE(.) - _section_04_addr;
        _section_05_addr = ABSOLUTE(.);
    }

    /* application common */
    . = ALIGN(4);
    .SECTION_06 :
    {
    	_section_05_size = ABSOLUTE(.) - _section_05_addr;
        _section_06_addr = ABSOLUTE(.);
    }

    /* application photo */
    . = ALIGN(4);
    .SECTION_07 :
    {
    	_section_06_size = ABSOLUTE(.) - _section_06_addr;
        _section_07_addr = ABSOLUTE(.);
    }

    /* application movie */
    . = ALIGN(4);
    .SECTION_08 :
    {
    	_section_07_size = ABSOLUTE(.) - _section_07_addr;
        _section_08_addr = ABSOLUTE(.);
    }

    /* application playback */
    . = ALIGN(4);
    .SECTION_09 :
    {
    	_section_08_size = ABSOLUTE(.) - _section_08_addr;
        _section_09_addr = ABSOLUTE(.);
    }

    /* others */
    . = ALIGN(4);
    .SECTION_10 :
    {
    	_section_09_size = ABSOLUTE(.) - _section_09_addr;
        _section_10_addr = ABSOLUTE(.);
		KEEP (*(.nvtversioninfo.begin*))
		_section_verinfostart = ABSOLUTE(.);
        KEEP(*(.version.info.*))
		_section_verinfoend = ABSOLUTE(.);
        KEEP (*(.nvtversioninfo.end*))
        _load_core2_entry_program_start_base = .;
        KEEP(cpu2util/core2_entry.o(.text));
        *(.text*)
        *(.got*)
        *(.rodata*)
        *(.data*)
        *(.data1*)
        *(.rodata1*)
        *(.reginfo*)
        *(.rel.dyn*)
        *(.sdata*) /* __fdlib_version in libm.a */
    }

    _section_10_size = ABSOLUTE(.) - _section_10_addr;

    . = ALIGN(4);
    .init ALIGN (0x4) : { . = .; KEEP (*(.init*)) }
    _etext = .; PROVIDE (etext = .);
    .data ALIGN (0x8) : { __ram_data_start = ABSOLUTE (.); _fdata = . ; *(.data*) *(.data.*) *(.gnu.linkonce.d*) *( .2ram.*) . = ALIGN (8); SORT(CONSTRUCTORS) }
    __rom_data_start = LOADADDR(.data);
	__ram_data_end = .; _edata = . ; PROVIDE (edata = .);

    /* collect zero initialized global data (mark "B" in sym file) */
    /* collect static local data (mark "b" in sym file) */
    _section_zi_addr = ABSOLUTE(.);
    _image_general_zi_zi_base = ABSOLUTE(.);

    __bss_start = .; _fbss = .; .sbss ALIGN (0x8) :
	{
	. = ALIGN(0x4000);
	_ttb = .;
	. = . + 0x4000;
	. = .; *(.dynsbss*) *(.sbss) *(.sbss.*) *(.scommon*) *(.gnu.linkonce.sb.*)
	}
    .bss ALIGN (0x8) :
	{
		*(.dynbss*) *(.bss*) *(.bss.*) *(COMMON) *(.gnu.linkonce.b.*)

	} __bss_end = .;

    /* NOTE:
    if using -fcommon compiler option, un-initialized global data will be isolated to .common section.
    if using -fno-common compiler option, un-initialized global data will be merged to .bss section.
    memory between _image_general_zi_zi_base and _image_general_zi_zi_limit will be fill to zero before _main().
    */
    . = ALIGN(4);
    .common : {
       __common_base = ABSOLUTE(.);
       /* collect "small" un-initialized global data (mark "C" in sym file) */
       *(.scommon*)
       /* collect "large" un-initialized global data (mark "B" in sym file) */
       *(COMMON)
       __common_limit = ABSOLUTE(.);
    }
    /* _image_general_zi_zi_limit = ABSOLUTE(.); */

    . = ALIGN(16);

    /* heap area start for eCos */
    __heap1 = ALIGN(16);	/* All rest of RAM reserved for heap. !!!TODO!!!manage kernel memory pool and stack */
    __heap1_end = . + 0x00300000;   /* reserved 3MB for ecos heap memory, 3MB is enough for EVB ecos network using
                                     , this value is project dependent should be adjusted by project*/
    . = __heap1_end;
    _ecos_mem_end = .;
    _ecos_mem_size = . - $BOARD_UITRON_ADDR;
    _ecos_mem_sizembit = _ecos_mem_size / (1024*1024) * 8;
    /*_ecos_bss_size = SIZEOF(.sbss) + SIZEOF(.bss);*/
	_ecos_bss_size = __bss_end - __bss_start;

    /* end of code and eCos stuff */
    _end = .; PROVIDE (end = .);

    /* stack area start for RTOS tasks */
    _image_general_zi_zi_limit = ABSOLUTE(.);
    _section_all_size = ABSOLUTE(.) - _section_01_addr;
    _stack_start = ALIGN(16);
}

EXTERN($SYMBOL_IPL)
EXTERN($SYMBOL_NETWORK)
EXTERN($SYMBOL_PLUG)
