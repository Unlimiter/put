#include <getopt.h>
#include <cstdlib>
#include <cstring>
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

void print(std::string msg) {
  if (error) {
    if (delay) {
      for (auto c : msg) {
        std::cerr << c;
        std::cerr.flush();
        wait(delay, delay_unit);
      }
    }
    else
      std::cerr << msg;
    if (timeout) {
      std::cerr.flush();
      wait(timeout, timeout_unit);
    }
    std::cerr << "\033[0m";
  }
  else {
    if (delay) {
      for (auto c : msg) {
        std::cout << c;
        std::cout.flush();
        wait(delay, delay_unit);
      }
    }
    else
      std::cout << msg;
    if (timeout) {
      std::cout.flush();
      wait(timeout, timeout_unit);
    }
    std::cout << "\033[0m";
  }
	if (newline)
    std::cout << '\n';
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
  /*"    \\NNN         the character with octal value NNN\n"
    "    \\xNN         the character with hexadecimal value NN\n"
    "    \\uNNNN       the character with hexadecimal value NNNN\n"*/
    "    \\cNNN        start printing with foreground color NNN, where it is a binary number\n"
    "    \\CNNN        start printing with background color NNN, where it is a binary number\n"
    "    \\dN          start printing with text attribute N, where it can be 0-5\n"
    "\n"
    "  Supported Time Units:\n"
    "    y    years           31536000s\n"
    "    m    months           2628000s\n"
    "    d    days               86400s\n"
    "    h    hours               3600s\n"
    "    min  minutes               60s\n"
    "    s    seconds                1s\n"
    "    ms   milliseconds        1e-3s\n"
    "    us   microseconds        1e-6s\n"
    "    ns   nanoseconds         1e-9s\n"
    "\n"
    "  Colors:\n"
    "    000  black\n"
    "    001  blue\n"
    "    010  green\n"
    "    011  cyan\n"
    "    100  red\n"
    "    101  magenta\n"
    "    110  yellow\n"
    "    111  white\n"
    "\n"
    "  Attributes:\n"
    "    0  no attribute\n"
    "    1  bold\n"
    "    2  underlined (on monochrome display adapters only)\n"
    "    3  blinking\n"
    "    4  inverted\n"
    "    5  concealed\n"
    ;
}

void print_units() {
  std::cout << "time units: y m d h s ms us ns\n";
}

bool isbdigit(char c) {
  if (c == '0' || c == '1')
    return true;
  return false;
}

bool isodigit(char c) {
  return c >= '0' && c <= '7';
}

void handle_escape() {
  /*replace_all(msg, "\\a", "\a");
  replace_all(msg, "\\b", "\b");
  replace_all(msg, "\\e", "\e");
  replace_all(msg, "\\f", "\f");
  replace_all(msg, "\\n", "\n");
  replace_all(msg, "\\r", "\r");
  replace_all(msg, "\\t", "\t");
  replace_all(msg, "\\v", "\v");
  replace_all(msg, "\\\\", "\\");
  replace_all(msg, "\\'", "\'");
  replace_all(msg, "\\\"", "\"");*/
  
  // I will support \NNN, \xNN and \uNNNN escape sequences in the future
  auto msg_length = msg.length();
  for (unsigned i = 0; i < msg_length; i++) {
    if (msg[i] == '\\') {
      switch (msg[i+1]) {
        case 'a':
          msg.replace(i, 1, "\a");
          break;
        case 'b':
          msg.replace(i, 1, "\b");
          break;
        case 'e':
          msg.replace(i, 1, "\e");
          break;
        case 'f':
          msg.replace(i, 1, "\f");
          break;
        case 'n':
          msg.replace(i, 1, "\n");
          break;
        case 'r':
          msg.replace(i, 1, "\r");
          break;
        case 't':
          msg.replace(i, 1, "\t");
          break;
        case 'v':
          msg.replace(i, 1, "\v");
          break;
        case '\\':
          msg.replace(i, 1, "\\");
          break;
        case '\'':
          msg.replace(i, 1, "'");
          break;
        case '"':
          msg.replace(i, 1, "\"");
          break;
        // foreground
        case 'c':
          if (msg_length > i + 4 && isbdigit(msg[i+2]) && isbdigit(msg[i+3]) && isbdigit(msg[i+4])) {
            if (msg[i+2] == '0') {
              if (msg[i+3] == '0') {
                if (msg[i+4] == '0')
                  // 000 (black)
                  msg.replace(i, 5, "\033[30m");
                else
                  // 001 (blue)
                  msg.replace(i, 5, "\033[34m");
              }
              else {
                if (msg[i+4] == '0')
                  // 010 (green)
                  msg.replace(i, 5, "\033[32m");
                else
                  // 011 (cyan)
                  msg.replace(i, 5, "\033[36m");
              }
            }
            else {
              if (msg[i+3] == '0') {
                if (msg[i+4] == '0')
                  // 100 (red)
                  msg.replace(i, 5, "\033[31m");
                else
                  // 101 (magenta)
                  msg.replace(i, 5, "\033[35m");
              }
              else {
                if (msg[i+4] == '0')
                  // 110 (yellow)
                  msg.replace(i, 5, "\033[33m");
                else
                  // 111 (white)
                  msg.replace(i, 5, "\033[37m");
              }
            }
            i += 2;
          }
          break;
        // background
        case 'C':
          if (msg_length > i + 4 && isbdigit(msg[i+2]) && isbdigit(msg[i+3]) && isbdigit(msg[i+4])) {
            if (msg[i+2] == '0') {
              if (msg[i+3] == '0') {
                if (msg[i+4] == '0')
                  // 000 (black)
                  msg.replace(i, 5, "\033[40m");
                else
                  // 001 (blue)
                  msg.replace(i, 5, "\033[44m");
              }
              else {
                if (msg[i+4] == '0')
                  // 010 (green)
                  msg.replace(i, 5, "\033[42m");
                else
                  // 011 (cyan)
                  msg.replace(i, 5, "\033[46m");
              }
            }
            else {
              if (msg[i+3] == '0') {
                if (msg[i+4] == '0')
                  // 100 (red)
                  msg.replace(i, 5, "\033[41m");
                else
                  // 101 (magenta)
                  msg.replace(i, 5, "\033[45m");
              }
              else {
                if (msg[i+4] == '0')
                  // 110 (yellow)
                  msg.replace(i, 5, "\033[43m");
                else
                  // 111 (white)
                  msg.replace(i, 5, "\033[47m");
              }
            }
            i += 2;
          }
          break;
        // attribute/decoration
        case 'd':
          if (msg[i+2] >= '0' && msg[i+2] <= '5') {
            switch (msg[i+2]) {
              case '0':
                msg.replace(i, 3, "\033[0m");
                break;
              case '1':
                msg.replace(i, 3, "\033[1m");
                break;
              case '2':
                msg.replace(i, 3, "\033[4m");
                break;
              case '3':
                msg.replace(i, 3, "\033[5m");
                break;
              case '4':
                msg.replace(i, 3, "\033[7m");
                break;
              case '5':
                msg.replace(i, 3, "\033[8m");
                break;
            }
            i++;
          }
          break;
      }
      i++;
    }
  }
}
