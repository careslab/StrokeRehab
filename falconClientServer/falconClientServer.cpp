//#include <iostream>
//#include <cstring>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <fcntl.h>
//#include <termios.h>
#include <unistd.h>

#define SERIAL_PORT "/dev/ttyACM0"

#include "falconClientServer/falconClientServer.h"
#include "falconClientServer.h"


//arduino includes 2/14
#include <fstream>

#define BUTTON 0
#define NUM_MOTORS 7
#define X_forward 1
#define X_backward 2
#define Y_forward 3
#define Y_backward 4
#define Z_forward 5
#define Z_backward 6


 // Open Serial port for writing

// chai3d imports from 02-multi-devices.cpp 
//1/15/25
//#include "chai3d.h"
//------------------------------------------------------------------------------
//#include <GLFW/glfw3.h>
//------------------------------------------------------------------------------
//using namespace chai3d;
using namespace std;
//
//
//modified cEffectMagnet to SocketClientServer starts below
//
//
namespace chai3d 
{
//------------------------------------------------------------------------------
//==============================================================================
/*!
    Constructor of SocketClientServer.

    \param  a_parent  Parent object.
*/
//==============================================================================
SocketClientServer::SocketClientServer()
{
    //m_enabledInside = false; //IDK what this is, commented out
}

//==============================================================================
/*!
    This method computes the resulting force effect.

    \param  a_toolPos        Position of tool.
    \param  a_toolVel        Velocity of tool.
    \param  a_toolID         Identification number of the force algorithm stored in the tool.
    \param  a_reactionForce  Return value for the computed force.

    \return __false__ if no interaction force occurs, __true__ otherwise.
*/
//==============================================================================


    //initialize server AND arduino 2/26 JML
    bool SocketClientServer::initializeServerAndSerial(int serverPort, std::string serialPort){

        initializeArduino(serialPort);
        sleep(0.5);
        initializeServer(serverPort);
        
    }


    // initialize client
    // REMEMBER: "SocketClientServer::" MUST prefix all functions
    //
    bool SocketClientServer::initializeClient(const std::string& server_ip, int port) {
         sock_fd = socket(AF_INET, SOCK_STREAM, 0);
         if (sock_fd < 0) {
             perror("Socket creation failed");
             return false;
        }    
        std::cout << "socket creation succeed with sock_fd: " << sock_fd << std::endl;

        address.sin_family = AF_INET;
        address.sin_port = htons(port);

          if (inet_pton(AF_INET, server_ip.c_str(), &address.sin_addr) <= 0) {
              perror("Invalid server address");
             return false;
            }
         std::cout << "server address OK"<< std::endl;           

        if (connect(sock_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
             perror("Connection to server failed");
             return false;
        }
         std::cout << "server connection succeed with sock_fd: " << sock_fd << std::endl;       

            return true;
    }
    //
    //initializeServer function
    bool SocketClientServer::initializeServer(int port) {
        sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (sock_fd < 0) {
            perror("Socket creation failed");
            return false;
        }

        int opt = 1;
        if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
            perror("setsockopt failed");
            return false;
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if (bind(sock_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            perror("Bind failed");
            return false;
        }

        if (listen(sock_fd, 3) < 0) {
            perror("Listen failed");
            return false;
        }


        // Accept an incoming connection
        int addrlen = sizeof(address);
        new_socket = accept(sock_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        //cout << "initial server: new_socket = " << new_socket << endl;
        if (new_socket < 0) {
            perror("Accept");
            exit(EXIT_FAILURE);
        }


        std::cout << "accepted connection on newsocket"<< new_socket << std::endl;  
        return true;
        
    }

    // Send cVector3d structure
    bool SocketClientServer::sendVector(const cVector3d& vector) {
             //std::cout << "entering sendVector"<< std::endl;  
        
        //cout << "in Client Server new_socket = " << new_socket << endl;

        if (send(new_socket, &vector, sizeof(vector), 0) < 0) {
            perror("Send failed");
            //std::cout << "send failed"<< std::endl;  
            return false;
        }

                    //std::cout << "sent: "<< vector << std::endl;  
        return true;
    }
    //

    //receive cVector3d structure
    bool SocketClientServer::receiveVector(cVector3d& vector) {
        if (recv(sock_fd, &vector, sizeof(vector), 0) <= 0) {
            perror("Receive failed");
            return false;
        }
        return true;
    }
    //

    bool SocketClientServer::acceptConnection(SocketClientServer& client) {
        socklen_t addrlen = sizeof(client.address);
        client.sock_fd = accept(sock_fd, (struct sockaddr*)&client.address, &addrlen);
        if (client.sock_fd < 0) {
            perror("Accept failed");
            return false;
        }
        return true;
    }
    //


bool SocketClientServer::initializeArduino(std::string port) {
    // Open the serial port in read and write mode
    //port = SERIAL_PORT;
    std::ofstream serialport (SERIAL_PORT);

    
    //serialport.open(port, std::ios::in | std::ios::out | std::ios::binary);

    if (!serialport.is_open()) {  // Fix the incorrect condition
        std::cerr << "initializeArduino: Could not open serial port " << port << std::endl;
        return false;
    }

    //std::cout << "initializeArduino: port =  " << serial_port << std::endl;
    //serial_port << 2 << std::endl; //confirm arduino serial DOES receive

    std::cout << "initializeArduino: Arduino connected on " << port << std::endl;
    serial_init = true;
   
    serial_port = std::move(serialport);

    return true;
}





    bool SocketClientServer::ActivateBuzzer (int number ){
        //std::cout << "ActivateBuzzer: port =  " << serial_port << std::endl;
        if (serial_init)
           std::cout << "ActivateBuzzer: port is initialized" << std:: endl;
        if (!serial_port.is_open()){
            std::cout << "ActivateBuzzer: port not opened" <<std::endl;
            return (false);
        }
        serial_port << number << std::endl;
        serial_port.flush();  // Ensure immediate transmission
        std::cout << "activateBuzzer  " << number << " to Arduino\n";  serial_port; 
        return (true);
    }

    bool SocketClientServer::ActivateMotor (int num, float value){

    
    }

    bool SocketClientServer::MovementActivateBuzzer(const cVector3d& velocity, bool buttonPress, double deadZone) {
    
    //std::cout<< "Entering MovementActivateBuzzer" << std::endl;
    
    // Detect changes in button state (rising/falling edge)
    bool newStateChange=false;
    double deadX = deadZone, deadY = deadZone, deadZ = deadZone; //0.16 works well in practice

    if(buttonPress != prevButtonPress){ 
        prevButtonPress=buttonPress; 
        newStateChange=true;
         std::cout<< "newStateChange via button TRUE" << std::endl;
        } 
    motor_flag[0]=buttonPress;

    //X direction 
    if (velocity(0) >= deadX && velocity(0)>= 0){
        //positive
        motor_flag[X_forward] = 1;
        newStateChange=true;
        
        }
    else if (velocity(0) <= -deadX && velocity(0)<= 0){
        //negative
        motor_flag[X_backward] =1;
        newStateChange=true;
    }else{
        //dead zone
        motor_flag[X_forward] = 0;
        motor_flag[X_backward] = 0;
    }       
    //Y direction 
    if (velocity(1) >= deadY && velocity(1)>= 0){
        //positive
        motor_flag[Y_forward] = 1;
        newStateChange=true;}
    else if (velocity(1) <= -deadY && velocity(1)<= 0){
        //negative
        motor_flag[Y_backward] =1;
        newStateChange=true;
    }else{
        //dead zone
        motor_flag[Y_forward] = 0;
        motor_flag[Y_backward] = 0;
    }   
    //Z direction 
    if (velocity(2) >= deadZ && velocity(2)>= 0){
        //positive
        motor_flag[Z_forward] = 1;
        newStateChange=true;}
    else if (velocity(2) <= -deadZ && velocity(2)<= 0){
        //negative
        motor_flag[Z_backward] =1;
        newStateChange=true;
    }else{
        //dead zone
        motor_flag[Z_forward] = 0;
        motor_flag[Z_backward] = 0;
    }
 
  
    if (newStateChange) {
        cout << "newStateChange for deadX: " << deadX; 

        std::cout<<"| Vx = |";
        std::cout<< velocity(0);
        std::cout<<"| Vy = ";
        std::cout<< velocity(1);       
        std::cout<<"| z = ";
        std::cout<< velocity(2);  
        std::cout << "" << std:: endl;             


        std::cout << "Sending motor flags to Arduino: ";
        std::cout << motor_flag[0] << " | ";
        std::cout << motor_flag[1] << " ";
        std::cout << motor_flag[2] << " | ";
        std::cout << motor_flag[3] << " ";
        std::cout << motor_flag[4] << " | ";
        std::cout << motor_flag[5] << " ";
        std::cout << motor_flag[6] << " | ";
        std::cout << "" << std:: endl;         
        // for (int i = 0; i < NUM_MOTORS; i++) {
        //     std::cout << motor_flag[i] << " ";
        // }
        // std::cout << std::endl;

        uint8_t motorByte = 0;
        for (int i = 0; i < NUM_MOTORS; i++) {
            motorByte |= (motor_flag[i] & 0x01) << i;
        }
        serial_port.write(reinterpret_cast<char*>(&motorByte), 1);  // Send 1 byte
        serial_port.flush();  // Optional: ensure immediate transmission

        std::cout << "Flushed to Arduino: " << static_cast<int>(motorByte) << std::endl;        


        //Send array as a space-separated string
        // for (int i = 0; i < NUM_MOTORS; i++) {
        //     serial_port << motor_flag[i] << " ";
        // }
        // serial_port << std::endl;
        // serial_port.flush();           


        // Update previous state
        prevButtonPress = buttonPress;
    }
    return true;

     //std::cout<< "Leaving MovementActivateBuzzer" << std::endl;

}






    //initialize haptic device function
    cGenericHapticDevicePtr SocketClientServer::initializeHapticDevice(){


    //--------------------------------------------------------------------------
    // HAPTIC DEVICE
    //--------------------------------------------------------------------------
    

    // create a haptic device handler
    handler = new cHapticDeviceHandler;

    // get a handle to the first haptic device
    if (!handler->getDevice(hapticDevice, 0)) {
       // Try to get the second device if the first one fails
        if (!handler->getDevice(hapticDevice, 1)) {
        std::cout << "Error getting device" << std::endl;
        return 0;
        }
    }
   

    

    // open a connection to haptic device
    hapticDevice->open();

    // calibrate device (if necessary)
    hapticDevice->calibrate();

    // retrieve information about the current haptic device
    cHapticDeviceInfo info = hapticDevice->getSpecifications();

    // if the device has a gripper, enable the gripper to simulate a user switch
    hapticDevice->setEnableGripperUserSwitch(true);

    return (hapticDevice);

    }











} //end namespace chai3d
//
//
//modified cEffectMagnet to SocketClientServer ends here
//
//










//

/*
namespace chai3d {

class SocketClientServer {
private:
    int sock_fd;
    struct sockaddr_in address;      std::cout << "" << std:: endl; 

public:
    SocketClientServer() : sock_fd(-1) {}

    ~SocketClientServer() {
        if (sock_fd != -1) {
            close(sock_fd);
        }
    }

    // Initialize as a client
    bool initializeClient(const std::string& server_ip, int port) {
        sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (sock_fd < 0) {
            perror("Socket creation failed");
            return false;
        }

        address.sin_family = AF_INET;
        address.sin_port = htons(port);

        if (inet_pton(AF_INET, server_ip.c_str(), &address.sin_addr) <= 0) {
            perror("Invalid server address");
            return false;
        }

        if (connect(sock_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            perror("Connection to server failed");
            return false;
        }

        return true;
    }

    // Initialize as a server
    bool initializeServer(int port) {
        sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (sock_fd < 0) {
            perror("Socket creation failed");
            return false;
        }

        int opt = 1;
        if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
            perror("setsockopt failed");
            return false;
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if (bind(sock_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            perror("Bind failed");
            return false;
        }

        if (listen(sock_fd, 3) < 0) {
            perror("Listen failed");
            return false;
        }

        return true;
    }

    // Send cVector3d structure
    bool sendVector(const cVector3d& vector) {
        if (send(sock_fd, &vector, sizeof(vector), 0) < 0) {
            perror("Send failed");
            return false;
        }
        return true;
    }

    // Receive cVector3d structure
    bool receiveVector(cVector3d& vector) {
        if (recv(sock_fd, &vector, sizeof(vector), 0) <= 0) {
            perror("Receive failed");
            return false;
        }
        return true;
    }

    // Accept a connection (for server use only)
    bool acceptConnection(SocketClientServer& client) {
        socklen_t addrlen = sizeof(client.address);
        client.sock_fd = accept(sock_fd, (struct sockaddr*)&client.address, &addrlen);
        if (client.sock_fd < 0) {
            perror("Accept failed");
            return false;
        }
        return true;
    }
};

}       //using namespace chai3d
*/

//------------------------------------------------------------------------------
//} // namespace chai3d
//-------------------------------------------