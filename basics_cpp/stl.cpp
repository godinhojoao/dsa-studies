#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  // vector playground
  cout << "\nvector -------------\n";
  vector<long long> a(10, -1);
  a[0] = 10;
  a[1] = 12;
  a.push_back(99);
  cout << "size:" << a.size() << "\n";
  a.pop_back();

  long long &b = a.front(); // a.back();
  cout << "before changing b: " << b << " \n";
  b = 23;
  sort(a.begin(), a.end()); // asc
  reverse(a.begin(), a.end());
  for (auto &x : a) {
    cout << x << "\n";
  }
  a.clear();

  cout << "vector ------------- \n\n";

  // map playground (if keys order doesn't matter, use unordered_map)
  cout << "\nmap -------------\n";
  unordered_map<string, long long> peopleAge;
  peopleAge["joao"] = 23;
  long long joaoAge = peopleAge["joao"];
  cout << "joaoAge: " << joaoAge << "\n";

  unordered_map<string, vector<string>> peoplePets;
  peoplePets["joao"].push_back("mendigata");
  peoplePets["joao"].push_back("mendi");
  peoplePets["fe"].push_back("bebe");

  if (peoplePets.count("joao")) {
    cout << "only joao here \n";

    cout << "joao and ";
    const auto &pets = peoplePets.at("joao");
    for (const auto &pet : pets) {
      cout << pet << ((&pet != &pets.back()) ? ", " : ".");
    }
    cout << "\n\n";
  }

  cout << "all here \n";
  for (const auto &person : peoplePets) {
    cout << person.first << " and ";
    for (const auto &pet : person.second) {
      cout << pet << ((&pet != &person.second.back()) ? ", " : ".");
    }
    cout << "\n";
  }

  cout << "below is the count using a map\n";
  unordered_map<string, long long> seenCount;
  seenCount["joao"]++; // if not exists starts as 0, then ++
  seenCount["mendi"]++;
  seenCount["mendi"]++;
  for (const auto &x : seenCount) {
    cout << x.first << " " << x.second << "\n";
  }
  cout << "map ------------- \n\n";

  // set playground (if keys order doesn't matter, use unordered_set)
  cout << "\nset -------------\n";
  unordered_set<string> seen;
  seen.insert("joao");
  seen.insert("joao2");
  seen.insert("joao3");
  cout << "seen.count(john): " << seen.count("john") << "\n";
  cout << "seen.count(joao): " << seen.count("joao") << "\n";
  seen.erase("joao");
  cout << "after erase seen.count(joao): " << seen.count("joao") << "\n";

  for (const auto &x : seen) {
    cout << x << "\n";
  }

  cout << "min and max using a set\n";
  set<long long> nums;
  nums.insert(12334);
  nums.insert(88);
  nums.insert(10000);
  long long smallest = *nums.begin(); // nums.begin() is an iterator so we get the value with * dereferencing
  cout << "smallest " << smallest << "\n";
  long long biggest = *nums.rbegin(); // rbegin = reversebegin = last hehe
  cout << "biggest " << biggest << "\n";
  cout << "set ------------- \n\n";

  // queue playground
  // queue = container adaptor of deque; it means it is a wrapper of a deque; https://en.cppreference.com/cpp/container/queue
  // doesn't support iterator, random access and all other methods that a container supports.
  cout << "\nqueue -------------\n";
  queue<long long> numsQueue;
  numsQueue.push(1);
  numsQueue.push(2);
  numsQueue.push(3);
  cout << "numsQueue.size() " << numsQueue.size() << "\n";
  cout << "numsQueue.empty() " << numsQueue.empty() << "\n";
  cout << "numsQueue.front() " << numsQueue.front() << "\n";         // front is like "top", doesn't remove
  cout << "numsQueue.size() " << numsQueue.size() << "\n";           // same size
  numsQueue.pop();                                                   // removes and return void
  cout << "AFTER POP numsQueue.size() " << numsQueue.size() << "\n"; // -1 size
  cout << "queue ------------- \n\n";

  // deque playground
  // deque = sequence container; https://en.cppreference.com/cpp/container/deque
  // supports iterators, random access. (.clear, dq[index], dq.begin() ...)
  cout << "\ndeque -------------\n";
  deque<long long> numsDeque;
  numsDeque.push_back(1);
  numsDeque.push_back(2);
  numsDeque.push_back(3);

  cout << "*dq.begin(): " << *numsDeque.begin() << "\n";
  cout << "dq[1]: " << numsDeque[1] << "\n";
  cout << "dq[2]: " << numsDeque[2] << "\n";

  cout << "deque using FIFO \n";
  deque<long long> copyofdq = numsDeque;
  while (!copyofdq.empty()) {
    cout << copyofdq.front() << (copyofdq.size() > 1 ? " - " : "\n");
    copyofdq.pop_front();
  }

  cout << "deque using LIFO \n";
  deque<long long> copyofdq2 = numsDeque;
  while (!copyofdq2.empty()) {
    cout << copyofdq2.back() << (copyofdq2.size() > 1 ? " - " : "\n");
    copyofdq2.pop_back();
  }
  numsDeque.clear();
  cout << "deque ------------- \n\n";

  // stack playground (container adaptor of deque)
  cout << "\nstack -------------\n";
  deque<long long> mydq = {1, 2, 3};
  stack<long long> numsStack(mydq);
  // numsStack.push(1);
  // numsStack.push(2);
  // numsStack.push(3);
  while (!numsStack.empty()) {
    cout << numsStack.top() << (numsStack.size() > 1 ? " - " : "\n");
    numsStack.pop();
  }
  cout << "stack ------------- \n\n";

  // priority_queue playground (container adaptor of vector)
  // priority_queue<T, Container, Compare>
  // default is maxHeap: priority_queue<T, vector<T>, less<T>>
  // less -> is 3 < 5? if true, 3 comes after 5 (lower value = lower priority)
  cout << "\npriorityQueue -------------\n";
  vector<long long> v = {1, 2, 3};

  priority_queue<long long> maxHeap(v.begin(), v.end());
  maxHeap.push(10);
  maxHeap.push(4);
  cout << "maxHeap: \n";
  while (!maxHeap.empty()) {
    cout << maxHeap.top() << (maxHeap.size() > 1 ? " - " : "\n");
    maxHeap.pop();
  }

  priority_queue<long long, vector<long long>, greater<long long>> minHeap(v.begin(), v.end()); // greater value = lower priority (minHeap)
  minHeap.push(10);
  minHeap.push(4);
  cout << "minHeap: \n";
  while (!minHeap.empty()) {
    cout << minHeap.top() << (minHeap.size() > 1 ? " - " : "\n");
    minHeap.pop();
  }
  cout << "priorityQueue ------------- \n\n";

  return 0;
}

/*
- receive 10 elements and set non existent as -1: vector<long long> a(10,-1);
- count items appearing: unordered_map + mapname[key]++;
- check if exists: unordered_set + setname.count(key);
- min and max: set and *setname.begin(), *setname.rbegin();
- queue: the goto for bfs;
- deque: the goto for easy to iterate + insert/remove both ends O(1); (vector has O(n) push_front/pop_front);
- stack: LIFO, the goto for brackets/monotonic stack/iterative dfs;
- priority_queue: always get the biggest/smallest fast O(log n), the goto for dijkstra (minHeap with greater<>);

https://en.cppreference.com/cpp/container

# container = stores and manages its own elements, provide operations to access and modify it.
  - e.g: vector, deque, set, map...
  - we can initialize containers with initializer lists, but container adaptors cannot; e.g vector<int> v = {10,2,3}; deque<int> dq = {1,2,3};
# container adaptor = does not manage its own storage, it wraps an existing container adding restrictions to enforce specific data structure behavior.
  - e.g: queue is a container adapator that wraps deque
  - we can construct container adaptors from the underlying container they wrap; e.g queue<int> myQ(someDeque); priority_queue<int> myPQ(vector.begin(), vector.end());


# sequence container = stores elements in linear order, access/edit by position (front, back, or index on some).
  - vector/deque = random access (v[index]).
  - list = no random access, only sequential.
  - e.g: vector, deque, list...
# associative container = stores elements organized and sorted by keys, desgined for efficient search/insertion/deletion using a key instead of index (on set the element is the key).
  - e.g: map, set... (keys)
  - unordered associative container = use a hashtable and do not sort by keys, no defined order, faster search/insert/remove on average O(1) and worst O(n).
    - e.g: unordered_map, unordered_set...
*/