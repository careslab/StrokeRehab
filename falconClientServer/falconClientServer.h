#ifndef SOCKETCLIENTSERVER_H
#define SOCKETCLIENTSERVER_H


#include <iostream>
#include <cstring>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "chai3d.h"


//------------------------------------------------------------------------------
#include <GLFW/glfw3.h>
//------------------------------------------------------------------------------

//1/16/25 addition for haptic devices

//------------------------------------------------------------------------------
#include "devices/CHapticDeviceHandler.h"
//------------------------------------------------------------------------------
#if defined(WIN32) | defined(WIN64)
#include <process.h>
#endif

#if defined(C_ENABLE_VIRTUAL_DEVICE_SUPPORT)
#include "devices/CVirtualDevice.h"
#endif

#if defined(C_ENABLE_DELTA_DEVICE_SUPPORT)
#include "devices/CDeltaDevices.h"
#endif

#if defined(C_ENABLE_PHANTOM_DEVICE_SUPPORT)
#include "devices/CPhantomDevices.h"
#endif

#if defined(C_ENABLE_LEAP_DEVICE_SUPPORT)
#include "devices/CLeapDevices.h"
#endif

#if defined(C_ENABLE_SIXENSE_DEVICE_SUPPORT)
#include "devices/CSixenseDevices.h"
#endif

#if defined(C_ENABLE_CUSTOM_DEVICE_SUPPORT)
#include "devices/CMyCustomDevice.h"
#endif
//------------------------------------------------------------------------------





namespace chai3d {

//using namespace std; 
//
#define PORT 5000
//REMEMBER PORT NUMBER
#define SERIAL_PORT "/dev/ttyACM0"


class SocketClientServer 
{
    //--------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR:
    //--------------------------------------------------------------------------

public:

    //! Constructor of cEffectMagnet.
    SocketClientServer();

    //! Destructor of cEffectMagnet.
    virtual ~SocketClientServer() {};




    //--------------------------------------------------------------------------
    // PUBLIC METHODS:
    //--------------------------------------------------------------------------

public:

    //Initialize server & arduino
    bool initializeServerAndSerial(int serverPort, std::string serialPort);

    // Initialize as a client
    bool initializeClient(const std::string& server_ip, int port);

    // Initialize as a server
    bool initializeServer(int port);    

    // Receive cVector3d structure
    bool receiveVector(cVector3d& vector);  

     // Send cVector3d structure
    bool sendVector(const cVector3d& vector);
     
    // Accept a connection (for server use only)
    bool acceptConnection(SocketClientServer& client);

    //
    bool initializeArduino(std::string port);
    
    //
    // initialize haptic device
    cGenericHapticDevicePtr initializeHapticDevice();
    
    // a pointer to the current haptic device
    cGenericHapticDevicePtr hapticDevice;
    //

    // a haptic device handler
    cHapticDeviceHandler* handler;
    
    
    //Arduino
    bool ActivateBuzzer (int number );
    bool ActivateMotor (int num, float value);

    bool MovementActivateBuzzer(const cVector3d& velocity, bool buttonPress, double deadZone);


    //--------------------------------------------------------------------------
    // PRIVATE MEMBERS:
    //--------------------------------------------------------------------------
private:

    int sock_fd;
    int new_socket;
    int prev_flag[7] = {0,0,0,0,0,0,0}; // Previous state for all 7 flags  (button0, +/- x, y, z,) 
    struct sockaddr_in address;
    bool prevButtonPress=false;
    bool prevStateChange = false;
    bool prevXChange=false;
    bool prevYChange=false; 
    bool prevZChange=false; 

    int motor_flag[7] = {0,0,0,0,0,0,0}; 

    
   
   //Arduino
   std::ofstream serial_port;
   int serial_init = false;
 

};




/* //old format
class SocketClientServer {
private:
    int sock_fd; // Socket file descriptor
    struct sockaddr_in address; // Socket address structure

public:
    // Constructor
    SocketClientServer();

 
    // Destructor
    ~SocketClientServer();

    // Initialize as a client
    bool initializeClient(const std::string& server_ip, int port);

    // Initialize as a server
    bool initializeServer(int port);

    // Send cVector3d structure
    bool sendVector(const cVector3d& vector);

    // Receive cVector3d structure
    bool receiveVector(cVector3d& vector);

    // Accept a connection (for server use only)
    bool acceptConnection(SocketClientServer& client);

    
};
*/

}

#endif // SOCKETCLIENTSERVER_H