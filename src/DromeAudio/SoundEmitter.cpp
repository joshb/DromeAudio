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
#include <DromeAudio/SoundEmitter.h>

namespace DromeAudio {

SoundEmitter::SoundEmitter(unsigned int sampleRate)
{
	m_sampleRate = sampleRate;

	m_loop = true;
	m_paused = false;
	m_volume = 1.0f;
	m_balance = 0.0f;

	m_sampleIndex = 0;
}

float
SoundEmitter::getSampleIndexFactor() const
{
	if(!m_sound)
		return 0.0f;

	return (float)m_sound->getSampleRate() / (float)m_sampleRate;
}

uint8_t
SoundEmitter::getNumChannels() const
{
	return m_sound->getNumChannels();
}

unsigned int
SoundEmitter::getSampleRate() const
{
	return m_sampleRate;
}

void
SoundEmitter::setSampleRate(unsigned int value)
{
	m_sampleRate = value;
}

SoundPtr
SoundEmitter::getSound() const
{
	return m_sound;
}

void
SoundEmitter::setSound(SoundPtr value)
{
	m_sound = value;
}

unsigned int
SoundEmitter::getNumSamples() const
{
	if(!m_sound)
		return 0;

	return (unsigned int)((float)m_sound->getNumSamples() / getSampleIndexFactor());
}

bool
SoundEmitter::getLoop() const
{
	return m_loop;
}

void
SoundEmitter::setLoop(bool value)
{
	m_loop = value;
}

bool
SoundEmitter::getPaused() const
{
	return m_paused;
}

void
SoundEmitter::setPaused(bool value)
{
	m_paused = value;
}

float
SoundEmitter::getVolume() const
{
	return m_volume;
}

void
SoundEmitter::setVolume(float value)
{
	m_volume = value;
}

float
SoundEmitter::getBalance() const
{
	return m_balance;
}

void
SoundEmitter::setBalance(float value)
{
	m_balance = value;
}

unsigned int
SoundEmitter::getSampleIndex() const
{
	return m_sampleIndex;
}

void
SoundEmitter::setSampleIndex(unsigned int value)
{
	m_sampleIndex = value;
}

bool
SoundEmitter::isDone()
{
	if(!m_sound)
		throw Exception("SoundEmitter::isDone(): Sound not set");

	return (m_loop == false && m_sampleIndex >= getNumSamples());
}

Sample
SoundEmitter::getSample(unsigned int sampleIndex) const
{
	if(!m_sound)
		throw Exception("SoundEmitter::getSample(): Sound not set");

	unsigned int index = (unsigned int)((float)sampleIndex * getSampleIndexFactor());
	return m_sound->getSample(index);
}

Sample
SoundEmitter::getNextSample()
{
	Sample sample = getSample(m_sampleIndex).balance(m_balance) * m_volume;

	// increment the sample index if not paused
	if(!m_paused) {
		++m_sampleIndex;

		// loop if necessary
		if(m_sampleIndex >= getNumSamples() && m_loop)
			m_sampleIndex = 0;
	}

	return sample;
}

SoundEmitterPtr
SoundEmitter::create(unsigned int sampleRate)
{
	return SoundEmitterPtr(new SoundEmitter(sampleRate));
}

} // namespace DromeAudio
