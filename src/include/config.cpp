#include "config.h"
using namespace std;

Config::Config(string cf)
{
	config_file = cf;
	openFile("r");
};

Config::~Config() {
	closeFile();
};

bool Config::openFile(char *flags)
{
	if ((read_setup = fopen(config_file.c_str(), flags)) == NULL) {
		puts("Fail to open config.txt!");
		return 0;
	}
	return 1;
}

void Config::closeFile()
{
	fclose(read_setup);
}

bool Config::_str_cmp(char* a, char *b)
{
	int sum = 0;
	for (int i = 0; b[i] != '\0'; i++)
		sum++;
	char tmp[200] = { "" };
	strncpy(tmp, a + 0, sum);
	for (int i = 0; a[i] != '\0'; i++)
	{
		if (a[i] == '\n')
			a[i] = (char)NULL;
	}
	return !strcmp(tmp, b);
}

void Config::clearBlank()
{
	string config_contxt;
	fseek(read_setup, 0, SEEK_SET);
	char str[200];
	while (fgets(str, 200, read_setup) != NULL) {
		if (str[0] != 10)
			config_contxt += str;
	}
	closeFile();
	openFile("w+");
	fprintf(read_setup, "%s", config_contxt.c_str());
	closeFile();
}

void Config::readConfig(int &len, char **param, float *value)
{

	len = 0;
	char str_org[200];
	char str[200];
	fseek(read_setup, 0, SEEK_SET);
	while (fgets(str_org, 200, read_setup) != NULL) {
		if (!_str_cmp(str_org, (char *)" "))
		{
			memcpy(str, str_org, 200);
			const char * split = " ";
			char *p = strtok(str, split);
			p = strtok(NULL, split);
			if (p != NULL)
			{
				memcpy(param[len], str_org, 200);
				sscanf(p, "%f", &value[len]);
				len++;
			}
		}
	}
}

float Config::get_param(char *paramName)
{
	float per = 0;
	char str[200];
	fseek(read_setup, 0, SEEK_SET);
	while (fgets(str, 200, read_setup) != NULL) {
		if (_str_cmp(str, paramName))
		{
			const char * split = " ";
			char *p = strtok(str, split);
			p = strtok(NULL, split);
			sscanf(p, "%f", &per);
			break;
		}

	}
	return per;
}

void Config::set_param(char *paramName, float val)
{
	float per = 0;
	char str[200];
	fseek(read_setup, 0, SEEK_SET);
	long file_pos_up = 0;
	long file_pos_down = 0;
	while (fgets(str, 200, read_setup) != NULL) {
		if (_str_cmp(str, paramName))
		{
			file_pos_down = ftell(read_setup);
			fseek(read_setup, -10L, SEEK_CUR);
			while (1)
			{
				char ch = fgetc(read_setup);
				if (ch==32)
				{
					file_pos_up = ftell(read_setup);
					break;
				}
			}
			break;
		}

	}
	string config_contxt;
	fseek(read_setup, 0L, SEEK_SET);
	char str_t = 0;
	while (file_pos_up > ftell(read_setup)) {
		str_t= fgetc(read_setup);
		config_contxt += str_t;
		
	}
	char buffer_max[20];
	sprintf(buffer_max, "%0.1f", val);
	config_contxt += buffer_max;
	if(str_t!=10)
		config_contxt += "\n";
	fseek(read_setup, file_pos_down, SEEK_SET);

	while (!feof(read_setup)) {
		str_t = fgetc(read_setup);
		if (str_t == NULL|| str_t<0) break;
		config_contxt += str_t;

	}
	closeFile();
	openFile("w+");
	fseek(read_setup, 0L, SEEK_SET);
	fprintf(read_setup, "%s", config_contxt.c_str());
	closeFile();
}