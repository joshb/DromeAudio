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
#include <cstring>
#include <DromeAudio/Exception.h>
#include <DromeAudio/Sound.h>
#ifdef WITH_VORBIS
	#include <DromeAudio/VorbisSound.h>
#endif /* WITH_VORBIS */
#include <DromeAudio/WavSound.h>
#include <DromeAudio/Util.h>
#include "Wav.h"

namespace DromeAudio {

/*
 * Sound class
 */
Sound::Sound()
{
}

Sample
Sound::getSample(unsigned int index, unsigned int targetRate) const
{
	unsigned int rate = getSampleRate();

	// map index from target sample rate if necessary
	if(targetRate != rate) {
		float f = (float)rate / (float)targetRate;
		index = (unsigned int)((float)index * f);
	}

	return getSample(index);
}

void
Sound::save(const char *filename, unsigned int numSamples)
{
	const unsigned char bytesPerSample = 2;
	const unsigned char numChannels = 2;

	// make sure the number of samples to save is good
	if(numSamples == 0) {
		if(getNumSamples() == 0)
			throw Exception("Sound::Save(): Cannot save %s; sound has unlimited number of samples and the number of samples to save was not provided", filename);

		numSamples = getNumSamples();
	}

	// open file for writing
	FILE *fp = fopen(filename, "wb");
	if(!fp)
		throw Exception("Sound::Save(): Unable to open %s for writing", filename);

	// calculate length of sample data
	unsigned int dataLength = numSamples * (unsigned int)bytesPerSample * (unsigned int)numChannels;

	// create riff header
	WavChunkHeader hdr;
	hdr.chunk_id[0] = 'R'; hdr.chunk_id[1] = 'I';
	hdr.chunk_id[2] = 'F'; hdr.chunk_id[3] = 'F';
	hdr.chunk_size = nativeToLittleUInt32(4 + (sizeof(WavChunkHeader) * 2) + sizeof(WavFmtChunk));
	fwrite(&hdr, sizeof(hdr), 1, fp);

	// write riff type
	char tmp[4];
	tmp[0] = 'W'; tmp[1] = 'A'; tmp[2] = 'V'; tmp[3] = 'E';
	fwrite(tmp, 4, 1, fp);

	// write fmt header
	hdr.chunk_id[0] = 'f'; hdr.chunk_id[1] = 'm';
	hdr.chunk_id[2] = 't'; hdr.chunk_id[3] = ' ';
	hdr.chunk_size = nativeToLittleUInt32(sizeof(WavFmtChunk));
	fwrite(&hdr, sizeof(hdr), 1, fp);

	// write fmt chunk
	WavFmtChunk fmt;
	fmt.type = nativeToLittleUInt16(1);
	fmt.channels = nativeToLittleUInt16(numChannels);
	fmt.rate = nativeToLittleUInt32(getSampleRate());
	fmt.bytes_per_second = nativeToLittleUInt32((unsigned int)bytesPerSample * (unsigned int)numChannels * getSampleRate());
	fmt.block_align = nativeToLittleUInt16((uint16_t)bytesPerSample * (uint16_t)numChannels);
	fmt.bits_per_sample = nativeToLittleUInt16((uint16_t)bytesPerSample * 8);
	fwrite(&fmt, sizeof(fmt), 1, fp);

	// write data header
	hdr.chunk_id[0] = 'd'; hdr.chunk_id[1] = 'a';
	hdr.chunk_id[2] = 't'; hdr.chunk_id[3] = 'a';
	hdr.chunk_size = nativeToLittleUInt32(dataLength);
	fwrite(&hdr, sizeof(hdr), 1, fp);

	// write data
	for(unsigned int i = 0; i < numSamples; i++) {
		uint16_t s[2];
		Sample sample = getSample(i);

		s[0] = nativeToLittleUInt16((uint16_t)(32767.0f * sample[0]));
		s[1] = nativeToLittleUInt16((uint16_t)(32767.0f * sample[1]));

		fwrite(s, sizeof(s), 1, fp);
	}

	// done
	fclose(fp);
}

SoundPtr
Sound::create(const char *filename)
{
	// look for file extension
	const char *tmp = filename + strlen(filename);
	while(tmp != filename && *tmp != '.')
		--tmp;
	if(tmp == filename)
		throw Exception("Sound::New(): filename has no extension");

	// create sound based on extension
#ifdef WITH_VORBIS
	if(strCaseCmp(tmp, ".ogg") == 0)
		return VorbisSound::create(filename);
#endif /* WITH_VORBIS */
	if(strCaseCmp(tmp, ".wav") == 0)
		return WavSound::create(filename);

	throw Exception("Sound::New(): Unsupported file extension (%s)", tmp);
	return NULL;
}

} // namespace DromeAudio
