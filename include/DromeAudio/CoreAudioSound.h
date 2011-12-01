/*
 * Copyright (C) 2010 Josh A. Beam
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

#ifndef __DROMEAUDIO_COREAUDIOSOUND_H__
#define __DROMEAUDIO_COREAUDIOSOUND_H__

#include <DromeAudio/Sound.h>

namespace DromeAudio {

class CoreAudioSound;
typedef RefPtr <CoreAudioSound> CoreAudioSoundPtr;

/** \brief A class for loading audio files using Core Audio on Mac OS X.
 */
class CoreAudioSound : public Sound
{
	protected:
		unsigned char m_numChannels;
		unsigned char m_bytesPerSample;
		unsigned int m_sampleRate;
		unsigned int m_numSamples;

		Sample *m_samples;

		CoreAudioSound(const char *filename);
		virtual ~CoreAudioSound();

	public:
		unsigned char getNumChannels() const;
		unsigned int getSampleRate() const;
		unsigned int getNumSamples() const;

		Sample getSample(unsigned int index) const;

		/**
		 * Loads an audio file using Core Audio.
		 * @param filename Path to the audio file to load.
		 * @return SoundPtr to the loaded sound.
		 */
		static CoreAudioSoundPtr create(const char *filename);
};

} // namespace DromeAudio

#endif /* __DROMEAUDIO_COREAUDIOSOUND_H__ */
