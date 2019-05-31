#include <getopt.h>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>

typedef unsigned char byte;
typedef unsigned long long ullong;
typedef long double ldouble;

namespace units {
  enum: byte {
    YEARS,
    MONTHS,
    DAYS,
    HOURS,
    MINS,
    SECS,
    MSECS,
    USECS,
    NSECS
  };
}

std::ostream* out = &std::cout;
std::string msg;
struct option longopts[] = {
  {"help", no_argument, 0, 'h'},
  {"version", no_argument, 0, 'v'},
  {"delay", required_argument, 0, 'd'},
  {"timeout", required_argument, 0, 't'},
  {"line", no_argument, 0, 'n'},
  {"escape", no_argument, 0, 'e'},
  {"error", no_argument, 0, 'E'},
  {0, 0, 0, 0},
};
std::string unit_strings[] = {"y", "m", "d", "h", "min", "s", "ms", "us", "ns"};
int opt, longindex;
ldouble
  delay = 0,
  timeout = 0;
byte
  delay_unit = units::MSECS,
  timeout_unit = units::MSECS;
bool
  newline = true,
  escape = false,
  error = false,
  colors = false,
  attributes = false;

#include "help.hpp"
#include "res.hpp"
#include "print.hpp"
