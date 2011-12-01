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

#include <cstring>
#ifdef _WIN32
	#include <windows.h>
#else
	#include <pthread.h>
#endif /* _WIN32 */
#include <DromeAudio/Exception.h>
#include <DromeAudio/Mutex.h>

namespace DromeAudio {

#ifndef _WIN32
class PThreadMutex : public Mutex
{
	protected:
		pthread_mutex_t m_mutex;

	public:
		PThreadMutex()
		{
			if(pthread_mutex_init(&m_mutex, NULL) != 0)
				throw Exception("PThreadMutex::PThreadMutex(): pthread_mutex_init failed");
		}

		~PThreadMutex()
		{
			if(pthread_mutex_destroy(&m_mutex) != 0)
				throw Exception("PThreadMutex::~PThreadMutex(): pthread_mutex_destroy failed");
		}

		void lock()
		{
			pthread_mutex_lock(&m_mutex);
		}

		void unlock()
		{
			pthread_mutex_unlock(&m_mutex);
		}
};
#endif

#ifdef _WIN32
class WinMutex : public Mutex
{
	protected:
		HANDLE m_mutex;

	public:
		WinMutex()
		{
			m_mutex = CreateMutex(NULL, false, NULL);
		}

		~WinMutex()
		{
			CloseHandle(m_Mutex);
		}

		void lock()
		{
			WaitForSingleObject(m_Mutex, INFINITE);
		}

		void unlock()
		{
			ReleaseMutex(m_Mutex);
		}
};
#endif

Mutex *
Mutex::create()
{
#if _WIN32
	return new WinMutex();
#else
	return new PThreadMutex();
#endif /* _WIN32 */
}

} // namespace DromeAudio
