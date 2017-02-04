#ifndef guard_heap_h
#define guard_heap_h

#include <vector>

template<class T, class K>
class MinHeap {
	typedef typename std::vector<T>::size_type size_type;

public:
	/* Constructors */
	MinHeap(): _heap() {}
	MinHeap(const std::vector<T>& a);
	/* Interface */
	void insert(const T& data, const K& key);
	void decreaseKey(const size_type& target, const K& new_key);
	T extractMin();

	T getMin() const;
	bool isEmpty() const;

private:
	template<class T, class K>
	struct Pair {
		T data;
		K key;
	};
	void minHeapify(size_type i);

	std::vector<Pair<T,K> > _heap;
};

template<class T, class K>
class MaxHeap {
	typedef typename std::vector<T>::size_type size_type;

public:
	/* Constructors */
	MaxHeap(): _heap() {}
	MaxHeap(const std::vector<T>& a);
	/* Interface */
	void insert(const T& data, const K& key);
	void increaseKey(const size_type& target, const K& new_key);
	T extractMax();

	T getMax() const;
	bool isEmpty() const;

private:
	template<class T, class K>
	struct Pair {
		T data;
		K key;
	};
	void maxHeapify(size_type i);

	std::vector<Pair<T,K> > _heap;
};
#include "heap.cpp"

#endif