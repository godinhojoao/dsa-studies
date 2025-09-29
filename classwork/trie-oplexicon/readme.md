## Trie

- Read oplexicon file and save everything into the trie **OK**

- Accepts words with the same prefix, e.g., joao, jooao **OK**

- Map not allowed symbols e.g, ã -> a, õ->o... **OK**

- Polarity lookup: The user should be able to input a word and the program should return its associated polarity. If the word does not exist, the program should inform the user. **OK**

- Polarity editing: The user should be able to search for a word and change its polarity. This change should be reflected in the trie structure in memory. **OK**

- File saving: The program should be able to save the updated version of the lexicon, with the polarity modifications, into a new file. **OK**

- Menu with options (search word, add word, edit word, save trie on new file and exit) **OK**

- Free memory on exit (test with valgrind) **OK**

- The time complexity of each operation (insertion, search, and edit) must be considered and justified in the project documentation. **OK**

## Time complexity of each operation:

- Search on trie: **O(n) where n is the word length**
- Inserting new word: **O(n) where n is the word length**
- Updating word: **O(n) where n is the word length**
- Saving entire trie on file:
  - as I have to traverse the entire trie **O(n), n=total nodes**

## I've searched about:

- Using **(int)val** to find char on ascii table
- Using **#define** because I forgot
- Reading and writing files using FILE \*fileptr, fopen, fclose, etc...
  - fgets to read file
  - sscanf to use a regex and get data correctly from file
- Mapping ã->a, õ->o, and more symbols.

# Run with valgrind: (Dr memory and valgrind are incompatible with my computer)

1 - install docker on oficial website
2 - Run on terminal: `docker build -t valgrind-runner . && docker run valgrind-runner && docker run --rm valgrind-runner`
