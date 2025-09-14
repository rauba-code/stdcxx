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

typename string::iterator string::begin() {
    return this->_ptr;
}

typename string::iterator string::end() {
    return this->_ptr + this->_sz;
}
