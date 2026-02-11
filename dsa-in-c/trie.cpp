// clang trie.cpp -o trie -lstdc++ && ./trie && rm -rf ./trie
#include <stdlib.h>
#include <iostream>

namespace trie {
constexpr int ACCEPTED_SYMBOLS_AMOUNT = 40;  // 26 letters, 10 digits, 4 symbols (- space . ')

struct TrieNode {
  struct TrieNode* next[ACCEPTED_SYMBOLS_AMOUNT];
  std::string word;
  char val;
  int isEndOfWord;
} typedef TrieNode;

struct SearchResult {
  std::string label;
};

TrieNode* init();
void insertWord(TrieNode* trieRoot, std::string word);
void search(TrieNode* node, std::string word, SearchResult results[], int* resultsCount);
void destroy(TrieNode* root);
}  // namespace trie

namespace trie {
namespace detail {
TrieNode* createNode(char nodeChar) {
  TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
  node->isEndOfWord = 0;
  node->val = nodeChar;
  for(int i = 0; i < ACCEPTED_SYMBOLS_AMOUNT; i++) {
    node->next[i] = NULL;
  }
  return node;
}

int getTrieCharIndex(char val) {
  if(val >= 'a' && val <= 'z')
    return val - 'a';  // 0 to 25

  if(val >= '0' && val <= '9')
    return 26 + (val - '0');  // 26 to 35

  switch(val) {
    case '-':
      return 36;
    case ' ':
      return 37;
    case '.':
      return 38;
    case '\'':
      return 39;
    default:
      return -1;  // unsupported character
  }
}

void findAllPossibleEnds(trie::TrieNode* currNode, SearchResult results[], int* resultsCount) {
  if(currNode == NULL) {
    return;
  }

  if(currNode->isEndOfWord) {
    results[*resultsCount].label = currNode->word;
    (*resultsCount)++;
  }

  for(int i = 0; i < trie::ACCEPTED_SYMBOLS_AMOUNT; i++) {
    findAllPossibleEnds(currNode->next[i], results, resultsCount);
  }
}
}  // namespace detail

TrieNode* init() {
  return detail::createNode('\0');
}

void insertWord(TrieNode* node, std::string word) {
  TrieNode* currNode = node;

  for(int i = 0; i < word.length(); i++) {
    char currCharToInsert = word[i];
    int trieCharIndex = detail::getTrieCharIndex(currCharToInsert);
    if(trieCharIndex == -1) {
      std::cout << "skipping char: " << currCharToInsert << " since it's not supported\n";
      continue;
    }

    if(currNode->next[trieCharIndex] == NULL) {
      currNode->next[trieCharIndex] = detail::createNode(currCharToInsert);
    }
    currNode = currNode->next[trieCharIndex];
  }

  currNode->isEndOfWord = 1;
  currNode->word = word;
}

void search(TrieNode* node, std::string word, SearchResult results[], int* resultsCount) {
  std::cout << "searching for " + word + "\n";
  *resultsCount = 0;

  TrieNode* currNode = node;
  for(int currWordIndex = 0; currWordIndex < word.length(); currWordIndex++) {
    char currChar = word[currWordIndex];
    int currTrieCharIndex = detail::getTrieCharIndex(currChar);

    if(currTrieCharIndex == -1) {
      std::cout << "skipping unsupported char: " << currChar << "\n";
      continue;
    }

    if(currNode->next[currTrieCharIndex] == NULL) {
      return;  // no matches found
    }
    currNode = currNode->next[currTrieCharIndex];
  }

  // can be end of word (user typed full word)
  if(currNode->isEndOfWord) {
    *resultsCount = 1;
    results[0].label = currNode->word;
    return;
  } else {
    // can be prefix of 1 or more words - find all possible completions
    detail::findAllPossibleEnds(currNode, results, resultsCount);
  }
};

void destroy(TrieNode* currNode) {
  if(currNode == NULL) {
    return;
  }

  for(int i = 0; i < ACCEPTED_SYMBOLS_AMOUNT; i++) {
    if(currNode->next[i] != NULL) {
      // DFS because we can't lose references
      destroy(currNode->next[i]);
    }
  }
  free(currNode);
}
}  // namespace trie

int main() {
  trie::TrieNode* rootNode = trie::init();

  trie::insertWord(rootNode, "joao tester");
  trie::insertWord(rootNode, "joao testador");
  trie::insertWord(rootNode, "joao godinho");

  trie::SearchResult results[300];

  while(true) {
    std::cout << "enter word: ";
    std::string input;
    std::getline(std::cin, input);  // read full line including spaces

    int searchResultsCount = 0;
    trie::search(rootNode, input, results, &searchResultsCount);
    for(int i = 0; i < searchResultsCount; i++) {
      std::cout << " -> " << results[i].label << "\n";
    }
    std::cout << "\n";
  }

  return 0;
}
