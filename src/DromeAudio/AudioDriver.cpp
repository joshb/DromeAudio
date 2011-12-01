/*
 * Copyright (C) 2008-2010 Josh A. Beam
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <DromeAudio/Exception.h>
#include <DromeAudio/AudioDriver.h>
#ifdef WITH_ALSA
	#include <DromeAudio/AudioDriverALSA.h>
#elif WITH_OSX
	#include <DromeAudio/AudioDriverOSX.h>
#elif WITH_SDL
	#include <DromeAudio/AudioDriverSDL.h>
#endif

namespace DromeAudio {

/*
 * AudioDriver class
 */
AudioDriver::AudioDriver()
{
	m_sampleRate = 0;
	m_audioContext = 0;
}

AudioDriver::~AudioDriver()
{
}

unsigned int
AudioDriver::getSampleRate() const
{
	return m_sampleRate;
}

AudioContext *
AudioDriver::getAudioContext() const
{
	return m_audioContext;
}

void
AudioDriver::setAudioContext(AudioContext *value)
{
	m_audioContext = value;
}

AudioDriver *
AudioDriver::create()
{
#ifdef WITH_ALSA
	return new AudioDriverALSA();
#elif WITH_OSX
	return new AudioDriverOSX();
#elif WITH_SDL
	return new AudioDriverSDL();
#else
	throw Exception("AudioDriver::create(): No suitable driver available");
#endif
}

} // namespace DromeAudio
