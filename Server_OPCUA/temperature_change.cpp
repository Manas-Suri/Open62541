
#include <stdlib.h> // For rand()
#include "PumpVariables.h"

// Function definition
void beforeReadTemperature(UA_Server *server, const UA_NodeId *sessionId, void *sessionContext,
                           const UA_NodeId *nodeid, void *nodeContext,
                           const UA_NumericRange *range, const UA_DataValue *data) {
    float temp = 1.0 * (rand() % 100) / 100 - 0.5;
    TemperatureValue += temp;
    UA_Variant value;
    UA_Variant_setScalar(&value, &TemperatureValue, &UA_TYPES[UA_TYPES_INT32]);
    UA_Server_writeValue(server, UA_NODEID_STRING(2, TemperatureString), value);
}

// All the values are taken as an example of serial number to server 
void addingAttributes(UA_VariableAttributes varAttr,
    UA_Int32 serialNumber,
    char SerialNumberString[],
    char SerialNum[],UA_Server *server,
    UA_NodeId rl_tempsens_Id)
{
        // Add the variable Serial Number to server
    // UA_VariableAttributes snAttr = UA_VariableAttributes_default;
    // UA_Int32 serialNumber = 12345678;
    // char SerialNumberString[] = "R1_TS1_SerialNumber";
    // char SerialNum[] = "SerialNumber";
    UA_Variant_setScalar(&snAttr.value, &serialNumber, &UA_TYPES[UA_TYPES_INT32]);
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, SerialNumberString), rl_tempsens_Id,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(2, SerialNum),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), snAttr, NULL, NULL);
}

