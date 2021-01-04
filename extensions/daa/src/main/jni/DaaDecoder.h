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

#ifndef EXOPLAYER_DAADECODER_H
#define EXOPLAYER_DAADECODER_H

#include <android/log.h>
#include <stdio.h>

#define LIB_LOG_TAG "libDaaDecoder_native"

#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LIB_LOG_TAG, \
                                             __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LIB_LOG_TAG, \
                                             __VA_ARGS__))

#define LOGD(...)
#define LOGV(...)

//#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LIB_LOG_TAG, \
//                                             __VA_ARGS__))
//#define LOGV(...) ((void)__android_log_print(ANDROID_LOG_VERBOSE, LIB_LOG_TAG, \
//                                             __VA_ARGS__))

#ifdef __cplusplus
extern "C"
{
#endif

#define ALOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, \
                                             __VA_ARGS__))
#define ALOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, \
                                             __VA_ARGS__))
#define ALOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, \
                                             __VA_ARGS__))
#define ALOGV(...) ((void)__android_log_print(ANDROID_LOG_VERBOSE, LIB_LOG_TAG, \
                                             __VA_ARGS__))

#ifdef __cplusplus
}
#endif

class DaaDecoder {
public:
  virtual ~DaaDecoder() {};
  virtual int open(bool status) = 0;
  virtual int decode(char *const inputBuffer, unsigned inputSize, char *const outputBuffer,
      int outBufSize) = 0;
  virtual int getOutputChannelCount() = 0;
  virtual int getSampleRate() = 0;
  virtual int getMaxOutputBufferSize() = 0;
  virtual long getLatencyUs() = 0;
  virtual int setParameter(int paramId, int paramValue) = 0;
};
#endif //EXOPLAYER_DAADECODER_H
