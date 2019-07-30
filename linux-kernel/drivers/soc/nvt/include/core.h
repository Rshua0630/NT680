extern struct smp_operations	nvt_ca9_smp_ops;
extern struct smp_operations	nvt_ca53_smp_ops;

extern void nvt_ca9_cpu_die(unsigned int cpu);

#define A9_MPCORE_SCU_OFFSET		   (0x0000)
#define A9_MPCORE_GIC_CPU_OFFSET	 (0x0100)
#define A9_MPCORE_GIT_OFFSET		   (0x0200)
#define A9_MPCORE_TWD_OFFSET		   (0x0600)
#define A9_MPCORE_GIC_DIST_OFFSET  (0x1000)

static inline unsigned long get_periph_base(void)
{
	unsigned long val;

	asm("mrc p15, 4, %0, c15, c0, 0 @ get CBAR"
	  : "=r" (val) : : "cc");
	return val;
}

static inline unsigned long get_periph_base_ca53(void)
{
	unsigned long val;

	asm("mrc p15, 1, %0, c15, c3, 0 @ get CBAR"
	  : "=r" (val) : : "cc");
	return val;
}
