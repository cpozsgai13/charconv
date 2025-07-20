#include <iostream>
#include <boost/charconv.hpp>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

//  Enumeration for manipulators to test.
//  Order is arbitrary as new types are tested.
enum class Manipulator {
  ShowPos = 0,
  Hex = 1,
  SetPrecision = 2,
  SetFill = 3
};

template<Manipulator M>
struct RunTestImpl;


template<>
struct RunTestImpl<Manipulator::ShowPos> {
  static std::string execute(int value) {
    char buf[32];
    char* ptr = buf;
    if (value >= 0) *ptr++ = '+';
    auto result = boost::charconv::to_chars(ptr, buf + sizeof(buf), value);
    return std::string(buf, result.ptr);
  }
};

template<>
struct RunTestImpl<Manipulator::Hex> {
  static std::string execute(int value) {
    char buf[32];
    char* ptr = buf;
    *ptr++ = '0'; *ptr++ = 'x';
    auto result = boost::charconv::to_chars(ptr, buf + sizeof(buf), value, 16);
    return std::string(buf, result.ptr);
  }
};

template<>
struct RunTestImpl<Manipulator::SetPrecision> {
  static std::string execute(double value, int places) {
    char buf[64];
    char* ptr = buf;
    if (value >= 0.0) *ptr++ = '+';
    auto result = boost::charconv::to_chars(ptr, buf + sizeof(buf), value,
      boost::charconv::chars_format::fixed, places);
    return std::string(buf, result.ptr);
  }
};

template<>
struct RunTestImpl<Manipulator::SetFill> {
  static std::string execute(int value, int len, char fill) {
    char buf[64];
    auto result = boost::charconv::to_chars(buf, buf + sizeof(buf), value);
    std::string str(buf, result.ptr);
    if (str.size() < static_cast<size_t>(len)) {
      std::string filled(len - str.size(), fill);
      return filled + str;
    }
    return str;  }
};

template<Manipulator M, typename T, typename... Args>
std::string RunTest(T value, Args&&... args) {
  return RunTestImpl<M>::execute(value, std::forward<Args>(args)...);
}

int main() {
    int pos = 42;
    int neg = -7;
    std::cout << "Positive: " << RunTest<Manipulator::ShowPos, int>(pos) << "\n";
    std::cout << "Positive Hex: " << RunTest<Manipulator::Hex, int>(pos) << "\n";
    std::cout << "Negative: " << RunTest<Manipulator::ShowPos, int>(neg) << "\n";
    std::cout << "Negative Hex: " << RunTest<Manipulator::Hex, int>(neg) << "\n";
    std::cout << "SetPrecision Positive: " << RunTest<Manipulator::SetPrecision, double>(1.544449, 2) << "\n";
    std::cout << "SetPrecision Negative: " << RunTest<Manipulator::SetPrecision, double>(-1.544449, 4) << "\n";
    std::cout << "SetFill(5, 2, '0'): " << RunTest<Manipulator::SetFill, int, int, char>(5, 2, '0') << "\n";
    std::cout << "SetFill(555, 8, 'a'): " << RunTest<Manipulator::SetFill, int, int, char>(555, 8, 'a') << "\n";
}


