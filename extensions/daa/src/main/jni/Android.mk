#
# Copyright (C) 2018 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
WORKING_DIR := $(call my-dir)

# create DAA as ndk prebuilt
include $(CLEAR_VARS)
LOCAL_PATH := $(WORKING_DIR)
LOCAL_MODULE := DAA
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/libdaa/include
LOCAL_SRC_FILES := $(LOCAL_PATH)/libdaa/bin/$(TARGET_ARCH_ABI)/lib_daa_release.a
include $(PREBUILT_STATIC_LIBRARY)

# build libDaaJNI.so
include $(CLEAR_VARS)
LOCAL_PATH := $(WORKING_DIR)
LOCAL_MODULE := DaaJNI
LOCAL_ARM_MODE := arm
LOCAL_CPP_EXTENSION := .cc
LOCAL_SRC_FILES := DaaDecoder_jni.cc DaaAc4Decoder.cc DaaDdpDecoder.cc
LOCAL_LDLIBS := -llog -lz -lm
LOCAL_STATIC_LIBRARIES := DAA
include $(BUILD_SHARED_LIBRARY)
