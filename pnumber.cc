#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

class invalid_pnumber : public std::invalid_argument {
public:
  explicit invalid_pnumber(const std::string &message)
      : std::invalid_argument(message) {}
};
class invalid_radix : public std::invalid_argument {
public:
  explicit invalid_radix(const std::string &message)
      : std::invalid_argument(message) {}
};
class invalid_precision : public std::invalid_argument {
public:
  explicit invalid_precision(const std::string &message)
      : std::invalid_argument(message) {}
};

class TPNumber {
public:
  TPNumber(double n, unsigned b, unsigned c) {
    number = n;
    radix = b;
    precision = c;
  }
  TPNumber(const std::string &n, const std::string &b, const std::string &c) {
    SetRadix(parse_radix(b));
    precision = parse_precision(c);
    number = parse_number(n, radix);
  }
  friend std::ostream &operator<<(std::ostream &out, const TPNumber &p) {
    out << p.ToString();
    return out;
  }
  TPNumber operator+(const TPNumber &rhs) const {
    return {number + rhs.number, radix, precision};
  }
  TPNumber operator-(const TPNumber &rhs) const {
    return {number - rhs.number, radix, precision};
  }
  TPNumber operator*(const TPNumber &rhs) const {
    return {number * rhs.number, radix, precision};
  }
  TPNumber operator/(const TPNumber &rhs) const {
    return {number / rhs.number, radix, precision};
  }
  bool operator==(const TPNumber &rhs) const { return number == rhs.number; }
  bool operator!=(const TPNumber &rhs) const { return number != rhs.number; }
  TPNumber operator!() const { return {1.0 / number, radix, precision}; }

  TPNumber pow2() const { return {number * number, radix, precision}; }

  double GetNumber() const { return number; }
  unsigned GetRadix() const { return radix; }
  std::string GetRadixAsStr() const { return std::to_string(radix); }
  unsigned GetPrecision() const { return precision; }
  std::string GetPrecisionAsStr() const { return std::to_string(precision); }

  std::string ToString() const {
    std::string result;
    if (radix == 10) {
      std::stringstream sresult(result);
      sresult << std::fixed << std::setprecision(precision) << number;
      return sresult.str();
    }
    static const char alphabet[] = "0123456789ABCDEF";
    unsigned n = (unsigned)number;
    double fdouble = (number - n);
    while (n) {
      result += alphabet[n % radix];
      n /= radix;
    }
    std::reverse(result.begin(), result.end());

    if (precision > 0) {
      char fstring[17]; // 0.<double guaranteed precision 15 digits>
      sprintf(fstring, "%.15f", fdouble);
      std::string fs(fstring);
      std::vector<int> fracVec;
      transform(fs.begin() + 2 /*skip 0.*/, fs.end(),
                std::back_inserter(fracVec),
                [](unsigned char c) -> int { return c - '0'; });

      fs.clear();
      fs.resize(precision, '0');
      for (unsigned i = 0;
           std::count(fracVec.begin(), fracVec.end(), 0) && i < precision;
           i++) {
        int carry = 0;

        for (int j = fracVec.size() - 1; j >= 0; j--) {
          int digit = fracVec[j] * radix + carry;

          if (digit > 9) {
            carry = digit / 10;
            digit %= 10;
          } else {
            carry = 0;
          }

          fracVec[j] = digit;
        }
        fs[i] = alphabet[carry];
      }
      result += '.' + fs;
    }
    return result;
  }

  void SetRadix(unsigned r) {
    if (r < 2 or r > 16) {
      throw invalid_radix(std::to_string(r));
    }
    radix = r;
  }
  void SetRadixAsStr(const std::string &rs) { SetRadix(parse_radix(rs)); }
  void SetPrecision(unsigned p) { precision = p; }
  void SetPrecisionAsStr(const std::string &ps) {
    precision = parse_precision(ps);
  }

private:
  double number;
  unsigned radix;
  unsigned precision;
  unsigned parse_radix(const std::string &rs) const {
    if (rs.size() > 0 && rs[0] != '-') {
      char *nend;
      unsigned r = strtoul(rs.c_str(), &nend, 10);
      if (*nend == '\0') {
        return r;
      }
    }
    // negative or not fully parsed
    throw invalid_radix(rs);
  };
  unsigned parse_precision(const std::string &ps) const {
    if (ps.size() > 0 && ps[0] != '-') {
      char *nend;
      unsigned p = strtoul(ps.c_str(), &nend, 10);
      if (*nend == '\0') {
        return p;
      }
    }
    // negative or not fully parsed
    throw invalid_precision(ps);
  }
  double parse_number(const std::string &ns, unsigned base) const {
    char *nend;
    double n = strtol(ns.c_str(), &nend, base);
    if (*nend == '.' && *(++nend) != '\0') {
      char *dot = nend;
      double fractional = strtol(nend, &nend, radix);
      if (*nend == '\0') {
        n += fractional / (double)pow(base, nend - dot);
      }
    }
    if (*nend != '\0') {
      throw invalid_pnumber(ns);
    }
    return n;
  }
};
