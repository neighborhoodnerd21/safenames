//============================================================================
// Name        : safenames.cpp
// Author      : neighborhoodnerd21
// Version     : 0.0.1
// Copyright   : Public Domain? 2026
// Description : Removes spaces from file names
//============================================================================

#include "CLI11.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

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

bool hasWhitespace(const std::string &s) {
  for (unsigned char ch : s) {
    if (std::isspace(ch))
      return true;
  }
  return false;
}

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

  if (hasWhitespace(newname)) {
    replace(newname.begin(), newname.end(), ' ', alternate);
  } else {
    cout << "Source does not contain any spaces." << endl;
    cout << "Filename unchanged." << endl;
    return 0;
  }

  std::ifstream f(original.c_str());

  if (f.good()) {
    if (std::rename(filename.c_str(), newname.c_str()) == 0) {
      cout << "File: " << original << " was moved to " << newname << endl;
    } else {
      cout << "File not found!" << endl;
      return -1;
    }
  }

  return 0;
}

int main(int argc, char **argv) {

  CLI::App app{"Removes spaces from source file name."};

  // setup flags
  auto u =
      app.add_flag("--underscore, -u", underscore, "Use dashes as delimiter");
  auto p = app.add_flag("--period, -p", period, "Use periods as delimiter");

  app.add_option("--source, source", source, "Source file")->required();

  // parse arguments
  CLI11_PARSE(app, argc, argv);

  original = source;

  if (u->count() > 0 && p->count() > 0) {
    cout << "Only one flag may be used at a time!" << endl;
    return 1;
  }

  if (underscore) {
    if (u->count() > 1) {
      cout << "Flag can only be passed once!!!" << endl;
      return 1;
    } else {
      changeName(original, '1');
    }
  } else if (period) {
    if (p->count() > 1) {
      cout << "Flag can only be passed once!!!" << endl;
      return 1;
    } else {
      changeName(original, '2');
    }
  } else {
    changeName(original, '0');
  }

  return 0;
}
