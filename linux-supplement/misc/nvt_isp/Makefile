
#CFLAGS := -Wall -fPIC

# 680: "NT9668x" / 510: "NT9651x"
EXTRA_CFLAGS := -Wno-date-time -D__SOC_680_PLATFORM__

obj-m := nvt_isp680.o
nvt_isp680-objs := ae.o ae_proc.o af.o af_proc.o awb.o awb_proc.o sensor.o lens.o isp_middleware.o isp_hardware.o isp_ipc.o \
                   isp_lib.o isp_alg_inf.o isp_input_inf.o isp_proc.o util_cfg.o isp_cfg.o isp_dump_cfg.o isp_drv.o isp_api.o

msg:
	@echo 'nvt_isp680 compile done'
