#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>  // For usleep

#define SERIAL_PORT "/dev/ttyACM0"

int main() {
    std::ofstream serialOutput(SERIAL_PORT);  // Open Serial port for writing
    
    if (!serialOutput.is_open()) {
        std::cerr << "Error: Could not open serial port " << SERIAL_PORT << std::endl;
        return 1;
    }

    std::cout << "Waiting for Arduino connection...\n";
    
    sleep(2);  // Wait for Arduino to reset

    while (true) {
        int flag;
        std::cout << "Enter 1 to rotate servo to 180°, 0 to return to 0°: ";
        std::cin >> flag;

        if (flag != 0 && flag != 1) {
            std::cout << "Invalid input! Enter 0 or 1.\n";
            continue;
        }

        serialOutput << flag << std::endl;  // Send flag to Arduino
        serialOutput.flush();  // Ensure data is sent immediately

        std::cout << "Sent flag: " << flag << " to Arduino\n";
        
        usleep(500000);  // Sleep for 500ms to avoid overwhelming serial communication
    }

    return 0;
}
