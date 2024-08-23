

#ifndef PUMPVARIABLES_H
#define PUMPVARIABLES_H

// #include <open62541/server.h>

// // Global variable declarations
// extern UA_Int32 temperature_value_;
// extern char temperature_string_[];

// extern UA_VariableAttributes sn_attr_;
// extern UA_Int32 serial_number_;
// extern char serial_number_string_[];
// extern char serial_num_[];

// void beforeReadTemperature(UA_Server *server_, const UA_NodeId *sessionId,
// void *sessionContext,
//                            const UA_NodeId *nodeid, void *nodeContext,
//                            const UA_NumericRange *range, const UA_DataValue
//                            *data);

// void addingAttributes(UA_VariableAttributes varAttr,UA_Int32 serial_number_,
//     char serial_number_string_[],char serial_num_[],UA_Server *server_,
//     UA_NodeId rl_tempsens_Id);

// #endif // PUMPVARIABLES_H

// ------------------------------------------------------------------------------------

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

class OPCUAServer {
 public:
  OPCUAServer();
  ~OPCUAServer();

  static void stopHandler(int sig);
  static void beforeReadTemperature(UA_Server *server_,
                                    const UA_NodeId *sessionId,
                                    void *sessionContext,
                                    const UA_NodeId *nodeid, void *nodeContext,
                                    const UA_NumericRange *range,
                                    const UA_DataValue *data);

  void addingAttributes(UA_VariableAttributes varAttr, UA_Int32 serial_number_,
                        char *serial_number_string_, char *serial_num_,
                        UA_Server *server_, UA_NodeId rl_tempsens_Id);

  int run(int argc, char *argv[]);

 private:
  UA_Int32 temperature_value_;
  char temperature_string_[30];

  UA_Int32 serial_number_;
  char serial_number_string_[30];
  char serial_num_[30];
  UA_VariableAttributes sn_attr_;

  UA_Server *server_;
  static volatile UA_Boolean running_;
};

#endif  // OPCUA_SERVER_H
