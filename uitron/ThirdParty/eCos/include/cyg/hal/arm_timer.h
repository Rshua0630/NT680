#ifndef ARM_TIMER_H
#define ARM_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#define ARM_TIMER_LOAD_OFFSET                   0x00       /**< Timer Load Register */
#define ARM_TIMER_COUNTER_OFFSET                0x04       /**< Timer Counter Register */
#define ARM_TIMER_CONTROL_OFFSET                0x08       /**< Timer Control Register */
#define ARM_TIMER_ISR_OFFSET                    0x0C       /**< Timer Interrupt Status Register */


#define ARM_TIMER_CONTROL_PRESCALER_MASK        0x0000FF00 /**< Prescaler */
#define ARM_TIMER_CONTROL_PRESCALER_SHIFT       8
#define ARM_TIMER_CONTROL_IRQ_ENABLE_MASK       0x00000004 /**< Intr enable */
#define ARM_TIMER_CONTROL_AUTO_RELOAD_MASK      0x00000002 /**< Auto-reload */
#define ARM_TIMER_CONTROL_ENABLE_MASK           0x00000001 /**< Timer enable */

#define ARM_TIMER_ISR_EVENT_FLAG_MASK           0x00000001 /**< Event flag */

#ifdef __cplusplus
}
#endif

#endif
