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

#include <cmath>
#include <DromeAudio/Exception.h>
#include <DromeAudio/SoundEffect.h>

namespace DromeAudio {

/*
 * SoundEffect class
 */
unsigned char
SoundEffect::getNumChannels() const
{
	return m_sound->getNumChannels();
}

unsigned int
SoundEffect::getSampleRate() const
{
	return m_sound->getSampleRate();
}

unsigned int
SoundEffect::getNumSamples() const
{
	return m_sound->getNumSamples();
}

SoundPtr
SoundEffect::getSound() const
{
	return m_sound;
}

void
SoundEffect::setSound(SoundPtr value)
{
	m_sound = value;
}

/*
 * PitchShiftSoundEffect class
 */
PitchShiftSoundEffect::PitchShiftSoundEffect(SoundPtr sound, float factor)
 : SoundEffect(sound)
{
	setFactor(factor);
}

unsigned int
PitchShiftSoundEffect::getNumSamples() const
{
	// calculate number of samples based on pitch shift factor
	return (unsigned int)((float)(m_sound->getNumSamples() / m_factor));
}

float
PitchShiftSoundEffect::getFactor() const
{
	return m_factor;
}

void
PitchShiftSoundEffect::setFactor(float value)
{
	if(value <= 0.0f)
		throw Exception("PitchShiftSoundEffect::setFactor(): Invalid factor value (%f)", value);

	m_factor = value;
}

Sample
PitchShiftSoundEffect::getSample(unsigned int index) const
{
	if(!m_sound)
		throw Exception("PitchShiftSoundEffect::getSample(): Sound not set");

	float f = (float)index * m_factor;
	float fl = floor((double)f);
	unsigned int index1 = (unsigned int)fl;
	unsigned int index2 = (unsigned int)ceil((double)f);

	Sample s1 = m_sound->getSample(index1);
	Sample s2 = m_sound->getSample(index2);

	return s1 + ((s2 - s1) * (f - fl));
}

PitchShiftSoundEffectPtr
PitchShiftSoundEffect::create(SoundPtr sound, float factor)
{
	return PitchShiftSoundEffectPtr(new PitchShiftSoundEffect(sound, factor));
}

/*
 * OscillatorSoundEffect class
 */
OscillatorSoundEffect::OscillatorSoundEffect(SoundPtr sound, float frequency)
 : SoundEffect(sound)
{
	setFrequency(frequency);
}

float
OscillatorSoundEffect::getFrequency() const
{
	return m_frequency;
}

void
OscillatorSoundEffect::setFrequency(float value)
{
	if(value < 0.0f)
		m_frequency = 0.0f;
	else
		m_frequency = value;
}

Sample
OscillatorSoundEffect::getSample(unsigned int index) const
{
	if(!m_sound)
		throw Exception("OscillatorSoundEffect::getSample(): Sound not set");

	Sample sample = m_sound->getSample(index);

	float tmp = (float)getSampleRate() / m_frequency;
	float f = sinf(M_PI * 2.0f * ((float)index / tmp));
	sample[0] *= f;
	sample[1] *= f;

	return sample;
}

OscillatorSoundEffectPtr
OscillatorSoundEffect::create(SoundPtr sound, float frequency)
{
	return OscillatorSoundEffectPtr(new OscillatorSoundEffect(sound, frequency));
}

/*
 * EchoSoundEffect class
 */
EchoSoundEffect::EchoSoundEffect(SoundPtr sound, float delay,
                                 float factor, unsigned int count)
 : SoundEffect(sound)
{
	setDelay(delay);
	setFactor(factor);
	setCount(count);
}

unsigned int
EchoSoundEffect::getNumSamples() const
{
	// increase number of samples to account for echo
	unsigned int increase = (unsigned int)((float)getSampleRate() * m_delay * (float)m_count);

	return m_sound->getNumSamples() + increase;
}

float
EchoSoundEffect::getDelay() const
{
	return m_delay;
}

void
EchoSoundEffect::setDelay(float value)
{
	m_delay = value;
}

float
EchoSoundEffect::getFactor() const
{
	return m_factor;
}

void
EchoSoundEffect::setFactor(float value)
{
	m_factor = value;
}

unsigned int
EchoSoundEffect::getCount() const
{
	return m_count;
}

void
EchoSoundEffect::setCount(unsigned int value)
{
	m_count = value;
}

Sample
EchoSoundEffect::getSample(unsigned int index) const
{
	if(!m_sound)
		throw Exception("EchoSoundEffect::GetSample(): Sound not set");

	Sample sample;
	if(index < m_sound->getNumSamples())
		sample = m_sound->getSample(index);

	// add echo to sample if necessary
	unsigned int min = (unsigned int)((float)getSampleRate() * m_delay);
	unsigned int count = 0;
	float factor = m_factor;
	while(count++ < m_count && index > min) {
		index -= min;

		if(index < m_sound->getNumSamples())
			sample += m_sound->getSample(index) * factor;

		factor *= factor;
	}

	return sample;
}

EchoSoundEffectPtr
EchoSoundEffect::create(SoundPtr sound, float delay, float factor, unsigned int count)
{
	return EchoSoundEffectPtr(new EchoSoundEffect(sound, delay, factor, count));
}

} // namespace DromeAudio
