/*
 * Copyright (C) 2008-2009 Josh A. Beam <josh@joshbeam.com>
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
#ifdef WITH_OSS
	#include <DromeAudio/AudioDriverOSS.h>
#endif /* WITH_OSS */
#ifdef WITH_ALSA
	#include <DromeAudio/AudioDriverALSA.h>
#endif /* WITH_ALSA */
#ifdef WITH_SDL
	#include <DromeAudio/AudioDriverSDL.h>
#endif /* WITH_SDL */

namespace DromeAudio {

/*
 * AudioDriver class
 */
AudioDriver::AudioDriver()
{
	m_sampleRate = 0;
	m_outputSampleIndex = 0;
}

AudioDriver::~AudioDriver()
{
}

void
AudioDriver::writeSamples(unsigned int numSamples)
{
	// if there's no sound, just write default sample data
	if(!m_outputSound) {
		for(unsigned int i = 0; i < numSamples; ++i)
			writeSample(Sample());
		return;
	}

	// write samples from output sound
	for(unsigned int i = 0; i < numSamples; ++i)
		writeSample(m_outputSound->getSample(m_outputSampleIndex++, getSampleRate()));
}

AudioDriver *
AudioDriver::create(Mode mode)
{
#ifdef WITH_OSS
	return new AudioDriverOSS(mode);
#elif WITH_ALSA
	return new AudioDriverALSA(mode);
#elif WITH_SDL
	if(mode == MODE_WRITE_ONLY)
		return new AudioDriverSDL();
#else
	throw Exception("AudioDriver::create(): No suitable driver available");
#endif
	return NULL;
}

} // namespace DromeAudio
