#pragma once
#include "karma.h"

template <typename Node_Data_Type>
struct Circular_Linked_List {
	struct Node {
		Node *prev;
		Node *next;
		Node_Data_Type data;
	};

	Node node;
	Allocator allocator;
};

template <typename T>
void circular_linked_list_init(Circular_Linked_List<T> *list, Allocator allocator = context.allocator) {
	list->allocator = context.allocator;
	list->node.next = &list->node;
	list->node.prev = &list->node;
}

template <typename T>
typename Circular_Linked_List<T>::Node *circular_linked_list_add(Circular_Linked_List<T> *list) {
	auto node = new(list->allocator)Circular_Linked_List<T>::Node;

	node->prev = list->node.prev;
	node->next = &list->node;

	list->node.prev->next = node;
	list->node.prev = node;

	return node;
}

template <typename T>
typename Circular_Linked_List<T>::Node *circular_linked_list_insert(typename Circular_Linked_List<T>::Node *point) {
	auto node = new(list->allocator)Circular_Linked_List<T>::Node;

	node->prev = point;
	node->next = point->next;

	point->next->prev = node;
	point->next = node;

	return node;
}

template <typename T>
void circular_linked_list_remove(Circular_Linked_List<T> *list, typename Circular_Linked_List<T>::Node *node) {
	auto new_next = node->next;
	auto new_prev = node->prev;
	new_next->prev = new_prev;
	new_prev->next = new_next;

	memory_free(node, list->allocator);
}

template <typename T>
typename Circular_Linked_List<T>::Node *iter_begin(Circular_Linked_List<T> *list) {
	return list->node.next;
}

template <typename T>
bool iter_continue(Circular_Linked_List<T> *list, typename Circular_Linked_List<T>::Node *node) {
	return node != &list->node;
}

template <typename T>
typename Circular_Linked_List<T>::Node *iter_next(typename Circular_Linked_List<T>::Node *node) {
	return node->next;
}

template <typename T>
void circular_linked_list_clear(Circular_Linked_List<T> *list) {
	for (auto node = iter_begin(list); iter_continue(list, node); node = iter_next<T>(node))
		memory_free(node, list->allocator);
	circular_linked_list_init(list, list->allocator);
}

template <typename T>
typename Circular_Linked_List<T>::Node *circular_linked_list_node_from_data(const T *data) {
	return (Circular_Linked_List<T>::Node *)(((char *)data) - offsetof(Circular_Linked_List<T>::Node, Circular_Linked_List<T>::Node::data));
}

#define for_list(type, ptr, l) for (auto ptr = iter_begin(l); iter_continue(l, ptr); ptr = iter_next<type>(ptr))
#define for_list_offset(type, ptr, offset, l) for (auto ptr = offset->next; iter_continue(l, ptr); ptr = iter_next<type>(ptr))
