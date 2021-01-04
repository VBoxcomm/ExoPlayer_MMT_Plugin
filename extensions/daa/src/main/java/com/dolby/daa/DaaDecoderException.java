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

import com.google.android.exoplayer2.audio.AudioDecoderException;

/**
 * Thrown when an DAA decoder error occurs.
 */
public final class DaaDecoderException extends AudioDecoderException {

  /* package */ DaaDecoderException(String message) {
    super(message);
  }

  /* package */ DaaDecoderException(String message, Throwable cause) {
    super(message, cause);
  }
}
