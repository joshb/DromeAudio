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
#include <DromeAudio/TriangleSound.h>

namespace DromeAudio {

/*
 * TriangleSound class
 */
TriangleSound::TriangleSound(float frequency)
{
	setFrequency(frequency);
}

unsigned char
TriangleSound::getNumChannels() const
{
	return 1;
}

unsigned int
TriangleSound::getSampleRate() const
{
	return 44100;
}

unsigned int
TriangleSound::getNumSamples() const
{
	return (unsigned int)((float)getSampleRate() / m_frequency);
}

Sample
TriangleSound::getSample(unsigned int index) const
{
	unsigned int numSamples = getNumSamples();
	index += numSamples / 4;
	index %= numSamples;
	numSamples /= 2;

	float f;
	if(index < numSamples)
		f = -1.0f + 2.0f * ((float)(index % numSamples) / (float)numSamples);
	else
		f = 1.0f - 2.0f * ((float)(index % numSamples) / (float)numSamples);

	Sample sample;
	sample[0] = f;
	sample[1] = f;

	return sample;
}

TriangleSoundPtr
TriangleSound::create(float frequency)
{
	return TriangleSoundPtr(new TriangleSound(frequency));
}

} // namespace DromeAudio
