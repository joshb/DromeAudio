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

#include <DromeAudio/Exception.h>
#include <DromeAudio/Sample.h>

namespace DromeAudio {

/*
 * Sample class
 */
Sample::Sample()
{
	m_channelValues[0] = 0.0f;
	m_channelValues[1] = 0.0f;
}

Sample
Sample::operator + (const Sample &s) const
{
	Sample sample;

	for(int i = 0; i < 2; i++)
		sample[i] = (*this)[i] + s[i];

	return sample;
}

Sample
Sample::operator - (const Sample &s) const
{
	Sample sample;

	for(int i = 0; i < 2; i++)
		sample[i] = (*this)[i] - s[i];

	return sample;
}

Sample
Sample::operator * (const Sample &s) const
{
	Sample sample;

	for(int i = 0; i < 2; i++)
		sample[i] = (*this)[i] * s[i];

	return sample;
}

Sample
Sample::operator * (float f) const
{
	Sample sample;

	for(int i = 0; i < 2; i++)
		sample[i] = (*this)[i] * f; 

	return sample;
}

Sample
Sample::operator / (const Sample &s) const
{
	Sample sample;

	for(int i = 0; i < 2; i++)
		sample[i] = (*this)[i] / s[i];

	return sample;
}

Sample
Sample::operator / (float f) const
{
	Sample sample;

	for(int i = 0; i < 2; i++)
		sample[i] = (*this)[i] / f;

	return sample;
}

Sample
Sample::clamp() const
{
	Sample sample;

	for(int i = 0; i < 2; i++) {
		sample[i] = (*this)[i];

		if(sample[i] < -1.0f)
			sample[i] = -1.0f;
		else if(sample[i] > 1.0f)
			sample[i] = 1.0f;
	}

	return sample;
}

Sample
Sample::balance(float balance) const
{
	Sample tmp = *this;

	if(balance < 0.0f) {
		float f = m_channelValues[1] * (-balance);
		tmp[0] += f;
		tmp[1] -= f;
	} else {
		float f = m_channelValues[0] * balance;
		tmp[0] -= f;
		tmp[1] += f;
	}

	return tmp;
}

Sample
Sample::fromInt8(const int8_t values[], unsigned int numChannels)
{
	const float max = 127.0f;
	Sample sample;

	switch(numChannels) {
		default:
			throw Exception("Sample::fromInt8(): Unsupported number of channels (%u)\n", numChannels);
			break;
		case 1:
			sample[0] = (float)values[0] / max;
			sample[1] = sample[0];
			break;
		case 2:
			sample[0] = (float)values[0] / max;
			sample[1] = (float)values[1] / max;
			break;
	}

	return sample;
}

Sample
Sample::fromInt16(const int16_t values[], unsigned int numChannels)
{
	const float max = 32767.0f;
	Sample sample;

	switch(numChannels) {
		default:
			throw Exception("Sample::fromInt16(): Unsupported number of channels (%u)\n", numChannels);
			break;
		case 1:
			sample[0] = (float)values[0] / max;
			sample[1] = sample[0];
			break;
		case 2:
			sample[0] = (float)values[0] / max;
			sample[1] = (float)values[1] / max;
			break;
	}

	return sample;
}

} // namespace DromeAudio
