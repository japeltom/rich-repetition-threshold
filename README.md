# rich-repetition-threshold
A backtracking algorithm for studying rich words that avoid specified powers

## Overview
This algorithms implements a backtracking search which searches for the lexicographically least rich word of length $L$ avoiding $r$-powers (or $r+$-powers) over an alphabet $\{0, 1, \ldots, k-1\}$ for a given rational number $r$ expressed as a ratio $p/q$ (if it exists). Testing for richness is implemented efficiently using the Eertree data structure with quick links [[1]](#1).

## Compilation

    g++ -O3 backtrack.cpp eertree.cpp -o backtrack

## References
<a id="1">[1]</a> 
M. Rubinchik, A. M. Shur. Eertree: An efficient data structure for processing palindromes in strings (2015) [arXiv:1506.04862](http://arxiv.org/abs/1506.04862).

