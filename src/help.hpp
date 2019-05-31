/*
 * Functions used to display useful information
 */

void print_units() {
  std::cout << "time units: y m d h s ms us ns\n";
}

void print_usage() {
  std::cout << "usage: put [-Eehnv] [-d <time>] [-t <time>] [<argument>...]\n";
}

void print_help() {
  std::cout <<
    "usage: put [<option>...] [<argument>...]\n"
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
    "    \\a          alert/bell/beep        07\n"
    "    \\b          backspace              08\n"
    "    \\e          escape character       1B\n"
    "    \\f          formfeed               0C\n"
    "    \\n          newline                0A\n"
    "    \\r          carriage return        0D\n"
    "    \\t          horizontal tab         09\n"
    "    \\v          vertical tab           0B\n"
    "    \\\\          backslash              5C\n"
    "    \\'          single quotation mark  27\n"
    "    \\\"          double quotation mark  22\n"
    "\n"
    "    \\xHH        the character with hexadecimal value HH\n"
    "    \\uHHHH      the character with hexadecimal value HHHH\n"
    "    \\UHHHHHHHH  the character with hexadecimal value HHHHHHHH\n"
    "    \\cHH        start using foreground color with hexadecimal value HH\n"
    "    \\CHH        start using background color with hexadecimal value HH\n"
    "    \\dN         start using text attribute N (0-5)\n"
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
