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

void wait(ldouble value, byte unit) {
  switch (unit) {
    case units::YEARS:
      std::this_thread::sleep_for(std::chrono::duration<ldouble, std::ratio<31536000, 1>>(value));
      break;
    case units::MONTHS:
      std::this_thread::sleep_for(std::chrono::duration<ldouble, std::ratio<2628000, 1>>(value));
      break;
    case units::DAYS:
      std::this_thread::sleep_for(std::chrono::duration<ldouble, std::ratio<86400, 1>>(value));
      break;
    case units::HOURS:
      std::this_thread::sleep_for(std::chrono::duration<ldouble, std::ratio<3600, 1>>(value));
      break;
    case units::MINS:
      std::this_thread::sleep_for(std::chrono::duration<ldouble, std::ratio<60, 1>>(value));
      break;
    case units::SECS:
      std::this_thread::sleep_for(std::chrono::duration<ldouble>(value));
      break;
    case units::MSECS:
      std::this_thread::sleep_for(std::chrono::duration<ldouble, std::ratio<1, 1000>>(value));
      break;
    case units::USECS:
      std::this_thread::sleep_for(std::chrono::duration<ldouble, std::ratio<1, 1000000>>(value));
      break;
    case units::NSECS:
      std::this_thread::sleep_for(std::chrono::duration<ldouble, std::ratio<1, 1000000000>>(value));
      break;
  }
}

void print_units() {
  std::cout << "time units: y m d h s ms us ns\n";
}

void print_usage() {
  std::cout << "usage: put [-hvneE|-d=DELAY|-t=TIMEOUT] [ARG...]\n";
}

void print_help() {
  std::cout <<
    "usage: put [OPTION] [ARG...]\n"
    "  Print ARGs to the standard output.\n"
    "\n"
    "  Options:\n"
    "    -h, --help             Print usage and exit\n"
    "    -v, --version          Print version and exit\n"
    "    -n, --line             Do not print newline at the end\n"
    "    -e, --escape           Interpret escape sequences\n"
    "    -E, --error            Print to the standard error output\n"
    "\n"
    "    -d, --delay DELAY      Set DELAY as the delay between each character output\n"
    "    -t, --timeout TIMEOUT  Set TIMEOUT as the timeout before exiting\n"
    "\n"
    "  Escape sequences:\n"
    "    \\a           alert/bell/beep        07\n"
    "    \\b           backspace              08\n"
    "    \\e           escape character       1B\n"
    "    \\f           formfeed               0C\n"
    "    \\n           newline                0A\n"
    "    \\r           carriage return        0D\n"
    "    \\t           horizontal tab         09\n"
    "    \\v           vertical tab           0B\n"
    "    \\\\           backslash              5C\n"
    "    \\'           single quotation mark  27\n"
    "    \\\"           double quotation mark  22\n"
    "    \\xHH         the ASCII character with hexadecimal value HH\n"
    "    \\cHH         start using foreground color with hexadecimal value HH\n"
    "    \\CHH         start using background color with hexadecimal value HH\n"
    "    \\dN          start using text attribute N (0-5)\n"
    "\n"
    "  Time Units:\n"
    "    y    years (common)  31536000s\n"
    "    m    months (common)  2628000s\n"
    "    d    days               86400s\n"
    "    h    hours               3600s\n"
    "    min  minutes               60s\n"
    "    s    seconds                1s\n"
    "    ms   milliseconds        1000s\n"
    "    us   microseconds        1e+6s\n"
    "    ns   nanoseconds         1e+9s\n"
    "\n"
    "  Colors (on 256-color terminals only):\n"
    "    Ranging from 0x00 to 0xff.\n"
    "\n"
    "  Text Attributes:\n"
    "    0  no attribute\n"
    "    1  bold\n"
    "    2  underlined (on monochrome display adapters only)\n"
    "    3  blinking\n"
    "    4  color-inverted\n"
    "    5  concealed\n"
    ;
}

// hex to decimal
byte htod(char c) {
  if (isdigit(c))
    return c - '0';
  switch (tolower(c)) {
    case 'a':
      return 10;
    case 'b':
      return 11;
    case 'c':
      return 12;
    case 'd':
      return 13;
    case 'e':
      return 14;
    case 'f':
      return 15;
    default:
      return 0;
  }
}

void print() {
  if (delay) {
    for (auto c : msg) {
      *out << c;
      out->flush();
      wait(delay, delay_unit);
    }
  }
  else
    *out << msg;
  if (timeout) {
    out->flush();
    wait(timeout, timeout_unit);
  }
  *out << "\e[0m";
	if (newline)
    *out << '\n';
}

void print_escape() {
  unsigned color = 0;
  for (unsigned i = 0; msg[i] != 0; i++) {
    if (msg[i] == '\\') {
      switch (msg[i+1]) {
        case 'a':
          *out << '\a';
          if (delay)
            wait(delay, delay_unit);
          break;
        case 'b':
          *out << '\b';
          if (delay)
            wait(delay, delay_unit);
          break;
        case 'e':
          *out << '\e';
          break;
        case 'f':
          *out << '\f';
          if (delay)
            wait(delay, delay_unit);
          break;
        case 'n':
          *out << '\n';
          if (delay)
            wait(delay, delay_unit);
          break;
        case 'r':
          *out << '\r';
          if (delay)
            wait(delay, delay_unit);
          break;
        case 't':
          *out << '\t';
          if (delay)
            wait(delay, delay_unit);
          break;
        case 'v':
          *out << '\v';
          if (delay)
            wait(delay, delay_unit);
          break;
        case '\\':
          *out << '\\';
          if (delay)
            wait(delay, delay_unit);
          break;
        case '\'':
          *out << '\'';
          if (delay)
            wait(delay, delay_unit);
          break;
        case '"':
          *out << '"';
          if (delay)
            wait(delay, delay_unit);
          break;
        case 'x':
          if (isxdigit(msg[i+2]) && isxdigit(msg[i+3])) {
            *out
              << (char)(
                htod(msg[i+2]) * 16
                + htod(msg[i+3])
              );
            i += 2;
          }
          break;
        // foreground
        case 'c':
        // background
        case 'C':
          if (isxdigit(msg[i+2]) && isxdigit(msg[i+3])) {
            color
              = htod(msg[i+2]) * 16
              + htod(msg[i+3]);
            *out
              << (msg[i+1] == 'c' ? "\e[38;5;" : "\e[48;5;")
              + std::to_string(color)
              + "m";
            i += 2;
            color = 0;
          }
          break;
        // attribute/decoration
        case 'd':
          if (msg[i+2] >= '0' && msg[i+2] <= '5') {
            switch (msg[i+2]) {
              case '0':
                *out << "\e[0m";
                break;
              case '1':
                *out << "\e[1m";
                break;
              case '2':
                *out << "\e[4m";
                break;
              case '3':
                *out << "\e[5m";
                break;
              case '4':
                *out << "\e[7m";
                break;
              case '5':
                *out << "\e[8m";
                break;
            }
            i++;
          }
          break;
      }
      i++;
    }
    else {
      *out << msg[i];
      out->flush();
      if (delay)
        wait(delay, delay_unit);
    }
  }
  if (timeout) {
    out->flush();
    wait(timeout, timeout_unit);
  }
  *out << "\e[0m";
  if (newline)
    *out << '\n';
}
