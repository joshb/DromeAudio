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

#include <cstdio>
#include <DromeAudio/Exception.h>
#include <DromeAudio/Endian.h>
#include <DromeAudio/WavSound.h>
#include "Wav.h"

namespace DromeAudio {

/*
 * WavSound class
 */
WavSound::WavSound(const char *filename)
{
	FILE *fp = fopen(filename, "rb");
	if(!fp)
		throw Exception("WavSound::WavSound(): Unable to open %s for reading", filename);

    // read riff header
	WavChunkHeader hdr;
	fread(&hdr, sizeof(hdr), 1, fp);
	hdr.chunk_size = littleToNativeUInt32(hdr.chunk_size);

	if(hdr.chunk_id[0] != 'R' || hdr.chunk_id[1] != 'I' ||
	   hdr.chunk_id[2] != 'F' || hdr.chunk_id[3] != 'F') {
		fclose(fp);
		throw Exception("WavSound::WavSound(): Invalid file [1]");
	}

	// read riff type
	char tmp[4];
	fread(tmp, sizeof(tmp), 1, fp);
	if(tmp[0] != 'W' || tmp[1] != 'A' || tmp[2] != 'V' || tmp[3] != 'E') {
		fclose(fp);
		throw Exception("WavSound::WavSound(): Invalid file [2]");
	}

    // read fmt header
	fread(&hdr, sizeof(hdr), 1, fp);
	hdr.chunk_size = littleToNativeUInt32(hdr.chunk_size);
	if(hdr.chunk_id[0] != 'f' || hdr.chunk_id[1] != 'm' ||
	   hdr.chunk_id[2] != 't' || hdr.chunk_id[3] != ' ') {
		fclose(fp);
		throw Exception("WavSound::WavSound(): Invalid file [3]");
	}

	// read fmt chunk
	WavFmtChunk fmt;
	fread(&fmt, sizeof(fmt), 1, fp);
	fmt.type = littleToNativeUInt16(fmt.type);
	fmt.channels = littleToNativeUInt16(fmt.channels);
	fmt.rate = littleToNativeUInt16(fmt.rate);
	fmt.bytes_per_second = littleToNativeUInt16(fmt.bytes_per_second);
	fmt.block_align = littleToNativeUInt16(fmt.block_align);
	fmt.bits_per_sample = littleToNativeUInt16(fmt.bits_per_sample);

	// make sure the compression type is supported
    if(fmt.type != 1) {
        fclose(fp);
        throw Exception("WavSound::WavSound(): Unsupported compression type; only uncompressed PCM is supported");
    }

	m_numChannels = fmt.channels;
	m_bytesPerSample = fmt.bits_per_sample / 8;
	m_sampleRate = fmt.rate;

	// look for data chunk
	bool dataFound = false;
	while(fread(&hdr, sizeof(hdr), 1, fp) == 1) {
		hdr.chunk_size = littleToNativeUInt32(hdr.chunk_size);

		if(hdr.chunk_id[0] == 'd' && hdr.chunk_id[1] == 'a' &&
		   hdr.chunk_id[2] == 't' && hdr.chunk_id[3] == 'a') {
			dataFound = true;
			break;
		} else {
			if(fseek(fp, hdr.chunk_size, SEEK_CUR) != 0)
				break;
		}
	}

	if(!dataFound) {
		fclose(fp);
		throw Exception("WavSound::WavSound(): No data chunk found");
	}

	m_dataSize = hdr.chunk_size;
	m_numSamples = m_dataSize / m_numChannels / m_bytesPerSample;

	// read data chunk
	m_data = new uint8_t [m_dataSize];
	fread(m_data, sizeof(uint8_t), m_dataSize, fp);

	// done
	fclose(fp);

	// do byte-swapping if necessary
	if(m_bytesPerSample == 2) {
		int16_t *data = (int16_t *)m_data;
		for(unsigned int i = 0; i < m_dataSize / 2; i++)
			data[i] = littleToNativeInt16(data[i]);
	}
}

WavSound::~WavSound()
{
	delete [] m_data;
}

Sample
WavSound::getSample(unsigned int index) const
{
	Sample sample;

	if(index >= m_numSamples)
		return sample;

	if(m_bytesPerSample == 2) {
		int16_t *data = ((int16_t *)m_data) + (index * m_numChannels);
		sample = Sample::fromInt16(data, m_numChannels);
	} else if(m_bytesPerSample == 1) {
		int8_t *data = ((int8_t *)m_data) + (index * m_numChannels);
		sample = Sample::fromInt8(data, m_numChannels);
	} else {
		throw Exception("WavSound::GetSample(): Unsupported number of bytes per sample (%u)\n", m_bytesPerSample);
	}

	return sample;
}

WavSoundPtr
WavSound::create(const char *filename)
{
	return WavSoundPtr(new WavSound(filename));
}

} // namespace DromeAudio
