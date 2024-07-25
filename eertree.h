#ifndef H_EERTREE
#define H_EERTREE

#include <cassert>
#include <string>
#include <vector>
using namespace std;

#define INT2CHAR(i) i + '0'
#define CHAR2INT(c) c - '0'

/*
 * Node in the Eertree that represents a palindrome.
*/
class Node {
    public:
    // Number of letters.
    int letters;
    // Length of the palindrome.
    int length;
    // Length of the prefix that had this palindrome as a suffix for the first
    // time.
    int prefix_length;
    // The index of the Node that has an edge to this Node.
    int parent;
    // Outgoing edges that represent palindromes that have the current
    // palindrome as a center.
    int* edges;
    // Suffix link that holds the index of the Node that corresponds to the LPS
    // of the current palindrome. Index 1 is the empty palindrome.
    int suffix_link;
    // The quick link that holds the index of the Node that corresponds to the
    // LPS of the current palindrome that is preceded by a letter different
    // from the letter that precedes the LPS of the current palindrome.
    int quick_link;

    Node();
    Node(int letters, int length, int prefix_length, int parent, int suffix_link, int quick_link);
    Node(const Node& N);
    ~Node();
};

class EerTree {

    private:
    int letters;
    vector<Node> tree;
    // List of indices to Nodes that indicate the longest palindromic suffix of
    // each prefix of the word. This is used to implement the pop method.
    vector<int> previous_lps;
    // Index to the Node that corresponds to the current longest palindromic
    // suffix.
    int max_pal_suf;

    public:
    string word;

    EerTree();
    EerTree(int letters, string word);
    int longest_pal_suffix_preceding(int node_idx, char a);
    void add(char a);
    void pop();
    bool is_rich();
};

#endif
