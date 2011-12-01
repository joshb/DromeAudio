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

#ifndef __DROMEAUDIO_ENDIAN_H__
#define __DROMEAUDIO_ENDIAN_H__

#include <stdint.h>

namespace DromeAudio {

enum Endianness {
	ENDIANNESS_UNKNOWN = 0,
	ENDIANNESS_LITTLE,
	ENDIANNESS_BIG
};

Endianness GetEndianness();

float LittleToNativeFloat(float f);
int32_t LittleToNativeInt32(int32_t i);
uint32_t LittleToNativeUInt32(uint32_t i);
int16_t LittleToNativeInt16(int16_t s);
uint16_t LittleToNativeUInt16(uint16_t s);

inline float NativeToLittleFloat(float f) { return LittleToNativeFloat(f); }
inline int32_t NativeToLittleInt32(int32_t i) { return LittleToNativeInt32(i); }
inline uint32_t NativeToLittleUInt32(uint32_t i) { return LittleToNativeUInt32(i); }
inline int16_t NativeToLittleInt16(int16_t s) { return LittleToNativeInt16(s); }
inline uint16_t NativeToLittleUInt16(uint16_t s) { return LittleToNativeUInt16(s); }

float BigToNativeFloat(float f);
int32_t BigToNativeInt32(int32_t i);
uint32_t BigToNativeUInt32(uint32_t i);
int16_t BigToNativeInt16(int16_t s);
uint16_t BigToNativeUInt16(uint16_t s);

inline float NativeToBigFloat(float f) { return BigToNativeFloat(f); }
inline int32_t NativeToBigInt32(int32_t i) { return BigToNativeInt32(i); }
inline uint32_t NativeToBigUInt32(uint32_t i) { return BigToNativeUInt32(i); }
inline int16_t NativeToBigInt16(int16_t s) { return BigToNativeInt16(s); }
inline uint16_t NativeToBigUInt16(uint16_t s) { return BigToNativeUInt16(s); }

} // namespace DromeAudio

#endif /* __DROMEAUDIO_ENDIAN_H__ */
