#ifndef SAVEVALUES_H
#define SAVEVALUES_H

#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
#include <open62541/plugin/log_stdout.h>

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class SaveValues {
 public:
  SaveValues();
  ~SaveValues();

  void createFile(string filename);

  void writeValues(string Values);

 private:
  ofstream myFile;
};

#endif