template <class T, T v> struct integral_constant {
  static constexpr T value = v;
  typedef T value_type;
  typedef integral_constant type;
  constexpr operator value_type() const noexcept { return value; }
  constexpr value_type operator()() const noexcept { return value; }
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

template <class T> struct is_integral : false_type {};

template <> struct is_integral<bool> : true_type {};
template <> struct is_integral<char> : true_type {};

template <> struct is_integral<signed char> : true_type {};
template <> struct is_integral<unsigned char> : true_type {};
template <> struct is_integral<short int> : true_type {};
template <> struct is_integral<unsigned short int> : true_type {};
template <> struct is_integral<int> : true_type {};
template <> struct is_integral<unsigned int> : true_type {};
template <> struct is_integral<long int> : true_type {};
template <> struct is_integral<unsigned long int> : true_type {};
template <> struct is_integral<long long int> : true_type {};
template <> struct is_integral<unsigned long long int> : true_type {};

template <class T> struct is_floating_point : false_type {};

template <> struct is_floating_point<float> : true_type {};
template <> struct is_floating_point<double> : true_type {};

template <class T>
struct is_arithmetic
    : integral_constant<bool, is_integral<T>::value ||
                                  is_floating_point<T>::value> {};

static_assert(is_integral<bool>::value == true, "assertion failed");
static_assert(is_integral<char>::value == true, "assertion failed");
static_assert(is_integral<signed char>::value == true, "assertion failed");
static_assert(is_integral<unsigned char>::value == true, "assertion failed");
static_assert(is_integral<short>::value == true, "assertion failed");
static_assert(is_integral<signed short>::value == true, "assertion failed");
static_assert(is_integral<unsigned short>::value == true, "assertion failed");
static_assert(is_integral<short int>::value == true, "assertion failed");
static_assert(is_integral<signed short int>::value == true, "assertion failed");
static_assert(is_integral<unsigned short int>::value == true,
              "assertion failed");
static_assert(is_integral<int>::value == true, "assertion failed");
static_assert(is_integral<signed int>::value == true, "assertion failed");
static_assert(is_integral<unsigned int>::value == true, "assertion failed");
static_assert(is_integral<long>::value == true, "assertion failed");
static_assert(is_integral<signed long>::value == true, "assertion failed");
static_assert(is_integral<unsigned long>::value == true, "assertion failed");
static_assert(is_integral<long int>::value == true, "assertion failed");
static_assert(is_integral<signed long int>::value == true, "assertion failed");
static_assert(is_integral<unsigned long int>::value == true,
              "assertion failed");
static_assert(is_integral<long long>::value == true, "assertion failed");
static_assert(is_integral<signed long long>::value == true, "assertion failed");
static_assert(is_integral<unsigned long long>::value == true,
              "assertion failed");
static_assert(is_integral<long long int>::value == true, "assertion failed");
static_assert(is_integral<signed long long int>::value == true,
              "assertion failed");
static_assert(is_integral<unsigned long long int>::value == true,
              "assertion failed");
