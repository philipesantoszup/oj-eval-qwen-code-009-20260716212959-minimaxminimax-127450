#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu
{

template<typename T>
class vector
{
public:
	class const_iterator;
	class iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::random_access_iterator_tag;

	private:
		T* ptr;
		
	public:
		iterator(T* p = nullptr) : ptr(p) {}
		
		iterator operator+(const int &n) const
		{
			return iterator(ptr + n);
		}
		iterator operator-(const int &n) const
		{
			return iterator(ptr - n);
		}
		int operator-(const iterator &rhs) const
		{
			return ptr - rhs.ptr;
		}
		iterator& operator+=(const int &n)
		{
			ptr += n;
			return *this;
		}
		iterator& operator-=(const int &n)
		{
			ptr -= n;
			return *this;
		}
		iterator operator++(int)
		{
			iterator tmp = *this;
			++ptr;
			return tmp;
		}
		iterator& operator++()
		{
			++ptr;
			return *this;
		}
		iterator operator--(int)
		{
			iterator tmp = *this;
			--ptr;
			return tmp;
		}
		iterator& operator--()
		{
			--ptr;
			return *this;
		}
		T& operator*() const
		{
			return *ptr;
		}
		bool operator==(const iterator &rhs) const
		{
			return ptr == rhs.ptr;
		}
		bool operator==(const const_iterator &rhs) const
		{
			return ptr == rhs.ptr;
		}
		bool operator!=(const iterator &rhs) const
		{
			return ptr != rhs.ptr;
		}
		bool operator!=(const const_iterator &rhs) const
		{
			return ptr != rhs.ptr;
		}
	};
	class const_iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::random_access_iterator_tag;

	private:
		const T* ptr;
		
	public:
		const_iterator(const T* p = nullptr) : ptr(p) {}
		
		const_iterator operator+(const int &n) const
		{
			return const_iterator(ptr + n);
		}
		const_iterator operator-(const int &n) const
		{
			return const_iterator(ptr - n);
		}
		int operator-(const const_iterator &rhs) const
		{
			return ptr - rhs.ptr;
		}
		const_iterator& operator+=(const int &n)
		{
			ptr += n;
			return *this;
		}
		const_iterator& operator-=(const int &n)
		{
			ptr -= n;
			return *this;
		}
		const_iterator operator++(int)
		{
			const_iterator tmp = *this;
			++ptr;
			return tmp;
		}
		const_iterator& operator++()
		{
			++ptr;
			return *this;
		}
		const_iterator operator--(int)
		{
			const_iterator tmp = *this;
			--ptr;
			return tmp;
		}
		const_iterator& operator--()
		{
			--ptr;
			return *this;
		}
		const T& operator*() const
		{
			return *ptr;
		}
		bool operator==(const iterator &rhs) const
		{
			return ptr == rhs.ptr;
		}
		bool operator==(const const_iterator &rhs) const
		{
			return ptr == rhs.ptr;
		}
		bool operator!=(const iterator &rhs) const
		{
			return ptr != rhs.ptr;
		}
		bool operator!=(const const_iterator &rhs) const
		{
			return ptr != rhs.ptr;
		}
	};

private:
	T* data_;
	size_t length_;
	size_t capacity_;
	
	void destroy_all()
	{
		for (size_t i = 0; i < length_; ++i)
		{
			data_[i].~T();
		}
	}
	
	void realloc(size_t new_cap)
	{
		T* new_data = static_cast<T*>(::operator new(new_cap * sizeof(T)));
		for (size_t i = 0; i < length_; ++i)
		{
			new (new_data + i) T(std::move(data_[i]));
		}
		destroy_all();
		::operator delete(data_);
		data_ = new_data;
		capacity_ = new_cap;
	}
	
public:
	vector() : data_(nullptr), length_(0), capacity_(0) {}
	
	vector(const vector &other) : data_(nullptr), length_(0), capacity_(0)
	{
		if (other.length_ > 0)
		{
			data_ = static_cast<T*>(::operator new(other.length_ * sizeof(T)));
			for (size_t i = 0; i < other.length_; ++i)
			{
				new (data_ + i) T(other.data_[i]);
			}
			length_ = other.length_;
			capacity_ = other.length_;
		}
	}
	
	~vector()
	{
		destroy_all();
		::operator delete(data_);
	}
	
	vector &operator=(const vector &other)
	{
		if (this != &other)
		{
			destroy_all();
			length_ = 0;
			if (capacity_ < other.length_)
			{
				::operator delete(data_);
				data_ = nullptr;
				capacity_ = 0;
			}
			if (other.length_ > 0)
			{
				if (!data_)
				{
					data_ = static_cast<T*>(::operator new(other.length_ * sizeof(T)));
					capacity_ = other.length_;
				}
				for (size_t i = 0; i < other.length_; ++i)
				{
					new (data_ + i) T(other.data_[i]);
				}
				length_ = other.length_;
			}
		}
		return *this;
	}
	
	T & at(const size_t &pos)
	{
		if (pos >= length_)
		{
			throw index_out_of_bound();
		}
		return data_[pos];
	}
	const T & at(const size_t &pos) const
	{
		if (pos >= length_)
		{
			throw index_out_of_bound();
		}
		return data_[pos];
	}
	T & operator[](const size_t &pos)
	{
		return data_[pos];
	}
	const T & operator[](const size_t &pos) const
	{
		return data_[pos];
	}
	const T & front() const
	{
		if (length_ == 0)
		{
			throw container_is_empty();
		}
		return data_[0];
	}
	const T & back() const
	{
		if (length_ == 0)
		{
			throw container_is_empty();
		}
		return data_[length_ - 1];
	}
	iterator begin()
	{
		return iterator(data_);
	}
	const_iterator begin() const
	{
		return const_iterator(data_);
	}
	const_iterator cbegin() const
	{
		return const_iterator(data_);
	}
	iterator end()
	{
		return iterator(data_ + length_);
	}
	const_iterator end() const
	{
		return const_iterator(data_ + length_);
	}
	const_iterator cend() const
	{
		return const_iterator(data_ + length_);
	}
	bool empty() const
	{
		return length_ == 0;
	}
	size_t size() const
	{
		return length_;
	}
	void clear()
	{
		destroy_all();
		length_ = 0;
	}
	
	iterator insert(iterator pos, const T &value)
	{
		size_t insert_pos = pos - begin();
		if (length_ + 1 > capacity_)
		{
			size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
			realloc(new_cap);
		}
		for (size_t i = length_; i > insert_pos; --i)
		{
			new (data_ + i) T(std::move(data_[i - 1]));
		}
		new (data_ + insert_pos) T(value);
		++length_;
		return iterator(data_ + insert_pos);
	}
	
	iterator insert(const size_t &ind, const T &value)
	{
		if (ind > length_)
		{
			throw index_out_of_bound();
		}
		return insert(begin() + ind, value);
	}
	
	iterator erase(iterator pos)
	{
		return erase(pos - begin());
	}
	
	iterator erase(const size_t &ind)
	{
		if (ind >= length_)
		{
			throw index_out_of_bound();
		}
		for (size_t i = ind; i < length_ - 1; ++i)
		{
			data_[i].~T();
			new (data_ + i) T(std::move(data_[i + 1]));
		}
		data_[length_ - 1].~T();
		--length_;
		return iterator(data_ + ind);
	}
	
	void push_back(const T &value)
	{
		if (length_ + 1 > capacity_)
		{
			size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
			realloc(new_cap);
		}
		new (data_ + length_) T(value);
		++length_;
	}
	
	void pop_back()
	{
		if (length_ == 0)
		{
			throw container_is_empty();
		}
		data_[length_ - 1].~T();
		--length_;
	}
};


}

#endif