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

#ifndef __DROMEAUDIO_AUDIOCONTEXT_H__
#define __DROMEAUDIO_AUDIOCONTEXT_H__

#include <vector>
#include <DromeAudio/Mutex.h>
#include <DromeAudio/AudioDriver.h>
#include <DromeAudio/SoundEmitter.h>

namespace DromeAudio {

/** \brief Keeps track of SoundEmitter objects and mixes their samples.
 */
class AudioContext
{
	protected:
		Mutex *m_mutex;
		unsigned int m_targetSampleRate;
		std::vector <SoundEmitterPtr> m_emitters;

	public:
		AudioContext(unsigned int targetSampleRate);
		virtual ~AudioContext();

		/**
		 * Gets the target sample rate of the audio context. This should be set to the sample rate of the AudioDriver that will be passed to WriteSamples().
		 * @return Target sample rate.
		 */
		unsigned int getTargetSampleRate() const;

		/**
		 * Attaches a SoundEmitter.
		 * @param emitter SoundEmitterPtr to the SoundEmitter to be attached.
		 */
		virtual void attachSoundEmitter(SoundEmitterPtr emitter);

		/**
		 * Detaches a SoundEmitter.
		 * @param emitter SoundEmitterPtr to the SoundEmitter to be detached.
		 */
		virtual void detachSoundEmitter(SoundEmitterPtr emitter);

		/**
		 * Creates a SoundEmitter to the given Sound and attaches it.
		 * @param sound SoundPtr to the Sound to be used by the new SoundEmitter.
		 * @return SoundEmitterPtr to the new SoundEmitter.
		 */
		virtual SoundEmitterPtr playSound(SoundPtr sound);

		/**
		 * Writes a number of samples to the given AudioDriver.
		 * @param driver Pointer to the AudioDriver to write samples with.
		 * @param numSamples The number of samples to be written.
		 */
		virtual void writeSamples(AudioDriver *driver, unsigned int numSamples);
};

}

#endif /* __DROMEAUDIO_AUDIOCONTEXT_H__ */
