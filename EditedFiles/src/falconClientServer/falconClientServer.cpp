//#include <iostream>
//#include <cstring>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <unistd.h>


#include "falconClientServer/falconClientServer.h"

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
    struct sockaddr_in address;

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
