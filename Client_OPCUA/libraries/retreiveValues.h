#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
#include <open62541/plugin/log_stdout.h>
#include <stdlib.h>
#include <string.h>

#include <vector>

using namespace std;
class RetreiveValues {
 public:
  RetreiveValues();
  ~RetreiveValues();

  vector<UA_String> outputFolderPath;

  static void stopHandler(int sig);

  bool connect(const std::string &serverUrl);

  bool browseNode(const UA_NodeId &nodeId, UA_BrowseResponse &bResp);

  bool findFolder(UA_BrowseResponse &bResp, const std::string &folderName,
                  UA_NodeId &foundId, UA_ReferenceDescription **foundRef);

  bool readTemperature(const UA_NodeId &tempSensorId, UA_Int32 &temperature);

 private:
  UA_Client *client_;

  bool browseRequestBuilder(UA_NodeId nodeId, UA_BrowseResponse &bResp);

  bool ObjectFinder(UA_BrowseResponse &bResp, UA_String ParentFolder,
                    UA_NodeId *foundId, UA_ReferenceDescription **foundRef);
};