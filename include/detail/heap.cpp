#ifndef GUARD_HEAP_CPP
#define GUARD_HEAP_CPP

#include <vector>
#include <limits>

#include "heap.h"

using namespace std;

inline size_t parent(const size_t & pos) {
	return (pos - 1)/2;
}

inline size_t right(const size_t & pos) {
	return pos * 2 + 2;
}

inline size_t left(const size_t & pos) {
	return pos * 2 + 1;
}

template<class T, class K>
MinHeap<T,K>::MinHeap(const vector<T>& a) {
	for (size_type i = this->_heap.size(); i-- > 0;) {
		this->maxHeapify(i);
	}
}

template<class T, class K>
void MinHeap<T,K>::insert(const T& data, const K& key) {
	if (this->_heap.empty()) {
		this->_heap.push_back({data,key});
	}
	else {
		this->_heap.push_back({data, std::numeric_limits<K>::max()});
		this->decreaseKey(_heap.size() - 1, key);
	}
}

template<class T, class K>
void MinHeap<T,K>::decreaseKey(const size_type& target, const K& new_key) {
	this->_heap[target].key = new_key;
	for (size_type i = target; i > 0 && _heap[i].key < _heap[parent(i)].key; i = parent(i)) {
		MinHeap<T,K>::Pair<T,K> tmp = this->_heap[i];
		this->_heap[i] = this->_heap[parent(i)];
		this->_heap[parent(i)] = tmp;
	}
}

template<class T, class K>
T MinHeap<T,K>::extractMin() {
	Pair<T,K> ret = this->_heap[0];
	this->_heap[0] = this->_heap[this->_heap.size() - 1];
	this->_heap.pop_back();

	if (!_heap.empty()) {
		this->minHeapify(0);
	}

	return ret.data;
}

template<class T, class K>
T MinHeap<T, K>::getMin() const {
	return this->_heap[0];
}

template<class T, class K>
bool MinHeap<T, K>::isEmpty() const {
	return this->_heap.empty();
}

template<class T, class K>
void MinHeap<T, K>::minHeapify(size_type i) {
	while (true) {
		size_type r = right(i);
		size_type l = left(i);
		size_type smallest = i;

		if (l < _heap.size()) {
			if (_heap[l].key < _heap[smallest].key) {
				smallest = l;
			}
			if (r < _heap.size() && _heap[r].key < _heap[smallest].key) {
				smallest = r;
			}
		}

		MinHeap<T, K>::Pair<T, K> tmp = this->_heap[i];
		this->_heap[i] = this->_heap[smallest];
		this->_heap[smallest] = tmp;
	
		if (smallest != i) {
			i = smallest;
		}
		else {
			break;
		}
	}
}

//* MAX HEAP */
template<class T, class K>
MaxHeap<T,K>::MaxHeap(const vector<T>& a) {
	for (size_type i = this->_heap.size(); i-- > 0;) {
		this->minHeapify(i);
	}
}

template<class T, class K>
void MaxHeap<T,K>::insert(const T& data, const K& key) {
	if (this->_heap.empty()) {
		this->_heap.push_back({data,key});
	}
	else {
		this->_heap.push_back({data, std::numeric_limits<K>::max()});
		this->increaseKey(_heap.size() - 1, key);
	}
}

template<class T, class K>
void MaxHeap<T,K>::increaseKey(const size_type& target, const K& new_key) {
	this->_heap[target].key = new_key;
	for (size_type i = target; i > 0 && _heap[i].key > _heap[parent(i)].key; i = parent(i)) {
		MaxHeap<T,K>::Pair<T,K> tmp = this->_heap[i];
		this->_heap[i] = this->_heap[parent(i)];
		this->_heap[parent(i)] = tmp;
	}
}

template<class T, class K>
T MaxHeap<T,K>::extractMax() {
	Pair<T,K> ret = this->_heap[0];
	this->_heap[0] = this->_heap[this->_heap.size() - 1];
	this->_heap.pop_back();

	if (!_heap.empty()) {
		this->maxHeapify(0);
	}

	return ret.data;
}

template<class T, class K>
T MaxHeap<T, K>::getMax() const {
	return this->_heap[0];
}

template<class T, class K>
bool MaxHeap<T, K>::isEmpty() const {
	return this->_heap.empty();
}

template<class T, class K>
void MaxHeap<T, K>::maxHeapify(size_type i) {
	while (true) {
		size_type r = right(i);
		size_type l = left(i);
		size_type biggest = i;

		if (l < _heap.size()) {
			if (_heap[l].key > _heap[biggest].key) {
				biggest = l;
			}
			if (r < _heap.size() && _heap[r].key > _heap[biggest].key) {
				biggest = r;
			}
		}

		MaxHeap<T, K>::Pair<T, K> tmp = this->_heap[i];
		this->_heap[i] = this->_heap[biggest];
		this->_heap[biggest] = tmp;
	
		if (biggest != i) {
			i = biggest;
		}
		else {
			break;
		}
	}
}

#endif