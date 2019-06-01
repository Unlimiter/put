/*
 * Put - Print arguments to the standard output.
 * Copyright (c) 2019 Unlimiter
 */

#include "put.hpp"

int main(int argc, char** argv) {
  std::string s;
  std::string unit;
  bool scanning_unit = false;
  bool is_float = false;
  bool float_error = false;
  std::string fg, bg;
  while ((opt = getopt_long(argc, argv, "+:hvd:t:neE", longopts, &longindex)) != -1) {
    switch (opt) {
      case 'h':
        print_help();
        exit(0);
        break;
      case 'v':
        std::cout << "put v2.0.8-a.0\nCopyright (c) 2019 Unlimiter\n";
        exit(0);
        break;
      case 'd':
        for (auto c : std::string(optarg)) {
          if (!scanning_unit) {
            if (c >= '0' && c <= '9')
              s += c;
            else if (c == '.') {
              s += '.';
              if (is_float)
                float_error = true;
              else {
                is_float = true;
              }
            }
            else
              scanning_unit = true;
          }
          if (scanning_unit) {
            unit += c;
          }
        }
        if (float_error) {
          std::cerr << "put: invalid number: " << s << '\n';
          exit(1);
        }
        if (unit.empty())
          unit = "ms";
        if (std::find(std::begin(unit_strings), std::end(unit_strings), unit) == std::end(unit_strings)) {
          std::cerr << "put: invalid time unit: " << unit << '\n';
          print_units();
          exit(1);
        }
        delay = strtold(s.c_str(), 0);
        if (unit == "y")
          delay_unit = units::YEARS;
        if (unit == "m")
          delay_unit = units::MONTHS;
        else if (unit == "d")
          delay_unit = units::DAYS;
        else if (unit == "h")
          delay_unit = units::HOURS;
        else if (unit == "min")
          delay_unit = units::MINS;
        else if (unit == "s")
          delay_unit = units::SECS;
        else if (unit == "ms")
          delay_unit = units::MSECS;
        else if (unit == "us")
          delay_unit = units::USECS;
        else if (unit == "ns")
          delay_unit = units::NSECS;
        s.clear();
        unit.clear();
        scanning_unit = false;
        is_float = false;
        float_error = false;
        break;
      case 't':
        for (auto c : std::string(optarg)) {
          if (!scanning_unit) {
            if (c >= '0' && c <= '9')
              s += c;
            else if (c == '.') {
              s += '.';
              if (is_float)
                float_error = true;
              else {
                is_float = true;
              }
            }
            else
              scanning_unit = true;
          }
          if (scanning_unit) {
            unit += c;
          }
        }
        if (float_error) {
          std::cerr << "put: invalid number: " << s << '\n';
          exit(1);
        }
        if (unit.empty())
          unit = "ms";
        if (std::find(std::begin(unit_strings), std::end(unit_strings), unit) == std::end(unit_strings)) {
          std::cerr << "put: invalid time unit: " << unit << '\n';
          print_units();
          exit(1);
        }
        timeout = strtold(s.c_str(), 0);
        if (unit == "y")
          delay_unit = units::YEARS;
        if (unit == "m")
          timeout_unit = units::MONTHS;
        else if (unit == "d")
          timeout_unit = units::DAYS;
        else if (unit == "h")
          timeout_unit = units::HOURS;
        else if (unit == "min")
          timeout_unit = units::MINS;
        else if (unit == "s")
          timeout_unit = units::SECS;
        else if (unit == "ms")
          timeout_unit = units::MSECS;
        else if (unit == "us")
          timeout_unit = units::USECS;
        else if (unit == "ns")
          timeout_unit = units::NSECS;
        s.clear();
        unit.clear();
        scanning_unit = false;
        is_float = false;
        float_error = false;
        break;
      case 'n':
        newline = false;
        break;
      case 'e':
        escape = true;
        break;
      case 'E':
        error = true;
        out = &std::cerr;
        break;
      case '?':
        if (optopt)
          std::cerr << "put: invalid option: " << (char)optopt << '\n';
        else
          std::cerr << "put: invalid option: " << argv[optind-1] << '\n';
        print_usage();
        exit(1);
      case ':':
        std::cerr << "put: requires argument: " << (char)optopt << '\n';
        print_usage();
        exit(1);
    }
  }
  for (int i = optind; i < argc; i++) {
    msg += argv[i];
    if (i + 1 != argc)
      msg += ' ';
  }
  if (escape)
    print_escape();
  else
    print();
}
