#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
#include <open62541/plugin/log_stdout.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "libraries/retreiveValues.h"
#include "libraries/saveValues.h"

using namespace std;
int main(void) {
  RetreiveValues client_;
  string ipAddress = "opc.tcp://lapt-owl-011:4840";
  string parentFolder = "MainFolder";
  string childFolder = "Room1";
  string parentNode = "Temperature Sensor";
  string childNode = "Temperature";
  string filename = "Parameters.txt";
  SaveValues outputfileprovided;
  UA_Int32 outputValue;

  if (!client_.connect(ipAddress)) {
    UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                 "Failed to connect to server.");
    return EXIT_FAILURE;
  }

  UA_BrowseResponse bResp;
  if (!client_.browseNode(UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                          bResp)) {
    UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                   "Failed to browse Objects folder.");
    return EXIT_FAILURE;
  }

  UA_NodeId parentFolderId;
  UA_ReferenceDescription *ref = NULL;
  if (!client_.findFolder(bResp, parentFolder, parentFolderId, &ref)) {
    UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                   "Roomup folder not found.");
    UA_BrowseResponse_clear(&bResp);
    return EXIT_FAILURE;
  }

  UA_BrowseResponse bResp2;
  if (!client_.browseNode(parentFolderId, bResp2)) {
    UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                   "Failed to browse Roomup folder.");
    UA_BrowseResponse_clear(&bResp);
    return EXIT_FAILURE;
  }

  UA_NodeId childFolderId;
  if (!client_.findFolder(bResp2, childFolder, childFolderId, &ref)) {
    UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                   "Room1 folder not found.");
    UA_BrowseResponse_clear(&bResp);
    UA_BrowseResponse_clear(&bResp2);
    return EXIT_FAILURE;
  }

  UA_BrowseResponse bResp3;
  if (!client_.browseNode(childFolderId, bResp3)) {
    UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                   "Failed to browse Room1 folder.");
    UA_BrowseResponse_clear(&bResp);
    UA_BrowseResponse_clear(&bResp2);
    return EXIT_FAILURE;
  }

  UA_NodeId tempSensorId;
  if (!client_.findFolder(bResp3, parentNode, tempSensorId, &ref)) {
    UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                   "Temperature Sensor not found.");
    UA_BrowseResponse_clear(&bResp);
    UA_BrowseResponse_clear(&bResp2);
    UA_BrowseResponse_clear(&bResp3);
    return EXIT_FAILURE;
  }

  UA_BrowseResponse bResp4;
  if (!client_.browseNode(tempSensorId, bResp4)) {
    UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                   "Failed to browse Temperature Sensor folder.");
    UA_BrowseResponse_clear(&bResp);
    UA_BrowseResponse_clear(&bResp2);
    UA_BrowseResponse_clear(&bResp3);
    return EXIT_FAILURE;
  }

  UA_NodeId childNodeId;
  if (!client_.findFolder(bResp4, childNode, childNodeId, &ref)) {
    UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                   "Temperature not found.");
    UA_BrowseResponse_clear(&bResp);
    UA_BrowseResponse_clear(&bResp2);
    UA_BrowseResponse_clear(&bResp3);
    UA_BrowseResponse_clear(&bResp4);
    return EXIT_FAILURE;
  }

  if (!client_.readTemperature(childNodeId, outputValue)) {
    UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                   "Failed to read Temperature value.");
  } else {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Value: %d",
                outputValue);
  }

  // Retrieve and print the values
  for (const auto &path : client_.outputFolderPath) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Found folder output: %.*s", (int)path.length, path.data);
  }

  // ----------------------------Now make a
  // file.txt-------------------------------
  string value;
  for (size_t i = 0; i < client_.outputFolderPath.size(); i++) {
    value += std::string(
        reinterpret_cast<const char *>(client_.outputFolderPath[i].data),
        client_.outputFolderPath[i].length);
    if (i < client_.outputFolderPath.size() - 1) {
      value += "->";  // or "\n" or any other delimiter you prefer
    }
  }
  UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Get data: %s",
              value.c_str());

  string values = string(value) + ": " + to_string(outputValue);
  outputfileprovided.createFile(filename);
  outputfileprovided.writeValues(values);

  // Clean up
  UA_BrowseResponse_clear(&bResp);
  UA_BrowseResponse_clear(&bResp2);
  UA_BrowseResponse_clear(&bResp3);
  UA_BrowseResponse_clear(&bResp4);

  return EXIT_SUCCESS;
}