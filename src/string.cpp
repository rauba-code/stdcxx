#include "string.h"

string::string() {
  this->_ptr = nullptr;
  this->_sz = 0;
  this->_is_owner = false;
}

string::string(const char *s) {
  this->_ptr = const_cast<char *>(s);
  this->_sz = strlen(s);
  this->_is_owner = false;
}

string::string(const string &s) {
  this->_ptr = s._ptr;
  this->_sz = s._sz;
  this->_is_owner = false;
}

string::~string() {
  if (this->_is_owner) {
    delete[] this->_ptr;
  }
}

typename string::iterator string::begin() const {
    return this->_ptr;
}

typename string::iterator string::end() const {
    return this->_ptr + this->_sz;
}

typename string::const_pointer string::c_str() const {
    return const_cast<const value_type*>(this->_ptr);
}

typename string::size_type string::size() const {
    return this->_sz;
}

typename string::size_type string::length() const {
    return this->_sz;
}

typename string::reference string::operator[](string::size_type i) const {
    return this->_ptr[i];
}

bool operator==(const string &a, const string &b) {
    if (a.length() != b.length()) {
        return false;
    }
    for (size_t i = 0; i < a.length(); i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}
