import { FilePathPrefixTree } from './dsa-in-js/filepath-prefix-trie.js'
import { spawn } from 'child_process';
import fs from 'fs';

// getting only current tracked files - after git add
const cmd = "git ls-files | grep -E '\\.md$|\\.c$|\\.cpp$|\\.js$' | sort -u";
const gitLog = spawn(cmd, { shell: true });

const trieRoot = new FilePathPrefixTree('\0', '.');

gitLog.stdout.on('data', (data) => {
  const filepaths = data.toString().split('\n').filter(Boolean);
  for (let filepath of filepaths) {
    FilePathPrefixTree.insertPath(trieRoot, filepath);
  }

  const lines = [];
  const printPath = ({ path, fullPath, currLevel }) => {
    const level = currLevel - 1;
    const indentTabs = '  '.repeat(level); // 2 tabs * level
    const isFile = path.includes('.');

    if (isFile) {
      const fileFullPath = fullPath ? `${fullPath}/${path}` : path;
      lines.push(`${indentTabs}- [${path}](${fileFullPath})`);
    } else {
      lines.push(`${indentTabs}- ${path}`);
    }
  };
  FilePathPrefixTree.traverse({ currNode: trieRoot, cbToProcessPathString: printPath, currLevel: 0 });

  const md = [
    '# DSA Studies',
    '',
    '- This repository serves as a comprehensive collection of resources and studies on Data Structures and Algorithms (DSA).',
    '- It includes explanations, examples, and analysis to help deepen understanding and mastery of key concepts in DSA.',
    '',
    '## Table of contents:',
    '',
    ...lines,
    ''
  ].join('\n');

  fs.writeFileSync('./readme.md', md);

});

gitLog.stderr.on('data', (data) => {
  console.error(`stderr: ${data.toString()}`);
});
