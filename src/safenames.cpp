//============================================================================
// Name        : safenames.cpp
// Author      : neighborhoodnerd21
// Version     : 0.0.1
// Copyright   : Public Domain? 2026
// Description : Removes spaces from file names
//============================================================================

#include "CLI11.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

using namespace std;

bool underscores{false};
bool periods{false};

string source;
string original;
string newname;

char alternate;

const char dash = '-';
const char dot = '.';
const char uscore = '_';

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
      return -1;
    }
  }

  return 0;
}

int main(int argc, char **argv) {

  CLI::App app;

  // setup flags
  CLI::Option *uflagcnt =
      app.add_flag("--underscore, -u", underscores, "Use dashes as delimiter");
  CLI::Option *pflagcnt =
      app.add_flag("--period, -p", periods, "Use periods as delimiter");

  app.add_option("--source, source", source, "Source file")->required();

  // parse arguments
  CLI11_PARSE(app, argc, argv);

  original = source;

  if (underscores) {
    if (uflagcnt->count() > 1) {
      cout << "Flag can only be passed once!!!" << endl;
      return 1;
    } else {
      changeName(original, '1');
    }
  } else if (periods) {
    if (pflagcnt->count() > 1) {
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
