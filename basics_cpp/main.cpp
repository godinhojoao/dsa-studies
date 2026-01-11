#include <iostream>
#include <stdexcept>
#include <vector>

// directives
#define N 3
constexpr int M = 3;  // better in c++ type safety

// "A container is a holder object that stores a collection of other objects"
template <typename Container>  // using a template to accept a generic type (name container is for convenience not a required name)

// Container a -> copy
// Container& a -> original, writable ---> in C we would receive a pointer to modify the original, for example: int a*
// const Container& a -> original and read-only
void printArrayUtility(const std::string prefix, const Container& a, size_t size) {
  if(!prefix.empty()) {
    std::cout << prefix + ": ";
  }

  for(int i = 0; i < size; i++) {
    char limiter = i == size - 1 ? '\n' : '-';
    std::cout << a[i] << limiter;
  }
}

void printDirectives() {
  std::cout << "this is N: " << N << " and this is M: " << M << "\n\n";
}

void demoInAndOut() {
  // std::cout means "go inside std and get cout"->::is the scope resolution operator, meaning cout belongs to std lib
  std::cout << "Give me your age: ";

  int x;
  std::cin >> x;

  std::cout << "Fine, your age is " << x << "!!!\n\n";

  // in with getline (diff from is that getline gets the spaces and cin stops if find spaces)
  std::cout << "Give me a char: ";
  char a;
  std::cin >> a;

  std::cout << "This is your char: " << a << "\n\n";

  std::cout << "Give me a string (until first space ->cin): ";
  std::string b;
  std::cin >> b;
  std::cout << "This is your string (until first space ->cin): " << b << "\n\n";

  // --> If you write "joao test", the words after the space read by std::cin will stay in the buffer and may mess up future inputs.
  // solution for this: clear the entire buffer after a cin
  // std::cin.ignore(length, delimiter)
  // or always use getline instead of cin

  // streamsize is a type and we use the std::numeric_limits to get its max value
  // if it doesn't find the delimiter \n in this case, it will keep holding until you add it to the buffer
  std::cout << "streamsize max value: " << std::numeric_limits<std::streamsize>::max() << "\n";
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  std::cout << "Give me a string (including spaces ->getline): ";
  std::string c;
  std::getline(std::cin, c);
  std::cout << "This is your string (including spaces ->getline): " << c << "\n\n";
}

void demoClasses() {
  // template -> already explained above: used to receive and return generic types
  // Class
  class Point {
    // if you do not define default is private:
    int zPrivate = 11;

   private:
    int xPrivate = 10;

   public:
    int x, y;  // properties or data members or class attributes

    // -> constructor two appproachs:
    // Point(int xArg, int yArg) : x(xArg), y(yArg) {}  // constructor
    Point(int xArg, int yArg) {
      x = xArg;
      y = yArg;
    }

    void move(int dx, int dy) {
      x += dx;
      y += dy;
    }

    int getXPrivate() const { return xPrivate; }

    // public getter for private member
    // this second const is to say "read only method"
    // if we instantiate our object as const we will not be able to use non-const methods -> so it matters
    // e.g.
    // const Point p1(1, 2);
    // p1.move(10, 12);  // error
    // p1.getXPrivate();  // ok if this is a const method
  };

  Point p1(1, 2);  // constructor called
  p1.move(3, 4);  // call method
  std::cout << "public property from p1: " << p1.x << "\n";  // access public member
  std::cout << "private property from p1: " << p1.getXPrivate() << "\n";

  // const object to get error:
  // const Point p2(1, 2);
  // p2.move(10,12); // error at compile time
  // the object has type qualifiers that are not compatible with the member function "Point::move"C/C++(1086)
  // main.cpp(116, 3): object type is: const Point

  // with default constructor if defined
  class Point2 {
   public:
    int x = 0, y = 0;
  };
  Point2 p2;  // x = 0, y = 0

  // inheritance --> using exception as base class to create a custom exception
  class CustomEx : public std::exception {
    std::string msg;

   public:
    CustomEx(const std::string& s) : msg(s) {}
    // what() is a virtual method from the base class std::exception and we are overriding it with our implementation on CustomEx
    const char* what() const noexcept override {
      // noexcept is this method saying to the compiler it will not throw exceptions at runtime
      // if we add this --> throw std::runtime_error("oops");
      // compiler will return a warning -> main.cpp:120:7: warning: 'what' has a non-throwing exception specification but can still throw [-Wexceptions]
      return msg.c_str();
    }
  };

  // arr -> same thing as in c
}

void demoHandlingExceptions() {
  class MyCustomExceptionClass : public std::exception {
    std::string msg;

   public:
    MyCustomExceptionClass(const std::string& s) : msg(s) {}
    const char* what() const noexcept override { return msg.c_str(); }
  };

  // making code safe with try/catch to handle exceptions
  try {
    int num;
    std::cout << "Give me a number (-1 invalid arg, 0 logic error): ";

    while(!(std::cin >> num)) {  // keep looping while input fails
      std::cin.clear();  // clear error state
      std::cin.ignore(1000, '\n');  // discard invalid input
      std::cout << "Invalid input. Enter a number: ";
    }
    std::cout << "You entered num: " << num << "\n";

    if(num < 0) {
      throw MyCustomExceptionClass("num must be positive");
    }
    if(num == 0) {
      throw std::logic_error("Division by zero is a logic error");
      // without try catch it would break our system with uncaught exception:
      //libc++abi: terminating due to uncaught exception of type std::logic_error: Division by zero is a logic error
      // [1]    61762 abort      ./main
    }
  } catch(const std::logic_error& ex) {
    std::cerr << "Logic error: " << ex.what() << "\n";
  } catch(const std::exception& ex) {
    std::cerr << "some type of exception msg:: " << ex.what() << "\n";
    std::cerr << "Exception type: " << typeid(ex).name() << "\n";  // typeid describes the type of an expression or typename
  }
}

// https://cplusplus.com/reference/stl/
// https://www.youtube.com/watch?v=6OoSgY6NVVk
void demoStdLibContainers() {
  int arr[3] = {1, 2, 3};
  printArrayUtility("arr in stack", arr, sizeof(arr) / sizeof(int));

  // #Fixed size array -> usually on stack but if global or static will be on heap
  // continguous same as c arr on stack
  std::array<int, 3> a = {1, 2, 3};
  printArrayUtility("std::arr", a, a.size());

  // #Dynamic array with std::vector -> memory managed automatically (badass) #include <vector>
  // continguous same as c arr on stack and can reallocate on growth
  // push_back is not always O(1); it becomes O(n) when the vector reallocates upon reaching its capacity
  // Don’t store pointers to vectors; resizing can invalidate them.
  // ->multiple built-in functions like resize, push_back, pop_back, empty (returns true if is empty), shrink_to_fit, swap(another vec), at
  // --> vec[4] is unsafe if out of bounds because it does NOT perform bounds checking
  // --> vec.at(4) is safe: throws std::out_of_range if the index is out of bounds
  std::vector<int> numVec = {1, 2, 3, 4};
  numVec.push_back(5);
  printArrayUtility("std::vector", numVec, numVec.size());
}

void demoStdLibAlgorithms() {}

void demoMemManagement() {}

// demoTemplates:
// func template accepting only int and double
template <typename T>
T maxVal(T a) {
  static_assert(
      std::is_same<T, int>::value ||
          std::is_same<T, double>::value,
      "Only int or double allowed");
  return a;
}

// class template
template <typename T>
class Box {
 public:
  T value;
  Box(T v) : value(v) {}

  void show() {
    std::cout << "show generic class template value: " << value << "\n";
  }
};

// template specialization
template <>
class Box<int> {
 public:
  int value;
  Box(int v) : value(v) {}
  void show() { std::cout << "int: " << value << "\n"; }

  void customMethodForIntBoxOnly() {
    std::cout << "customMethodForIntBoxOnly\n";
  }
};

// https://www.learncpp.com/cpp-tutorial/function-templates/
void demoTemplates() {
  // func template
  maxVal(3);
  maxVal(2.5);
  // maxVal('a');  // compile time error since the static_assert fails
  maxVal<int>('a');  // force int cast bypass the static_assert in compile time

  // class template + template specialization
  Box<int> a(3);
  a.show();
  a.customMethodForIntBoxOnly();
  Box<double> b(2.5);
  b.show();
  // b.customMethodForIntBoxOnly(); // compile time: class "Box<double>" has no member "customMethodForIntBoxOnly"C/C++(135)
  Box<std::string> c("testing");
  c.show();
  // c.customMethodForIntBoxOnly(); // compile time: class "Box<double>" has no member "customMethodForIntBoxOnly"C/C++(135)
}

// demoNamespaces:
namespace calculator {
void sum(int a, int b) {
  std::cout << "sum: " << a + b << "\n";
}
void subtract(int a, int b) {
  std::cout << "subtract: " << a - b << "\n";
}
}  // namespace calculator

namespace outer {
namespace inner {

// variables inside namespace
int configtest = 100;
const double pi = 3.14159;

void hello() { std::cout << "Hello\n"; }
}  // namespace inner
}  // namespace outer

// anonymous namespace: limit visibility of funcs
// this is only available at this file, if other files try to use that it will break
namespace {
void secretFunction() {
  std::cout << "Only visible in this file\n";
}

class Test {
 public:
  int x;
  Test(int a) : x(a) {}
  void show() { std::cout << "testing show x: " << x << "\n"; }
};
}  // namespace

// convention (namespace "detail" to put implementation details in which users should not touch)
namespace myLib {
using std::cout;  // brings only cout from std into this namespace;
// other names are NOT imported and are still accessible only via std::

// namespace as alias (it doesn't work for variables):
namespace io = std;

// using as type alias:
using IntVec = std::vector<int>;

namespace detail {
void helper() {
  IntVec v;
  v.push_back(1);

  int x;
  cout << "internal from detail, do not touch\n";
  // cin >> x;  // identifier "cin" is undefinedC/C++(20)
}
}  // namespace detail

void run() {
  detail::helper();
}
}  // namespace myLib

// Convention: avoid polluting the global scope; create a namespace for your functions, classes, etc.

void demoNamespaces() {
  // namespaces are above this func:
  calculator::sum(12, 3);
  calculator::subtract(1, 1);

  outer::inner::hello();
  std::cout << "outer::inner::configtest: " << outer::inner::configtest << "\n";
  std::cout << "outer::inner::pi: " << outer::inner::pi << "\n";

  namespace test = std;
  test::cout << "test worked" << "\n";

  Test t(3);  // access class via anonymous namespace, so we made it only available for this file
  t.show();

  myLib::run();
  myLib::cout << "test\n";
  myLib::io::cout << "test2\n";
}

void demoFileStreams() {}

int main() {
  // printDirectives();

  // demoInAndOut();

  // demoClasses();

  // demoHandlingExceptions();

  // demoStdLibContainers();  //not done add more std lib stuff

  // demoStdLibAlgorithms();  //not done add more std lib stuff

  // demoMemManagement();  // not done -> compare heap, std lib, stack, etc.. Dynamic memory, new, delete, etc.. new[] delete[], smart pointers std::unique_ptr, std::shared_ptr
  // talk about RAII inside it

  // demoTemplates();

  // demoNamespaces();

  // demoFileStreams(); // not done
  return 0;
}