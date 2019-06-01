/*
 * Other functions used throughout the program.
 */

// sleep
void wait(ldouble value, byte unit) {
  out->flush();
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

// get unicode character from integer and put it in 'chars'
void get_unicode_char(unsigned code, char chars[5]) {
  /* 'chars' is an array containing the ASCII characters that represents the Unicode character */
  
  if (code <= 0x7f) {
    chars[0] = code;
    chars[1] = 0;
    chars[2] = 0;
    chars[3] = 0;
    chars[4] = 0;
  }
  else if (code <= 0x7ff) {
    // one continuation byte
    chars[0] = 0xc0 | (code & 0x1f);
    chars[1] = 0x80 | (code & 0x3f);
    code = (code >> 6);
    chars[2] = 0;
    chars[3] = 0;
    chars[4] = 0;
  }
  else if (code <= 0xFFFF) {
    // two continuation bytes
    chars[0] = 0xe0 | (code & 0xf);
    chars[1] = 0x80 | (code & 0x3f);
    code = (code >> 6);
    chars[2] = 0x80 | (code & 0x3f);
    code = (code >> 6);
    chars[3] = 0;
    chars[4] = 0;
  }
  else if (code <= 0x10ffff) {
    // three continuation bytes
    chars[0] = 0xf0 | (code & 0x7);
    chars[1] = 0x80 | (code & 0x3f);
    code = (code >> 6);
    chars[2] = 0x80 | (code & 0x3f);
    code = (code >> 6);
    chars[3] = 0x80 | (code & 0x3f);
    code = (code >> 6);
    chars[4] = 0;
  }
  else {
    // unicode replacement character
    chars[0] = 0xbd;
    chars[1] = 0xbf;
    chars[2] = 0xef;
    chars[3] = 0;
    chars[4] = 0;
  }
}
