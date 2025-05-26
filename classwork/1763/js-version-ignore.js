function getInvalidIndex(s) {
  for (let i = 0; i < s.length; i++) {
    let hasUpper = false;
    let hasLower = false;
    for (let j = 0; j < s.length; j++) {
      if (s[i].toLowerCase() === s[j]) hasLower = true;
      if (s[i].toUpperCase() === s[j]) hasUpper = true;
    }
    if (!(hasUpper && hasLower)) {
      return i;// index not valid
    }
  }
  return -1;
}

function getSubstrings(s, index) { // return array of strings
  let substrings = [];

  if (index >= 1) {
    let newLeftString = ''
    for (let i = 0; i < index; i++) {
      newLeftString += s[i];
    }
    substrings.push(newLeftString);
  }

  let newRightString = ''
  for (let i = index + 1; i < s.length; i++) {
    newRightString += s[i];
  }
  substrings.push(newRightString)
  return substrings
}

/**
 * @param {string} s
 * @return {string}
 */
var longestNiceSubstring = function (s) {
  if (!s || s.length <= 1) {
    return ''
  }

  const invalidIndex = getInvalidIndex(s);
  if (invalidIndex < 0) { // valid
    return s;
  }

  const substrings = getSubstrings(s, invalidIndex);
  const left = longestNiceSubstring(substrings[0]);
  const right = longestNiceSubstring(substrings[1]);

  const leftLength = left && left.length;
  const rightLength = right && right.length;
  if (leftLength == rightLength) {
    return left;
  }
  return rightLength > leftLength ? right : left;
};

const res = longestNiceSubstring('YazaAay');
console.log('res', res)