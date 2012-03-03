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
 * OR BUSAWSS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __DROMEAUDIO_SAWSOUND_H__
#define __DROMEAUDIO_SAWSOUND_H__

#include <DromeAudio/Sound.h>

namespace DromeAudio {

class SawSound;
typedef RefPtr <SawSound> SawSoundPtr;

class SawSound : public Sound
{
	protected:
		float m_frequency;

		SawSound(float frequency);

	public:
		unsigned int getNumSamples() const;

		void setParameter(const std::string &name, float value);

		float getFrequency() const;
		void setFrequency(float value);

		Sample getSample(unsigned int index) const;

		static SawSoundPtr create(float frequency);
};

} // namespace DromeAudio

#endif /* __DROMEAUDIO_SAWSOUND_H__ */
