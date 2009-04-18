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

#ifndef __DROMEAUDIO_SAMPLE_H__
#define __DROMEAUDIO_SAMPLE_H__

#include <DromeAudio/Endian.h>

namespace DromeAudio {

/** \brief Represents a single, multi-channel sample of audio data.
 *
 * Currently supports only two channel (stereo) samples, but this may change in the future.
 */
class Sample
{
	protected:
		float m_channelValues[2];

	public:
		Sample();
		Sample(float values[], unsigned int numChannels);

		/**
		 * @param index Must be 0 (left channel) or 1 (right channel).
		 * @return Value of the channel at the given index.
		 */
		float &operator [] (int index) { return m_channelValues[index]; }

		/**
		 * @param index Must be 0 (left channel) or 1 (right channel).
		 * @return Value of the channel at the given index.
		 */
		const float &operator [] (int index) const { return m_channelValues[index]; }

		Sample operator + (const Sample &s) const;
		void operator += (const Sample &s) { *this = *this + s; }

		Sample operator - (const Sample &s) const;
		void operator -= (const Sample &s) { *this = *this - s; }

		Sample operator * (const Sample &s) const;
		void operator *= (const Sample &s) { *this = *this * s; }

		Sample operator * (float  f) const;
		void operator *= (float f) { *this = *this * f; }

		Sample operator / (const Sample &s) const;
		void operator /= (const Sample &s) { *this = *this / s; }

		Sample operator / (float f) const;
		void operator /= (float f) { *this = *this / f; }

		/**
		 * Clamps the sample's channel values to a range of [-1, 1], where -1 represents the lowest possible channel value and 1 represents the highest.
		 * @return Clamped Sample.
		 */
		Sample clamp() const;

		/**
		 * Changes the sample's stereo balance, with a value of -1 moving the sample completely to the left channel, 0 being normal, and 1 moving the sample completely to the right channel.
		 * @return Balanced Sample.
		 */
		Sample balance(float balance) const;

		/**
		 * @return Sample created from an array of floats.
		 */
		static Sample fromFloat(const float values[], unsigned int numChannels);

		/**
		 * @return Sample created from an array of 8-bit integers.
		 */
		static Sample fromInt8(const int8_t values[], unsigned int numChannels);

		/**
		 * @return Sample created from an array of 16-bit integers.
		 */
		static Sample fromInt16(const int16_t values[], unsigned int numChannels);
};

} // namespace DromeAudio

#endif /* __DROMEAUDIO_SAMPLE_H__ */
