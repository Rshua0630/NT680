// This is similar to uItron, eCos of CodeInfo.s

.extern CONFIG_SYS_TEXT_BASE
.extern __bss_start
.extern uboot_end


 //.data.coreinfo
 .section ".data.coreinfo","aw"
 .org  0x00 /* CODEINFO SIGNATURE OFFSET, DO NOT MODIFY! */
 .ascii             "CODEINFO        "
 .org  0x10 /* ZI_SECTION_OFFSET, DO NOT MODIFY! */
 .word              __bss_start
 .word              uboot_end
 .org  0x18 /* CODE_SECTION_OFFSET, DO NOT MODIFY! */
 .word              CONFIG_SYS_TEXT_BASE
 .word              uboot_end
 .word              0xffffffff /* MARK: END OF CODE_SECTION */

 .org    0x10C
 .ascii             "_END" /* MARK: END OF CODEINFO */