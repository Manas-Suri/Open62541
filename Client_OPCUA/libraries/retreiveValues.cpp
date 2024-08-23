#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
#include <open62541/plugin/log_stdout.h>
#include <iostream>
#include <string>
#include <vector>
#include "retreiveValues.h"

RetreiveValues::RetreiveValues() : client_(UA_Client_new()) {
        UA_ClientConfig_setDefault(UA_Client_getConfig(client_));
    }

RetreiveValues::~RetreiveValues() {
        UA_Client_delete(client_);
    }

bool RetreiveValues::connect(const std::string &serverUrl) {
        UA_StatusCode retval = UA_Client_connect(client_, serverUrl.c_str());
        if (retval != UA_STATUSCODE_GOOD) {
            std::cerr << "Failed to connect to server. StatusCode: 0x" << std::hex << retval << std::endl;
            return false;
        }
        return true;
    }

bool RetreiveValues::browseNode(const UA_NodeId &nodeId, UA_BrowseResponse &bResp) {
    return browseRequestBuilder(nodeId, bResp);
}

bool RetreiveValues::findFolder(UA_BrowseResponse &bResp, const std::string &folderName, UA_NodeId &foundId, UA_ReferenceDescription **foundRef) {
    UA_String ParentFolder = UA_String_fromChars(folderName.c_str());
    bool result = ObjectFinder(bResp, ParentFolder, &foundId, foundRef);
    UA_String_clear(&ParentFolder);
// ------------------------------------------------------------
    outputFolderPath.pop_back();
    UA_String outputFolder = UA_String_fromChars(folderName.c_str());
    outputFolderPath.push_back(outputFolder);
// ------------------------------------------------------------
    return result;
}

bool RetreiveValues::readTemperature(const UA_NodeId &tempSensorId, UA_Int32 &temperature) {
    UA_Variant value;
    UA_Variant_init(&value);
    UA_StatusCode retval = UA_Client_readValueAttribute(client_, tempSensorId, &value);

    if (retval == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_INT32])) {
        temperature = *(UA_Int32 *)value.data;
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Temperature: %d", temperature);
        return true;
    } else {
        std::cerr << "Failed to read Temperature value. StatusCode: 0x" << std::hex << retval << std::endl;
        UA_Variant_delete(&value);
        return false;
    }
}

bool RetreiveValues::browseRequestBuilder(UA_NodeId nodeId, UA_BrowseResponse &bResp) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Browsing node: namespaceIndex=%u, identifierType=%d, identifier=%.*s",
                nodeId.namespaceIndex, nodeId.identifierType,
                (int)nodeId.identifier.string.length, nodeId.identifier.string.data);

    UA_BrowseRequest request;
    UA_BrowseRequest_init(&request);
    request.requestedMaxReferencesPerNode = 0;
    request.nodesToBrowse = UA_BrowseDescription_new();
    request.nodesToBrowseSize = 1;
    request.nodesToBrowse[0].nodeId = nodeId;
    request.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL;

    bResp = UA_Client_Service_browse(client_, request);
    UA_BrowseRequest_clear(&request);

    if (bResp.responseHeader.serviceResult != UA_STATUSCODE_GOOD) {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Browse request failed. StatusCode: 0x%08x", bResp.responseHeader.serviceResult);
        return false;
    }
    return true;
}

bool RetreiveValues::ObjectFinder(UA_BrowseResponse &bResp, UA_String ParentFolder, UA_NodeId *foundId, UA_ReferenceDescription **foundRef) {
    for (size_t i = 0; i < bResp.resultsSize; ++i) {
        for (size_t j = 0; j < bResp.results[i].referencesSize; ++j) {
            UA_ReferenceDescription *currentRef = &(bResp.results[i].references[j]);
            if (UA_String_equal(&currentRef->browseName.name, &ParentFolder)) {
                *foundId = currentRef->nodeId.nodeId;
                *foundRef = currentRef;
                UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Found folder: %.*s", (int)ParentFolder.length, ParentFolder.data);
                outputFolderPath.push_back(ParentFolder);
                
                return true;
            }
        }
    }
    UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Folder not found: %.*s", (int)ParentFolder.length, ParentFolder.data);
    return false;
}

