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
	{ 0x12da5bb2, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0xa3029851, __VMLINUX_SYMBOL_STR(nvt_ipc_msg_rcv) },
	{ 0x4ceaaa92, __VMLINUX_SYMBOL_STR(nvt_ipc_msg_snd) },
	{ 0xd6ee688f, __VMLINUX_SYMBOL_STR(vmalloc) },
	{ 0x46acc4ef, __VMLINUX_SYMBOL_STR(single_open) },
	{ 0x28b7019c, __VMLINUX_SYMBOL_STR(nvt_ipc_msg_rel) },
	{ 0x8bd02cf, __VMLINUX_SYMBOL_STR(single_release) },
	{ 0x92785fa6, __VMLINUX_SYMBOL_STR(nvt_ipc_shm_rel) },
	{ 0x44e3300d, __VMLINUX_SYMBOL_STR(__register_chrdev) },
	{ 0x514cc273, __VMLINUX_SYMBOL_STR(arm_copy_from_user) },
	{ 0xda8ec279, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x85df9b6c, __VMLINUX_SYMBOL_STR(strsep) },
	{ 0x999e8297, __VMLINUX_SYMBOL_STR(vfree) },
	{ 0xe05177d1, __VMLINUX_SYMBOL_STR(seq_read) },
	{ 0xadae42fe, __VMLINUX_SYMBOL_STR(kthread_create_on_node) },
	{ 0x80f30ede, __VMLINUX_SYMBOL_STR(__platform_driver_register) },
	{ 0x189c5980, __VMLINUX_SYMBOL_STR(arm_copy_to_user) },
	{ 0x4f594d8a, __VMLINUX_SYMBOL_STR(proc_remove) },
	{ 0xdd62505d, __VMLINUX_SYMBOL_STR(__init_waitqueue_head) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0x5f754e5a, __VMLINUX_SYMBOL_STR(memset) },
	{ 0xf95b3c34, __VMLINUX_SYMBOL_STR(proc_mkdir) },
	{ 0x332bc748, __VMLINUX_SYMBOL_STR(device_del) },
	{ 0x37befc70, __VMLINUX_SYMBOL_STR(jiffies_to_msecs) },
	{ 0xf65b679b, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x71872311, __VMLINUX_SYMBOL_STR(kthread_stop) },
	{ 0x29c95904, __VMLINUX_SYMBOL_STR(nvt_ipc_msg_get) },
	{ 0x84b183ae, __VMLINUX_SYMBOL_STR(strncmp) },
	{ 0x11eff206, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0xad330df8, __VMLINUX_SYMBOL_STR(nvt_ipc_shm_flush) },
	{ 0x2cdb4060, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x34811d4e, __VMLINUX_SYMBOL_STR(nvt_ipc_ftok) },
	{ 0x4760cc1b, __VMLINUX_SYMBOL_STR(platform_device_unregister) },
	{ 0x59e5070d, __VMLINUX_SYMBOL_STR(__do_div64) },
	{ 0x42a4f0ce, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0x29a22b73, __VMLINUX_SYMBOL_STR(platform_device_register) },
	{ 0x3bd1b1f6, __VMLINUX_SYMBOL_STR(msecs_to_jiffies) },
	{ 0x1000e51, __VMLINUX_SYMBOL_STR(schedule) },
	{ 0xd62c833f, __VMLINUX_SYMBOL_STR(schedule_timeout) },
	{ 0x54a9db5f, __VMLINUX_SYMBOL_STR(_kstrtoul) },
	{ 0x141ad2ba, __VMLINUX_SYMBOL_STR(wake_up_process) },
	{ 0x717f0b7, __VMLINUX_SYMBOL_STR(sched_setscheduler) },
	{ 0x59237210, __VMLINUX_SYMBOL_STR(__wake_up) },
	{ 0xb3f7646e, __VMLINUX_SYMBOL_STR(kthread_should_stop) },
	{ 0xa81f865, __VMLINUX_SYMBOL_STR(prepare_to_wait_event) },
	{ 0xec2cd296, __VMLINUX_SYMBOL_STR(proc_create_data) },
	{ 0xacee4727, __VMLINUX_SYMBOL_STR(nvt_ipc_shm_get) },
	{ 0xd15ac7c0, __VMLINUX_SYMBOL_STR(seq_lseek) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x9d669763, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xff16526f, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x87280639, __VMLINUX_SYMBOL_STR(finish_wait) },
	{ 0xfb961d14, __VMLINUX_SYMBOL_STR(__arm_ioremap) },
	{ 0xb25a57f2, __VMLINUX_SYMBOL_STR(platform_driver_unregister) },
	{ 0xf190d86b, __VMLINUX_SYMBOL_STR(__class_create) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

