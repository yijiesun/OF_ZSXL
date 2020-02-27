#pragma once
#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <stdio.h>
using namespace std;

class Config {
private:
	string config_file;
	FILE *read_setup;
	bool _str_cmp(char* a, char *b);
public:
	Config(string cf);
	~Config();
	bool openFile(char *flags);
	void closeFile();
	void clearBlank();
	float get_param(char *paramName);
	void set_param(char *paramName, float val);
	void readConfig(int &len,char **param,float *value);
};

#endif