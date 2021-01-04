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
package com.dolby.daa;

import java.nio.ByteBuffer;

/**
 * JNI wrapper for the DAA decoder.
 */
/* package */ final class DaaDecoderJni {

  private long nativeDecoderContext;

  public DaaDecoderJni(boolean isAC4Content, boolean isSyncFrame, boolean isJocContent) throws
      DaaDecoderException {
    if (!DaaLibrary.isAvailable()) {
      throw new DaaDecoderException("Failed to load decoder native libraries.");
    }
    nativeDecoderContext = DaaInit(isAC4Content, isSyncFrame, isJocContent);
    if (nativeDecoderContext == 0) {
      throw new DaaDecoderException("Failed to initialize decoder");
    }
  }

  public int decodeSample(ByteBuffer inputBuffer, int inputSize, ByteBuffer outputBuffer,
      int outputSize) {
    return DaaDecode(nativeDecoderContext, inputBuffer, inputSize, outputBuffer, outputSize);
  }

  public void flush() {
    //TODO: Nothing
  }

  public void reset(long newPosition) {
    //TODO: Nothing
  }

  public void release() {
    DaaClose(nativeDecoderContext);
    nativeDecoderContext = 0;
  }

  public int getOutputBufferSize() {
    return DaaGetMaxOutputBufferSize(nativeDecoderContext);
  }

  public int getChannelCount() {
    return DaaGetChannelCount(nativeDecoderContext);
  }

  public int getSampleRate() {
    return DaaGetSampleRate(nativeDecoderContext);
  }

  public long getLatencyUs() {
    return DaaGetLatencyUs(nativeDecoderContext);
  }

  public int setParameter(int paramId, int paramValue){
    return DaaSetParameter(nativeDecoderContext, paramId, paramValue);
  }

  private native long DaaInit(boolean isAC4Content, boolean isSyncFrame, boolean isJocContent);

  private native int DaaDecode(long context, ByteBuffer inputBuffer, int inputSize,
      ByteBuffer outputBuffer, int outputSize);

  private native void DaaClose(long context);

  private native int DaaGetMaxOutputBufferSize(long context);

  private native int DaaGetChannelCount(long context);

  private native int DaaGetSampleRate(long context);

  private native long DaaGetLatencyUs(long context);

  private native int DaaSetParameter(long context, int paramId, int paramValue);

}
