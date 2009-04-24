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

#ifndef __DROMEAUDIO_AUDIODRIVER_H__
#define __DROMEAUDIO_AUDIODRIVER_H__

#include <vector>
#include <DromeAudio/Endian.h>
#include <DromeAudio/Mutex.h>
#include <DromeAudio/Sound.h>

namespace DromeAudio {

/** \brief The abstract class for accessing audio interfaces.
 */
class AudioDriver
{
	/**
	 * \example DromeAudioPlayer.cpp
	 */

	public:
		enum Mode {
			MODE_WRITE_ONLY = 0,
			MODE_READ_ONLY,
			MODE_READ_WRITE
		};

	protected:
		Mutex *m_mutex;
		unsigned int m_sampleRate;
		SoundPtr m_outputSound;
		unsigned int m_outputSampleIndex;

		/**
		 * Writes an audio sample to be played.
		 * @param sample Sample object.
		 */
		virtual void writeSample(const Sample &sample) = 0;

		/**
		 * Reads an audio sample to be recorded.
		 * @return Sample object.
		 */
		virtual Sample readSample() = 0;

		/**
		 * Writes samples from the output Sound.
		 * @numSamples Number of samples to write.
		 */
		void writeSamples(unsigned int numSamples);

	public:
		AudioDriver();
		virtual ~AudioDriver();

		/**
		 * @return The sample rate at which audio should be written to the driver.
		 */
		unsigned int getSampleRate() const;

		/**
		 * @return SoundPtr to the Sound being used for audio output.
		 */
		SoundPtr getOutputSound() const;

		/**
		 * Sets the output Sound associated with the driver. If not NULL, the driver will play samples from the sound as audio output.
		 * @param value SoundPtr to the Sound to use for audio output.
		 */
		void setOutputSound(SoundPtr value);

		/**
		 * @return The current sample index of the output sound.
		 */
		unsigned int getOutputSampleIndex() const;

		/**
		 * Sets the sample index of the output sound.
		 * @param value New sample index.
		 */
		void setOutputSampleIndex(unsigned int value);

		/**
		 * @return Name of the driver, such as "AudioDriverALSA".
		 */
		virtual const char *getDriverName() const = 0;

		/**
		 * Creates a new AudioDriver using the most appropriate derived class available.
		 * @return Pointer to new AudioDriver object.
		 */
		static AudioDriver *create(Mode mode = MODE_WRITE_ONLY);
};

}

#endif /* __DROMEAUDIO_AUDIODRIVER_H__ */
