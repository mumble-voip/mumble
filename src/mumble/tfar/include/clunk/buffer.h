#ifndef CLUNK_BUFFER_H__
#define CLUNK_BUFFER_H__

/* clunk - cross-platform 3D audio API built on top SDL library
 * Copyright (C) 2007-2014 Netive Media Group
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/


#include <clunk/types.h>
#include <clunk/export_clunk.h>
#include <string>

namespace clunk {

/*!
	\brief Memory buffer
	This class contains single memory buffer, allocated by malloc. 
	It auto frees it when it goes out of scope.	
*/

class CLUNKAPI Buffer {
public:
	//! Default ctor, empty buffer.
	inline Buffer(): ptr(NULL), size(0) {}
	//! Copy ctor
	inline Buffer(const Buffer& c) : ptr(NULL), size(0) { *this = c; }
	/*!
		\brief Instantly allocates 'size' memory
		\param[in] size size of the memory buffer 
	*/ 
	inline Buffer(int size): ptr(NULL), size(0) { set_size(size); }

	//! Destructor, deallocates buffer if needed
	inline ~Buffer() { free(); }
	
	//! Gets pointer to the memory buffer
	inline void *get_ptr() const { return ptr; }
	//! Gets pointer to the memory buffer (const)
	inline size_t get_size() const { return size; }
	/*!
		\brief Tests if buffer was empty
		\return returns true if the buffer is empty or deallocated.
	*/
	inline bool empty() const { return ptr == NULL; }

	/*! 
		\brief Leaks buffer's content. Use it with care.
		Leaks buffer's content. 
		Useful for exception-safe passing of malloc'ed memory to some library function which later deallocates it.
	*/
	inline void unlink() { ptr = NULL; size = 0; }

	//! Default operator=
	const Buffer& operator=(const Buffer& c);

	//! Frees buffer. you do not have to call it exlicitly everywhere! 
	void free();
	/*!
		\brief Sets size of the buffer
		reallocates buffer to the given size. May throw exception! 
		\param[in] s size of the new buffer.
	*/
	void set_size(size_t s);
	/*! \brief Sets buffer content to a given data.
		Copies given data to the buffer. Note, that functions allocates memory for a new buffer. Do not forget to deallocate 'p' if needed. 
		\param[in] p source pointer
		\param[in] s size of the data to be copied.
	*/
	void set_data(const void *p, const size_t s);
	/*! \brief Sets buffer content to a given data.
		Copies given data to the buffer. Note, that functions allocates memory for a new buffer if 'own' == false. Do not forget to deallocate 'p' if needed. 
		\param[in] p source pointer
		\param[in] s size of the data to be copied.
		\param[in] own grab pointer and deallocate it later automatically. 
	*/
	void set_data(void *p, const size_t s, const bool own = false);

	//! Fills contents of the buffer with given byte.
	void fill(int b);

	//! Appends given buffer.	
	void append(const Buffer &other);
	//! Appends given buffer.	
	void append(const void *data, const size_t size);

	/*! 
		\brief Add more bytes to the end of the buffer.
		\param[in] more number of bytes to be allocated.
	*/
	void *reserve(int more);

	//! Returns nice string representation for the buffer. Useful for debugging.
	const std::string dump() const;
	
	//! Pops n bytes from the front
	void pop(size_t n); 

protected:
	void *ptr;
	size_t size;
};

}

#endif
