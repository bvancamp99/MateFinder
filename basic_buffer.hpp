#pragma once

#include "error.hpp"

template <typename T, size_t BUF_SIZE>
class BasicBuffer {
public:
	size_t capacity = BUF_SIZE;
	size_t size = 0;

	void append(T val) {
		if (size >= capacity) { ERROR("buffer is full"); }
		data[size++] = val;
	}

	T get(size_t i) {
		if (i >= size) { ERROR("out of index"); }
		return data[i];
	}

	void clear() {
		size = 0;
	}

	T* begin() {
		return data;
	}

	T* end() {
		return data + size;
	}

private:
	T data[BUF_SIZE];
};
