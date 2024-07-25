#include <iostream>
#include <string>
#include "eertree.h"

/*
 * Check if the given string has a suffix that is a repetition with exponent
 * top/bottom.
 */
bool suffix_repetition(string& w, int top, int bottom) {
    int n = w.length();
    int p = 1;
    while (n*bottom >= p*top) {
        int i = 0;
        while (w[n - 1 - i] == w[n - 1 - i - p]) {
            if ((i + 1 + p)*bottom >= p*top) {
                return true;
            }
            i++;
            if (i + p >= n) {
                break;
            }
        }
        p++;
    }
    return false;
}

/*
 * Check if the given string has a suffix that is a repetition with exponent
 * strictly greater than top/bottom.
 */
bool suffix_repetition_plus(string& w, int top, int bottom) {
    int n = w.length();
    int p = 1;
    while (n*bottom > p*top) {
        int i = 0;
        while (w[n - 1 - i] == w[n - 1 - i - p]) {
            if ((i + 1 + p)*bottom > p*top) {
                return true;
            }
            i++;
            if (i + p >= n) {
                break;
            }
        }
        p++;
    }
    return false;
}

string backtrack(bool (*suffix_condition)(string&, int, int), int max_length, int letters, int top, int bottom, string prefix = "") {
    char max_letter = INT2CHAR(letters - 1);
    int longest = prefix.length();
    EerTree eertree = EerTree(letters, prefix);
    char extension = '0';

    // Check that the given prefix is rich and does not satisfy the suffix
    // condition for all prefixes.
    if (not eertree.is_rich()) {
        cout << "The given prefix is not rich." << endl;
        exit(1);
    }
    for (int i = 1; i <= prefix.length(); i++) {
        string p = prefix.substr(0, i);
        if (suffix_condition(p, top, bottom)) {
            cout << "A prefix of the given prefix satisfies the suffix condition." << endl;
            exit(1);
        }
    }

    // This keeps track of the first appearance of each letter. This is used to
    // prune out isomorphic words.
    vector<int> letter_positions = vector<int>(letters, -1);
    char max_allowed_letter = '0';
    int filled = -1;
    for (int i = 0; i < prefix.length(); i++) {
        bool foo = prefix[i] > max_letter;
        if (prefix[i] > max_letter) {
            cout << "The given prefix has more than " << letters << " letters." << endl;
            exit(1);
        }
        if (letter_positions[CHAR2INT(prefix[i])] == -1) {
            if (CHAR2INT(prefix[i]) != filled + 1) {
                cout << "The first occurrences of letters in the prefix must be in integer order." << endl;
                exit(1);
            }
            letter_positions[CHAR2INT(prefix[i])] = i;
            max_allowed_letter++;
            filled++;
        }
    }

    while (true) {
        eertree.add(extension);

        // Update the letter occurrences if the new letter does not previously
        // appear.
        if (letter_positions[CHAR2INT(extension)] == -1) {
            letter_positions[CHAR2INT(extension)] = eertree.word.length() - 1;
            max_allowed_letter++;
        }

        if (eertree.is_rich() and not suffix_condition(eertree.word, top, bottom)) {
          // Adding the letter was successful.
          if (eertree.word.length() > longest) {
              longest = eertree.word.length();
              cout << longest << " " << eertree.word << endl;
          }
          if (eertree.word.length() == max_length) {
              return eertree.word;
          }
          extension = '0';
        }
        else {
          // Adding the letter was unsuccessful. Backtrack.
          bool allowed_to_increment = false;
          char a;
          while (!allowed_to_increment && eertree.word.length() - prefix.length() > 0) {
              a = eertree.word.back();
              eertree.pop();

              // Check if we happened to remove a letter completely.
              if (eertree.word.length() == letter_positions[CHAR2INT(max_allowed_letter - 1)]) {
                  letter_positions[CHAR2INT(max_allowed_letter - 1)] = -1;
                  max_allowed_letter--;
              }

              allowed_to_increment = a < min(max_letter, max_allowed_letter);
          }

          if (eertree.word.length() - prefix.length() <= 0 && a == min(max_letter, max_allowed_letter)) {
              // We backtracked to the beginning and must terminate.
              break;
          }
          else {
              // Increment the letter a to be the new extending letter.
              extension = a + 1;
          }
        }
    }
    return string("");
}

void help(string program) {
    cout << "Usage: " << program << " max_length n_letters top bottom" << endl;
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        help(string(argv[0]));
    }
    auto max_length = atoi(argv[1]);
    auto letters = atoi(argv[2]);

    int top;
    int bottom;
    bool power_plus = false;
    string s_top = string(argv[3]);
    string s_bottom = string(argv[4]);
    if (s_top[s_top.length() - 1] == '+') {
        top = stoi(s_top.substr(0, s_top.length() - 1));
        power_plus = true;
    }
    else {
        top = stoi(s_top);
    }
    if (s_bottom[s_bottom.length() - 1] == '+') {
        bottom = stoi(s_bottom.substr(0, s_bottom.length() - 1));
        power_plus = true;
    }
    else {
        bottom = stoi(s_bottom);
    }

    string prefix;
    if (argc > 5) {
        prefix = string(argv[5]);
    }
    else {
        prefix = string("");
    }

    bool (*suffix_condition)(string&, int, int);
    if (power_plus) {
        suffix_condition = suffix_repetition_plus;
    }
    else {
        suffix_condition = suffix_repetition;
    }

    string found = backtrack(suffix_condition, max_length, letters, top, bottom, prefix);

    if (found.length() == 0) {
        cout << "The pattern is unavoidable!" << endl;
    }
    else {
        cout << "Found a word of length " << found.length() << endl;
    }

    return 0;
}

