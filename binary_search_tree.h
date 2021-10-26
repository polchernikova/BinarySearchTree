#ifndef BINARY_SEARCH_TREE_H_
#define BINARY_SEARCH_TREE_H_

#include <iostream>
#include <vector>
#include <stack>

template<typename T>
class BinarySearchTree {
private:
    struct TreeNode;
public:
    class ConstIterator
            : std::iterator<std::bidirectional_iterator_tag, T> {
        friend class BinarySearchTree;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using pointer = value_type *;
        using reference = value_type &;
        using difference_type = std::ptrdiff_t;

        const T &operator*() {
            return ptr_->value_;
        }

        const T *operator->() {
            return &(ptr_->value_);
        }

        ConstIterator &operator++() {
            if (ptr_->right_ != nullptr) {
                // go down to the left_ branch of right_ child
                ptr_ = ptr_->right_;
                while (ptr_->left_ != nullptr) {
                    ptr_ = ptr_->left_;
                }
            } else if (ptr_->parent_ != nullptr) {
                // go up to the left_ while possible
                while (ptr_->parent_ != nullptr && ptr_->parent_->left != ptr_) {
                    ptr_ = ptr_->parent_;
                }
                ptr_ = ptr_->parent_;
            } else {
                ptr_ = nullptr;
            }
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator new_it(ptr_, tree);
            if (ptr_->right_ != nullptr) {
                // go down to the left_ branch of right_ child
                ptr_ = ptr_->right_;
                while (ptr_->left_ != nullptr) {
                    ptr_ = ptr_->left_;
                }
            } else if (ptr_->parent_ != nullptr) {
                // go up to the left_ while possible
                while (ptr_->parent_ != nullptr && ptr_->parent_->left != ptr_) {
                    ptr_ = ptr_->parent_;
                }
                ptr_ = ptr_->parent_;
            } else {
                ptr_ = nullptr;
            }
            return new_it;
        }

        ConstIterator &operator--() {
            // if end()
            if (ptr_ == nullptr) { return *this; }
            if (ptr_->left_ != nullptr) {
                // go down to the right_ branch of left_ child
                ptr_ = ptr_->left_;
                while (ptr_->right_ != nullptr) {
                    ptr_ = ptr_->right_;
                }
            } else if (ptr_->parent_ != nullptr) {
                // go up to the right_ while possible
                while (ptr_->parent_ != nullptr && ptr_->parent_->right != ptr_) {
                    ptr_ = ptr_->parent_;
                }
                ptr_ = ptr_->parent_;
            } else {
                ptr_ = nullptr;
            }
            return *this;
        }

        ConstIterator operator--(int) {
            ConstIterator new_it(ptr_, tree);
            // if end()
            if (ptr_ == nullptr) {
                ptr_ = tree->head_;
                // search for max element
                while (ptr_->right_ != nullptr) {
                    ptr_ = ptr_->right_;
                }
                return new_it;
            }
            if (ptr_->left_ != nullptr) {
                // go down to the left_ branch of right_ child
                ptr_ = ptr_->left_;
                while (ptr_->right_ != nullptr) {
                    ptr_ = ptr_->right_;
                }
            } else if (ptr_->parent_ != nullptr) {
                // go up to the left_ while possible
                while (ptr_->parent_ != nullptr && ptr_->parent_->right != ptr_) {
                    ptr_ = ptr_->parent_;
                }
                ptr_ = ptr_->parent_;
            } else {
                ptr_ = nullptr;
            }
            return new_it;
        }

        bool operator==(const ConstIterator &rhs) const {
            return (ptr_ == rhs.ptr_);
        }

        bool operator!=(const ConstIterator &rhs) const {
            return (ptr_ != rhs.ptr_);
        }

    private:
        explicit ConstIterator(TreeNode *node, BinarySearchTree *tree) : ptr_
                                                                                 (node),
                                                                         tree(
                                                                                 tree) {}

        BinarySearchTree::TreeNode *ptr_{nullptr};
        BinarySearchTree *tree;
    };

    BinarySearchTree() = default;

    BinarySearchTree(std::initializer_list<T> elements);

    BinarySearchTree(const BinarySearchTree &other_tree);

    BinarySearchTree(BinarySearchTree &&other_tree) noexcept;

    BinarySearchTree &operator=(const BinarySearchTree &other_tree);

    BinarySearchTree &operator=(BinarySearchTree &&other_tree) noexcept;

    ConstIterator begin() const;

    ConstIterator end() const;

    void erase(ConstIterator iter);

    ConstIterator find(const T &value);

    ~BinarySearchTree();

    int size() const;

    bool empty() const;

    bool contains(const T &value) const;

    int count(const T &value) const;

    template<typename T1>
    void insert(T1 &&value);

    template<typename... Args>
    void emplace(Args &&... args);

    void erase(const T &value);

    std::vector<T> to_vector() const;

    bool operator==(const BinarySearchTree &rhs) const;

    bool operator!=(const BinarySearchTree &rhs) const;


private:
    struct TreeNode {
    public:
        TreeNode() = default;

        template<typename T1>
        explicit TreeNode(T1 &&value) : value_(std::forward<T1>(value)), left_
                (nullptr), right_(nullptr), parent_(nullptr) {}

        template<typename... Args>
        explicit TreeNode(Args &&... args) : value_(std::forward<Args>(args)...),
                                             left_(nullptr), right_(nullptr),
                                             parent_(nullptr) {}


        bool IsLeaf() const;

        bool IsHead() const;

        bool IsNotFull() const;

        void ResetPointers();

        friend class BinarySearchTree;

    private:
        T value_;
        TreeNode *left_{nullptr};  // less
        TreeNode *right_{nullptr};  // greater or equal
        TreeNode *parent_{nullptr};
    };

    void Swap(BinarySearchTree *first, BinarySearchTree *second);

    TreeNode *Find(const T &value) const;

    void Detach(TreeNode *node);

    void DeleteSubtree(TreeNode *node);

    // cope subtree with head = node to the subtree with head = other_node
    // assuming that |other_node| != nullptr
    static void CopySubtree(TreeNode *node, TreeNode *other_node);

    TreeNode *head_{nullptr};
    int size_{0};
};

template<typename T>
typename BinarySearchTree<T>::ConstIterator BinarySearchTree<T>::begin() const {
    if (size_ == 0) {
        return end();
    }
    TreeNode *min_element = head_;
    // search for min element
    while (min_element->left_ != nullptr) {
        min_element = min_element->left_;
    }
    ConstIterator begin_iter(min_element,
                             const_cast<BinarySearchTree<T> *>(this));
    return begin_iter;
}

template<typename T>
typename BinarySearchTree<T>::ConstIterator BinarySearchTree<T>::end() const {
    return ConstIterator(nullptr, const_cast<BinarySearchTree<T> *>(this));
}

template<typename T>
void BinarySearchTree<T>::erase(ConstIterator iter) {
    Detach(iter.ptr_);
    delete iter.ptr_;
}

template<typename T>
typename BinarySearchTree<T>::ConstIterator BinarySearchTree<T>::find(const T
                                                                      &value) {
    TreeNode *node = Find(value);
    if (node == nullptr) {
        return end();
    }
    ConstIterator iter(node, this);
    return iter;
}

template<typename T>
BinarySearchTree<T>::BinarySearchTree(std::initializer_list<T> elements) {
    for (auto element: elements) {
        insert(element);
    }
}


template<typename T>
BinarySearchTree<T>::BinarySearchTree(BinarySearchTree &&other_tree) noexcept {
    Swap(this, &other_tree);
}

template<typename T>
BinarySearchTree<T> &BinarySearchTree<T>::operator=(BinarySearchTree
                                                    &&other_tree) noexcept {
    if (*this == other_tree) {
        return *this;
    }
    Swap(this, &other_tree);
    return *this;
}

template<typename T>
void BinarySearchTree<T>::CopySubtree(TreeNode *node, TreeNode
*other_node) {
    if (node->left_ != nullptr) {
        if (other_node->left_ == nullptr) {
            other_node->left_ = new TreeNode(node->left_->value_);
            other_node->left_->parent_ = nullptr;
        }
        other_node->left_->parent_ = other_node;
        CopySubtree(node->left_, other_node->left_);
    }
    if (node->right_ != nullptr) {
        if (other_node->right_ == nullptr) {
            other_node->right_ = new TreeNode(node->right_->value_);
            other_node->right_->parent_ = nullptr;
        }
        other_node->right_->parent_ = other_node;
        CopySubtree(node->right_, other_node->right_);
    }
}

template<typename T>
BinarySearchTree<T>::BinarySearchTree(const BinarySearchTree &other_tree) {
    if (other_tree.size_ == 0) {
        return;
    }
    DeleteSubtree(head_);
    head_ = new TreeNode(other_tree.head_->value_);
    CopySubtree(other_tree.head_, head_);
    size_ = other_tree.size_;
}

template<typename T>
BinarySearchTree<T> &
BinarySearchTree<T>::operator=(const BinarySearchTree &other_tree) {
    if (*this == other_tree) {
        return *this;
    }
    if (other_tree.size_ == 0) {
        return *this;
    }
    DeleteSubtree(head_);
    head_ = new TreeNode(other_tree.head_->value_);
    head_->parent_ = nullptr;
    CopySubtree(other_tree.head_, head_);
    size_ = other_tree.size_;
    return *this;
}

template<typename T>
BinarySearchTree<T>::~BinarySearchTree() {
    DeleteSubtree(head_);
}

template<typename T>
int BinarySearchTree<T>::size() const {
    return size_;
}

template<typename T>
bool BinarySearchTree<T>::empty() const {
    return (size_ == 0);
}

template<typename T>
bool BinarySearchTree<T>::contains(const T &value) const {
    return (Find(value) != nullptr);
}

template<typename T>
template<typename T1>
void BinarySearchTree<T>::insert(T1 &&value) {
    TreeNode *node = new TreeNode(std::forward<T1>(value));
    if (size_ == 0) {
        size_ = 1;
        head_ = node;
        return;
    }
    TreeNode *parent_node = head_;
    while (parent_node != nullptr) {
        if (value < parent_node->value_ && parent_node->left_ != nullptr) {
            parent_node = parent_node->left_;
        } else if (!(value < parent_node->value_)
                   && parent_node->right_ != nullptr) {
            parent_node = parent_node->right_;
        } else {
            break;
        }
    }
    if (value < parent_node->value_) {
        parent_node->left_ = node;
    } else {
        parent_node->right_ = node;
    }
    node->parent_ = parent_node;
    size_++;
}

template<typename T>
template<typename... Args>
void BinarySearchTree<T>::emplace(Args &&... args) {
    auto *node = new TreeNode(std::forward<Args>(args)...);
    if (size_ == 0) {
        size_ = 1;
        head_ = node;
        return;
    }
    TreeNode *parent_node = head_;
    while (parent_node != nullptr) {
        if (node->value_ < parent_node->value_ && parent_node->left_ != nullptr) {
            parent_node = parent_node->left_;
        } else if (!(node->value_ < parent_node->value_) &&
                   parent_node->right_ != nullptr) {
            parent_node = parent_node->right_;
        } else {
            break;
        }
    }
    if (node->value_ < parent_node->value_) {
        parent_node->left_ = node;
    } else {
        parent_node->right_ = node;
    }
    node->parent_ = parent_node;
    size_++;
}

template<typename T>
int BinarySearchTree<T>::count(const T &value) const {
    int result{0};
    for (TreeNode *node = head_; node != nullptr;) {
        if (node->value_ == value) {
            result++;
            node = node->right_;
        } else {
            if (node->value_ < value) {
                node = node->right_;
            } else {
                node = node->left_;
            }
        }
    }
    return result;
}

template<typename T>
void BinarySearchTree<T>::erase(const T &value) {
    if (Find(value) == nullptr) {
        return;
    }
    TreeNode *node = Find(value);
    Detach(node);
    delete node;
}

template<typename T>
void BinarySearchTree<T>::Swap(BinarySearchTree *first, BinarySearchTree
*second) {
    std::swap(first->size_, second->size_);
    std::swap(first->head_, second->head_);
}

template<typename T>
typename BinarySearchTree<T>::TreeNode *
BinarySearchTree<T>::Find(const T &value) const {
    for (TreeNode *node = head_; node != nullptr;) {
        if (node->value_ == value) {
            return node;
        } else {
            if (node->value_ < value) {
                node = node->right_;
            } else {
                node = node->left_;
            }
        }
    }
    return nullptr;
}

template<typename T>
void BinarySearchTree<T>::Detach(TreeNode *node) {
    if (node->IsLeaf()) {
        if (node->IsHead()) {
            head_ = nullptr;
        } else {
            if (node->parent_->left_ == node) {
                node->parent_->left_ = nullptr;
            } else {
                node->parent_->right_ = nullptr;
            }
            node->parent_ = nullptr;
        }
        size_--;
    } else if (node->IsNotFull()) {
        TreeNode *child = (node->left_ == nullptr) ? node->right_ : node->left_;
        if (node->IsHead()) {
            head_ = child;
            head_->parent_ = nullptr;
            node->ResetPointers();
        } else {
            if (node->parent_->left_ == node) {
                node->parent_->left_ = child;
                child->parent_ = node->parent_;
            } else {
                node->parent_->right_ = child;
                child->parent_ = node->parent_;
            }
            node->ResetPointers();
        }
        size_--;
    } else {
        TreeNode *new_node = node->right_;
        while (new_node->left_ != nullptr) {
            new_node = new_node->left_;
        }
        Detach(new_node);
        if (node->IsHead()) {
            head_ = new_node;
        } else {
            if (node->parent_->right_ == node) {
                node->parent_->right_ = new_node;
            } else {
                node->parent_->left_ = new_node;
            }
        }
        if (node->left_ != nullptr) {
            node->left_->parent_ = new_node;
        }
        if (node->right_ != nullptr) {
            node->right_->parent_ = new_node;
        }
        new_node->left_ = node->left_;
        new_node->right_ = node->right_;
        if (node->IsHead()) {
            new_node->parent_ = nullptr;
        } else {
            new_node->parent_ = node->parent_;
        }
        node->ResetPointers();
    }
}

template<typename T>
bool BinarySearchTree<T>::TreeNode::IsLeaf() const {
    return (left_ == nullptr && right_ == nullptr);
}

template<typename T>
bool BinarySearchTree<T>::TreeNode::IsHead() const {
    return (parent_ == nullptr);
}

template<typename T>
bool BinarySearchTree<T>::TreeNode::IsNotFull() const {
    return (left_ == nullptr || right_ == nullptr);
}

template<typename T>
void BinarySearchTree<T>::TreeNode::ResetPointers() {
    parent_ = nullptr;
    left_ = nullptr;
    right_ = nullptr;
}

template<typename T>
std::vector<T> BinarySearchTree<T>::to_vector() const {
    std::vector<T> result;
    for (ConstIterator it = begin(); it != end(); it++) {
        result.push_back(*it);
    }
    return result;
}

template<typename T>
bool BinarySearchTree<T>::operator==(const BinarySearchTree &rhs) const {
    if (size_ != rhs.size_) {
        return false;
    }
    ConstIterator rhs_it = rhs.begin();
    for (ConstIterator it = begin(); it != end(); it++) {
        if (!(*it == *rhs_it)) {
            return false;
        }
        rhs_it++;
    }
    return true;
}

template<typename T>
bool BinarySearchTree<T>::operator!=(const BinarySearchTree &rhs) const {
    return !(*this == rhs);
}

template<typename T>
void BinarySearchTree<T>::DeleteSubtree(TreeNode *node) {
    if (node == nullptr) {
        return;
    }
    if (node->left_ != nullptr) {
        DeleteSubtree(node->left_);
    }
    if (node->right_ != nullptr) {
        DeleteSubtree(node->right_);
    }
    Detach(node);
    delete node;
}

#endif  // BINARY_SEARCH_TREE_H_
