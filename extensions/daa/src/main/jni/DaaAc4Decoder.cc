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
#include <cstdlib>
#include <cstring>

#include "DaaAc4Decoder.h"

int DaaAc4Decoder::open(bool isSyncFrame) {
  LOGD("DaaAc4Decoder::open");

  int err;
  dlb_decode_query_ip queryIp;
  dlb_decode_query_info_op queryOp;
  dlb_decode_query_mem_op queryMemOp;
  char *pDecMem;

  mDelayUs = 0;
  mOutputChannelCount = 2; /* DAA library output channel is always 2 */
  mDecoder = NULL;
  mTimescale = 48000;

  /* Initialize DAA AC4 configuration based on stream type */
  memset(&queryIp, 0, sizeof(queryIp));
  memset(&queryOp, 0, sizeof(queryOp));
  memset(&queryMemOp, 0, sizeof(queryMemOp));

  queryIp.output_datatype = DLB_BUFFER_SHORT_16;
  queryIp.input_bitstream_format = DLB_DECODE_INPUT_FORMAT_AC4;
  queryIp.input_bitstream_type = isSyncFrame ?
      DLB_DECODE_INPUT_TYPE_AC4_SIMPLE_TRANSPORT :
      DLB_DECODE_INPUT_TYPE_AC4_RAW_FRAME;
  queryIp.timescale = mTimescale;

  err = dlb_decode_query_info(&queryIp, &queryOp);
  if (err != DLB_DECODE_ERR_NO_ERROR) {
    LOGE(" Failed to query decoder information");
    return -1;
  }
  LOGV("DAA-AC4 Decoder information:\n\
      DAA library version: %s \n\
      DAA decoder version: %s",
      queryOp.daa_version,
      queryOp.core_decoder_version);

  err = dlb_decode_query_memory(&queryIp, &queryMemOp);
  if (err != DLB_DECODE_ERR_NO_ERROR) {
    LOGE("Failed to query decoder memory");
    return -1;
  }
  LOGV("DAA-AC4 Memory requirements:\n\
      Decoder Size: %d bytes\n\
      Output Buffers: %d bytes",
      (int)queryMemOp.decoder_size,
      (int)queryMemOp.output_buffer_size);
  mMaxOutputBufferSize = queryMemOp.output_buffer_size;

  /* Allocate memory for decoder */
  pDecMem = (char *)calloc(1, queryMemOp.decoder_size);
  if (pDecMem == NULL) {
    LOGE("Failed to allocate decoder memory");
    return -1;
  }
  mDecoder = (void *)((long)(pDecMem));

  err = dlb_decode_open(&queryIp, mDecoder);
  if (err != DLB_DECODE_ERR_NO_ERROR) {
    LOGE("Failed to open DAA-AC4 decoder");
    if (mDecoder != NULL){
      free(mDecoder);
      mDecoder = NULL;
    }
    return -1;
  }

  /* Set decoder parameter */

  /* Initialise dlb_buffer for output buffer */
  mDlbOutBuf.ppdata = (void **)calloc(1, sizeof(void *) * DLB_DECODE_MAX_PCMOUT_CHANNELS);
  if (mDlbOutBuf.ppdata == NULL) {
    LOGE("Failed to allocate memory for dlbBuffer");
    if (mDecoder != NULL){
      free(mDecoder);
      mDecoder = NULL;
    }
    return -1;
  }
  memset(mDlbOutBuf.ppdata, 0, sizeof(void *) * mOutputChannelCount);

  mDlbOutBuf.nchannel = (unsigned)mOutputChannelCount;
  mDlbOutBuf.nstride = mDlbOutBuf.nchannel;
  mDlbOutBuf.data_type = DLB_BUFFER_SHORT_16;

#ifdef AUDIO_DUMP
  mDaaOutFile = fopen("/sdcard/ac4out.pcm", "wb");
  if(mDaaOutFile == NULL){
    LOGE("AC4Decoder::open dump file failed");
  } else {
    LOGD("AC4Decoder::open dump file success");
  }
#endif

  LOGD("DaaAc4Decoder::open exit");
  return 0;
}

int DaaAc4Decoder::decode(
    char *const inputBuffer, unsigned int inputSize, char *const outputBuffer, int outputSize) {
  LOGD("DaaAc4Decoder::decode");

  size_t outSamplesInBytes = 0;
  int err = DLB_DECODE_ERR_NO_ERROR;
  unsigned int bytesConsumed = 0;
  int timesliceCompleted = 0;
  dlb_decode_io_params ioParams = {0};
  mDlbOutBuf.ppdata[0] = outputBuffer;
  ioParams.pcm_output_buf = &mDlbOutBuf;
    
  /* Read an encoded frame from input buffer */
  err = dlb_decode_addbytes(
      mDecoder, inputBuffer, inputSize, 0, &bytesConsumed, &timesliceCompleted);

  LOGV("dlb_decode_addbytes()\n\
      mem = %p: avail = %d, bytesconsumed = %d, timeslicecompleted = %d, err = %d",
      (void*)inputBuffer, inputSize, bytesConsumed, timesliceCompleted, err);
  /* Process a frame and write output into dlb buffer */
  if (err == DLB_DECODE_ERR_NO_ERROR && timesliceCompleted) {
    err = dlb_decode_process(mDecoder, &ioParams);
    if (err != DLB_DECODE_ERR_NO_ERROR) {
      LOGE("dlb_decode_process failed %d.", err);
      return -1;
    } else {
      outSamplesInBytes =
          ioParams.output_samples_num * ioParams.output_channels_num * sizeof(short);
      mSampleRate = (int)ioParams.output_sample_rate;
      mDelayUs = (long)((ioParams.output_timestamp / (float)mTimescale) * 1000000 * (-1));
      LOGD("dlb_decode_process() output_samples = %d err = %d", ioParams.output_samples_num, err);
#ifdef AUDIO_DUMP
      if (mDaaOutFile != NULL) {
        fwrite(outputBuffer, outSamplesInBytes, 1, mDaaOutFile);
      }
#endif
    }
  } else {
    LOGE("dlb_decode_addbytes failed %d.", err);
    return -1;
  }
  LOGD("DaaAc4Decoder::decode exit");

  return (int)outSamplesInBytes;
}

int DaaAc4Decoder::getMaxOutputBufferSize() {
  return (int)mMaxOutputBufferSize;
}

long DaaAc4Decoder::getLatencyUs() {
  return mDelayUs;
}

int DaaAc4Decoder::getOutputChannelCount() {
  return (int)mOutputChannelCount;
}

int DaaAc4Decoder::getSampleRate() {
  return (mSampleRate ? mSampleRate : 48000);
}

int DaaAc4Decoder::setParameter(int paramId, int paramValue) {
  return dlb_decode_setparam(mDecoder, paramId, &paramValue, sizeof(paramValue));
}

DaaAc4Decoder::~DaaAc4Decoder() {
  if (mDecoder != NULL) {
    dlb_decode_close(mDecoder);
    mDecoder = NULL;
  }
  if (mDlbOutBuf.ppdata != NULL) {
    free(mDlbOutBuf.ppdata);
    mDlbOutBuf.ppdata = NULL;
  }
#ifdef AUDIO_DUMP
  if (mDaaOutFile != NULL) {
    fclose(mDaaOutFile);
  }
#endif
}
