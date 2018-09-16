#pragma once

#include <cstddef>

namespace mrsuyi {
template <class T>
struct hash {};

// string related
inline size_t __hash_string(const char* s) {
  unsigned long h = 0;
  for (; *s; ++s)
    h = 5 * h + *s;
  return h;
}
template <>
struct hash<char*> {
  std::size_t operator()(const char* s) const { return __hash_string(s); }
};
template <>
struct hash<const char*> {
  std::size_t operator()(const char* s) const { return __hash_string(s); }
};

// single number
template <>
struct hash<bool> {
  std::size_t operator()(bool n) const { return n; }
};
template <>
struct hash<char> {
  std::size_t operator()(char n) const { return n; }
};
template <>
struct hash<signed char> {
  std::size_t operator()(signed char n) const { return n; }
};
template <>
struct hash<unsigned char> {
  std::size_t operator()(unsigned char n) const { return n; }
};
template <>
struct hash<char16_t> {
  std::size_t operator()(char16_t n) const { return n; }
};
template <>
struct hash<char32_t> {
  std::size_t operator()(char32_t n) const { return n; }
};
template <>
struct hash<wchar_t> {
  std::size_t operator()(wchar_t n) const { return n; }
};
template <>
struct hash<short> {
  std::size_t operator()(short n) const { return n; }
};
template <>
struct hash<unsigned short> {
  std::size_t operator()(unsigned short n) const { return n; }
};
template <>
struct hash<int> {
  std::size_t operator()(int n) const { return n; }
};
template <>
struct hash<unsigned int> {
  std::size_t operator()(unsigned int n) const { return n; }
};
template <>
struct hash<long> {
  std::size_t operator()(long n) const { return n; }
};
template <>
struct hash<unsigned long> {
  std::size_t operator()(unsigned long n) const { return n; }
};
template <>
struct hash<long long> {
  std::size_t operator()(long long n) const { return n; }
};
template <>
struct hash<unsigned long long> {
  std::size_t operator()(unsigned long long n) const { return n; }
};
template <>
struct hash<float> {
  std::size_t operator()(float n) const { return n; }
};
template <>
struct hash<double> {
  std::size_t operator()(double n) const { return n; }
};
}  // namespace mrsuyi
