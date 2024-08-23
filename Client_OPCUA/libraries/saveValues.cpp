#include "saveValues.h"

#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
#include <open62541/plugin/log_stdout.h>

#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

SaveValues::SaveValues() {}

SaveValues::~SaveValues() { myFile.close(); }

void SaveValues::createFile(string filename) {
  string path =
      "/home/INTRANET.SALZ.GMBH/manas.suri/Work/Rpi_Imager/Open62541/"
      "Client_OPCUA/";
  myFile.open(path + filename);
  if (!myFile) {
    cerr << "Error opening file: " << path + filename << endl;
  }
}

void SaveValues::writeValues(string values) {
  if (myFile.is_open()) {
    time_t timestamp = time(nullptr);  // initialize timestamp
    string timeStr = ctime(&timestamp);
    timeStr.pop_back();  // remove trailing newline character
    myFile << timeStr + " " + values << endl;
  } else {
    cerr << "Error: File is not open." << endl;
  }
}
