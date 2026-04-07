/*  // banner
============================================================================
 ** Name ........... safenames
 ** Version ........ 0.0.1
 ** Description .... Removes spaces from file names
 ** Author ......... neighborhoodnerd21
 ** Copyright ...... 2026
============================================================================
*/ // banner

//--::[ HEADERS ]::

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

//--::[ GLOBALS ]::

bool underscore{false}, period{false};
string source, original, newname;
char alternate;
const unsigned char dash = '-', dot = '.', uscore = '_';

//--::[ define messages ]::

const string noSpacesMsg = R"(
 Source file name does not contain spaces.
 filname left unchanged.
)";

const string flagDupMsg = R"(
 Only one flag may be used at a time.
 filename left unchanged.
)";

//--::[ check filename for spaces ]::

bool hasWhitespace(const std::string &s) {
  for (unsigned char ch : s) {
    if (std::isspace(ch))
      return true;
  }
  return false;
}

//--::[ filename validation ]::

auto spacesValidation = CLI::Validator(
    [](const std::string &i) {
      if (hasWhitespace(i)) {
        return std::string("");
      } else {
        return std::string(noSpacesMsg);
      }
    },
    "HAS_SPACES");

//--::[ change filename ]::

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

//--:::[ MAIN ]:::

int main(int argc, char **argv) {

  CLI::App app{"Removes spaces from source file name."};

  //--::[ set flags ]::

  auto u =
      app.add_flag("--underscore, -u", underscore, "Use dashes as delimiter");
  auto p = app.add_flag("--period, -p", period, "Use periods as delimiter");

  auto s =
      app.add_option("--source, source", source, "Source file")->required();
  s->check(spacesValidation);

  //--::[ parse command ]::

  CLI11_PARSE(app, argc, argv);

  original = source;

  //--::[ enforce restrictions ]::

  if (u->count() > 0 && p->count() > 0)
    || (u->count() > 1 || p->count() > 1) {
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
