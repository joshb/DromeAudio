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

#include <cstdio>
#include <DromeAudio/AudioContext.h>

namespace DromeAudio {

/*
 * AudioContext class
 */
AudioContext::AudioContext(unsigned int targetSampleRate)
{
	m_mutex = Mutex::create();
	m_targetSampleRate = targetSampleRate;
}

AudioContext::~AudioContext()
{
	delete m_mutex;
}

unsigned int
AudioContext::getTargetSampleRate() const
{
	return m_targetSampleRate;
}

void
AudioContext::attachSoundEmitter(SoundEmitterPtr emitter)
{
	m_mutex->lock();
	m_emitters.insert(m_emitters.end(), emitter);
	m_mutex->unlock();
}

void
AudioContext::detachSoundEmitter(SoundEmitterPtr emitter)
{
	m_mutex->lock();

	for(unsigned int i = 0; i < m_emitters.size(); i++) {
		if(m_emitters[i] == emitter) {
			m_emitters.erase(m_emitters.begin() + i);
			break;
		}
	}

	m_mutex->unlock();
}

SoundEmitterPtr
AudioContext::playSound(SoundPtr sound)
{
	SoundEmitterPtr emitter = SoundEmitter::create(m_targetSampleRate);
	emitter->setSound(sound);

	attachSoundEmitter(emitter);
	return emitter;
}

void
AudioContext::writeSamples(AudioDriver *driver, unsigned int numSamples)
{
	m_mutex->lock();

	for(unsigned int i = 0; i < numSamples; i++) {
		Sample sample;

		// mix samples from all emitters
		for(unsigned int j = 0; j < m_emitters.size(); j++)
			sample += m_emitters[j]->getNextSample();

		// write sample data
		driver->writeSample(sample.clamp());
	}

	m_mutex->unlock();
}

} // namespace DromeAudio
