/*
 * Copyright (C) 2008-2012 Josh A. Beam
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
#include <DromeAudio/SawSound.h>

namespace DromeAudio {

/*
 * SawSound class
 */
SawSound::SawSound(float frequency)
{
	setFrequency(frequency);
}

unsigned int
SawSound::getNumSamples() const
{
	return (unsigned int)((float)getSampleRate() / m_frequency);
}

void
SawSound::setParameter(const std::string &name, float value)
{
	if(name == "frequency")
		setFrequency(value);
	else
		Sound::setParameter(name, value);
}

float
SawSound::getFrequency() const
{
	return m_frequency;
}

void
SawSound::setFrequency(float value)
{
	m_frequency = value;
}

Sample
SawSound::getSample(unsigned int index) const
{
	float f = (((float)index / (float)(getNumSamples() - 1)) - 0.5f) * 2.0f;

	Sample sample;
	sample[0] = f;
	sample[1] = f;

	return sample;
}

SawSoundPtr
SawSound::create(float frequency)
{
	return SawSoundPtr(new SawSound(frequency));
}

} // namespace DromeAudio
