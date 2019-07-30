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
	{ 0x6bc3fbc0, __VMLINUX_SYMBOL_STR(__unregister_chrdev) },
	{ 0xa179288f, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xa3029851, __VMLINUX_SYMBOL_STR(nvt_ipc_msg_rcv) },
	{ 0x4ceaaa92, __VMLINUX_SYMBOL_STR(nvt_ipc_msg_snd) },
	{ 0x28b7019c, __VMLINUX_SYMBOL_STR(nvt_ipc_msg_rel) },
	{ 0xffa00d87, __VMLINUX_SYMBOL_STR(arm_dma_ops) },
	{ 0x92785fa6, __VMLINUX_SYMBOL_STR(nvt_ipc_shm_rel) },
	{ 0x44e3300d, __VMLINUX_SYMBOL_STR(__register_chrdev) },
	{ 0x514cc273, __VMLINUX_SYMBOL_STR(arm_copy_from_user) },
	{ 0xda8ec279, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0xadae42fe, __VMLINUX_SYMBOL_STR(kthread_create_on_node) },
	{ 0x189c5980, __VMLINUX_SYMBOL_STR(arm_copy_to_user) },
	{ 0x82e5e223, __VMLINUX_SYMBOL_STR(wait_for_completion) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0x5f754e5a, __VMLINUX_SYMBOL_STR(memset) },
	{ 0x332bc748, __VMLINUX_SYMBOL_STR(device_del) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x29c95904, __VMLINUX_SYMBOL_STR(nvt_ipc_msg_get) },
	{ 0x11eff206, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0xad330df8, __VMLINUX_SYMBOL_STR(nvt_ipc_shm_flush) },
	{ 0x2cdb4060, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x34811d4e, __VMLINUX_SYMBOL_STR(nvt_ipc_ftok) },
	{ 0x42a4f0ce, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0x3bd1b1f6, __VMLINUX_SYMBOL_STR(msecs_to_jiffies) },
	{ 0x5d849e5e, __VMLINUX_SYMBOL_STR(wait_for_completion_interruptible_timeout) },
	{ 0x141ad2ba, __VMLINUX_SYMBOL_STR(wake_up_process) },
	{ 0xacee4727, __VMLINUX_SYMBOL_STR(nvt_ipc_shm_get) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xff16526f, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x1a7191e7, __VMLINUX_SYMBOL_STR(complete) },
	{ 0xf190d86b, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0xf1b866b1, __VMLINUX_SYMBOL_STR(completion_done) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

