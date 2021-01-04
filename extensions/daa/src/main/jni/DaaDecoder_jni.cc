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

#include <jni.h>
#include <android/log.h>
#include <cstdlib>
#include "DaaDdpDecoder.h"
#include "DaaAc4Decoder.h"

#define DECODER_FUNC(RETURN_TYPE, NAME, ...) \
  extern "C" { \
  JNIEXPORT RETURN_TYPE \
    Java_com_dolby_daa_DaaDecoderJni_## NAME( \
        JNIEnv* env, jobject thiz, ##__VA_ARGS__);\
  } \
  JNIEXPORT RETURN_TYPE \
    Java_com_dolby_daa_DaaDecoderJni_## NAME( \
        JNIEnv* env, jobject thiz, ##__VA_ARGS__)

typedef struct Context {
    DaaDecoder *decoder;
}Context;

DECODER_FUNC(jlong, DaaInit, jboolean isAC4Content, jboolean isSyncFrame, jboolean isJocContent) {
  Context *context = new Context();

  if (isAC4Content) {
    context->decoder = new DaaAc4Decoder();
    context->decoder->open(isSyncFrame);
  } else {
    context->decoder = new DaaDdpDecoder();
    context->decoder->open(isJocContent);
  }

  return reinterpret_cast<intptr_t>(context);
}

DECODER_FUNC(jint, DaaDecode, jlong jDecoder, jobject jInputBuffer, jint inputSize,
    jobject jOutputBuffer, jint outputSize) {
  if (!jDecoder) {
    LOGE("jDecoder must be non-NULL.");
    return -1;
  }
  if (!jInputBuffer || !jOutputBuffer) {
    LOGE("Input and output buffers must be non-NULL");
    return -1;
  }
  if (inputSize < 0) {
    LOGE("Invalid input buffer size: %d.", inputSize);
    return -1;
  }
  if (outputSize < 0) {
    LOGE("Invalid output buffer length: %d.", outputSize);
    return -1;
  }

  Context* context = reinterpret_cast<Context*>(jDecoder);
  int outputByte = 0;

  char* const inputBuffer = reinterpret_cast<char* const>(
      env->GetDirectBufferAddress(jInputBuffer));
  char* const outputBuffer = reinterpret_cast<char* const>(
      env->GetDirectBufferAddress(jOutputBuffer));
  outputByte = context->decoder->decode(inputBuffer, (unsigned int)inputSize, outputBuffer, outputSize);
  return outputByte;
}

DECODER_FUNC(jint, DaaGetChannelCount, jlong jContext) {
  Context* context = reinterpret_cast<Context*>(jContext);
  return context->decoder->getOutputChannelCount();
}

DECODER_FUNC(jint, DaaGetSampleRate, jlong jContext) {
  Context* context = reinterpret_cast<Context*>(jContext);
  return context->decoder->getSampleRate();
}

DECODER_FUNC(jint, DaaGetMaxOutputBufferSize, jlong jContext) {
  Context* context = reinterpret_cast<Context*>(jContext);
  return context->decoder->getMaxOutputBufferSize();
}

DECODER_FUNC(jlong, DaaGetLatencyUs, jlong jContext) {
  Context* context = reinterpret_cast<Context*>(jContext);
  return context->decoder->getLatencyUs();
}

DECODER_FUNC(jint, DaaSetParameter, jlong jContext, jint jParamId, jint jParamValue) {
  Context* context = reinterpret_cast<Context*>(jContext);
  return context->decoder->setParameter(jParamId, jParamValue);
}

DECODER_FUNC(void, DaaClose, jlong jContext) {
  Context* context = reinterpret_cast<Context*>(jContext);
  if (context != NULL) {
    if (context->decoder != NULL) {
      delete(context->decoder);
    }
    delete(context);
  }
}