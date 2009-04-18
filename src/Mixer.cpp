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

#include <cmath>
#include <DromeAudio/Mixer.h>

namespace DromeAudio {

/*
 * Mixer class
 */
Mixer::Mixer(MixerMode mode)
{
	m_mode = mode;
}

unsigned char
Mixer::getNumChannels() const
{
	unsigned char numChannels = 0;

	if(m_sound1.isSet())
		numChannels = m_sound1->getNumChannels();
	if(m_sound2.isSet() && m_sound2->getNumChannels() > numChannels)
		numChannels = m_sound2->getNumChannels();

	return numChannels;
}

unsigned int
Mixer::getSampleRate() const
{
	unsigned int rate = 0;

	if(m_sound1.isSet())
		rate = m_sound1->getSampleRate();
	if(m_sound2.isSet() && m_sound2->getSampleRate() > rate)
		rate = m_sound2->getSampleRate();

	return rate;
}

unsigned int
Mixer::getNumSamples() const
{
	return 0;
}

Sample
Mixer::getSample(unsigned int index) const
{
	Sample sample;
	unsigned int rate = getSampleRate();

	// get sample from first sound
	if(m_sound1.isSet())
		sample = m_sound1->getSample(index, rate);

	// mix with sample from second sound
	if(m_sound2.isSet()) {
		Sample tmp = m_sound2->getSample(index, rate);

		switch(m_mode) {
			default:
			case MIXER_MODE_ADD:
				sample += tmp;
				break;
			case MIXER_MODE_SUBTRACT:
				sample -= tmp;
				break;
			case MIXER_MODE_MULTIPLY:
				sample *= tmp;
				break;
		}
	}

	return sample;
}

MixerPtr
Mixer::create(MixerMode mode)
{
	return MixerPtr(new Mixer(mode));
}

} // namespace DromeAudio
