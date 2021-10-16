/*
 * fft.h
 *
 *  Created on: 3 ott 2021
 *      Author: Brian
 */

#ifndef INC_FFT_H_
#define INC_FFT_H_

#include "system.h"

void start_fft();
void timer_fft(lv_timer_t* timer);
void fft_sample_callback();
void exit_fft();

double hamming(int n, int N) {
	return (0.54 - 0.46 * cos(2.0 * M_PI * (double)n / (double)(N-1)));
}

uint32_t log_2(uint32_t N) {
	uint32_t k = N;
	uint32_t i = 0;
	while (k) {
		k >>= 1;
		i++;
	}
	return i - 1;
}

uint32_t ispowof2(uint32_t n) {
	return n > 0 && (n & (n - 1)) == 0;
}

uint32_t reverse(uint32_t N, uint32_t n) {
	uint32_t j, p = 0;
	for (j = 1; j <= log_2(N); j++) {
		if (n & (1 << (uint32_t) (log_2(N) - j)))
			p |= (1 << (j - 1));
	}
	return p;
}

void ordina(double complex f1[], uint32_t N) {
	double complex f2[N];
	for (uint32_t i = 0; i < N; i++)
		f2[i] = f1[reverse(N, i)];
	for (uint32_t j = 0; j < N; j++)
		f1[j] = f2[j];
}

void transform(double complex f[], uint32_t N) {
	ordina(f, N);
	double complex W[N / 2];
	W[1] = cos(-2.0 * M_PI / N) + sin(-2.0 * M_PI / N) * I;
	W[0] = 1.0 + 0.0 * I;
	for (uint32_t i = 2; i < N / 2; i++)
		W[i] = cpowf(W[1], i);
	uint32_t n = 1;
	uint32_t a = N / 2;
	for (uint32_t j = 0; j < log_2(N); j++) {
		for (uint32_t i = 0; i < N; i++) {
			if (!(i & n)) {
				double complex temp = f[i];
				double complex Temp = W[(i * a) % (n * a)] * f[i + n];
				f[i] = temp + Temp;
				f[i + n] = temp - Temp;
			}
		}
		n *= 2;
		a = a / 2;
	}
}

void FFT(double complex f[], uint32_t N, double d) {
	for (uint32_t n = 0; n < N; n++) f[n] *= hamming(n, N);
	transform(f, N);
	for (uint32_t n = 0; n < N; n++) f[n] *= d;
}



#endif /* INC_FFT_H_ */
