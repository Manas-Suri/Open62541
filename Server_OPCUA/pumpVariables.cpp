#include "pumpVariables.h"

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

volatile UA_Boolean OPCUAServer::running_ = true;

OPCUAServer::OPCUAServer() : temperature_value_(20), serial_number_(12345678) {
  strcpy(temperature_string_, "R1_TS1_Temperature");
  strcpy(serial_number_string_, "R1_TS1_SerialNumber");
  strcpy(serial_num_, "SerialNumber");
  sn_attr_ = UA_VariableAttributes_default;
}

OPCUAServer::~OPCUAServer() { UA_Server_delete(server_); }

void OPCUAServer::stopHandler(int sig) {
  UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
  running_ = false;
}

void OPCUAServer::beforeReadTemperature(
    UA_Server *server_, const UA_NodeId *sessionId, void *sessionContext,
    const UA_NodeId *nodeid, void *nodeContext, const UA_NumericRange *range,
    const UA_DataValue *data) {
  OPCUAServer *self = static_cast<OPCUAServer *>(nodeContext);
  if (self) {
    float temp = 1.0 * (rand() % 100) / 100 - 0.5;
    self->temperature_value_ += temp + 1;
    UA_Variant value;
    UA_Variant_setScalar(&value, &(self->temperature_value_),
                         &UA_TYPES[UA_TYPES_INT32]);
    UA_Server_writeValue(server_,
                         UA_NODEID_STRING(2, self->temperature_string_), value);
  } else {
    UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                 "Session context is null in beforeReadTemperature");
  }
}

void OPCUAServer::addingAttributes(UA_VariableAttributes varAttr,
                                   UA_Int32 serial_number_,
                                   char *serial_number_string_,
                                   char *serial_num_, UA_Server *server_,
                                   UA_NodeId rl_tempsens_Id) {
  UA_Variant_setScalar(&sn_attr_.value, &serial_number_,
                       &UA_TYPES[UA_TYPES_INT32]);
  UA_Server_addVariableNode(server_, UA_NODEID_STRING(2, serial_number_string_),
                            rl_tempsens_Id,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(2, serial_num_),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                            sn_attr_, NULL, NULL);
  UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
              "Serial number attribute added: %d", serial_number_);
}

int OPCUAServer::run(int argc, char *argv[]) {
  signal(SIGINT, stopHandler);
  signal(SIGTERM, stopHandler);

  server_ = UA_Server_new();
  if (!server_) {
    UA_LOG_FATAL(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                 "Failed to create server_");
    return EXIT_FAILURE;
  }
  UA_ServerConfig *config = UA_Server_getConfig(server_);

  if (argc > 1) {
    // We have a hostname
    UA_String host = UA_STRING(argv[1]);
    UA_String_clear(&config->applicationDescription.applicationUri);
    UA_String_copy(&host, &config->applicationDescription.applicationUri);
    UA_ServerConfig_setMinimal(config, 4840, NULL);
  }

  if (argc > 1) {
    UA_String hostname;
    UA_String_init(&hostname);
    hostname.length = strlen(argv[1]);
    hostname.data = (UA_Byte *)argv[1];
  }

  UA_Int16 ns_room1 = UA_Server_addNamespace(server_, "Room1");
  UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
              "New Namespace added with Nr. %d", ns_room1);

  UA_NodeId folderup;
  char folderupName[] = "MainFolder";
  UA_ObjectAttributes folderAttrup = UA_ObjectAttributes_default;
  UA_StatusCode status = UA_Server_addObjectNode(
      server_, UA_NODEID_NULL, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
      UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
      UA_QUALIFIEDNAME(ns_room1, folderupName),
      UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE), folderAttrup, NULL, &folderup);
  if (status != UA_STATUSCODE_GOOD) {
    UA_LOG_FATAL(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                 "Failed to add folder node");
    return EXIT_FAILURE;
  }

  // Step 1: Create a folder node
  UA_NodeId folderId;
  char folderName[] = "Room1";
  UA_ObjectAttributes folderAttr = UA_ObjectAttributes_default;
  status = UA_Server_addObjectNode(
      server_, UA_NODEID_NULL,
      folderup,  // UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER)
      UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
      UA_QUALIFIEDNAME(ns_room1, folderName),
      UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE), folderAttr, NULL, &folderId);
  if (status != UA_STATUSCODE_GOOD) {
    UA_LOG_FATAL(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                 "Failed to add folder node");
    return EXIT_FAILURE;
  }

  UA_NodeId rl_tempsens_Id;
  char TemperatureSensor[] = "Temperature Sensor";
  UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
  status =
      UA_Server_addObjectNode(server_, UA_NODEID_NULL, folderId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                              UA_QUALIFIEDNAME(ns_room1, TemperatureSensor),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                              oAttr, NULL, &rl_tempsens_Id);
  if (status != UA_STATUSCODE_GOOD) {
    UA_LOG_FATAL(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                 "Failed to add object node");
    return EXIT_FAILURE;
  }

  UA_VariableAttributes vnAttr = UA_VariableAttributes_default;
  char VendorCompany[] = "Pump King Ltd.";
  UA_String vendorName = UA_STRING(VendorCompany);
  char VendorNameString[] = "R1_TS1_VendorName";
  char Vendor[] = "VendorName";
  UA_Variant_setScalar(&vnAttr.value, &vendorName, &UA_TYPES[UA_TYPES_STRING]);
  status = UA_Server_addVariableNode(
      server_, UA_NODEID_STRING(ns_room1, VendorNameString), rl_tempsens_Id,
      UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), UA_QUALIFIEDNAME(2, Vendor),
      UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), vnAttr, NULL, NULL);
  if (status != UA_STATUSCODE_GOOD) {
    UA_LOG_FATAL(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                 "Failed to add variable node: VendorName");
    return EXIT_FAILURE;
  }

  addingAttributes(sn_attr_, serial_number_, serial_number_string_, serial_num_,
                   server_, rl_tempsens_Id);

  UA_VariableAttributes tpAttr = UA_VariableAttributes_default;
  char Temp[] = "Temperature";
  UA_Variant_setScalar(&tpAttr.value, &temperature_value_,
                       &UA_TYPES[UA_TYPES_INT32]);
  UA_NodeId temperatureNodeId;
  status = UA_Server_addVariableNode(
      server_, UA_NODEID_STRING(2, temperature_string_), rl_tempsens_Id,
      UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), UA_QUALIFIEDNAME(2, Temp),
      UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), tpAttr, this,
      &temperatureNodeId);
  if (status != UA_STATUSCODE_GOOD) {
    UA_LOG_FATAL(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                 "Failed to add variable node: Temperature");
    return EXIT_FAILURE;
  }

  UA_ValueCallback callback;
  callback.onRead = beforeReadTemperature;
  callback.onWrite = NULL;
  status = UA_Server_setVariableNode_valueCallback(server_, temperatureNodeId,
                                                   callback);
  if (status != UA_STATUSCODE_GOOD) {
    UA_LOG_FATAL(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                 "Failed to set value callback for Temperature");
    return EXIT_FAILURE;
  }

  UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "starting server_...");
  UA_StatusCode retval = UA_Server_run(server_, &running_);
  UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "stopping server_...");

  return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}
