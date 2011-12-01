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

#ifndef __DROMEAUDIO_AUDIODRIVER_H__
#define __DROMEAUDIO_AUDIODRIVER_H__

#include <vector>
#include <DromeAudio/Endian.h>
#include <DromeAudio/Sample.h>

namespace DromeAudio {

class AudioContext;

/** \brief The abstract class for accessing audio interfaces.
 */
class AudioDriver
{
	/**
	 * \example DromeAudioPlayer.cpp
	 */

	protected:
		unsigned int m_sampleRate;
		AudioContext *m_audioContext;

	public:
		AudioDriver();
		virtual ~AudioDriver();

		/**
		 * @return The sample rate at which audio should be written to the driver.
		 */
		unsigned int getSampleRate() const;

		/**
		 * @return Pointer to the AudioContext associated with the driver.
		 */
		AudioContext *getAudioContext() const;

		/**
		 * Sets the AudioContext associated with the driver. If not NULL, the driver will asynchronously call the AudioContext::writeSamples() function of the given AudioContext to write sample data when necessary.
		 * @param value Pointer to the AudioContext to associate with the driver.
		 */
		void setAudioContext(AudioContext *value);

		/**
		 * @return Name of the driver, such as "AudioDriverALSA".
		 */
		virtual const char *getDriverName() const = 0;

		/**
		 * Writes an audio sample to be played.
		 * @param sample Sample object.
		 */
		virtual void writeSample(const Sample &sample) = 0;

		/**
		 * Creates a new AudioDriver using the most appropriate derived class available.
		 * @return Pointer to new AudioDriver object.
		 */
		static AudioDriver *create();
};

}

#endif /* __DROMEAUDIO_AUDIODRIVER_H__ */
