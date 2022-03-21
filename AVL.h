//
// Created by warri on 05.02.2022.
//

#ifndef AVL_AVL_H
#define AVL_AVL_H


#include <set>
#include <cstddef>
#include <assert.h>


template<class T = int>
class Set {

private:
    struct node {
        T key;
        T max;
        node *left, *right;
        node *par;
        int height;
        node(T new_key) : key(new_key), max(new_key), left(nullptr), right(nullptr), par(nullptr), height(1) {}
    };


public:
    Set() = default;


    void insert(const T &elem) {
        root = insert(root, elem);
    }

    template<class Iterator>
    Set(Iterator first, Iterator last) {
        for (Iterator it = first; it != last; ++it) {
            insert(*it);
        }
    }

    Set(std::initializer_list<T> elems) {
        for (auto i : elems) {
            insert(i);
        }
    }
    Set(const Set& s) {
        root = deep_copy(s.root);
        _size = s._size;
    }
    Set& operator=(const Set& rhs) {
        if (this == &rhs) {
            return *this;
        }
        clear_all(root);
        root = deep_copy(rhs.root);
        _size = rhs._size;
        return *this;
    }


    size_t size() const {
        return _size;
    }

    bool empty() const {
        return _size == 0;
    }

    void erase(const T &elem) {
        root = erase(root, elem);
    }
    void add(T x) {
        root = insert(root, x);
    }
    bool contains(const T& x) const {
        return find(root, x) != nullptr;
    }

    ~Set() {
        clear_all(root);
    }

private:

    node *root = nullptr;
    size_t _size = 0;


public:
    struct iterator {
    private:
        node *_begin = nullptr, *_end = nullptr;
        node *cur = nullptr;
    public:
        iterator() = default;

        iterator(node *v, node *b, node *e){
            cur = v;
            _begin = b;
            _end = e;
        }

        iterator operator++() {
            iterator it(cur, _begin, _end);

            if (cur == _end) {
                cur = nullptr;
                return iterator(nullptr, _begin, _end);
            }
            if (cur->right) {
                it.cur = findmin(cur->right);
            } else {
                it.cur = cur;
                while (it.cur->par != nullptr && it.cur->par->right == it.cur) {
                    it.cur = it.cur->par;
                }
                it.cur = it.cur->par;
            }
            cur = it.cur;
            return it;
        }

        iterator operator++(int) {
            iterator it(cur, _begin, _end);

            if (cur == _end) {
                cur = nullptr;
                return iterator(nullptr, _begin, _end);
            }
            if (cur->right) {
                it.cur = findmin(cur->right);
            } else {
                it.cur = cur;
                while (it.cur->par != nullptr && it.cur->par->right == it.cur) {
                    it.cur = it.cur->par;
                }
                it.cur = it.cur->par;
            }
            cur = it.cur;
            return it;
        }

        iterator operator--() {
            if (cur == nullptr) {
                cur = _end;
                return iterator(_end, _begin, _end);
            }
            iterator it(cur, _begin, _end);
            if (cur->left) {
                it.cur = findmax(cur->left);
            } else {
                it.cur = cur;
                while (it.cur->par && it.cur->par->left == it.cur) {
                    it.cur = it.cur->par;
                }
                it.cur = cur->par;
            }
            cur = it.cur;
            return it;
        }

        iterator operator--(int) {
            if (cur == nullptr) {
                cur = _end;
                return iterator(_end, _begin, _end);
            }
            iterator it(cur, _begin, _end);
            if (cur->left) {
                it.cur = findmax(cur->left);
            } else {
                it.cur = cur;
                while (it.cur->par && it.cur->par->left == it.cur) {
                    it.cur = it.cur->par;
                }
                it.cur = cur->par;
            }
            cur = it.cur;
            return it;
        }

        T operator*() const {
            if (cur == nullptr) {
                assert(false);
            }
            return cur->key;
        }

        T *operator->() const {
            return &cur->key;
        }

        bool operator==(const iterator &another) const {
            if (another._begin == _begin) {
                return another.cur == cur;
            }
            return false;
        }

        bool operator!=(iterator another) const {
            return !(this->operator==(another));
        }

    };

public:
    iterator begin() const {
        return iterator(findmin(root), findmin(root), findmax(root));
    }

    iterator end() const {
        return iterator(nullptr, findmin(root), findmax(root));
    }

    iterator find(const T &elem) const {
        return iterator(find(root, elem), findmin(root), findmax(root));
    }

    iterator lower_bound(const T &elem) const {
        return iterator(find_lower_bound(root, elem), findmin(root), findmax(root));
    }

private:

    int height(node *vertex) {
        if (vertex == nullptr) {
            return 0;
        }
        return vertex->height;
    }

    int bfactor(node *vertex) {
        return height(vertex->right) - height(vertex->left);
    }

    void fixheight(node *vertex) {
        int hl = height(vertex->left);
        int hr = height(vertex->right);
        vertex->height = (hl > hr ? hl : hr) + 1;
    }

    void upd(node *v) {
        if (v == nullptr) {
            return;
        }
        v->max = v->key;
        if (v->left != nullptr) {
            v->left->par = v;
            if (v->max < v->left->max) {
                v->max = v->left->max;
            }
        }
        if (v->right != nullptr) {
            v->right->par = v;
            if (v->max < v->right->max) {
                v->max = v->right->max;
            }
        }
        v->par = nullptr;
    }

    node *rotateright(node *v) {
        node *q = v->left;
        v->left = q->right;
        upd(v);
        q->right = v;
        upd(q);
        fixheight(v);
        fixheight(q);
        return q;
    }

    node *rotateleft(node *v) {
        node *p = v->right;
        v->right = p->left;
        upd(v);
        p->left = v;
        upd(p);
        fixheight(v);
        fixheight(p);
        return p;
    }

    node *balance(node *v) {
        fixheight(v);
        if (bfactor(v) == 2) {
            if (bfactor(v->right) < 0)
                v->right = rotateright(v->right);
            upd(v);
            return rotateleft(v);
        }
        if (bfactor(v) == -2) {
            if (bfactor(v->left) > 0) {
                v->left = rotateleft(v->left);
            }
            upd(v);
            return rotateright(v);
        }
        upd(v);
        return v;
    }

    node *insert(node *p, const T &k)
    {
        if (p == nullptr) {
            ++_size;
            return new node(k);
        }
        if (k < p->key) {
            p->left = insert(p->left, k);
        } else if (p->key < k) {
            p->right = insert(p->right, k);
        }
        upd(p);
        return balance(p);
    }


    static node *findmin(node *p) {
        if (p == nullptr) {
            return nullptr;
        }
        return p->left ? findmin(p->left) : p;
    }

    static node *findmax(node *p) {
        if (p == nullptr) {
            return nullptr;
        }
        return p->right ? findmax(p->right) : p;
    }

    node *removemin(node *p) {
        if (p->left == nullptr)
            return p->right;
        p->left = removemin(p->left);
        upd(p);
        return balance(p);
    }

    node *find(node *v, const T& elem) const {
        if (v == nullptr) {
            return nullptr;
        }
        if (Equal(v->key, elem)) {
            return v;
        }
        if (v->key < elem) {
            return find(v->right, elem);
        } else {
            return find(v->left, elem);
        }
    }
    node *erase(node *p, T k) {
        if (p == nullptr) {
            return p;
        }
        if (k < p->key) {
            p->left = erase(p->left, k);
            upd(p);
        } else if (p->key < k) {
            p->right = erase(p->right, k);
            upd(p);
        } else {
            --_size;
            node *q = p->left;
            upd(q);
            node *r = p->right;
            upd(r);
            delete p;
            if (!r) return q;
            node *min = findmin(r);
            min->right = removemin(r);
            upd(min->right);
            upd(min->left);
            min->left = q;
            return balance(min);
        }
        return balance(p);
    }

    bool Equal(const T &a, const T &b) const {
        return !(a < b) && !(b < a);
    }

    node *find_lower_bound(node *p, T k) const {
        if (p == nullptr) {
            return nullptr;
        }
        if (p->left != nullptr && (k < p->left->max || Equal(k, p->left->max))) {
            return find_lower_bound(p->left, k);
        }
        if ((k < p->key || Equal(k, p->key))) {
            return p;
        }
        return find_lower_bound(p->right, k);
    }


    node *deep_copy(node *v) {
        if (v == nullptr) {
            return nullptr;
        }
        node *u = new node(v->key);
        u->left = deep_copy(v->left);
        u->right = deep_copy(v->right);
        upd(u);
        return u;
    }

    void clear_all(node *v) {
        if (v == nullptr) {
            return;
        }
        clear_all(v->left);
        clear_all(v->right);
        delete v;
    }

};


#endif //AVL_AVL_H
