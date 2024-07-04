#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include <iostream>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include "PumpVariables.h"

using namespace std;

// Global variable definitions
UA_Int32 TemperatureValue = 20;
char TemperatureString[] = "R1_TS1_Temperature";

UA_Int32 serialNumber = 12345678;
char SerialNumberString[] = "R1_TS1_SerialNumber";
char SerialNum[] = "SerialNumber";
UA_VariableAttributes snAttr = UA_VariableAttributes_default;



static volatile UA_Boolean running = true;

static void stopHandler(int sig) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
    running = false;
}

int main(int argc, char *argv[]) {
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    UA_Server *server = UA_Server_new();
    UA_ServerConfig_setDefault(UA_Server_getConfig(server));

    if (argc > 1) {
        UA_String hostname;
        UA_String_init(&hostname);
        hostname.length = strlen(argv[1]);
        hostname.data = (UA_Byte*)argv[1];
    }

    // Add a new namespace to the server
    UA_Int16 ns_room1 = UA_Server_addNamespace(server, "Room1");
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "New Namespace added with Nr. %d", ns_room1);

    // Add a new object called Temperature Sensor
    UA_NodeId rl_tempsens_Id;
    char TemperatureSensor[] = "Temperature Sensor";
    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    UA_Server_addObjectNode(server, UA_NODEID_NULL,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                            UA_QUALIFIEDNAME(3, TemperatureSensor), UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                            oAttr, NULL, &rl_tempsens_Id);

    // Add the variable Vendor Name to server
    UA_VariableAttributes vnAttr = UA_VariableAttributes_default;
    char VendorCompany[] = "Pump King Ltd.";
    UA_String vendorName = UA_STRING(VendorCompany);
    char VendorNameString[] = "R1_TS1_VendorName";
    char Vendor[] = "VendorName";
    UA_Variant_setScalar(&vnAttr.value, &vendorName, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, VendorNameString), rl_tempsens_Id,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(2, Vendor),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), vnAttr, NULL, NULL);

    // Add the variable Serial Number to server
    // UA_VariableAttributes snAttr = UA_VariableAttributes_default;
    // UA_Int32 serialNumber = 12345678;
    // char SerialNumberString[] = "R1_TS1_SerialNumber";
    // char SerialNum[] = "SerialNumber";
    // UA_Variant_setScalar(&snAttr.value, &serialNumber, &UA_TYPES[UA_TYPES_INT32]);
    // UA_Server_addVariableNode(server, UA_NODEID_STRING(2, SerialNumberString), rl_tempsens_Id,
    //                           UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
    //                           UA_QUALIFIEDNAME(2, SerialNum),
    //                           UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), snAttr, NULL, NULL);

    addingAttributes(snAttr,serialNumber,SerialNumberString,SerialNum,server,rl_tempsens_Id);

    // Add the variable Temperature to server
    UA_VariableAttributes tpAttr = UA_VariableAttributes_default;
    char Temp[] = "Temperature";
    UA_Variant_setScalar(&tpAttr.value, &TemperatureValue, &UA_TYPES[UA_TYPES_INT32]);
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, TemperatureString), rl_tempsens_Id,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(2, Temp),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), tpAttr, NULL, NULL);

    // Add Callback to Temperature Node
    UA_ValueCallback callback;
    callback.onRead = beforeReadTemperature;
    callback.onWrite = NULL;
    UA_Server_setVariableNode_valueCallback(server, UA_NODEID_STRING(2, TemperatureString), callback);

    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "starting server...");
    UA_StatusCode retval = UA_Server_run(server, &running);
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "stopping server...");

    UA_Server_delete(server);
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}
