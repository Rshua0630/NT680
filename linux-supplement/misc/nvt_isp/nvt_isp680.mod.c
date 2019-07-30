#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x7ee08c7f, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x2d3385d3, __VMLINUX_SYMBOL_STR(system_wq) },
	{ 0xa179288f, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x12da5bb2, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0xa3029851, __VMLINUX_SYMBOL_STR(nvt_ipc_msg_rcv) },
	{ 0x4ceaaa92, __VMLINUX_SYMBOL_STR(nvt_ipc_msg_snd) },
	{ 0x349cba85, __VMLINUX_SYMBOL_STR(strchr) },
	{ 0x46acc4ef, __VMLINUX_SYMBOL_STR(single_open) },
	{ 0xcca27eeb, __VMLINUX_SYMBOL_STR(del_timer) },
	{ 0x97255bdf, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0x28b7019c, __VMLINUX_SYMBOL_STR(nvt_ipc_msg_rel) },
	{ 0x8bd02cf, __VMLINUX_SYMBOL_STR(single_release) },
	{ 0x20f482b, __VMLINUX_SYMBOL_STR(no_llseek) },
	{ 0x6148bb72, __VMLINUX_SYMBOL_STR(vfs_llseek) },
	{ 0x20000329, __VMLINUX_SYMBOL_STR(simple_strtoul) },
	{ 0xc356b144, __VMLINUX_SYMBOL_STR(seq_printf) },
	{ 0x9e067cb9, __VMLINUX_SYMBOL_STR(filp_close) },
	{ 0xfa2bcf10, __VMLINUX_SYMBOL_STR(init_timer_key) },
	{ 0x514cc273, __VMLINUX_SYMBOL_STR(arm_copy_from_user) },
	{ 0x85df9b6c, __VMLINUX_SYMBOL_STR(strsep) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xe05177d1, __VMLINUX_SYMBOL_STR(seq_read) },
	{ 0x80f30ede, __VMLINUX_SYMBOL_STR(__platform_driver_register) },
	{ 0x7d11c268, __VMLINUX_SYMBOL_STR(jiffies) },
	{ 0x189c5980, __VMLINUX_SYMBOL_STR(arm_copy_to_user) },
	{ 0xe2d5255a, __VMLINUX_SYMBOL_STR(strcmp) },
	{ 0x4f594d8a, __VMLINUX_SYMBOL_STR(proc_remove) },
	{ 0x1e7ce177, __VMLINUX_SYMBOL_STR(PDE_DATA) },
	{ 0xe707d823, __VMLINUX_SYMBOL_STR(__aeabi_uidiv) },
	{ 0x62a79a6c, __VMLINUX_SYMBOL_STR(param_ops_charp) },
	{ 0x5202c4c9, __VMLINUX_SYMBOL_STR(vfs_read) },
	{ 0x3ef15ec0, __VMLINUX_SYMBOL_STR(misc_register) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0x5f754e5a, __VMLINUX_SYMBOL_STR(memset) },
	{ 0xd627480b, __VMLINUX_SYMBOL_STR(strncat) },
	{ 0x76f6c5ef, __VMLINUX_SYMBOL_STR(kmalloc_order) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x29c95904, __VMLINUX_SYMBOL_STR(nvt_ipc_msg_get) },
	{ 0x328a05f1, __VMLINUX_SYMBOL_STR(strncpy) },
	{ 0x84b183ae, __VMLINUX_SYMBOL_STR(strncmp) },
	{ 0xe281a418, __VMLINUX_SYMBOL_STR(proc_mkdir_data) },
	{ 0x1e6d26a8, __VMLINUX_SYMBOL_STR(strstr) },
	{ 0x5008d17b, __VMLINUX_SYMBOL_STR(down) },
	{ 0xc8fd727e, __VMLINUX_SYMBOL_STR(mod_timer) },
	{ 0x34811d4e, __VMLINUX_SYMBOL_STR(nvt_ipc_ftok) },
	{ 0x4760cc1b, __VMLINUX_SYMBOL_STR(platform_device_unregister) },
	{ 0xd9605d4c, __VMLINUX_SYMBOL_STR(add_timer) },
	{ 0x61651be, __VMLINUX_SYMBOL_STR(strcat) },
	{ 0x42a4f0ce, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0x29a22b73, __VMLINUX_SYMBOL_STR(platform_device_register) },
	{ 0xd9ce8f0c, __VMLINUX_SYMBOL_STR(strnlen) },
	{ 0x12a38747, __VMLINUX_SYMBOL_STR(usleep_range) },
	{ 0xc2165d85, __VMLINUX_SYMBOL_STR(__arm_iounmap) },
	{ 0xb1c5d632, __VMLINUX_SYMBOL_STR(single_open_size) },
	{ 0x7afa89fc, __VMLINUX_SYMBOL_STR(vsnprintf) },
	{ 0xec2cd296, __VMLINUX_SYMBOL_STR(proc_create_data) },
	{ 0xd15ac7c0, __VMLINUX_SYMBOL_STR(seq_lseek) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x9d669763, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x6e13c32a, __VMLINUX_SYMBOL_STR(up) },
	{ 0xfb961d14, __VMLINUX_SYMBOL_STR(__arm_ioremap) },
	{ 0xb742fd7, __VMLINUX_SYMBOL_STR(simple_strtol) },
	{ 0xb2d48a2e, __VMLINUX_SYMBOL_STR(queue_work_on) },
	{ 0xb25a57f2, __VMLINUX_SYMBOL_STR(platform_driver_unregister) },
	{ 0x149c7752, __VMLINUX_SYMBOL_STR(param_ops_uint) },
	{ 0x890aea26, __VMLINUX_SYMBOL_STR(misc_deregister) },
	{ 0xa9989310, __VMLINUX_SYMBOL_STR(vfs_write) },
	{ 0xe914e41e, __VMLINUX_SYMBOL_STR(strcpy) },
	{ 0x132ad726, __VMLINUX_SYMBOL_STR(filp_open) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

