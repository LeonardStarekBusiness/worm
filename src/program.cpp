#include "worm/program.h"
#include <stdexcept>
#include <string>

Program::Program(char *filename, bool _keep_tmpfile)
{
	file = filename;
	keep_tmpfile = _keep_tmpfile;
}

std::string find_replace(std::string str, std::string s1, std::string s2)
{
    size_t n;
    std::string *rstr = &str;
    while ((n = rstr->find(s1)) != std::string::npos)
    {
        rstr->erase(n, s1.length());
        rstr->insert(n, s2);
    }
    return (*rstr);
}


void Program::preprocess(void)
{
	std::string tempfile = find_replace(file, ".worm", ".tmp");
	std::ifstream fd_in(file.c_str(), std::ios_base::in);
	std::ofstream fd_out(tempfile.c_str(), std::ios_base::out);
	if (!fd_in.is_open() || !fd_out.is_open())
		throw std::runtime_error("Could not open file!");
	std::string line("");
	std::vector<std::string> progr;
	//Inline functions
	bool in_function = false;
	std::string func_name("");
	while (std::getline(fd_in, line))
	{
		if (line.size() == 0)
			continue ;
		progr.push_back(std::string(line));
		if (in_function && line.at(0) != '\t')
			in_function = false;
		if (!in_function && line.at(0) == '\t')
			throw std::runtime_error("Misleading indentation!");
		size_t i = line.find("func");
		if (i != std::string::npos)
		{
			if (in_function)
				throw std::runtime_error("Misaligned function declaration at: " + line);
			func_name = line;
			func_name.erase(0, 5);
			i = func_name.find("(");
			if (i == std::string::npos)
				throw std::runtime_error("Expected parenthesis!");
			else
				func_name.erase(i);
			func_name = find_replace(func_name, " ", "");
			funcs[func_name] = std::string("");
			in_function = true;
			continue ;
		}
		if (in_function)
		{
			line.erase(0,1); //remove tab
			funcs[func_name].append(line);
			funcs[func_name].append("\n");
		}
	}
	for (size_t i = 0; i < progr.size(); i++)
	{
		line = progr.at(i);
		//function call found
		if (progr.at(i).find("(") != std::string::npos && progr.at(i).find("func") == std::string::npos)
		{
			func_name = line;
			size_t j = func_name.find("(");
			if (j != std::string::npos)
				func_name.erase(j);
			func_name = find_replace(func_name, " ", "");
			if (funcs.count(func_name))
				progr.at(i) = funcs[func_name];
		}
	}
	for (size_t i = 0; i < progr.size(); i++)
	{
		if (progr.at(i).at(0) == '\t' || (progr.at(i).find("func") != std::string::npos))
			progr.at(i) = std::string("__del");
	}
	for (size_t i = 0; i < progr.size(); i++)
	{
		if (progr.at(i) != std::string("__del"))
			fd_out << progr.at(i) << std::endl;
	}
	fd_in.close();
	fd_out.close();
	file = tempfile;
}

void Program::execute(void)
{
	if (keep_tmpfile)
		return ;
	std::ifstream fd_in(file.c_str(), std::ios_base::in);
	std::string line;
	if (!fd_in.is_open())
		throw std::runtime_error("Could not open file!");
	while (std::getline(fd_in, line))
		std::cout << "executing... " << line << std::endl;
	fd_in.close();
}

Program::~Program()
{
	if (keep_tmpfile)
		return ;
	else
	 	std::remove(file.c_str());
}

