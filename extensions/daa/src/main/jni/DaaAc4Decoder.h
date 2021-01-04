/*
 *  This program is protected under international and U.S. copyright laws as
 *  an unpublished work. This program is confidential and proprietary to the
 *  copyright owners. Reproduction or disclosure, in whole or in part, or the
 *  production of derivative works therefrom without the express permission of
 *  the copyright owners is prohibited.
 *
 *                 Copyright (C) 2018 by Dolby Laboratories,
 *                             All rights reserved.
 */
#ifndef EXOPLAYER_DAAAC4DECODER_H
#define EXOPLAYER_DAAAC4DECODER_H

#include "DaaDecoder.h"
#include "dlb_decode_api.h"
#include "dlb_buffer.h"

class DaaAc4Decoder : public DaaDecoder {
#define AUDIO_DUMP 1
public:
  ~DaaAc4Decoder();
  virtual int open(bool isSyncFrame);
  virtual int decode(
      char *const inputBuffer, unsigned int inputSize, char *const outputBuffer, int outBufSize);
  virtual int getOutputChannelCount();
  virtual int getSampleRate();
  virtual int getMaxOutputBufferSize();
  virtual long getLatencyUs();
  virtual int setParameter(int paramId, int paramValue);

private:
  size_t mOutputChannelCount;
  int mSampleRate;
  size_t mMaxOutputBufferSize;
  long mDelayUs;

  void *mDecoder;
  dlb_buffer mDlbOutBuf;
  unsigned long mTimescale;

#ifdef AUDIO_DUMP
  FILE* mDaaOutFile;
#endif
};

#endif //EXOPLAYER_DAAAC4DECODER_H
