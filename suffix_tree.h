#ifndef H_SUFFIXTREE
#define H_SUFFIXTREE

#include <string>
using namespace std;

#define INT2CHAR(i) i + '0'
#define CHAR2INT(c) c - '0'

/*
 * Node of a suffix tree.
*/
class SuffixNode {
    public:
    // Number of letters.
    int letters;
    bool is_terminal;
    // Outgoing edges that represent continuations of the current suffix.
    SuffixNode **edges;

    SuffixNode();
    SuffixNode(int letters);
    ~SuffixNode();
};

/*
 * A suffix tree that can be used to check if a given word has a suffix that is
 * among the words that make up the tree.
 */
class SuffixTree {
    private:
    int letters;
    SuffixNode *root;

    public:
    SuffixTree();
    SuffixTree(int letters);
    void add(const string& w);
    bool has_suffix(const string& w);
};

#endif
