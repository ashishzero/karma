#pragma once
#include "karma.h"
#include "lin_maths.h"
#include <algorithm>

inline u32 murmur3_32(const void *ptr, size_t len, u32 seed) {
	const u8 *key = (u8 *)ptr;
	u32       h   = seed;
	u32       k;
	/* Read in groups of 4. */
	for (size_t i = len >> 2; i; i--) {
		// Here is a source of differing results across endiannesses.
		// A swap here has no effects on hash properties though.
		k = *((u32 *)key);
		key += sizeof(u32);

		k *= 0xcc9e2d51;
		k = (k << 15) | (k >> 17);
		k *= 0x1b873593;

		h ^= k;
		h = (h << 13) | (h >> 19);
		h = h * 5 + 0xe6546b64;
	}
	/* Read the rest. */
	k = 0;
	for (size_t i = len & 3; i; i--) {
		k <<= 8;
		k |= key[i - 1];
	}
	// A swap is *not* necessary here because the preceeding loop already
	// places the low bytes in the low places according to whatever endianness
	// we use. Swaps only apply when the memory is copied in a chunk.

	k *= 0xcc9e2d51;
	k = (k << 15) | (k >> 17);
	k *= 0x1b873593;
	h ^= k;
	/* Finalize. */
	h ^= len;
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;
	return h;
}

//
//
//

#define swap_by_exchange(a, b, t) (t = a, a = b, b = t)

template <typename Type>
inline void swap(Type *a, Type *b) {
	Type t = *a;
	*a     = *b;
	*b     = t;
}

template <typename Type>
inline void swap_vec(Type *a, Type *b, s64 n) {
	if (n > 0) {
		Type t;
		for (; n > 0; a += 1, b += 1, n -= 1)
			swap_by_exchange(*a, *b, t);
	}
}

template <typename Type, typename Compare>
inline void sort_insert(Type *a, s64 n, Compare cmp) {
	int i, j;
	for (i = 1; i < n; ++i) {
		Type t = a[i];
		j      = i - 1;
		for (; j >= 0 && cmp(a[j], t) > 0; --j) {
			swap(a + j, a + j + 1);
		}
		a[j + 1] = t;
	}
}

template <typename Type, typename Compare>
void sort(Type *a, s64 n, Compare cmp) {
	std::sort(a, a + n, cmp);
}

// NOTE: *left* is inclusive but *right* is exclusive
template <typename T, typename Compare>
void merge_buttom_up(T *src, s64 left, s64 mid, s64 right, T *dst, Compare comp) {
	s64 i = left, j = mid;
	for (s64 k = left; k < right; k++) {
		if (i < mid && (j >= right || comp(src[i], src[j]) <= 0)) {
			dst[k] = src[i];
			i += 1;
		} else {
			dst[k] = src[j];
			j += 1;
		}
	}
}

template <typename T, typename Compare>
void sort_merge(T *src, T *aux, s64 n, Compare comp) {
	T *t;
	for (s64 width = 1; width < n; width = 2 * width) {
		for (s64 i = 0; i < n; i = i + 2 * width) {
			merge_buttom_up(src, i, min_value(i + width, n), min_value(i + 2 * width, n), aux, comp);
		}
		swap_by_exchange(src, aux, t);
	}
}

template <typename T, typename Compare>
void heap_make(T *root, s64 height, Compare comp) {
	while (height != 0) {
		int parent = (height - 1) / 2;
		if (comp(root[parent], root[height]) > 0) {
			swap(root + parent, root + height);
			height = parent;
		} else {
			break;
		}
	}
}

template <typename T, typename Compare>
void heap_push(T *root, s64 height, const T &node, Compare comp) {
	root[height] = node;
	heap_make(root, height, comp);
}

template <typename T, typename Compare>
void heap_pop(T *root, s64 height, Compare comp) {
	assert(height != 0);

	height -= 1;
	root[0] = root[height];

	s64 current = 0;
	s64 left    = 2 * current + 1;

	while (left < height) {
		s64 index = left;

		s64 right = 2 * current + 2;
		if (right < height) {
			if (comp(root[index], root[right]) > 0) {
				index = right;
			}
		}

		if (comp(root[current], root[index]) > 0) {
			swap(root + current, root + index);
			current = index;
		} else {
			break;
		}

		left = 2 * current + 1;
	}
}

template <typename T, typename Compare>
void sort_heap(T *a, s64 n, Compare comp) {
	for (s64 i = 1; i < n; ++i) {
		heap_make(a, i, comp);
	}
}
