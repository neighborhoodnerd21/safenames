//============================================================================
// Name        : cli-app.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "CLI11.hpp"
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

using namespace std;

bool underscores{false};
bool periods{false};

string source = " ";
string original = " ";
string newname = " ";

char alternate = ' ';

const char dash = '-';
const char dot = '.';
const char uscore = '_';

int exitCode = 0;

int changeName(string filename, int altValue) {

	// set alternate char and set newname string

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
			exitCode = -1;
		}
	}

	return exitCode;
}

int main(int argc, char **argv) {

	CLI::App app;

	// setup flags
	CLI::Option *uflagcnt = app.add_flag("--underscore, -u", underscores,
										 "Use dashes as delimiter");
	CLI::Option *pflagcnt =
		app.add_flag("--period, -p", periods, "Use periods as delimiter");

	app.add_option("--source, source", source, "Source file");

	// parse arguments
	CLI11_PARSE(app, argc, argv);

	if (source != " ") {
		original = source;
		if (underscores) {
			if (uflagcnt->count() > 1) {
				cout << "Flag can only be passed once!!!" << endl;
				exitCode = -1;
			} else {
				changeName(original, '1');
			}
		} else if (periods) {
			if (pflagcnt->count() > 1) {
				cout << "Flag can only be passed once!!!" << endl;
				exitCode = -1;
			} else {
				changeName(original, '2');
			}
		} else {
			changeName(original, '0');
		}
	} else {
    cout << "Missing source file argument" << endl;
  }

	return exitCode;
}
