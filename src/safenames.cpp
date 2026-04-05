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

//--::[ GLOBALS ]::

using namespace std;

bool underscore{false};
bool period{false};

string source;
string original;
string newname;

char alternate;

const char dash = '-';
const char dot = '.';
const char uscore = '_';

//--::[ define messages ]::

const string nospacesMsg = R"(

 Source file name does not contain spaces.

 filname left unchanged.
)";

const string notargetMsg = R"(

 You must specify a path for the target file.

 Usage:

 safenames "target\ file.txt"

 safenames -h
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
        return std::string(nospacesMsg);
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

  //--::[ parse arguments ]::

  CLI11_PARSE(app, argc, argv);

  original = source;

  //--::[ enforce restrictions ]::

  if (u->count() > 0 && p->count() > 0) {
    cout << "Only one flag may be used at a time!" << endl;
    return 1;
  }

  if (underscore) {
    if (u->count() > 1) {
      cout << "Flag can only be passed once!!!" << endl;
      return EXIT_FAILURE;
    } else {
      changeName(original, '1');
    }
  } else if (period) {
    if (p->count() > 1) {
      cout << "Flag can only be passed once!!!" << endl;
      return EXIT_FAILURE;
    } else {
      changeName(original, '2');
    }
  } else {
    changeName(original, '0');
  }

  return EXIT_SUCCESS;
}
