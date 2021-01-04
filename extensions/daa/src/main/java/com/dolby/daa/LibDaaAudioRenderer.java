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

import android.os.Handler;
import android.util.Log;

import com.google.android.exoplayer2.C;
import com.google.android.exoplayer2.ExoPlaybackException;
import com.google.android.exoplayer2.Format;
import com.google.android.exoplayer2.audio.AudioProcessor;
import com.google.android.exoplayer2.audio.AudioRendererEventListener;
import com.google.android.exoplayer2.drm.DrmSessionManager;
import com.google.android.exoplayer2.drm.ExoMediaCrypto;
import com.google.android.exoplayer2.util.MimeTypes;
import com.google.android.exoplayer2.audio.SystemClockSynchronizedSimpleDecoderAudioRenderer;

/*
 * Decodes and renders audio using the native DAA decoder.
 */
public class LibDaaAudioRenderer extends SystemClockSynchronizedSimpleDecoderAudioRenderer {
  private static final int NUM_BUFFERS = 16;
  // The number of input and output buffers
  private static final int INITIAL_INPUT_BUFFER_SIZE = 4096;

  private DaaDecoder mDaaDecoder;

  public LibDaaAudioRenderer() {
    this(null, null);
  }

  /*
   * @param eventHandler A handler to use when delivering events to {@code eventListener}. May be
   *     null if delivery of events is not required.
   * @param eventListener A listener of events. May be null if delivery of events is not required.
   * @param audioProcessors Optional {@link AudioProcessor}s that will process audio before output.
   */
  public LibDaaAudioRenderer(Handler eventHandler, AudioRendererEventListener eventListener,
      AudioProcessor... audioProcessors) {
    super(eventHandler, eventListener, audioProcessors);
    Log.d("LibDaaAudioRenderer", "LibDAAAudioRenderer");
  }


  @Override
  protected int supportsFormatInternal(DrmSessionManager<ExoMediaCrypto> drmSessionManager,
      Format format) {
    Log.d("LibDaaAudioRenderer", "supportsFormatInternal, format:"+format.sampleMimeType);
    boolean isSupportedMimeType = false;
    if (MimeTypes.AUDIO_AC4.equalsIgnoreCase(format.sampleMimeType)
        || MimeTypes.AUDIO_AC3.equalsIgnoreCase(format.sampleMimeType)
        || MimeTypes.AUDIO_E_AC3.equalsIgnoreCase(format.sampleMimeType)
        || MimeTypes.AUDIO_E_AC3_JOC.equalsIgnoreCase(format.sampleMimeType)) {
      isSupportedMimeType = true;
    }

    if (!DaaLibrary.isAvailable() || !isSupportedMimeType) {
      Log.e("LibDaaAudioRenderer","supportsFormatInternal:FORMAT_UNSUPPORTED_TYPE");
      return FORMAT_UNSUPPORTED_TYPE;
    } else if (!supportsFormatDrm(drmSessionManager, format.drmInitData)) {
      Log.e("LibDaaAudioRenderer","supportsFormatInternal:FORMAT_UNSUPPORTED_DRM");
      return FORMAT_UNSUPPORTED_DRM;
    } else {
      Log.d("LibDaaAudioRenderer","supportsFormatInternal:FORMAT_HANDLED");
      return FORMAT_HANDLED;
    }
  }

  @Override
  protected DaaDecoder createDecoder(Format format, ExoMediaCrypto mediaCrypto)
      throws DaaDecoderException {
    Log.d("LibDaaAudioRenderer", "createDecoder");
    mDaaDecoder = new DaaDecoder(
        NUM_BUFFERS, NUM_BUFFERS, INITIAL_INPUT_BUFFER_SIZE,
        MimeTypes.AUDIO_AC4.equalsIgnoreCase(format.sampleMimeType), true,
        MimeTypes.AUDIO_E_AC3_JOC.equalsIgnoreCase(format.sampleMimeType));
    return mDaaDecoder;
  }

    public Format getOutputFormat() {
      int channelCount = mDaaDecoder.getChannelCount();
      int sampleRate = mDaaDecoder.getSampleRate();
      return Format.createAudioSampleFormat(null, MimeTypes.AUDIO_RAW, null, Format.NO_VALUE,
          Format.NO_VALUE, channelCount, sampleRate, C.ENCODING_PCM_16BIT,null,
          null, 0, null);
    }

  @Override
  public void handleMessage(int messageType, Object message) throws ExoPlaybackException {
    Log.d("LibDaaAudioRenderer", "LibDaaAudioRenderer,handleMessage, messageType = "
        + messageType);
    switch(messageType) {
      default:
        super.handleMessage(messageType, message);
        break;
    }
  }
}
