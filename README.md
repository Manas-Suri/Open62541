# Open62541 application

This project showcases the application of a server-client communication in opcua communication protocol. A variable is sent from client to server then after receiving the data at the server end. This received data along the file location is written to a file along with timestamp.

## Prerequisites

Please install the open62541 package from official github repository [Link](https://github.com/open62541/open62541)

```bash
git clone git@github.com:open62541/open62541.git

```

Packages that need to be installed

```bash
sudo apt update
sudo apt install build-essential gdb cmake
```

Additional **UA-Expert** is recommended to see client side variables [Link](https://www.unified-automation.com/products/development-tools/uaexpert.html)

Additional we use **VSCode** as development **IDE**

## Installation

Use this Code in both **Client_OPCUA** and in **Server_OPCUA** 


```bash
mkdir build
cd build
cmake ..
make
```

## Run

from `./build` directory run in separate terminal

```sh
./Open62541Example
```



## Contact

manassuri1998@gmail.com
