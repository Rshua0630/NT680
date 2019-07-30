#ifndef CYGONCE_HAL_CACHE_H
#define CYGONCE_HAL_CACHE_H

/* Cache properties */
// Ben Tsai modify for 680 start
#if 0
#define HAL_ICACHE_LINE_SIZE                (32)
#define HAL_DCACHE_LINE_SIZE                (32)
#else
#define HAL_ICACHE_LINE_SIZE                (64)
#define HAL_DCACHE_LINE_SIZE                (64)
#endif
// Ben Tsai modify for 680 end
#define HAL_DCACHE_ALIGNED(x)               (((x) % HAL_DCACHE_LINE_SIZE) == 0)

#define HAL_ICACHE_WAYS                     (4)
#define HAL_DCACHE_WAYS                     (4)

#define HAL_ICACHE_SETS                     (256)
#define HAL_DCACHE_SETS                     (256)

#define HAL_ICACHE_SIZE                     (HAL_ICACHE_LINE_SIZE * HAL_ICACHE_WAYS * HAL_ICACHE_SETS)
#define HAL_DCACHE_SIZE                     (HAL_DCACHE_LINE_SIZE * HAL_DCACHE_WAYS * HAL_DCACHE_SETS)

/* L2 on/off */
void hal_scache_setup(void);
void hal_scache_enable(void);
void hal_scache_disable(void);

/* L2 sync */
void hal_scache_sync(void);

/* L2 cache (all) */
void hal_scache_invalidate_all(void);
void hal_scache_flush_invalidate_all(void);

/* L2 cache (by MVA) */
void hal_scache_invalidate(unsigned long start, unsigned long size);
void hal_scache_flush(unsigned long start, unsigned long size);
void hal_scache_flush_invalidate(unsigned long start, unsigned long size);

/* L1 on/off */
void hal_icache_enable(void);
void hal_dcache_enable(void);
#define HAL_ICACHE_ENABLE()                 hal_icache_enable()
#define HAL_DCACHE_ENABLE()                 hal_dcache_enable()

void hal_icache_disable(void);
void hal_dcache_disable(void);
#define HAL_ICACHE_DISABLE()	            hal_icache_disable()
#define HAL_DCACHE_DISABLE()	            hal_dcache_disable()

/* L1 sync */
void hal_icache_sync(void);
void hal_dcache_sync(void);
#define HAL_ICACHE_SYNC()                   hal_icache_sync()
#define HAL_DCACHE_SYNC()                   hal_dcache_sync()

/* L1 (all) */
void hal_icache_invalidate_all(void);
void hal_dcache_invalidate_all(void);
void hal_dcache_flush_invalidate_all(void);
#define HAL_ICACHE_INVALIDATE_ALL()         hal_icache_invalidate_all()
#define HAL_DCACHE_INVALIDATE_ALL()         hal_dcache_invalidate_all()
#define HAL_DCACHE_FLUSH_INVALIDATE_ALL()   hal_dcache_flush_invalidate_all()

/* L1 D-cache (by MVA) */
void hal_dcache_flush(unsigned long start, unsigned long size);
void hal_dcache_invalidate(unsigned long start, unsigned long size);
void hal_dcache_flush_invalidate(unsigned long start, unsigned long size);
#define HAL_DCACHE_FLUSH(base, size)            hal_dcache_flush(base, size)
#define HAL_DCACHE_INVALIDATE(base, size)       hal_dcache_invalidate(base, size)
#define HAL_DCACHE_FLUSH_INVALIDATE(base, size) hal_dcache_flush_invalidate(base, size)

/* For DMA */
void _dma_cache_inv(unsigned long start, unsigned long size);
void _dma_cache_wback(unsigned long start, unsigned long size);
void _dma_cache_wback_inv(unsigned long start, unsigned long size);
/* Use this function to allocate cache-aligned DMA buffer from system heap.
 * Usage:
 *     dma_buf: The cache-aligned pointer for DMA operations.
 *     normal_buf: The origin non-cache-aligned pointer. You have to free the buffer using this one.
 *     size: Allocation size. Should be aligned to cache line size.
 * Example:
 *     char *normal_buf;
 *     char *dma_buf;
 *
 *     dma_malloc(&normal_buf, &dma_buf, 1024);
 *     ...
 *     // Do DMA operations using dma_buf
 *     ...
 *     // Release memory using normal_buf
 *     free(normal_buf);
 */
void dma_malloc(void **normal_buf, void **dma_buf, unsigned long size);

#endif
