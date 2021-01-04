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

import com.google.android.exoplayer2.decoder.DecoderInputBuffer;
import com.google.android.exoplayer2.decoder.SimpleDecoder;
import com.google.android.exoplayer2.decoder.SimpleOutputBuffer;

import java.nio.ByteBuffer;

/**
 * Daa decoder.
 */
/* package */ final class DaaDecoder extends
    SimpleDecoder<DecoderInputBuffer, SimpleOutputBuffer, DaaDecoderException> {

  private DaaDecoderJni mDecoderJni;
  private int mChannelCount;
  private int mSampleRate;
  private final int mMaxOutputBufferSize;

  /* The following definitions must be aligned with ones in dlb_decode_api.h */
  private final int DLB_DECODE_CTL_DAP_ONOFF_ID = 0;
  private final int DLB_DECODE_CTL_ENDPOINT_ID  = 1;
  private final int DLB_DECODE_DAP_OFF = 0;
  private final int DLB_DECODE_DAP_ON = 1;
  private final int DLB_DECODE_ENDP_STEREO_SPEAKER = 1;
  private final int DLB_DECODE_ENDP_STEREO_HEADPHONE = 2;

  /**
   * Creates an DAA decoder.
   *
   * @param numInputBuffers The number of input buffers.
   * @param numOutputBuffers The number of output buffers.
   * @param initialInputBufferSize The initial size of each input buffer.
   * @param isAC4Content The input content is AC-4 content or not.
   * @param isSyncFrame The AC-4 content is sync frame or not(raw frame).
   * @param isJocContent The input content is DD+JOC content or not(legacy DD+ content).
   * @throws DaaDecoderException Thrown if an exception occurs when initializing the decoder.
   */
  public DaaDecoder(
      int numInputBuffers,
      int numOutputBuffers,
      int initialInputBufferSize,
      boolean isAC4Content,
      boolean isSyncFrame,
      boolean isJocContent)
      throws DaaDecoderException {
    super(new DecoderInputBuffer[numInputBuffers], new SimpleOutputBuffer[numOutputBuffers]);

    mDecoderJni = new DaaDecoderJni(isAC4Content, isSyncFrame, isJocContent);
    setInitialInputBufferSize(initialInputBufferSize);
    mMaxOutputBufferSize = mDecoderJni.getOutputBufferSize();
    mChannelCount = mDecoderJni.getChannelCount();
    mSampleRate = mDecoderJni.getSampleRate();
    mDecoderJni.setParameter(DLB_DECODE_CTL_DAP_ONOFF_ID, DLB_DECODE_DAP_ON);
    mDecoderJni.setParameter(DLB_DECODE_CTL_ENDPOINT_ID, DLB_DECODE_ENDP_STEREO_HEADPHONE);
  }

  @Override
  public String getName() {
    return "libdaa";
  }

  @Override
  protected DecoderInputBuffer createInputBuffer() {
    return new DecoderInputBuffer(DecoderInputBuffer.BUFFER_REPLACEMENT_MODE_DIRECT);
  }

  @Override
  protected SimpleOutputBuffer createOutputBuffer() {
    return new SimpleOutputBuffer(this);
  }

  @Override
  protected DaaDecoderException createUnexpectedDecodeException(Throwable error) {
    return new DaaDecoderException("Unexpected decode error", error);
  }

  @Override
  protected DaaDecoderException decode(DecoderInputBuffer inputBuffer,
      SimpleOutputBuffer outputBuffer, boolean reset) {
    if (reset) {
      mDecoderJni.flush();
    }
    ByteBuffer inputData = inputBuffer.data;
    int  inputSize = inputData.limit();
    ByteBuffer outputData = outputBuffer.init(inputBuffer.timeUs, mMaxOutputBufferSize);
    int result = mDecoderJni.decodeSample(inputData, inputSize, outputData, mMaxOutputBufferSize);

    if (result < 0) {
      return new DaaDecoderException("Frame decoding failed");
    } else if (result > 0){
      mChannelCount = mDecoderJni.getChannelCount();
      mSampleRate = mDecoderJni.getSampleRate();
      long delayUs = mDecoderJni.getLatencyUs();
      outputBuffer.timeUs -= delayUs;
    }
    outputBuffer.data.position(0);
    outputBuffer.data.limit(result);
    return null;
  }

  @Override
  public void release() {
    super.release();
    mDecoderJni.release();
    mDecoderJni = null;
  }

  public int getChannelCount() {
    return mChannelCount;
  }

  public int getSampleRate() {
    return mSampleRate;
  }
}
