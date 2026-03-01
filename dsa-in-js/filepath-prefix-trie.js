export class FilePathPrefixTrie {
  constructor(path, fullPath, files) {
    if (!path) { throw new Error('required path') }
    this.path = path;
    this.fullPath = fullPath;
    this.files = files ?? [];
    this.foldersMap = new Map();
  }

  static insertPath(root, filePath) {
    const splittedParts = filePath.split('/').filter(Boolean);// remove empty string or any falsy value
    let currNode = root;
    for (let i = 0; i < splittedParts.length; i++) {
      const pathPart = splittedParts[i];
      const isFile = pathPart.includes('.');
      const fullPath = currNode.fullPath ? `${currNode.fullPath}/${pathPart}` : pathPart;
      if (isFile) {
        currNode.files.push(pathPart);
        return;
      }

      let nextNode = currNode.foldersMap.get(pathPart);

      if (!nextNode) {
        nextNode = new FilePathPrefixTrie(pathPart, fullPath);
        currNode.foldersMap.set(pathPart, nextNode);
      }

      currNode = nextNode;
    }
  }

  // simillar to the "tree" of unix terminal but not too fancy
  static traverse({ currNode, cbToProcessPathString, currLevel }) {
    if (currNode == null) return;
    if (currNode.path !== '\0') {
      cbToProcessPathString({ path: currNode.path, fullPath: currNode.fullPath, currLevel: currLevel });
    }

    for (const file of currNode.files) {
      cbToProcessPathString({ path: file, fullPath: currNode.fullPath, currLevel: currLevel + 1 });
    }

    for (const folder of currNode.foldersMap.values()) {
      FilePathPrefixTrie.traverse({ currNode: folder, cbToProcessPathString, currLevel: currLevel + 1 });
    }
  }
}

// const filePaths = [
//   'users/joao/images/test.png',
//   'users/joao/images/abc.png',
//   'users/joao/testing',
//   'users/joao/files/mydoc.docx',
//   'users/joao/files/mypdf.pdf'
// ]

// const trieRoot = new FilePathPrefixTrie('\0', './'); // empty root node
// filePaths.forEach(filePath => FilePathPrefixTrie.insertPath(trieRoot, filePath))

// const printPath = ({ path, currLevel }) => {
//   let blankSpacesForLevel = '';
//   for (let i = 0; i < currLevel; i++) {
//     blankSpacesForLevel += ' ';
//   }
//   console.log(`${blankSpacesForLevel}|__${path}`);
// }
// FilePathPrefixTrie.traverse({ currNode: trieRoot, cbToProcessPathString: printPath, currLevel: 0 });

// ➜  dsa-in-js git:(main) ✗ node filepath-preffix-trie.js
// |__users
//   |__joao
//    |__images
//     |__test.png
//     |__abc.png
//    |__testing
//    |__files
//     |__mydoc.docx
//     |__mypdf.pdf
