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

#ifndef __DROMEAUDIO_SOUND_H__
#define __DROMEAUDIO_SOUND_H__

#include <string>
#include <DromeAudio/Ref.h>
#include <DromeAudio/Endian.h>
#include <DromeAudio/Sample.h>

namespace DromeAudio {

class Sound;
typedef RefPtr <Sound> SoundPtr;

/** \brief The abstract class that all classes implementing types of sounds should derive from.
 *
 * This class only includes the basic methods for retrieving sound data, making it possible for derived classes to return samples stored in memory, streamed samples, or dynamically generated samples.
 */
class Sound : public RefClass
{
	/**
	 * \example DromeAudioPlayer.cpp
	 */

	protected:
		Sound();
		virtual ~Sound() { }

	public:
		/**
		 * @return The number of audio channels that the sound consists of.
		 */
		virtual unsigned char getNumChannels() const;

		/**
		 * @return The sample rate (number of samples per second of audio) of the sound.
		 */
		virtual unsigned int getSampleRate() const;

		/**
		 * @return The total number of samples that the sound consists of. May be 0 to indicate an unlimited number of samples.
		 */
		virtual unsigned int getNumSamples() const;

		/**
		 * Retrieves one sample of audio data from the sound.
		 * @param index The index of the Sample to retrieve. Should be less than the value returned by getNumSamples() (if getNumSamples() does not equal 0).
		 * @return The Sample at the specified index.
		 */
		virtual Sample getSample(unsigned int index) const = 0;

		virtual void setParameter(const std::string &name, float value);
		virtual void setParameter(const std::string &name, SoundPtr value);

		/**
		 * Saves the sound to a WAV file.
		 * @param filename The path of the file to save.
		 * @param numSamples The number of samples (starting from the first sample in the sound) to save.
		 */
		void save(const char *filename, unsigned int numSamples = 0);

		/**
		 * Attempts to load a sound using the appropriate format-specific class depending on the extension of the given filename. For example, the WavSound class will be used to load a file with a ".wav" extension.
		 * @param filename Path to the file to load.
		 * @return SoundPtr to the loaded sound.
		 */
		static SoundPtr create(const char *filename);
};

} // namespace DromeAudio

#endif /* __DROMEAUDIO_SOUND_H__ */
