
# What is this?
A utility that links commands to actions, where the commands, in the form of strings, are provided through TCP sockets. 

## Dependencies:

### __g++ and make__
```bash
sudo apt-get install g++ make
```

### __boost C++ libs__ (threading and networking)
Tested on Debian-based distros; install with:
```bash
sudo apt-get install libboost-all-dev
```

## To build:
```bash
Make the directory netsysctrld/build
cd build
cmake ../src
make
```

## To clean:
```bash
cd netsysctrld/build
make clean
```

## To remove build artifacts:
```bash
rm -rf netsysctrld/build/*
```

## To run:

### Arg parser
In the previous versions, the audio control name and output device were positional arguments. As of this update, 
standard linux command line arguments can be specified using "-" and "--". To see the full list of arguments supported,
run
```bash
./netsysctrld -h
```
As of 1/17/18, the supported args are:
```
SHORT(-)  EXTENDED(--)  TAKES PARAM  DESCRIPTION                                                                                                                        
--------  ------------  -----------  -----------                                                                                                                        
    h     help              no       Prints the arguments supported by this program                                                                                     
    a     audiocontrol      yes      Sets the audio control name used for volume control; see output of 'amixer scontrols'. Default: "Master"                                                        
    p     password          yes      Sets the password which clients must provide to use this server. Default: no password                                                                      
    P     port              yes      Set the TCP port number of this netsysctrld server. Default: 2832         
```
Note that the defaults for each argument are specified at the end of their description.

#### General Argument Usage
To use arguments with netsysctrld, reference the supported arguments in the table above. Short arguments are a single character, and are prefixed with a "-", such as "-a". Extended arguments are more verbose, and are prefixed with a "--", such as "--audiocontrol". The "TAKES PARAM" category indicates whether each argument requires a parameter. If "yes" is specified, then a parameter must be provided with the given argument, such as "-a Master" or "--outputdevice Pulse". If "no" is specified, than no parameter should be given for the argument. 

### Password Usage
As of 1/17/18, netsysctrld supports password protection. The password is to be provided at runtime with the argument "-p" or "--password". The password must not start with a "-" or "--". The password is required when a client first connects to netsysctrld. If an invalid password is provided, the connection is dropped. If the user-provided password is correct, full control to netsysctrld is granted. 

## To connect to the daemon
### If the daemon is running on the same machine
Run:
```bash
telnet localhost 2018
```
### If the daemon is running on a different machine
Run:
```bash
telnet <IP of machine running daemon> 2018
```

### Once you're connected to the daemon:
The **HELP** command shows the commands available to use
