// Better Strings
#ifndef BETTER_STRING_LIB
#define BETTER_STRING_LIB

#include <string>
#include <vector>

#define LEX_END -1

namespace better {
//==========================================================
// Improved string behavior
// Extensible model for better strings
// Almost all of the actual logic is implemented here
//==========================================================
class lexicography{
public:
  // Overloaded Operators
  bool operator==(const lexicography& rhs) const;
  bool operator!=(const lexicography& rhs) const;
  bool operator>(const lexicography& rhs) const;
  bool operator<(const lexicography& rhs) const;
  char& operator[](unsigned int i);
  const char& operator[](unsigned int i) const;

  //=======================================
  // Mutable functions
  //=======================================
  void reserve(unsigned capacity);
  void resize(unsigned size, char replace = ' ');
  void reverse();
  void assign(const char* str);
  void assign(const std::string& str);
  void assign(const lexicography& str);
  void trim(char value = ' ');
  void capitalize(unsigned index = 0);
  void sort(bool include_whitespace = false, bool include_special = false);
  
  void insert(char chr, unsigned at);
  void insert(const char* str, unsigned at);
  void replace(char chr, char value);
  void replace(const char* str, const char* value);
  void erase(char chr);
  void erase(const char* str);
  void erase(unsigned start, unsigned end = LEX_END);
  void clear();

  void to_lower();
  void to_upper();

  // Additive operators
  void append(const char* str);
  void append(const std::string& str);
  void append(const lexicography& str);
  void push_back(char chr);
  void pop_back();

  void prepend(const char* str);
  void prepend(const std::string& str);
  void prepend(const lexicography& str);
  void push_front(char chr);
  void pop_front();

  // Special Getters
  char& front();
  char& back();

  // Special conversions
  void from_bool(bool value);
  void from_int(int value);
  void from_double(double value);
  
  //=======================================
  // Immutable functions
  //=======================================
  const char* c_str() const;
  int compare(const char* str) const;
  int compare(const std::string& str) const;
  int compare(const lexicography& str) const;

  // Insensitive compare
  int icompare(const char* str) const;
  int icompare(const std::string& str) const;
  int icompare(const lexicography& str) const;

  int find(char chr) const;
  int find(const char* str) const;
  int find_last(char chr) const;
  int find_last(const char* str) const;

  unsigned count(char chr) const;
  unsigned count(const char* str) const;
  
  bool get_bool() const;
  int get_int() const;
  int fast_int() const;
  unsigned get_unsigned() const;
  unsigned fast_unsigned() const;
  double get_double() const;
  double fast_double() const;
  
  unsigned length() const;

  // Analysis fucntions
  bool ends_with(const char* str) const;
  bool starts_with(const char* str) const;
  bool empty() const;
  bool palindrome(bool absolute = true) const;

  //=======================================
  // Static functions
  //=======================================
  static bool whitespace(char chr);
  static bool letter(char chr);
  static bool numeric(char chr);
  static bool special(char chr);

  //=======================================
  // Immutable virtual functions
  //=======================================
  virtual unsigned size() const = 0;
  virtual unsigned capacity() const = 0;
  virtual unsigned refrences() const = 0;

private:
  //=======================================
  // Special functions
  //=======================================
  virtual char* mutable_data() = 0;
  virtual const char* immutable_data() const = 0;
  virtual void require(unsigned bytes) = 0;
};
  
//==========================================================
// Improved copy and stack space
// - Late binding
// - Refrence counting
// - size: 4 bytes
//==========================================================
class string : public lexicography{
public:
  // Constructors / Deconstructors
  string();
  string(bool value);
  string(int value);
  string(double value);
  string(const char* str);
  string(const char* str, unsigned pos, unsigned len = std::string::npos);
  string(const std::string& str);
  string(const lexicography& rhs);
  string(const string& rhs);
  ~string();

  string& operator=(const string& rhs);
  
  void swap(lexicography& str);
  
  void join(const std::vector<string>& strs);
  void join_lines(const std::vector<string>& strs);
  void format();
  void format_lines();

  //=======================================
  // Immutable functions
  //=======================================
  string substr(unsigned pos, unsigned len = std::string::npos) const;
  string lowercase() const;
  string uppercase() const;

  // Getters
  unsigned size() const;
  unsigned capacity() const;
  unsigned refrences() const;

  // Special Functions
  std::vector<string> tokenize(bool newline_delim = false) const;

private:
  // Data buffer containing all relevant infomation
  char* data;

  // Data buffer containing all relevant infomation
  char* mutable_data();
  const char* immutable_data() const;
  void require(unsigned bytes);

  // Various offsets for data values
  static const size_t _ref_key = 0;
  static const size_t _size_key = _ref_key + sizeof(_ref_key);
  static const size_t _cap_key = _size_key + sizeof(_size_key);
  static const size_t _data_key = _cap_key + sizeof(_cap_key);
};

};

// Operators
better::string operator+(const better::string& btr, const char* str);
better::string operator+(const char* str, const better::string& btr);
better::string operator+(const better::string& btr, int value);
better::string operator+(int value, const better::string& btr);
better::string operator+(const better::string& btr, double value);
better::string operator+(double value, const better::string& btr);
better::string operator+(const better::string& btr, bool value);
better::string operator+(bool value, const better::string& btr);

#endif