/*
 * Copyright (C) 2008 Josh A. Beam
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
#include <DromeAudio/Endian.h>

namespace DromeAudio {

static Endianness endianness = ENDIANNESS_UNKNOWN;

Endianness
GetEndianness()
{
	if(endianness == ENDIANNESS_UNKNOWN) {
		unsigned char c[] = { 1, 0 };
		unsigned short *p = (unsigned short *)c;

		if(*p == 1)
			endianness = ENDIANNESS_LITTLE;
		else
			endianness = ENDIANNESS_BIG;
	}

	return endianness;
}

static float
swap_float(float f)
{
	float retval;
	uint8_t *p1 = (uint8_t *)&f;
	uint8_t *p2 = (uint8_t *)&retval;

	p2[0] = p1[3];
	p2[1] = p1[2];
	p2[2] = p1[1];
	p2[3] = p1[0];

	return retval;
}

static int32_t
swap_int(int32_t i)
{
	int32_t retval;
	uint8_t *p1 = (uint8_t *)&i;
	uint8_t *p2 = (uint8_t *)&retval;

	p2[0] = p1[3];
	p2[1] = p1[2];
	p2[2] = p1[1];
	p2[3] = p1[0];

	return retval;
}

static uint32_t
swap_uint(uint32_t i)
{
	uint32_t retval;
	uint8_t *p1 = (uint8_t *)&i;
	uint8_t *p2 = (uint8_t *)&retval;

	p2[0] = p1[3];
	p2[1] = p1[2];
	p2[2] = p1[1];
	p2[3] = p1[0];

	return retval;
}

static int16_t
swap_short(int16_t s)
{
	int16_t retval;
	uint8_t *p1 = (uint8_t *)&s;
	uint8_t *p2 = (uint8_t *)&retval;

	p2[0] = p1[1];
	p2[1] = p1[0];

	return retval;
}

static uint16_t
swap_ushort(uint16_t s)
{
	uint16_t retval;
	uint8_t *p1 = (uint8_t *)&s;
	uint8_t *p2 = (uint8_t *)&retval;

	p2[0] = p1[1];
	p2[1] = p1[0];

	return retval;
}

float
LittleToNativeFloat(float f)
{
	if(GetEndianness() == ENDIANNESS_BIG)
		return swap_float(f);

	return f;
}

int32_t
LittleToNativeInt32(int32_t i)
{
	if(GetEndianness() == ENDIANNESS_BIG)
		return swap_int(i);

	return i;
}

uint32_t
LittleToNativeUInt32(uint32_t i)
{
	if(GetEndianness() == ENDIANNESS_BIG)
		return swap_uint(i);

	return i;
}

int16_t
LittleToNativeInt16(int16_t s)
{
	if(GetEndianness() == ENDIANNESS_BIG)
		return swap_short(s);

	return s;
}

uint16_t
LittleToNativeUInt16(uint16_t s)
{
	if(GetEndianness() == ENDIANNESS_BIG)
		return swap_ushort(s);

	return s;
}

float
BigToNativeFloat(float f)
{
	if(GetEndianness() != ENDIANNESS_BIG)
		return swap_float(f);

	return f;
}

int32_t
BigToNativeInt32(int32_t i)
{
	if(GetEndianness() != ENDIANNESS_BIG)
		return swap_int(i);

	return i;
}

uint32_t
BigToNativeUInt32(uint32_t i)
{
	if(GetEndianness() != ENDIANNESS_BIG)
		return swap_uint(i);

	return i;
}

int16_t
BigToNativeInt16(int16_t s)
{
	if(GetEndianness() != ENDIANNESS_BIG)
		return swap_short(s);

	return s;
}

uint16_t
BigToNativeUInt16(uint16_t s)
{
	if(GetEndianness() != ENDIANNESS_BIG)
		return swap_ushort(s);

	return s;
}

} // namespace DromeAudio
