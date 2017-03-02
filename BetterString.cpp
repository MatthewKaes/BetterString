#include "BetterString.h"
#include <stdlib.h>

#define NULL 0
#define BTRSTRING_MAX 2147483647

//Constants
const char _digits10[] = {'0','1','2','3','4','5','6','7','8','9'};

using namespace better;

//=========================================================
// Lexicography
//=========================================================
const char* lexicography::c_str() const
{
  return immutable_data();
}

void lexicography::reserve(unsigned _capacity)
{
  require(_capacity);
}

void lexicography::resize(unsigned _size, char replace)
{
  unsigned m_size = size();
  
  require(_size);

  char* m_data = mutable_data();
  if(_size > m_size)
  {
    for(unsigned i = m_size; i < _size; i++)
    {
      m_data[i] = replace;
    }
  }
  m_data[_size] = '\0';
}

void lexicography::reverse()
{
  char* m_data = mutable_data();
  unsigned m_size = size();
  unsigned m_size_r = m_size / 2;
  
  m_size -= 1;
  for(unsigned i = 0; i < m_size_r; i++)
  {
    char t = m_data[i];
    m_data[i] = m_data[m_size - i];
    m_data[m_size - i] = t;
  }
}

void lexicography::assign(const char* str)
{
  // Get the size
  unsigned size = strlen(str);
  
  // Allocate our size + the size of metadata + the zero byte.
  require(size);

  // Copy the data
  memcpy(mutable_data(), str, size + 1);
}

void lexicography::assign(const std::string& str)
{ 
  // Allocate our size + the size of metadata + the zero byte.
  require(str.size());

  // Copy the data
  memcpy(mutable_data(), str.c_str(), str.size() + 1);
}

void lexicography::assign(const lexicography& str)
{
  // Allocate our size + the size of metadata + the zero byte.
  require(str.size());

  // Copy the data
  memcpy(mutable_data(), str.immutable_data(), str.size() + 1);
}
void lexicography::trim(char value)
{
  unsigned index = size() - 1;
  const char* m_data = immutable_data();
  while(index && m_data[index] == value)
  {
    index--;
  }
  if(index != size() - 1)
  {
    resize(index);
  }
}

void lexicography::capitalize(unsigned index)
{
  char* m_data = mutable_data();    

  if(m_data[index] >= 'a' && m_data[index] <= 'z')
  {
    m_data[index] += 'A' - 'a';
  }
}

void lexicography::sort(bool include_whitespace, bool include_special)
{
  unsigned counts[UCHAR_MAX + 1] = {0};
  char* m_data = mutable_data();    
  unsigned m_size = size();
  unsigned total = 0;
  
  for(unsigned i = 0; i < m_size; i++)
  {
    if(!include_whitespace && whitespace(m_data[i]))
    {
      continue;
    }
    if(!include_special && special(m_data[i]))
    {
      continue;
    }
    counts[m_data[i]]++;
    total++;
  }

  for(unsigned i = 0; i <= UCHAR_MAX; i++)
  {
    for(unsigned c = 0; c < counts[i]; c++)
    {
      *(m_data++) = static_cast<char>(i);
    }
  }

  *m_data = '\0';
  require(total);
}

void lexicography::insert(char chr, unsigned at)
{
  unsigned m_size = size();
  
  require(m_size + 1);
  
  if(at >= m_size)
    at = m_size;

  char* m_data = mutable_data();
  memmove(m_data + at + 1, m_data + at, m_size - at + 1);
  m_data[at] = chr;
}

void lexicography::insert(const char* str, unsigned at)
{
  unsigned m_size = size();
  unsigned r_size = strlen(str);
  
  require(m_size + r_size);
  
  if(at >= m_size)
    at = m_size;

  char* m_data = mutable_data();
  memmove(m_data + at + r_size, m_data + at, m_size - at + r_size);
  memcpy(m_data + at, str, r_size);
}

void lexicography::replace(char chr, char value)
{
  unsigned index = size() - 1;
  char* m_data = mutable_data();
  for(unsigned i = 0; i < index; i++)
  {
    if(m_data[i] == chr)
    {
      m_data[i] = value;
    }
  }
}

void lexicography::replace(const char* str, const char* value)
{
  unsigned replacements = count(str);
  if(!replacements)
    return;

  // Sizes and data
  unsigned m_size = size();
  unsigned f_size = strlen(str);
  unsigned r_size = strlen(value);
  int diff = r_size - f_size;
  unsigned new_size = m_size + diff * replacements;

  // Get the size of the new container
  require(m_size + diff * replacements);

  char* m_data = mutable_data();

  if(diff > 0)
  {
    for(unsigned i = 0; i < new_size; i++)
    {
      // Minor optimization for expensive strings
      if(m_data[i] == str[0])
      {
        bool found = true;
        for(unsigned s = 1; s < f_size; s++)
        {
          if(m_data[i + s] != str[s])
          {
            found = false;
            break;
          }
        }
        if(found)
        {
          memmove(m_data + i + r_size, m_data + i + f_size, new_size - i - f_size + 1);
          for(unsigned r = 0; r < r_size; r++)
          {
            m_data[i + r] = value[r];
          }
          i += r_size - 1;
        }
      }
    }
  }
  else
  {
    unsigned write_index = 0;
    for(unsigned i = 0; i < m_size; i++)
    {
      // Minor optimization for expensive strings
      if(m_data[i] == str[0])
      {
        bool found = true;
        for(unsigned s = 1; s < f_size; s++)
        {
          if(m_data[i + s] != str[s])
          {
            found = false;
            break;
          }
        }
        if(found)
        {
          for(unsigned r = 0; r < r_size; r++)
          {
            m_data[write_index++] = value[r];
          }
          i += f_size - 1;
          continue;
        }
      }
      m_data[write_index++] = m_data[i];
    }
    m_data[new_size] = '\0';
  }
}

void lexicography::erase(char chr)
{
  unsigned write_index = 0;
  char* m_data = mutable_data();
  unsigned m_size = size();

  for(unsigned i = 0; i < m_size; i++)
  {
    if(m_data[i] != chr)
    {
      m_data[write_index++] = m_data[i];
    }
  }
  
  m_data[write_index] = '\0';
  require(write_index);
}

void lexicography::erase(const char* str)
{
  replace(str, "");
}

void lexicography::erase(unsigned start, unsigned end)
{
  char* m_data = mutable_data();
  unsigned m_size = size();

  if(start < 0 || start >= m_size)
  {
    return;
  }

  if(end >= m_size || end == LEX_END)
  {
    m_data[start] = 0;
    require(start);
    return;
  }

  memmove(m_data + start, m_data + end, m_size - end + 1);
  
  require(m_size - end + start);
}

void lexicography::clear()
{
  resize(0);
}

void lexicography::to_lower()
{
  char* m_data = mutable_data();
  unsigned m_size = size();

  for(unsigned i = 0; i < m_size; i++)
  {
    if(m_data[i] >= 'A' && m_data[i] <= 'Z')
    {
      m_data[i] -= 'A' - 'a';
    }
  }
}

void lexicography::to_upper()
{
  char* m_data = mutable_data();
  unsigned m_size = size();

  for(unsigned i = 0; i < m_size; i++)
  {
    if(m_data[i] >= 'a' && m_data[i] <= 'z')
    {
      m_data[i] += 'A' - 'a';
    }
  }
}

void lexicography::append(const char* str)
{
  const char* r_data = str;
  unsigned r_size = strlen(str);

  unsigned m_size = size();
  require(m_size + r_size);
  char* m_data = mutable_data();
  
  memcpy(m_data + m_size, r_data, r_size + 1);
}

// Apply a simple adaptor for end users
void lexicography::append(const std::string& str)
{
  append(str.c_str());
}

void lexicography::append(const lexicography& str)
{
  const char* r_data = str.immutable_data();
  unsigned r_size = str.size();

  unsigned m_size = size();
  require(m_size + r_size);
  char* m_data = mutable_data();

  memcpy(m_data + m_size, r_data, r_size + 1);
}

void lexicography::push_back(char chr)
{
  unsigned m_size = size();
  require(m_size + 1);
  char* m_data = mutable_data();
  m_data[m_size] = chr;
  m_data[m_size + 1] = '\0';
}

void lexicography::pop_back()
{
  resize(size() - 1);
}

void lexicography::prepend(const char* str)
{  
  unsigned m_size = size();
  unsigned r_size = strlen(str);

  require(m_size + r_size);
  char* m_data = mutable_data();
  
  // Move memory to make room
  memmove(m_data + r_size, m_data, m_size + r_size);
  memcpy(m_data, str, r_size);
}

void lexicography::prepend(const std::string& str)
{
  unsigned m_size = size();
  unsigned r_size = str.size();

  require(m_size + r_size);
  char* m_data = mutable_data();
  
  // Move memory to make room
  memmove(m_data + r_size, m_data, m_size + r_size);
  memcpy(m_data, str.c_str(), r_size);
}

void lexicography::prepend(const lexicography& str)
{
  unsigned m_size = size();
  unsigned r_size = str.size();

  require(m_size + r_size);
  char* m_data = mutable_data();
  
  // Move memory to make room
  memmove(m_data + r_size, m_data, m_size + r_size);
  memcpy(m_data, str.immutable_data(), r_size);
}

void lexicography::push_front(char chr)
{
  unsigned m_size = size();
  require(m_size + 1);
  char* m_data = mutable_data();
  
  // Move memory to make room
  memmove(m_data + 1, m_data, m_size + 1);
  m_data[0] = chr;
}

void lexicography::pop_front()
{
  unsigned m_size = size();
  char* m_data = mutable_data();
  
  // Move memory to make room
  memmove(m_data, m_data + 1, m_size);
  require(m_size - 1);
}

char& lexicography::front()
{
  return mutable_data()[0];
}

char& lexicography::back()
{
  return mutable_data()[size() - 1];
}

void lexicography::from_bool(bool value)
{
  if(value)
    assign("true");
  else
    assign("false");
}

void lexicography::from_int(int value)
{
  char* m_data;
  unsigned len;

  // Get Length
  int abs_v = abs(value);
  if(abs_v < 10) len = 1;
  else if(abs_v < 100) len = 2;
  else if(abs_v < 1000) len = 3;
  else if(abs_v < 10000) len = 4;
  else if(abs_v < 100000) len = 5;
  else if(abs_v < 1000000) len = 6;
  else if(abs_v < 10000000) len = 7;
  else if(abs_v < 100000000) len = 8;
  else if(abs_v < 1000000000) len = 9;
  else len = 10;
  
  // Sign check
  if(value < 0)
  {
    require(len + 1);
    m_data = mutable_data();
    m_data[0] = '-';
    m_data++;
    value *= -1;
  }
  else
  {
    require(len);
    m_data = mutable_data();
  }

  // Loop unrolling
  switch(len) 
  { 
    // 2147483647 or upto 10 digits
    case 10:
      m_data[len - 10] = _digits10[value / 1000000000];
    case  9:
      m_data[len - 9] = _digits10[value / 100000000 % 10];
    case  8:
      m_data[len - 8] = _digits10[value / 10000000 % 10];
    case  7:
      m_data[len - 7] = _digits10[value / 1000000 % 10];
    case  6:
      m_data[len - 6] = _digits10[value / 100000 % 10];
    case  5:
      m_data[len - 5] = _digits10[value / 10000 % 10];
    case  4:
      m_data[len - 4] = _digits10[value / 1000 % 10];
    case  3:
      m_data[len - 3] = _digits10[value / 100 % 10];
    case  2:
      m_data[len - 2] = _digits10[value / 10 % 10];
    case  1:
      m_data[len - 1] = _digits10[value % 10];
  }
  m_data[len] = '\0';
}

void lexicography::from_double(double value)
{ 
  if(value < 0.0000000001 && value > -0.0000000001)
  {
    assign("0.0");
    return;
  }
  from_int(static_cast<int>(value));

  push_back('.');

  value -= static_cast<int>(value);
  if(value < 0.0000000001)
  {
    push_back('0');
  }
  else
  {
    int percision = 1;
    while(value > 0.000001 && percision < 12)
    {
      value *= 10;
      int temp = static_cast<int>(value);
      value -= temp;
      if(value > 0.999999)
      {
        temp += 1;
        value -= 1;
      }
      percision += 1;
      push_back(_digits10[temp]);
    }
  }
}

bool lexicography::operator==(const lexicography& rhs) const
{
  if(&rhs == this)
    return true;

  return compare(rhs) == 0;
}

bool lexicography::operator!=(const lexicography& rhs) const
{
  return !(*this == rhs);
}

bool lexicography::operator>(const lexicography& rhs) const
{
  const char* m_data = immutable_data();
  const char* r_data = rhs.immutable_data();

  unsigned v_size = size() < rhs.size() ? size() : rhs.size();

  for(unsigned i = 0; i < v_size; i++)
  {
    if(m_data[i] > r_data[i])
    {
      return true;
    }
    else if(m_data[i] < r_data[i])
    {
      return false;
    }
  }
  
  return size() > rhs.size();
}

bool lexicography::operator<(const lexicography& rhs) const
{  
  const char* m_data = immutable_data();
  const char* r_data = rhs.immutable_data();

  unsigned v_size = size() < rhs.size() ? size() : rhs.size();

  for(unsigned i = 0; i < v_size; i++)
  {
    if(m_data[i] < r_data[i])
    {
      return true;
    }
    else if(m_data[i] > r_data[i])
    {
      return false;
    }
  }
  
  return size() < rhs.size();
}

char& lexicography::operator[](unsigned int i)
{ 
  // Force to get mutabale data
  return mutable_data()[i];
}

const char& lexicography::operator[](unsigned int i) const
{ 
  // Immutable getter
  return immutable_data()[i];
}

// Flopped true false support for legacy string
int lexicography::compare(const char* str) const
{
  const char* m_data = immutable_data();
  const char* s_data = str;
  if(m_data == s_data)
    return false;

  unsigned m_size = size();
  if(m_size != strlen(str))
    return true;

  for(unsigned i = 0; i < m_size; i++)
  {
    if(m_data[i] != s_data[i])
      return true;
  }
  return false;
}

// Flopped true false support for legacy string
int lexicography::compare(const lexicography& rhs) const
{
  const char* m_data = immutable_data();
  const char* s_data = rhs.immutable_data();
  if(m_data == s_data)
    return false;

  unsigned m_size = size();
  if(m_size != rhs.size())
    return true;

  for(unsigned i = 0; i < m_size; i++)
  {
    if(m_data[i] != s_data[i])
      return true;
  }
  return false;
}

// Apply a simple adaptor for end users
int lexicography::compare(const std::string& str) const
{
  return compare(str.c_str());
}

int lexicography::icompare(const char* str) const
{
  const char* m_data = immutable_data();
  const char* s_data = str;
  if(m_data == s_data)
    return false;

  unsigned m_size = size();
  if(m_size != strlen(str))
    return true;

  for(unsigned i = 0; i < m_size; i++)
  {
    char m_c = m_data[i];
    char s_c = s_data[i];

    if(m_c >= 'A' && m_c <= 'Z')
    {
      m_c -= 'A' - 'a';
    }

    if(s_c >= 'A' && s_c <= 'Z')
    {
      s_c -= 'A' - 'a';
    }

    if(m_c != s_c)
      return true;
  }
  return false;
}

int lexicography::icompare(const std::string& str) const
{
  return icompare(str.c_str());
}

int lexicography::icompare(const lexicography& str) const
{
  return icompare(str.c_str());
}

int lexicography::find(char chr) const
{
  const char* m_data = immutable_data();
  int m_size = size();
  for(int i = 0; i < m_size; i++)
  {
    if(m_data[i] == chr)
      return i;
  }
  return -1;
}

int lexicography::find(const char* str) const
{
  const char* m_data = immutable_data();
  int m_size = size();
  unsigned r_size = strlen(str);
  for(int i = 0; i < m_size; i++)
  {
    // Minor optimization for expensive strings
    if(m_data[i] == str[0])
    {
      bool found = true;
      for(unsigned s = 1; s < r_size; s++)
      {
        if(m_data[i + s] != str[s])
        {
          found = false;
          break;
        }
      }
      if(found)
        return i;
    }
  }
  return -1;
}

int lexicography::find_last(char chr) const
{
  const char* m_data = immutable_data();
  int m_size = size();
  for(int i = m_size - 1; i >= 0; i--)
  {
    if(m_data[i] == chr)
      return i;
  }
  return -1;
}

int lexicography::find_last(const char* str) const
{
  const char* m_data = immutable_data();
  int m_size = size();
  unsigned r_size = strlen(str);
  for(int i = m_size - 1; i >= 0; i--)
  {
    // Minor optimization for expensive strings
    if(m_data[i] == str[0])
    {
      bool found = true;
      for(unsigned s = 1; s < r_size; s++)
      {
        if(m_data[i + s] != str[s])
        {
          found = false;
          break;
        }
      }
      if(found)
        return i;
    }
  }
  return -1;
}

unsigned lexicography::count(char chr) const
{
  const char* m_data = immutable_data();
  unsigned m_size = size();
  unsigned cnt = 0;
  for(unsigned i = 0; i < m_size; i++)
  {
    if(m_data[i] == chr)
      cnt++;
  }
  return cnt;
}

unsigned lexicography::count(const char* str) const
{
  const char* m_data = immutable_data();
  unsigned m_size = size();
  unsigned r_size = strlen(str);
  unsigned cnt = 0;
  for(unsigned i = 0; i < m_size - r_size; i++)
  {
    // Minor optimization for expensive strings
    if(m_data[i] == str[0])
    {
      bool found = true;
      for(unsigned s = 1; s < r_size; s++)
      {
        if(m_data[i + s] != str[s])
        {
          found = false;
          break;
        }
      }
      if(found)
        cnt++;
    }
  }
  return cnt;
}

bool lexicography::get_bool() const
{
  if(compare("true"))
    return true;
  return false;
}

int lexicography::get_int() const
{
  const char* m_data = immutable_data();
  int value = 0;
  int sign = 1;
  int m_size = size();

  // Sign checking
  if(m_data[0] == '-') 
  {
    sign = -1;
    ++m_data;
    --m_size;
  }
  else if(m_data[0] == '+')
  {
    ++m_data;
    --m_size;
  }

  // Loop unrolling
  switch(m_size) 
  { 
    // 2147483647 or upto 10 digits
    case 10:
      if(m_data[m_size - 10] < '0' || m_data[m_size - 10] > '9')
        return 0;
      value += (m_data[m_size - 10] - '0') * 1000000000;
    case  9:
      if(m_data[m_size - 9] < '0' || m_data[m_size - 9] > '9')
        return 0;
      value += (m_data[m_size - 9] - '0') * 100000000;
    case  8:
      if(m_data[m_size - 8] < '0' || m_data[m_size - 8] > '9')
        return 0;
      value += (m_data[m_size - 8] - '0') * 10000000;
    case  7:
      if(m_data[m_size - 7] < '0' || m_data[m_size - 7] > '9')
        return 0;
      value += (m_data[m_size - 7] - '0') * 1000000;
    case  6:
      if(m_data[m_size - 6] < '0' || m_data[m_size - 6] > '9')
        return 0;
      value += (m_data[m_size - 6] - '0') * 100000;
    case  5:
      if(m_data[m_size - 5] < '0' || m_data[m_size - 5] > '9')
        return 0;
      value += (m_data[m_size - 5] - '0') * 10000;
    case  4:
      if(m_data[m_size - 4] < '0' || m_data[m_size - 4] > '9')
        return 0;
      value += (m_data[m_size - 4] - '0') * 1000;
    case  3:
      if(m_data[m_size - 3] < '0' || m_data[m_size - 3] > '9')
        return 0;
      value += (m_data[m_size - 3] - '0') * 100;
    case  2:
      if(m_data[m_size - 2] < '0' || m_data[m_size - 2] > '9')
        return 0;
      value += (m_data[m_size - 2] - '0') * 10;
    case  1:
      if(m_data[m_size - 1] < '0' || m_data[m_size - 1] > '9')
        return 0;
      value += (m_data[m_size - 1] - '0');
      return value *= sign;
    default:
      // sizes greater then 10 or 0
      return 0;
  }
}

int lexicography::fast_int() const
{
  const char* m_data = immutable_data();
  int value = 0;
  int sign = 1;
  int m_size = size();

  // Sign checking
  if(m_data[0] == '-') 
  {
      sign = -1;
      ++m_data;
      --m_size;
  }

  // Loop unrolling
  switch(m_size) 
  { 
    // 2147483647 or upto 10 digits
    case 10:
      value += (m_data[m_size - 10] - '0') * 1000000000;
    case  9:
      value += (m_data[m_size - 9] - '0') * 100000000;
    case  8:
      value += (m_data[m_size - 8] - '0') * 10000000;
    case  7:
      value += (m_data[m_size - 7] - '0') * 1000000;
    case  6:
      value += (m_data[m_size - 6] - '0') * 100000;
    case  5:
      value += (m_data[m_size - 5] - '0') * 10000;
    case  4:
      value += (m_data[m_size - 4] - '0') * 1000;
    case  3:
      value += (m_data[m_size - 3] - '0') * 100;
    case  2:
      value += (m_data[m_size - 2] - '0') * 10;
    case  1:
      value += (m_data[m_size - 1] - '0');
      return value *= sign;
    default:
      // sizes greater then 10 or 0
      return 0;
  }
}
unsigned lexicography::get_unsigned() const
{
  const char* m_data = immutable_data();
  int value = 0;
  int sign = 1;
  int m_size = size();

  // Loop unrolling
  switch(m_size) 
  { 
    // 4294967295 or upto 10 digits
    case 10:
      if(m_data[m_size - 10] < '0' || m_data[m_size - 10] > '9')
        return 0;
      value += (m_data[m_size - 10] - '0') * 1000000000;
    case  9:
      if(m_data[m_size - 9] < '0' || m_data[m_size - 9] > '9')
        return 0;
      value += (m_data[m_size - 9] - '0') * 100000000;
    case  8:
      if(m_data[m_size - 8] < '0' || m_data[m_size - 8] > '9')
        return 0;
      value += (m_data[m_size - 8] - '0') * 10000000;
    case  7:
      if(m_data[m_size - 7] < '0' || m_data[m_size - 7] > '9')
        return 0;
      value += (m_data[m_size - 7] - '0') * 1000000;
    case  6:
      if(m_data[m_size - 6] < '0' || m_data[m_size - 6] > '9')
        return 0;
      value += (m_data[m_size - 6] - '0') * 100000;
    case  5:
      if(m_data[m_size - 5] < '0' || m_data[m_size - 5] > '9')
        return 0;
      value += (m_data[m_size - 5] - '0') * 10000;
    case  4:
      if(m_data[m_size - 4] < '0' || m_data[m_size - 4] > '9')
        return 0;
      value += (m_data[m_size - 4] - '0') * 1000;
    case  3:
      if(m_data[m_size - 3] < '0' || m_data[m_size - 3] > '9')
        return 0;
      value += (m_data[m_size - 3] - '0') * 100;
    case  2:
      if(m_data[m_size - 2] < '0' || m_data[m_size - 2] > '9')
        return 0;
      value += (m_data[m_size - 2] - '0') * 10;
    case  1:
      if(m_data[m_size - 1] < '0' || m_data[m_size - 1] > '9')
        return 0;
      value += (m_data[m_size - 1] - '0');
      return value *= sign;
    default:
      // sizes greater then 10 or 0
      return 0;
  }
}
unsigned lexicography::fast_unsigned() const
{
  const char* m_data = immutable_data();
  unsigned value = 0;
  int sign = 1;
  int m_size = size();

  // Loop unrolling
  switch(m_size) 
  { 
    // 2147483647 or upto 10 digits
    case 10:
      value += (m_data[m_size - 10] - '0') * 1000000000;
    case  9:
      value += (m_data[m_size - 9] - '0') * 100000000;
    case  8:
      value += (m_data[m_size - 8] - '0') * 10000000;
    case  7:
      value += (m_data[m_size - 7] - '0') * 1000000;
    case  6:
      value += (m_data[m_size - 6] - '0') * 100000;
    case  5:
      value += (m_data[m_size - 5] - '0') * 10000;
    case  4:
      value += (m_data[m_size - 4] - '0') * 1000;
    case  3:
      value += (m_data[m_size - 3] - '0') * 100;
    case  2:
      value += (m_data[m_size - 2] - '0') * 10;
    case  1:
      value += (m_data[m_size - 1] - '0');
      return value *= sign;
    default:
      // sizes greater then 10 or 0
      return 0;
  }
}
double lexicography::get_double() const
{
  int index = find('.');
  int m_size = size();
  int sign = 1;
  int start = 0;

  // Convert as integer
  if(index == -1)
  {
    if(m_size <= 10)
    {
      return static_cast<double>(get_int());
    }
  }
  
  const char* m_data = immutable_data();
  // Sign checking
  if(m_data[0] == '-') 
  {
      sign = -1;
      ++start;
  }

  unsigned power = 1;
  double output = 0.0;
  for(int i = index - 1; i >= start; i--)
  {
    if(m_data[i] < '0' || m_data[i] > '9')
      return 0.0;
    output += (m_data[i] - '0') * power;
    power *= 10;
  }

  power = 10;
  for(int i = index + 1; i < m_size; i++)
  {
    if(m_data[i] < '0' || m_data[i] > '9')
      return 0.0;
    output += static_cast<double>(m_data[i] - '0') / power;
    power *= 10;
  }
  return output * sign;
}

double lexicography::fast_double() const
{
  int index = find('.');
  int m_size = size();
  int sign = 1;
  
  const char* m_data = immutable_data();
  // Sign checking
  if(m_data[0] == '-') 
  {
      sign = -1;
      m_data++;
      m_size--;
  }

  unsigned power = 1;
  double output = 0.0;
  for(int i = index - 1; i >= 0; i--)
  {
    output += (m_data[i] - '0') * power;
    power *= 10;
  }

  power = 10;
  for(int i = index + 1; i < m_size; i++)
  {
    output += static_cast<double>(m_data[i] - '0') / power;
    power *= 10;
  }
  return output * sign;
}

unsigned lexicography::length() const
{
  return size();
}

bool lexicography::ends_with(const char* str) const
{
  unsigned m_size = size();
  unsigned r_size = strlen(str);
  const char* m_data = immutable_data();

  if(r_size > m_size)
    return false;

  for(unsigned i = 0; i < r_size; i++)
  {
    if(m_data[m_size - r_size + i] != str[i])
      return false;
  }

  return true;
}

bool lexicography::starts_with(const char* str) const
{
  unsigned r_size = strlen(str);
  const char* m_data = immutable_data();

  if(r_size > size())
    return false;

  for(unsigned i = 0; i < r_size; i++)
  {
    if(m_data[i] != str[i])
      return false;
  }

  return true;
}

bool lexicography::empty() const
{
  return size() == 0;
}

bool lexicography::palindrome(bool absolute) const
{
  unsigned l = 0;
  unsigned r = size() - 1;
  const char* m_data = immutable_data();

  while(l < r)
  {
    // Remove non-text
    if(!absolute)
    {
      while(r > l && (whitespace(m_data[r]) || special(m_data[r])))
      {
        r--;
      }

      while(r > l && (whitespace(m_data[l]) || special(m_data[l])))
      {
        l++;
      }
    }

    char l_c = m_data[l];
    char r_c = m_data[r];

    // Remove case sensitivity
    if(!absolute)
    {
      if(!absolute && l_c >= 'a' && l_c <= 'z')
      {
        l_c += 'A' - 'a';
      }

      if(r_c >= 'a' && r_c <= 'z')
      {
        r_c += 'A' - 'a';
      }
    }

    if(l_c != r_c)
      return false;

    r--;
    l++;
  }

  return true;
}

bool lexicography::whitespace(char chr)
{
  if(chr == ' ' || chr == '\n')
    return true;
  
  return false;
}

bool lexicography::letter(char chr)
{
  if((chr >= 'a' && chr <= 'z') || (chr >= 'A' && chr <= 'Z'))
    return true;
  
  return false;
}

bool lexicography::numeric(char chr)
{
  if(chr >= '0' && chr <= '9')
    return true;

  return false;
}

bool lexicography::special(char chr)
{
  if(!letter(chr) && !whitespace(chr) && !numeric(chr))
    return true;

  return false;
}

//=========================================================
// Better std::string
//=========================================================
string::string()
{
  data = NULL;
}

string::string(bool value)
{
  data = NULL;
  from_bool(value);
}

string::string(int value)
{
  data = NULL;
  from_int(value);
}

string::string(double value)
{
  data = NULL;
  from_double(value);
}

string::string(const char* str)
{
  // Get the size
  unsigned size = strlen(str);
  
  // Allocate our size + the size of metadata + the zero byte.
  data = reinterpret_cast<char*>(malloc(_data_key + size * 2 + 1));
  *reinterpret_cast<unsigned*>(data + _size_key) = size;
  *reinterpret_cast<unsigned*>(data + _cap_key) = size * 2;
  *reinterpret_cast<unsigned*>(data + _ref_key) = 1;

  // Copy the data
  memcpy(data + _data_key, str, size + 1);
}

string::string(const char* str, unsigned pos, unsigned len)
{
  unsigned r_size = strlen(str);
  unsigned size;
  
  data = NULL;

  if(pos > BTRSTRING_MAX)
    pos = 0;

  if(pos >= r_size || len == 0)
  {
    assign("");
    return;
  }

  if(len == -1 || pos + len >= r_size)
    size = r_size - pos;
  else
    size = len;
  
  data = reinterpret_cast<char*>(malloc(_data_key + size * 2 + 1));
  *reinterpret_cast<unsigned*>(data + _size_key) = size;
  *reinterpret_cast<unsigned*>(data + _cap_key) = size * 2;
  *reinterpret_cast<unsigned*>(data + _ref_key) = 1;

  // Copy the data
  memcpy(data + _data_key, str + pos, size);
  data[_data_key + size] = '\0';
}

string::string(const std::string& str)
{
  // Get the size
  unsigned size = str.size();
  
  // Allocate our size + the size of metadata + the zero byte.
  data = reinterpret_cast<char*>(malloc(_data_key + size * 2 + 1));
  *reinterpret_cast<unsigned*>(data + _size_key) = size;
  *reinterpret_cast<unsigned*>(data + _cap_key) = size * 2;
  *reinterpret_cast<unsigned*>(data + _ref_key) = 1;

  // Copy the data
  memcpy(data + _data_key, str.c_str(), size + 1);
}

string::string(const lexicography& str)
{
  // Get the size
  unsigned size = str.size();
  
  // Allocate our size + the size of metadata + the zero byte.
  data = reinterpret_cast<char*>(malloc(_data_key + size * 2 + 1));
  *reinterpret_cast<unsigned*>(data + _size_key) = size;
  *reinterpret_cast<unsigned*>(data + _cap_key) = size * 2;
  *reinterpret_cast<unsigned*>(data + _ref_key) = 1;

  // Copy the data
  memcpy(data + _data_key, str.c_str(), size + 1);
}


string::string(const string& rhs)
{
  // Aquire the data we are refrencing
  data = rhs.data;
  
  // Increment reference counter
  ++*reinterpret_cast<unsigned*>(data + _ref_key);
}

string::~string()
{
  // Increment reference counter
  --*reinterpret_cast<unsigned*>(data + _ref_key);
  if(*reinterpret_cast<unsigned*>(data + _ref_key) == 0)
  {
    free(data);
  }
}

string& string::operator=(const string& rhs)
{
  // Aquire the data we are refrencing
  data = rhs.data;
  
  // Increment reference counter
  ++*reinterpret_cast<unsigned*>(data + _ref_key);

  return *this;
}

void string::swap(lexicography& str)
{
  string cpy(str);
  str.assign(*this);
  assign(cpy);
}

void string::join(const std::vector<string>& strs)
{
  clear();
  for(unsigned i = 0; i < strs.size(); i++)
  {
    append(strs[i]);
    push_back(' ');
  }
}

void string::join_lines(const std::vector<string>& strs)
{
  clear();
  for(unsigned i = 0; i < strs.size(); i++)
  {
    append(strs[i]);
    push_back('\n');
  }
}

void string::format()
{
  std::vector<string> tokens = tokenize();
  
  tokens[0].capitalize();

  for(unsigned i = 1; i < tokens.size(); i++)
  {
    if(tokens[i - 1].back() == '.' || tokens[i - 1].back() == '?' ||
      tokens[i - 1].back() == '!' || (tokens[i].front() == 'i' && tokens[i].size() == 1))
    {
      tokens[i].capitalize();
    }
  }

  join(tokens);
  pop_back();
}

void string::format_lines()
{
  std::vector<string> line_tokens = tokenize(true);

  for(unsigned k = 0; k < line_tokens.size(); k++)
  {
    std::vector<string> tokens = line_tokens[k].tokenize();

    tokens[0].capitalize();

    for(unsigned i = 1; i < tokens.size(); i++)
    {
      if(tokens[i - 1].back() == '.' || tokens[i - 1].back() == '?' ||
        tokens[i - 1].back() == '!' || (tokens[i].front() == 'i' && tokens[i].size() == 1))
      {
        tokens[i].capitalize();
      }
    }

    line_tokens[k].join(tokens);
  }

  join_lines(line_tokens);
  pop_back();
}

string string::substr(unsigned pos, unsigned len) const
{
  return string(immutable_data(), pos, len);
}

string string::lowercase() const
{
  string lower(*this);
  lower.to_lower();
  return lower;
}

string string::uppercase() const
{
  string upper(*this);
  upper.to_upper();
  return upper;
}

unsigned string::size() const
{
  if(!data)
    return 0;
  return *reinterpret_cast<unsigned*>(data + _size_key);
}

unsigned string::capacity() const
{
  if(!data)
    return 0;
  return *reinterpret_cast<unsigned*>(data + _cap_key);
}

unsigned string::refrences() const
{
  if(!data)
    return 0;
  return *reinterpret_cast<unsigned*>(data + _ref_key);
}

  // Special Functions
std::vector<string> string::tokenize(bool newline_delim) const
{
  std::vector<string> tokens;
  const char* m_data = immutable_data();
  unsigned index = 0;
  int start = -1;

  while(m_data[index] != '\0')
  {
    if(!newline_delim && !whitespace(m_data[index]) ||
       newline_delim && m_data[index] != '\n')
    {
      if(start == -1)
        start = index;
    }
    else if(start != -1)
    {
      tokens.push_back(substr(start, index - start));
      start = -1;
    }
    index++;
  }

  if(start != -1)
  {
    tokens.push_back(substr(start, index - start));
  }

  return tokens;
}

char* string::mutable_data()
{
  if(!data)
    return "";

  // Check for late mutation
  if(*reinterpret_cast<unsigned*>(data + _ref_key) > 1)
  {
    char* n_data = reinterpret_cast<char*>(malloc(_data_key + *reinterpret_cast<unsigned*>(data + _size_key) * 2 + 1));
    memcpy(n_data, data, _data_key + *reinterpret_cast<unsigned*>(data + _size_key) * 2 + 1);

    --*reinterpret_cast<unsigned*>(data + _ref_key);  
    *reinterpret_cast<unsigned*>(n_data + _ref_key) = 1;
    data = n_data;
  }

  return data + _data_key;
}

const char* string::immutable_data() const
{
  if(!data)
    return "";

  return data + _data_key;
}
void string::require(unsigned bytes)
{
  if(!data || bytes + 1 > *reinterpret_cast<unsigned*>(data + _cap_key))
  {
    unsigned cap = (capacity() + bytes) * 2;
    char* n_data = reinterpret_cast<char*>(malloc(_data_key + cap + 1));

    if(data)
    {
      memcpy(n_data, data, _data_key + cap + 1);
      --*reinterpret_cast<unsigned*>(data + _ref_key);  
      if(*reinterpret_cast<unsigned*>(data + _ref_key) == 0)
      {
        free(data);
      }
    }
    
    *reinterpret_cast<unsigned*>(n_data + _cap_key) = cap;  
    *reinterpret_cast<unsigned*>(n_data + _ref_key) = 1;
    data = n_data;
  }
  *reinterpret_cast<unsigned*>(data + _size_key) = bytes;  
}

string operator+(const string& btr, const char* str)
{
  string new_str(btr);
  new_str.append(str);
  return new_str;
}

string operator+(const char* str, const string& btr)
{
  string new_str(str);
  new_str.append(btr);
  return new_str;
}

string operator+(const string& btr, int value)
{
  string new_str(btr);
  new_str.append(string(value));
  return new_str;
}

string operator+(int value, const string& btr)
{
  string new_str(value);
  new_str.append(btr);
  return new_str;
}

string operator+(const string& btr, double value)
{
  string new_str(btr);
  new_str.append(string(value));
  return new_str;
}

string operator+(double value, const string& btr)
{
  string new_str(value);
  new_str.append(btr);
  return new_str;
}

string operator+(const string& btr, bool value)
{
  string new_str(btr);
  new_str.append(string(value));
  return new_str;
}

string operator+(bool value, const string& btr)
{
  string new_str(value);
  new_str.append(btr);
  return new_str;
}
