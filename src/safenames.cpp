/*
============================================================================
 ** Name ........... safenames
 ** Version ........ 0.0.1
 ** Description .... Removes spaces from file names
 ** Author ......... neighborhoodnerd21
 ** Copyright ...... 2026
============================================================================
*/

/* --[ HEADERS ]-- */

#include "CLI11.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

using namespace std;

/* --[ GLOBALS ]-- */

bool underscore{false}, period{false};
string source, original, newname;
char alternate;
const unsigned char dash = '-', dot = '.', uscore = '_';

/* --[ MESSAGES ]-- */

const string description = R"(
 $ safenames

 replaces all whitespace characters in a filename with
 aternative characters.
)";

const string noSpacesMsg = R"(
 Source file name does not contain spaces.
 filname left unchanged.
)";

const string flagDupMsg = R"(
 Only one flag may be used at a time.
 filename left unchanged.
)";

/* --[ VALIDATOR ]-- */

struct WhiteSpaceValidator : public CLI::Validator {
  WhiteSpaceValidator() {
    name_ = "SPACES", func_ = [](const std::string &str) {
      for (unsigned char ch : str) {
        if (std::isspace(ch))
          return std::string();
      }
      return std::string(noSpacesMsg);
    };
  }
};

const static WhiteSpaceValidator hasWhiteSpace;

/* --[ RENAME ]-- */

int changeName(string filename, int altValue) {

  switch (altValue) {
  case '1':
    alternate = uscore;
    break;
  case '2':
    alternate = dot;
    break;
  default:
    alternate = dash;
    break;
  }

  newname = filename;

  replace(newname.begin(), newname.end(), ' ', alternate);

  std::ifstream f(original.c_str());

  if (f.good()) {
    if (std::rename(filename.c_str(), newname.c_str()) == 0) {
      cout << "File: " << original << " was moved to " << newname << endl;
    } else {
      cout << "File not found!" << endl;
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}

/* --[ MAIN ]-- */

int main(int argc, char **argv) {

  CLI::App app{description};

  app.get_formatter()->enable_option_type_names(false);

  auto u =
      app.add_flag("--underscore, -u", underscore, "Use dashes as delimiter");
  auto p = app.add_flag("--period, -p", period, "Use periods as delimiter");

  auto s =
      app.add_option("--source, source", source, "Source file")->required();

  s->check(hasWhiteSpace);

  CLI11_PARSE(app, argc, argv);

  original = source;

  if ((u->count() > 0 && p->count() > 0) ||
      (u->count() > 1 || p->count() > 1)) {
    cout << flagDupMsg << endl;
    return EXIT_FAILURE;
  }

  if (underscore) {
    changeName(original, '1');
  } else if (period) {
    changeName(original, '2');
  } else {
    changeName(original, '0');
  }

  return EXIT_SUCCESS;
}
