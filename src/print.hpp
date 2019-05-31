/*
 * Functions for writing to the standard output/error.
 */

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
  char chars[5];
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
            get_unicode_char(
              htod(msg[i+2]) * 16 + htod(msg[i+3]),
              chars
            );
            *out << chars;
            i += 2;
          }
          else
            goto default_;
          break;
        case 'u':
          if (
            isxdigit(msg[i+2])
            && isxdigit(msg[i+3])
            && isxdigit(msg[i+4])
            && isxdigit(msg[i+5])
          ) {
            get_unicode_char(
              htod(msg[i+2]) * 4096
                + htod(msg[i+3]) * 256
                + htod(msg[i+4]) * 16
                + htod(msg[i+5]),
              chars
            );
            *out << chars;
            i += 4;
          }
          else
            goto default_;
          break;
        case 'U':
          if (
            isxdigit(msg[i+2])
            && isxdigit(msg[i+3])
            && isxdigit(msg[i+4])
            && isxdigit(msg[i+5])
            && isxdigit(msg[i+6])
            && isxdigit(msg[i+7])
            && isxdigit(msg[i+8])
            && isxdigit(msg[i+9])
          ) {
            get_unicode_char(
              htod(msg[i+2]) * 268435456
                + htod(msg[i+3]) * 16777216
                + htod(msg[i+4]) * 1048576
                + htod(msg[i+5]) * 65536
                + htod(msg[i+6]) * 4096
                + htod(msg[i+7]) * 256
                + htod(msg[i+8]) * 16
                + htod(msg[i+9]),
              chars
            );
            *out << chars;
            i += 8;
          }
          else
            goto default_;
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
          else
            goto default_;
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
          else
            goto default_;
          break;
        default:
        // label for printing escape sequences literally if not valid
        default_:
          *out << '\\' << msg[i+1];
          out->flush();
          if (delay)
            wait(delay, delay_unit);
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
