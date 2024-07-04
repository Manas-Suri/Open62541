

#ifndef PUMPVARIABLES_H
#define PUMPVARIABLES_H

#include <open62541/server.h>

// Global variable declarations
extern UA_Int32 TemperatureValue;
extern char TemperatureString[30];

extern UA_VariableAttributes snAttr;
extern UA_Int32 serialNumber;
extern char SerialNumberString[];
extern char SerialNum[];

void beforeReadTemperature(UA_Server *server, const UA_NodeId *sessionId, void *sessionContext,
                           const UA_NodeId *nodeid, void *nodeContext,
                           const UA_NumericRange *range, const UA_DataValue *data);

void addingAttributes(UA_VariableAttributes varAttr,UA_Int32 serialNumber,
    char SerialNumberString[],char SerialNum[],UA_Server *server,
    UA_NodeId rl_tempsens_Id);

#endif // PUMPVARIABLES_H
