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

#include "CudnnCommon.h"
#include "VertexFactory.h"
#include "CudnnConvolution.h"
#include "CudnnPooling.h"

namespace bleak {

template<typename RealType>
void InitializeCudnnModuleTemplate() {
  VertexFactory<RealType> &clVertexFactory = VertexFactory<RealType>::GetInstance();

  clVertexFactory.template Register<CudnnConvolution1D<RealType>>();
  clVertexFactory.template Register<CudnnConvolution2D<RealType>>();
  clVertexFactory.template Register<CudnnConvolution3D<RealType>>();

  clVertexFactory.template Register<CudnnMaxPooling1D<RealType>>();
  clVertexFactory.template Register<CudnnMaxPooling2D<RealType>>();
  clVertexFactory.template Register<CudnnMaxPooling3D<RealType>>();

  clVertexFactory.template Register<CudnnMeanPooling1D<RealType>>();
  clVertexFactory.template Register<CudnnMeanPooling2D<RealType>>();
  clVertexFactory.template Register<CudnnMeanPooling3D<RealType>>();
}

void InitializeCudnnModule() {
  InitializeCudnn();

  InitializeCudnnModuleTemplate<float>();
  InitializeCudnnModuleTemplate<double>();
}

} // end namespace bleak
