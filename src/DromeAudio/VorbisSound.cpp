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
#include <DromeAudio/Exception.h>
#include <DromeAudio/Endian.h>
#include <DromeAudio/VorbisSound.h>
#include <vorbisfile.h>

namespace DromeAudio {

/*
 * VorbisSound class
 */
VorbisSound::VorbisSound(const char *filename)
{
	OggVorbis_File vf;

	// open file
	if(ov_fopen((char *)filename, &vf) != 0)
		throw Exception("VorbisSound::VorbisSound(): ov_fopen failed");

	// get vorbis information
	vorbis_info *info = ov_info(&vf, 0);
	m_numChannels = (unsigned char)info->channels;
	m_bytesPerSample = 2;
	m_sampleRate = (unsigned int)info->rate;
	m_numSamples = (unsigned int)ov_pcm_total(&vf, 0);

	// allocate memory
	m_dataSize = m_numSamples * m_bytesPerSample * m_numChannels;
	m_data = new uint8_t [m_dataSize];

	// decode file
	int bigendianp = (GetEndianness() == ENDIANNESS_BIG) ? 1 : 0;
	int bitstream = 0;
	char *data = (char *)m_data;
	int result;
	while((result = ov_read(&vf, (char *)data, 4096, bigendianp, 2, 1, &bitstream)) > 0)
		data += result;

	// done
	ov_clear(&vf);
}

VorbisSound::~VorbisSound()
{
	delete [] m_data;
}

unsigned char
VorbisSound::getNumChannels() const
{
	return m_numChannels;
}

unsigned int
VorbisSound::getSampleRate() const
{
	return m_sampleRate;
}

unsigned int
VorbisSound::getNumSamples() const
{
	return m_numSamples;
}

Sample
VorbisSound::getSample(unsigned int index) const
{
	Sample sample;

	index %= m_numSamples;

	if(m_bytesPerSample == 2) {
		int16_t *data = ((int16_t *)m_data) + (index * m_numChannels);
		sample = Sample::fromInt16(data, m_numChannels);
	} else if(m_bytesPerSample == 1) {
		int8_t *data = ((int8_t *)m_data) + (index * m_numChannels);
		sample = Sample::fromInt8(data, m_numChannels);
	} else {
		throw Exception("VorbisSound::getSample(): Unsupported number of bytes per sample (%u)", m_bytesPerSample);
	}

	return sample;
}

VorbisSoundPtr
VorbisSound::create(const char *filename)
{
	return VorbisSoundPtr(new VorbisSound(filename));
}

} // namespace DromeAudio
