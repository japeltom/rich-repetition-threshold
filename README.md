# rich-repetition-threshold
A backtracking algorithm for studying rich words that avoid specified powers

## Overview
This algorithms implements a backtracking search which searches for the lexicographically least rich word of length $L$ avoiding $r$-powers (or $r+$-powers) over an alphabet $\\{0, 1, \ldots, k-1\\}$ for a given rational number $r$ expressed as a ratio $p/q$ (if it exists). Testing for richness is implemented efficiently using the Eertree data structure with quick links [[1]](#1).

## Compilation

    g++ -O3 backtrack.cpp eertree.cpp suffix_tree.cpp -o backtrack

## Usage

    ./backtrack L k p q prefix forbidden_factors

For example,

    ./backtrack 1000 3 2 1

searches for a rich word of length $1000$ over a ternary alphabet that avoids squares. The run of the program shows that such a word does not exist. The run shows that the longest and lexicographically least ternary word avoiding squares is $0102010$. Running

    ./backtrack 1000 3 2+ 1

attempts to find a $3$-letter rich word of length $1000$ that avoids $2+$-powers. The longest found word is of length $78$. The parameter prefix specifies that only words with this given prefix are searched for. A list of comma-seperated forbidden factors can also be given. For example

    ./backtrack 1000 3 7 3 012 00,22

searches for ternary words that are rich, avoid 7/3 powers, begin with 012, and do not contain the factor 00 or 22. The longest such word has length 18.

## References
<a id="1">[1]</a> 
M. Rubinchik, A. M. Shur. Eertree: An efficient data structure for processing palindromes in strings (2015) [arXiv:1506.04862](http://arxiv.org/abs/1506.04862).

