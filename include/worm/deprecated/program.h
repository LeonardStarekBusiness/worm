#ifndef WORM_PROGRAM_H
#define WORM_PROGRAM_H

#include <map>
#include <string>
#include <vector>

class Program {
private:
  std::string file;
  std::vector<std::string> lines;
  std::map<std::string, std::string> funcs;
  bool keep_tmpfile;

public:
  Program(char* filename, bool _keep_tmpfile);
  ~Program();

  void execute(void);
  void preprocess(void);
};

#endif // WORM_PROGRAM_H
