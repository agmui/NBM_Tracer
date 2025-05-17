#ifndef __BUFFER
#define __BUFFER

#include <stdlib.h>

template<class T = int>
class Buffer
{
public:
	Buffer()
	{
		this->w = 0;
		this->h = 0;
		this->data = NULL;
	}

	Buffer(unsigned int w, unsigned int h)
	{
		this->w = w;
		this->h = h;

		alloc();
	}

	Buffer(Buffer const & buffer)
	{
		copy(buffer);
	}

	Buffer & operator=(Buffer const & buffer)
	{
		if(this == &buffer)
			return *this;

		dealloc();
		copy(buffer);

		return *this;
	}

	~Buffer()
	{
		dealloc();
	}

	T at(unsigned int x, unsigned int y) const
	{
		return data[x+y*this->w];
	}

	T & at(unsigned int x, unsigned int y)
	{
		return data[x+y*this->w];
	}

	size_t getWidth() const
	{ return this->w; }

	size_t getHeight() const
	{ return this->h; }

private:
	unsigned int w;
	unsigned int h;
	T * data;

	void alloc()
	{
		size_t size = this->w * this->h;
		if(size == 0)
			return;

		data = (T*) malloc(this->w * this->h * sizeof(T) );
	}

	void dealloc()
	{
		if(this->data != NULL)
			free(data);
	}

	void copy(Buffer const & buffer)
	{
		this->w = buffer.w;
		this->h = buffer.h;

		alloc();
		for(size_t i=0; i<this->w*this->h; i++)
			this->data[i] = buffer.data[i];
	}
};

#endif
