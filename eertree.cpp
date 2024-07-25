#include "eertree.h"

Node::Node() {
}

Node::Node(int letters, int length, int prefix_length, int parent, int suffix_link, int quick_link) {
    this->letters = letters;
    this->length = length;
    this->prefix_length = prefix_length;
    this->parent = parent;
    this->suffix_link = suffix_link;
    this->quick_link = quick_link;
    this->edges = (int*) malloc(this->letters*sizeof(int));
    for (int i = 0; i < this->letters; i++) {
        this->edges[i] = -1;
    }
}

Node::Node(const Node& N) {
    this->letters = N.letters;
    this->length = N.length;
    this->prefix_length = N.prefix_length;
    this->parent = N.parent;
    this->suffix_link = N.suffix_link;
    this->quick_link = N.quick_link;
    this->edges = (int*) malloc(this->letters*sizeof(int));
    for (int i = 0; i < this->letters; i++) {
        this->edges[i] = N.edges[i];
    }
}

Node::~Node() {
    free(this->edges);
}

EerTree::EerTree() {
}

EerTree::EerTree(int letters, string word) {
    this->letters = letters;
    // Setup the tree with the two initial special nodes.
    this->tree = {Node(letters, -1, -1, -1, 0, 0), Node(letters, 0, 0, 0, 0, 0)};
    this->max_pal_suf = 1;

    for (char a : word) {
        this->add(a);
    }
}

/*
 * Finds the longest proper palindromic suffix preceded by the letter a of the
 * palindrome corresponding to the given node index. The return value is a node
 * index, and equals 0 (special node -1) if not found.
*/
int EerTree::longest_pal_suffix_preceding(int node_idx, char a) {
    int current_idx = node_idx;
    int prev_idx = current_idx;
    // Notice that 0 is the index of the special node -1.
    while (current_idx != 0) {
        int b = this->word.length() - 1 - this->tree[current_idx].length;
        if (b >= 0 && this->word[b] == a) {
            break;
        }
        prev_idx = current_idx;
        current_idx = this->tree[current_idx].suffix_link;
        if (current_idx == 0) break;
        b = this->word.length() - 1 - this->tree[current_idx].length;
        if (b >= 0 && this->word[b] == a) {
            break;
        }
        current_idx = this->tree[prev_idx].quick_link;
    }
    return current_idx;
}

void EerTree::add(char a) {
    // Let the current string be T.
    int current_pal_suffix = this->max_pal_suf;

    // Search for the longest palindrome suffix that is preceded by the letter
    // a or get the index of the special node -1 if it does not exist.
    int pal_suffix_idx = this->longest_pal_suffix_preceding(this->max_pal_suf, a);
    Node& pal_suffix = this->tree[pal_suffix_idx];

    // Check if the found node has an outgoing edge with the letter a.
    if (pal_suffix.edges[CHAR2INT(a)] != -1) {
        // Found the edge, so no new palindrome was created. Nothing needs to
        // be done except to add the letter a to the read word and to update
        // the longest palindromic suffix.
        this->previous_lps.push_back(this->max_pal_suf);
        this->max_pal_suf = pal_suffix.edges[CHAR2INT(a)];
        this->word.push_back(a);
        return;
    }

    // The current palindromic suffix surrounded by the new letter a forms a
    // novel palindromic suffix or we have a new letter. We need to create a
    // new Node for the palindrome and add it to the tree.
    int new_pal_length = pal_suffix.length + 2;
    // The default values correspond to the case where we are adding a new
    // letter, and the suffix link will point to the special node 0 (with index
    // 1) and the edges will be added to the special node -1 (with index 0).
    int suffix_link_target_idx = 1;
    int edge_add_idx = 0;
    if (new_pal_length > 1) {
        // The new palindrome is not a letter, and we need to search for its
        // longest palindromic proper suffix that is preceded by the letter a.
        //
        // This is done by taking the suffix link and searching for a
        // palindrome that is preceded by a by continuing the traversing until
        // the special node 0 is encountered. Then the edge corresponding to
        // the letter is traversed to find the suffix link target. This edge
        // always exists.
        int suffix_link_suffix_idx = this->longest_pal_suffix_preceding(pal_suffix.suffix_link, a);
        suffix_link_target_idx = this->tree[suffix_link_suffix_idx].edges[CHAR2INT(a)];
        edge_add_idx = pal_suffix_idx;
    }

    // Figure out the quick link.
    int quick_link_target_idx;
    // This is the position of the letter that precedes the LPS of the current
    // palindrome.
    int b = this->word.length() - this->tree[suffix_link_target_idx].length;
    if (b >= this->word.length()) {
        // We get here only when the new palindrome is a letter. We point the
        // quick link to the special node 0 (with index 1) for correct
        // behavior.
        quick_link_target_idx = 1;
    }
    else {
      // This is the position letter that precedes the LPS of the LPS of the
      // current palindrome.
      int c = this->word.length() - this->tree[this->tree[suffix_link_target_idx].suffix_link].length;
      if (c >= this->word.length()) {
          // We get here only when the LPS is a letter. We point the quick link
          // to the special node 0 (with index 1) for correct behavior.
          quick_link_target_idx = 1;
      }
      else {
          if (this->word[b] == this->word[c]) {
              quick_link_target_idx = this->tree[suffix_link_target_idx].quick_link;
          }
          else {
              quick_link_target_idx = this->tree[suffix_link_target_idx].suffix_link;
          }
      }
    }

    // Add a new Node to the tree, update edges, longest palindromic suffix,
    // and the word read so far.
    this->tree.push_back(Node(this->letters, new_pal_length, this->word.length() + 1, edge_add_idx, suffix_link_target_idx, quick_link_target_idx));
    this->tree[edge_add_idx].edges[CHAR2INT(a)] = this->tree.size() - 1;
    this->previous_lps.push_back(this->max_pal_suf);
    this->max_pal_suf = this->tree.size() - 1;
    this->word.push_back(a);
}

/*
 * Remove the last letter of the current word and restore the state of EerTree
 * to that situation.
 */
void EerTree::pop() {
    if (this->word.length() == 0) return;

    // Check if the previous step added a new palindrome or not.
    if (this->tree[this->max_pal_suf].prefix_length == this->word.length()) {
        // A new palindrome was added.
        //
        // We need to find its parent and remove the edge to the current Node.
        int parent = this->tree[this->max_pal_suf].parent;
        this->tree[parent].edges[CHAR2INT(this->word.back())] = -1;
        // Remove the Node from the tree.
        this->tree.pop_back();
    }

    // Remove last letter of the read word.
    this->word.pop_back();
    // Restore previous longest palindromic suffix.
    this->max_pal_suf = this->previous_lps.back();
    this->previous_lps.pop_back();
}

bool EerTree::is_rich() {
    return this->tree.size() - 2 == this->word.length();
}

