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

#ifndef __DROMEAUDIO_REF_H__
#define __DROMEAUDIO_REF_H__

namespace DromeAudio {

/** \brief Provides a reference counting mechanism for classes that derive from it.
 *
 * Its initial reference count is 1. When its reference count reaches 0, it will automatically delete itself. The RefPtr class should be used for pointers to RefClass-derived classes, as it will automatically increment and decrement the reference count.
 */
class RefClass
{
	protected:
		int m_RefCount;

	public:
		RefClass() { m_RefCount = 0; }
		virtual ~RefClass() { }

		inline void Ref() { ++m_RefCount; }
		inline void Unref() { if(--m_RefCount == 0) delete this; }
};

/** \brief Smart pointer class template for classes that derive from RefClass.
 *
 * As a RefPtr is set, it will increment the reference count of the RefClass-derived object that it's set to point to. When the RefPtr is destroyed or is set to no longer point to an object, the object's reference count will be decremented.
 */
template <typename T> class RefPtr
{
	protected:
		T *ptr;

	public:
		RefPtr() { ptr = 0; }

		RefPtr(const RefPtr <T> &arg)
		{
			ptr = arg.ptr;
			if(ptr)
				ptr->Ref();
		}

		RefPtr(T *arg)
		{
			ptr = arg;
			if(ptr)
				ptr->Ref();
		}

		virtual ~RefPtr() { if(ptr) ptr->Unref(); }

		inline bool IsSet() const { return (ptr != 0); }

		// operators
		inline T * operator -> () { return ptr; }
		inline const T * operator -> () const { return ptr; }

		inline bool operator ! () const  { return (ptr ? false : true); }

		inline void operator = (RefPtr <T> arg)
		{
			if(ptr)
				ptr->Unref();
			ptr = arg.ptr;
			if(ptr)
				ptr->Ref();
		}

		inline void operator = (T *arg)
		{
			if(ptr)
				ptr->Unref();
			ptr = arg;
			if(ptr)
				ptr->Ref();
		}

		inline void operator = (T &arg)
		{
			if(ptr)
				ptr->Unref();
			ptr = &arg;
			if(ptr)
				ptr->Ref();
		}

		inline bool operator == (const RefPtr <T> &arg) const
		{
			return (ptr == arg.ptr);
		}

		inline bool operator != (const RefPtr <T> &arg) const
		{
			return (ptr != arg.ptr);
		}

		// cast overload
		template <typename U>
		inline operator RefPtr <U> ()
		{
			return RefPtr <U> (static_cast <U *> (ptr));
		}
};

} // namespace DromeAudio

#endif /* __DROMEAUDIO_REF_H__ */
