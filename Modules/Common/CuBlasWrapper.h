/*-
 * Copyright (c) 2020 Nathan Lay (enslay@gmail.com)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR(S) ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR(S) BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#ifndef BLEAK_CUBLASWRAPPER_H
#define BLEAK_CUBLASWRAPPER_H

#include "BlasWrapper.h"
#include "cublas_v2.h"

namespace bleak {
namespace gpu_blas {

// Setup per-thread context
void Initialize();

// Level 1

extern template void swap<float>(int n, float *x, int incx, float *y, int incy);
extern template void swap<double>(int n, double *x, int incx, double *y, int incy);

extern template void scal<float>(int n, const float &a, float *x, int incx);
extern template void scal<double>(int n, const double &a, double *x, int incx);

extern template void copy<float>(int n, const float *x, int incx, float *y, int incy);
extern template void copy<double>(int n, const double *x, int incx, double *y, int incy);

extern template void axpy<float>(int n, const float &a, const float *x, int incx, float *y, int incy);
extern template void axpy<double>(int n, const double &a, const double *x, int incx, double *y, int incy);

extern template float dot<float>(int n, const float *x, int incx, const float *y, int incy);
extern template double dot<double>(int n, const double *x, int incx, const double *y, int incy);

extern template void dot<float>(int n, const float *x, int incx, const float *y, int incy, float &result);
extern template void dot<double>(int n, const double *x, int incx, const double *y, int incy, double &result);

extern template float nrm2<float>(int n, const float *x, int incx);
extern template double nrm2<double>(int n, const double *x, int incx);

extern template void nrm2<float>(int n, const float *x, int incx, float &result);
extern template void nrm2<double>(int n, const double *x, int incx, double &result);

extern template float asum<float>(int n, const float *x, int incx);
extern template double asum<double>(int n, const double *x, int incx);

extern template void asum<float>(int n, const float *x, int incx, float &result);
extern template void asum<double>(int n, const double *x, int incx, double &result);

extern template int amax<float>(int n, const float *x, int incx);
extern template int amax<double>(int n, const double *x, int incx);

extern template void amax<float>(int n, const float *x, int incx, int &result);
extern template void amax<double>(int n, const double *x, int incx, int &result);

// Level 2
extern template void gemv<float>(char trans, int m, int n, const float &alpha, const float *a, int lda, const float *x, int incx, const float &beta, float *y, int incy);
extern template void gemv<double>(char trans, int m, int n, const double &alpha, const double *a, int lda, const double *x, int incx, const double &beta, double *y, int incy);

// Level 3
extern template void gemm<float>(char transa, char transb, int m, int n, int k, const float &alpha, const float *a, int lda, const float *b, int ldb, const float &beta, float *c, int ldc);
extern template void gemm<double>(char transa, char transb, int m, int n, int k, const double &alpha, const double *a, int lda, const double *b, int ldb, const double &beta, double *c, int ldc);

} // end namespace gpu_blas
} // end namespace bleak

#endif // !BLEAK_CUBLASWRAPPER_H