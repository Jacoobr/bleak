/*-
 * Copyright (c) 2018 Nathan Lay (enslay@gmail.com)
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

#ifndef BLEAK_HINGELOSS_H
#define BLEAK_HINGELOSS_H

#include <algorithm>
#include "Vertex.h"

namespace bleak {

template<typename RealType>
class HingeLoss : public Vertex<RealType> {
public:
  bleakNewVertex(HingeLoss, Vertex<RealType>,
    bleakAddInput("inData"),
    bleakAddInput("inLabels"),
    bleakAddOutput("outLoss"),
    bleakAddProperty("penaltyWeight", m_fPenaltyWeight),
    bleakAddProperty("margin", m_fMargin));

  bleakForwardVertexTypedefs();

  virtual ~HingeLoss() = default;

  virtual bool SetSizes() override {
    bleakGetAndCheckInput(p_clInData, "inData", false);
    bleakGetAndCheckInput(p_clInLabels, "inLabels", false);
    bleakGetAndCheckOutput(p_clOutLoss, "outLoss", false);

    if (m_fMargin <= 0.0f) {
      std::cerr << GetName() << ": Error: margin is expected to be a positive quantity." << std::endl;
      return false;
    }

    const ArrayType &clInData = p_clInData->GetData();
    const ArrayType &clInLabels = p_clInLabels->GetData();

    if (!clInData.GetSize().Valid() || !clInLabels.GetSize().Valid()) {
      std::cerr << GetName() << ": Error: inData and/or inLabel input sizes are invalid." << std::endl;
      return false;
    }

    if (clInData.GetSize().GetDimension() < 2) {
      std::cerr << GetName() << ": Error: inData is expected to be at least 2D." << std::endl;
      return false;
    }

    if (clInData.GetSize()[1] < 2) {
      std::cerr << GetName() << ": Error: Invalid number of channels in " << clInData.GetSize() << ". Number of channels should be 2 or larger." << std::endl;
      return false;
    }

    if (clInLabels.GetSize().GetDimension()+1 != clInData.GetSize().GetDimension()) {
      std::cerr << GetName() << ": Error: Incompatible dimensions: inLabels = " << clInLabels.GetSize().GetDimension() << ", inData = " << clInData.GetSize().GetDimension() << std::endl;
      return false;
    }

    if (clInLabels.GetSize()[0] != clInData.GetSize()[0] || clInLabels.GetSize().SubSize(1) != clInData.GetSize().SubSize(2)) {
      std::cerr << GetName() << ": Error: Incompatible sizes: inLabels = " << clInLabels.GetSize() << ", inData = " << clInData.GetSize() << std::endl;
      return false;
    }

    Size clSize = { 1 };

    p_clOutLoss->GetData().SetSize(clSize);
    p_clOutLoss->GetGradient().SetSize(clSize);

    return true;
  }

  virtual bool Initialize() override {
    return true; // Nothing to do
  }

  virtual void Forward() override {
    bleakGetAndCheckInput(p_clInData, "inData");
    bleakGetAndCheckInput(p_clInLabels, "inLabels");
    bleakGetAndCheckOutput(p_clOutLoss, "outLoss");

    const ArrayType &clInData = p_clInData->GetData();
    const ArrayType &clInLabels = p_clInLabels->GetData();

    RealType &outLoss = *p_clOutLoss->GetData().data();

    const RealType * const p_inData = clInData.data();
    const RealType * const p_inLabels = clInLabels.data();

    const int iOuterNum = clInData.GetSize()[0];
    const int iNumClasses = clInData.GetSize()[1];
    const int iInnerNum = clInData.GetSize().Product(2);

    outLoss = RealType();

    for (int i = 0; i < iOuterNum; ++i) {
      for (int k = 0; k < iInnerNum; ++k) {
        const int iLabel = (int)(p_inLabels[i*iInnerNum + k]);

        if (iLabel < 0 || iLabel >= iNumClasses) // TODO: Warning?
          continue;

        const RealType labelInput = p_inData[(i*iNumClasses + iLabel)*iInnerNum + k];

        for (int j = 0; j < iLabel; ++j)
          outLoss += std::max(RealType(0), p_inData[(i*iNumClasses + j)*iInnerNum + k] - labelInput + RealType(m_fMargin));

        for (int j = iLabel+1; j < iNumClasses; ++j)
          outLoss += std::max(RealType(0), p_inData[(i*iNumClasses + j)*iInnerNum + k] - labelInput + RealType(m_fMargin));
      }
    }

    outLoss *= RealType(m_fPenaltyWeight)/iOuterNum;
  }

  virtual void Backward() override {
    bleakGetAndCheckInput(p_clInData, "inData");
    bleakGetAndCheckInput(p_clInLabels, "inLabels");
    bleakGetAndCheckOutput(p_clOutLoss, "outLoss");

    const ArrayType &clInData = p_clInData->GetData();
    ArrayType &clInDataGradient = p_clInData->GetGradient();
    const ArrayType &clInLabels = p_clInLabels->GetData(); // We don't backpropagate labels

    if (!clInDataGradient.Valid())
      return; // Nothing to do

    const RealType &outLoss = *p_clOutLoss->GetData().data();
    RealType &outLossGradient = *p_clOutLoss->GetGradient().data();

    if (IsLeaf())
      outLossGradient = RealType(1);

    const RealType * const p_inData = clInData.data();
    RealType * const p_inDataGradient = clInDataGradient.data();
    const RealType * const p_inLabels = clInLabels.data();

    const int iOuterNum = clInData.GetSize()[0];
    const int iNumClasses = clInData.GetSize()[1];
    const int iInnerNum = clInData.GetSize().Product(2);

    const RealType scale = RealType(m_fPenaltyWeight)*outLossGradient/RealType(iOuterNum);

    for (int i = 0; i < iOuterNum; ++i) {
      for (int k = 0; k < iInnerNum; ++k) {
        const int iLabel = (int)(p_inLabels[i*iInnerNum + k]);

        if (iLabel < 0 || iLabel >= iNumClasses) // TODO: Warning
          continue;

        const RealType labelInput = p_inData[(i*iNumClasses + iLabel)*iInnerNum + k];

        for (int j = 0; j < iLabel; ++j) {
          const RealType diff = (p_inData[(i*iNumClasses + j)*iInnerNum + k] - labelInput + RealType(m_fMargin) > RealType(0))*scale;
          p_inDataGradient[(i*iNumClasses + j)*iInnerNum + k] += diff;
          p_inDataGradient[(i*iNumClasses + iLabel)*iInnerNum + k] -= diff;
        }

        for (int j = iLabel+1; j < iNumClasses; ++j) {
          const RealType diff = (p_inData[(i*iNumClasses + j)*iInnerNum + k] - labelInput + RealType(m_fMargin) > RealType(0))*scale;
          p_inDataGradient[(i*iNumClasses + j)*iInnerNum + k] += diff;
          p_inDataGradient[(i*iNumClasses + iLabel)*iInnerNum + k] -= diff;
        }
      }
    }
  }

protected:
  HingeLoss() = default;

private:
  float m_fPenaltyWeight = 1.0f;
  float m_fMargin = 1.0f;
};

} // end namespace bleak

#endif // !BLEAK_HINGELOSS_H
