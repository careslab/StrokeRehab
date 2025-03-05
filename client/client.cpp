#include <iostream>/**/
#include "falconClientServer/falconClientServer.h"
#include "chai3d.h"
#include <math.h>
//------------------------------------------------------------------------------
#include <GLFW/glfw3.h>
//------------------------------------------------------------------------------
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <vector>


using namespace chai3d;
using namespace std;
int Kp;
//vector<double> elems(100,0.0);

//namespace chai3d {
//SocketClientServer client;
chai3d::SocketClientServer client;
cVector3d receivedForceVector (0,0,0);
    
// a pointer to the current haptic device
cGenericHapticDevicePtr hapticDevice;


//1/16/24
//persistent undefined references to any function/constructor/instance of "SocketClientServer" when using make
//something isnt referencing
//UPDATE FIXED: namespace definition errors. 

int main(int argc, char* argv[]) {
    // Create an instance of SocketClientServer for the client  
    if(argc < 2){
        Kp = -100;
    }else{
        Kp = std::atoi(argv[1]);
    }

    //see falconClientServer.cpp for updated first come first serve acquisition
   hapticDevice = client.initializeHapticDevice();



    // Initialize the client to connect to the server
     if (!client.initializeClient("127.0.0.1", PORT)) {
        cerr << "Failed to connect to server" << endl;
     
    }
    
    //client.acceptConnection();
    //initiialize device to 0 mark
    hapticDevice->setForceAndTorqueAndGripperForce(receivedForceVector, 0, 0);

    // retrieve information about the current haptic device
    cHapticDeviceInfo hapticDeviceInfo = hapticDevice->getSpecifications();
    
    // retrieve the highest stiffness this device can render
    double hapticDeviceMaxStiffness = hapticDeviceInfo.m_maxLinearStiffness;
    const double HAPTIC_STIFFNESS   = 2500.0; //2500 good middleground

    while(true){
    
        if (!client.receiveVector(receivedForceVector)) {
            cerr << "Failed to receive vector from server" << endl;
            return -1;
        }
        // Output the received vector data
        //cout << "Connected: " <<receivedForceVector << endl;
        //cout << "x = " << receivedForceVector.x << ", y = " << receivedForceVector.y << ", z = " << receivedForceVector.z << endl;

            // compute linear force
            //kp reference chart for 01-mydevice
            // -200: was too sharp
            // -1: weak but smooth
            //  -50: acceptable 
            //kp reference chart for 09-magnets
            //-50 was FAR too strong
            //-20 was a bit too much
            //-1 was too weak            



            //double Kp = -100; // [N/m] //depreciated in favor of ARGc ARGV
        //remember negative for orientation

        //compute average derivative of elems
        //double D = 0.0;
        //for(int n = 0; n <= 98; n++) {
            //double a = elems[n];
            //double b = elems[n+1];
            //D = D + (b-a)/(0.001);
        //}
        //D = D/100;

        //compute integral over elems
        //double I = 0.0;
        //for(int n = 0; n <= 99; n++) {
            //double a = elems[n];
            //I = I + a*0.001;
        //}

        cVector3d position;
            hapticDevice->getPosition(position);
            cVector3d dirHaptic = receivedForceVector - position;
            //cVector3d forceHaptic = 0.7*(HAPTIC_STIFFNESS * dirHaptic) + 4*(HAPTIC_STIFFNESS * D * dirHaptic) + 6*(HAPTIC_STIFFNESS * I * dirHaptic);
            cVector3d forceHaptic = (HAPTIC_STIFFNESS * dirHaptic);

        //double x=dirHaptic.get(0);
        //double y=dirHaptic.get(1);
        //double z=dirHaptic.get(2);
        //double radius = sqrt(x*x + y*y + z*z);

        //elems.erase(elems.begin());
        //elems.push_back(radius);

            // scale the force according to the max stiffness the device can render
        double stiffnessRatio = 1.0;
        if (hapticDeviceMaxStiffness < HAPTIC_STIFFNESS)
        {
            stiffnessRatio = hapticDeviceMaxStiffness / HAPTIC_STIFFNESS;
        }
        hapticDevice->setForce(stiffnessRatio * forceHaptic);



    }

    return 0;

    
}



//}       //using namespace chai3d






//old manual clientcode
//replaced by falconClientServer.cpp

/*

int main(int argc, char* argv[]) {
    // Create an instance of SocketClientServer for the client  

    // Initialize the client to connect to the server
     if (!client.initializeClient("127.0.0.1", PORT)) {
        cerr << "Failed to connect to server" << endl;
        return -1;
    }
    

    // Receive the vector data from the server
    cVector3d receivedForceVector;

    //client.acceptConnection();
    
    if (!client.receiveVector(receivedForceVector)) {
        cerr << "Failed to receive vector from server" << endl;
        return -1;
    }

    // Output the received vector data
    cout << "Received force vector from server:" <<receivedForceVector << endl;
    //cout << "x = " << receivedForceVector.x << ", y = " << receivedForceVector.y << ", z = " << receivedForceVector.z << endl;

    return 0;

    
    }

*/