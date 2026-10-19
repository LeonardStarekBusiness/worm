#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <stdexcept>

class Program
{
	private:
		std::string file;
		std::vector<std::string> lines;
		std::map<std::string, std::string> funcs;
		bool keep_tmpfile;
	public:
		Program(char *filename, bool _keep_tmpfile);
		~Program();

		void execute(void);
		void preprocess(void);
};