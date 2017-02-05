#ifndef GUARD_LTCL_DETAIL_RBTREE_H
#define GUARD_LTCL_DETAIL_RBTREE_H

/* WORK IN PROGRESS */

namespace ltc {

namespace detail {

template<class T, class K>
struct rb_node {
	rb_node* right;
	rb_node* left;
	rb_node* parent;
	bool isRed;
	K key;
	T value;
};

template<class T, class Key, class Alloc = ltc::allocator<rb_node>>
class RBTree {
public:
	RBTree();
	~RBTree();

	void insert(T value, K key);
	bool search(T value, K key);
	void remove(T value, K key);

	bool isEmpty() const;
private:
	void insertNode(rb_node* new_node);
	void removeNode(rb_node* node);

	rb_node* findNode(int key);
	rb_node* findMax(rb_node* node) const;
	rb_node* findMin(rb_node* node) const;

	void fixTreeInsert(rb_node* node);
	void fixTreeRemove(rb_node* node);
	void leftRotate(rb_node* node);
	void rightRotate(rb_node* node);

	//transplant changes the node2's parent and 
	//updates node1->parent's children as needed
	void transplant(rb_node* node1, rb_node* node2);

	void deleteTree(rb_node* node);

	rb_node* _root {nullptr};
	rb_node* _nil {nullptr};
};

}//namespace ltc

}//namespace detail

template<class T, class K, class A>
ltc::detail::RBTree<T,K,A>::RBTree():_root(0) {
	_nil = new node_s {nullptr, nullptr, nullptr, false, nullptr;
	*_nil = {0, 0, 0, false, 0};
}
template<class T, class K, class A>
ltc::detail::RBTree<T,K,A>::~RBTree() {
	if (this->_root) {
		deleteTree(_root);
	}
	delete this->_nil;
}

template<class T, class K, class A>
ltc::detail::RBTree<T,K,A>::void insert(int key) { 
	node_s* new_node = new node_s;
	*new_node = {this->_nil, this->_nil, this->_nil, true, key};//creates new red node
	insertNode(new_node);
}
template<class T, class K, class A>
ltc::detail::RBTree<T,K,A>::bool search(int key) {
	node_s* node = findNode(key);
	if (node && node->key == key)
		return true;
	return false;
}
template<class T, class K, class A>
ltc::detail::RBTree<T,K,A>::void remove(int key) {
	node_s* old_node = findNode(key);
	if (old_node && old_node->key == key) {
		removeNode(old_node);
	}
}

bool isEmpty() const {
	return _root == 0;
}

void insertNode(rb_node* new_node) {
	//first part of this is exactly the same as in BST
	//if _root is null
	if (this->_root == 0) {
		this->_root = new_node;
		this->_root->isRed = false;
		return;
	}
	/* else */

	//find where to insert
	rb_node* target = this->_root;
	while (true) {
		if (new_node->key <= target->key) {
			if (target->left != this->_nil) {
				target = target->left;
			}
			else {
				target->left = new_node;
				break;
			}
		}
		else {
			if (target->right != this->_nil) {
				target = target->right;
			}
			else {
				target->right = new_node;
				break;
			}
		}
	}

	new_node->parent = target;
	new_node->right = new_node->left = this->_nil;

	//now we fix the Red-Black properties.
	fixTreeInsert(new_node);
}

void removeNode(rb_node* node) {
	bool original_color_was_red = node->isRed;
	rb_node* fix_up_location = node;
	//if node only has one child
	//	if node has only left child (or none)
	if (node->right == this->_nil) {
		fix_up_location = node->left;
		transplant(node, node->left);
	}
	//	if node has only the right child
	else if (node->left == this->_nil) {
		fix_up_location = node->right;
		transplant(node, node->right);
	}
	//if node has both children
	else {
	//	find successor
		rb_node* successor = findMin(node->right);
		original_color_was_red = successor->isRed;
		fix_up_location = successor->right;
	//	if node's successor is it's child
		if (successor->parent == node) {
			fix_up_location->parent = successor;
		}
	//	else
		else {
			transplant(successor, successor->right);
			successor->right = node->right;
			successor->right->parent = successor;
		}
	//	do stuff there that's independ of whether successor is node's child
		successor->left = node->left;
		successor->left->parent = successor;
		successor->isRed = node->isRed;
	}
	//fix suff
	//	if original color was black 
	if (original_color_was_red == false) {
	//  (if we removed a node who's color was red we couldn't have introduced violations
	//	to rb-tree properties)
		fixTreeRemove(fix_up_location);
	}
}

rb_node* findNode(int key) {
	if (this->_root == 0) {
		return this->_root;
	}
	rb_node* current = this->_root;
	while (true) {
		if (key <= current->key) {
			if (key == current->key) {
				return current;
			}
			if (current->left != this->_nil) {
				current = current->left;
			}
			else {
				break;
			}
		}
		else {
			if (current->right != this->_nil) {
				current = current->right;
			}
			else {
				break;
			}
		}
	}
	return current;
}
rb_node* findMax(rb_node* node) const {
	while (node->right != this->_nil) {
		node = node->right;
	}
	return node;
}
rb_node* findMin(rb_node* node) const {
	while (node->left != this->_nil) {
		node = node->left;
	}
	return node;
}

void fixTreeInsert(rb_node* node) {
	while (node->parent->isRed == true) {
		if (node->parent == node->parent->parent->left) {
			rb_node* uncle = node->parent->parent->right;
			//case 1
			//if node's uncle is red
			if (uncle->isRed) {
				node->parent->isRed = false;
				uncle->isRed = false;
				node->parent->parent->isRed = true;
				node = node->parent->parent;
			}
			//case 2 and 3
			else {
				//case 2
				//uncle is black, and node's parent is also red
				//and node is a right child
				if (node == node->parent->right) {
					node = node->parent;
					leftRotate(node);
				}
				//case 3
				//like case 2 but now node is the left child
				node->parent->parent->isRed = true;
				node->parent->isRed = false;
				rightRotate(node->parent->parent);
				break;
			}

		}
		else {
			rb_node* uncle = node->parent->parent->left;
			//case 1
			//if node's uncle is red
			if (uncle->isRed) {
				node->parent->isRed = false;
				uncle->isRed = false;
				node->parent->parent->isRed = true;
				node = node->parent->parent;
			}
			//case 2 and 3
			else {
				//case 2
				//uncle is black, and node's parent is also red
				//and node is a right child
				if (node == node->parent->right) {
					node = node->parent;
					leftRotate(node);
				}
				//case 3
				//like case 2 but now node is the left child
				node->parent->parent->isRed = true;
				node->parent->isRed = false;
				leftRotate(node->parent->parent);
				break;
			}
		}
	}
	this->_root->isRed = false;
}
void fixTreeRemove(rb_node* node) {
	while (node != this->_root && node->isRed == false) {
		if (node == node->parent->left) {
			rb_node* bro_node = node->parent->right;
			//case 1: if x's bro is red
			if (bro_node->isRed) {
				bro_node->isRed = false;
				bro_node->parent->isRed = true;
				leftRotate(bro_node->parent);
				//now bro_node is black
				bro_node = node->parent->right;
			}
			//case 2,3,4: if x's bro is black
			//case 2: if both of x's children are black
			if (bro_node->left->isRed == false && bro_node->right->isRed == false) {
				bro_node->isRed = true;
				node = node->parent;
			}
			//case 3: if bro's left child is red, and right child is black
			else {
				if (bro_node->left->isRed && bro_node->right->isRed == false) {
					bro_node->left->isRed = false;
					bro_node->isRed = true;
					rightRotate(bro_node);
					bro_node = node->parent->right;
				}
				//case 4: if bro's right child is red
				if (bro_node->right->isRed) {
					bro_node->isRed = node->parent->isRed;
					node->parent->isRed = false;
					bro_node->right->isRed = false;
					leftRotate(node->parent);
					node = this->_root;
				}
			}
		}
		else {
			rb_node* bro_node = node->parent->right;
			//case 1: if x's bro is red
			if (bro_node->isRed) {
				bro_node->isRed = false;
				bro_node->parent->isRed = true;
				rightRotate(bro_node->parent);
				//now bro_node is black
				bro_node = node->parent->left;
			}
			//case 2,3,4: if x's bro is black
			//case 2: if both of x's children are black
			if (bro_node->right->isRed == false && bro_node->left->isRed == false) {
				bro_node->isRed = true;
				node = node->parent;
			}
			//case 3: if bro's left child is red, and right child is black
			else {
				if (bro_node->right->isRed && bro_node->left->isRed == false) {
					bro_node->right->isRed = false;
					bro_node->isRed = true;
					leftRotate(bro_node);
					bro_node = node->parent->left;
				}
				//case 4: if bro's right child is red
				if (bro_node->left->isRed) {
					bro_node->isRed = node->parent->isRed;
					node->parent->isRed = false;
					bro_node->left->isRed = false;
					rightRotate(node->parent);
					node = this->_root;
				}
			}
		}
	}
	node->isRed = false;
}
void leftRotate(rb_node* node) {
	rb_node* y = node->right;
	node->right = y->left;
	if (y->left != this->_nil) {
		y->left->parent = node;
	}
	y->parent = node->parent;
	if (node->parent == this->_nil) {
		this->_root = y;
	}
	else if (node == node->parent->left) {
		node->parent->left = y;
	}
	else {
		node->parent->right = y;
	}
	y->left = node;
	node->parent = y;
}
void rightRotate(rb_node* node) {
	rb_node* y = node->left;
	node->left = y->right;
	if (y->right != this->_nil) {
		y->right->parent = node;
	}
	y->parent = node->parent;
	if (node->parent == this->_nil) {
		this->_root = y;
	}
	else if (node == node->parent->right) {
		node->parent->right = y;
	}
	else {
		node->parent->right = y;
	}
	y->right = node;
	node->parent = y;
}
//all transplant does is change the node2's parent and updates node1->parent's children
//as needed
void transplant(rb_node* node1, rb_node* node2) {
	if (node1->parent == this->_nil) {
		this->_root = node2;
	}
	else if (node1 == node1->parent->left) {
		node1->parent->left = node2;
	}
	else {
		node1->parent->right = node2;
	}
	node2->parent = node1->parent;
}

void deleteTree(rb_node* node) {
	if (node != this->_nil) {
		deleteTree(node->right);
		deleteTree(node->left);
		delete node;
	}
}

#endif// !GUARD_LTCL_DETAIL_RBTREE_H
