#ifndef ARM_PERF_H
#define ARM_PERF_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void arm_perf_array_add_c(uint8_t *arr, const uint8_t *num, uint32_t w, uint32_t h, uint32_t times);
void arm_perf_array_mul_c(uint8_t *arr, const uint8_t *num, uint32_t w, uint32_t h, uint32_t times);
void arm_perf_array_sub_c(uint8_t *arr, const uint8_t *num, uint32_t w, uint32_t h, uint32_t times);
void arm_perf_array_shift_add_c(uint8_t *arr2, uint8_t *arr1, uint8_t *arr0);
void arm_perf_fir_filter_c(short *output, const short *input, const short *kernel, int width, int kernelSize);

void arm_perf_array_add_neon(uint8_t *arr, const uint8_t *num, uint32_t w, uint32_t h, uint32_t times);
void arm_perf_array_mul_neon(uint8_t *arr, const uint8_t *num, uint32_t w, uint32_t h, uint32_t times);
void arm_perf_array_shift_add_neon(uint8_t *arr2, uint8_t *arr1, uint8_t *arr0);
void arm_perf_array_sub_neon(uint8_t *arr, const uint8_t *num, uint32_t w, uint32_t h, uint32_t times);
void arm_perf_fir_filter_neon(short *output, const short *input, const short *kernel, int width, int kernelSize);

#endif /* ARM_PERF_H */
