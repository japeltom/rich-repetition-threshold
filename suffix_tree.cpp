#include "suffix_tree.h"

SuffixNode::SuffixNode() {
}

SuffixNode::SuffixNode(int letters) {
    this->letters = letters;
    this->is_terminal = false;
    this->edges = (SuffixNode**) malloc(this->letters*sizeof(SuffixNode*));
    for (int i = 0; i < letters; i++) {
        this->edges[i] = nullptr;
    }
}

SuffixNode::~SuffixNode() {
    free(this->edges);
}

SuffixTree::SuffixTree() {
}

SuffixTree::SuffixTree(int letters) {
    this->letters = letters;
    this->root = new SuffixNode(letters);
}

void SuffixTree::add(const string& w) {
    SuffixNode *current = this->root;
    for (int i = w.length() - 1; i >= 0; i--) {
        if (current->edges[CHAR2INT(w[i])] != nullptr) {
            current = current->edges[CHAR2INT(w[i])];
        }
        else {
            SuffixNode *new_node = new SuffixNode(this->letters);
            current->edges[CHAR2INT(w[i])] = new_node;
            current = new_node;
        }
    }
    current->is_terminal = true;
}

bool SuffixTree::has_suffix(const string& w) {
    SuffixNode *current = this->root;
    for (int i = w.length() - 1; i >= 0; i--) {
        if (current->is_terminal) {
            return true;
        }
        if (current->edges[CHAR2INT(w[i])] == nullptr) {
            return false;
        }
        else {
            current = current->edges[CHAR2INT(w[i])];
        }
    }
    return current->is_terminal;
}

