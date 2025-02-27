//==============================================================================
/*
    Software License Agreement (BSD License)
    Copyright (c) 2003-2024, CHAI3D
    (www.chai3d.org)

    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above
    copyright notice, this list of conditions and the following
    disclaimer in the documentation and/or other materials provided
    with the distribution.

    * Neither the name of CHAI3D nor the names of its contributors may
    be used to endorse or promote products derived from this software
    without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
    ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE. 

    \author    <http://www.chai3d.org>
    \author    Francois Conti
    \version   3.3.0
*/
//==============================================================================

//------------------------------------------------------------------------------
#include "chai3d.h"
#include <chrono>
#include <fstream>
#include <ctime>
//------------------------------------------------------------------------------
#include <GLFW/glfw3.h>
//------------------------------------------------------------------------------
using namespace chai3d;
using namespace std;
//------------------------------------------------------------------------------
#include "falconClientServer/falconClientServer.h"
//------------------------------------------------------------------------------
// GENERAL SETTINGS
//------------------------------------------------------------------------------

// stereo Mode
/*
    C_STEREO_DISABLED:            Stereo is disabled 
    C_STEREO_ACTIVE:              Active stereo for OpenGL NVDIA QUADRO cards
    C_STEREO_PASSIVE_LEFT_RIGHT:  Passive stereo where L/R images are rendered next to each other
    C_STEREO_PASSIVE_TOP_BOTTOM:  Passive stereo where L/R images are rendered above each other
*/
cStereoMode stereoMode = C_STEREO_DISABLED;

// fullscreen mode
bool fullscreen = false;

// mirrored display
bool mirroredDisplay = false;


//------------------------------------------------------------------------------
// DECLARED VARIABLES
//------------------------------------------------------------------------------

// a world that contains all objects of the virtual environment
cWorld* world;

// a camera to render the world in the window display
cCamera* camera;

// a viewport to display the scene viewed by the camera
cViewport* viewport = nullptr;

// a light source to illuminate the objects in the world
cDirectionalLight *light;

// a haptic device handler
cHapticDeviceHandler* handler;

// a pointer to the current haptic device
cGenericHapticDevicePtr hapticDevice;

// a virtual tool representing the haptic device in the scene
cToolCursor* tool;

// a mesh object to model a color palette
cMesh* palette;

// a mesh object to model a piece of canvas
cMesh* canvas;

// copy of blank canvas texture
cImagePtr canvasOriginal;

// selected paint color
cColorb paintColor;

// a colored background
cBackground* background;

// a font for rendering text
cFontPtr font;

// a label to explain what is happening
cLabel* labelMessage;

// a label to display the rate [Hz] at which the simulation is running
cLabel* labelRates;

// a label to display the accuracy of the drawing
cLabel* labelAccuracy;

// a label to display the timer
cLabel* labelTimer;

// a label for the win time
cLabel* labelDrawTime;

// a flag that indicates if the haptic simulation is currently running
bool simulationRunning = false;

// a flag that indicates if the haptic simulation has terminated
bool simulationFinished = true;

// a frequency counter to measure the simulation graphic rate
cFrequencyCounter freqCounterGraphics;

// a frequency counter to measure the simulation haptic rate
cFrequencyCounter freqCounterHaptics;

// haptic thread
cThread* hapticsThread;

// a handle to window display context
GLFWwindow* window = nullptr;

// current size of GLFW window
int windowW = 0;
int windowH = 0;

// current size of GLFW framebuffer
int framebufferW = 0;
int framebufferH = 0;

// swap interval for the display context (vertical synchronization)
int swapInterval = 1;

// prespecified color values
cColorb blue;
cColorb red;
cColorb yellow;
cColorb green;
cColorb brushblue;
cColorb brushred;
cColorb brushyellow;
cColorb brushgreen;

// Arrays to hold color values
int trueRed[1000][1000];
int trueGreen[1000][1000];
int trueBlue[1000][1000];
int drawnRed[1000][1000];
int drawnGreen[1000][1000];
int drawnBlue[1000][1000];

//accuracy variable
double accuracy;

//clientServerSoftware class test variable
SocketClientServer server;

// variables to activate shape drawing from key press
bool drawing1 = false;
bool drawing2 = false;
bool drawing3 = false;
bool drawing4 = false;
bool drawing5 = false;
bool drawing6 = false;

// variable to hold button 0 state for edge driven activation
bool b0PreviousState = false;

// timer variables
// Timer Started Variable
int timerStarted = 0;
chrono::high_resolution_clock::time_point timeStart = chrono::high_resolution_clock::now();

// Elapsed Time Variable
chrono::duration<double> elapsedTime = chrono::seconds(0);

// Draw Time Variable
chrono::duration<double> drawTime = chrono::seconds(0);

//------------------------------------------------------------------------------
// DECLARED FUNCTIONS
//------------------------------------------------------------------------------

// callback when the window is resized
void onWindowSizeCallback(GLFWwindow* a_window, int a_width, int a_height);

// callback when the window framebuffer is resized
void onFrameBufferSizeCallback(GLFWwindow* a_window, int a_width, int a_height);

// callback when an error GLFW occurs
void onErrorCallback(int a_error, const char* a_description);

// callback when a key is pressed
void onKeyCallback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods);

// callback when window content scaling is modified
void onWindowContentScaleCallback(GLFWwindow* a_window, float a_xscale, float a_yscale);

// this function renders the scene
void renderGraphics(void);

// this function contains the main haptics simulation loop
void renderHaptics(void);

// this function closes the application
void close(void);

// this function clears the canvas to white
void clearCanvas() {
    int canvasWidth = (int)canvas->m_texture->m_image->getWidth();
    int canvasHeight = (int)canvas->m_texture->m_image->getHeight();

    for (int x=0; x<canvasWidth; x++)
                {
                    for (int y=0; y<canvasHeight; y++)
                    {
                           cColorb newColor;

                            // compute new color
                            GLubyte R = (GLubyte)254.0;
                            GLubyte G = (GLubyte)254.0;
                            GLubyte B = (GLubyte)254.0;
                            newColor.set(R, G, B); 

                            // assign new color to pixel
                            if ((x >= 0) && (y >= 0) && (x < (int)canvas->m_texture->m_image->getWidth()) && (y < (int)canvas->m_texture->m_image->getHeight()))
                            {
                                canvas->m_texture->m_image->setPixelColor(x, y, newColor);
                            }
                        
                    }
                }

                // update texture
                canvas->m_texture->markForUpdate();
}

// this function draws a line on the canvas


void drawCanvasLine(double x1, double y1, double x2, double y2, double size, string incolor) {
    int canvasWidth = (int)canvas->m_texture->m_image->getWidth();
    int canvasHeight = (int)canvas->m_texture->m_image->getHeight();
    cColorb newColor;
    if (incolor == "blue"){newColor = blue;}
    else if (incolor == "red"){newColor = red;}
    else if (incolor == "yellow"){newColor = yellow;}
    else if (incolor == "green"){newColor = green;}
    // Calculate line direction
    double dx = x2 - x1;
    double dy = y2 - y1;

    // Special case: handle vertical line
    if (x1 == x2) {
        // Draw vertical line
        for (int x = x1 - size; x <= x1 + size; x++) {
        for (int y = 0; y < canvasHeight; y++) {
            // Check if pixel is within the line bounds
            if (y >= std::min(y1, y2) && y <= std::max(y1, y2)) {
                // Color the pixel
                canvas->m_texture->m_image->setPixelColor(x, y, newColor);
            }
        }
        }
    }
    // Special case: handle horizontal line
    else if (y1 == y2) {
        // Draw horizontal line
        for (int y = y1 - size; y <= y1 + size; y++) {
        for (int x = 0; x < canvasWidth; x++) {
            // Check if pixel is within the line bounds
            if (x >= std::min(x1, x2) && x <= std::max(x1, x2)) {
                // Color the pixel
                canvas->m_texture->m_image->setPixelColor(x, y, newColor);
            }
        }
        }
    }

    // Iterate through all pixels in the canvas
    else {
    for (int x = 0; x < canvasWidth; x++) {
        for (int y = 0; y < canvasHeight; y++) {

            // First, check if the pixel is within the bounding box of the line segment
            if (x >= std::min(x1, x2) - size/2 && x <= std::max(x1, x2) + size/2 &&
                y >= std::min(y1, y2) - size/2 && y <= std::max(y1, y2) + size/2) {
                
                // Calculate the perpendicular distance of the pixel (x, y) to the line
                double num = std::abs(dy * x - dx * y + x2 * y1 - y2 * x1);
                double denom = std::sqrt(dy * dy + dx * dx);
                double distance = num / denom;

                // If the pixel is within the line thickness (main line drawing)
                if (distance <= size) {

                    // Assign new color to pixel
                    if ((x >= 0) && (y >= 0) && (x < canvasWidth) && (y < canvasHeight)) {
                        canvas->m_texture->m_image->setPixelColor(x, y, newColor);
                    }
                }
            }

            
        }
    }
    }
    for (int x = 0; x < canvasWidth; x++) {
    for (int y = 0; y < canvasHeight; y++) {
    // Round the ends of the line (rounded caps)
            // Calculate the distance from the pixel (x, y) to both endpoints
            double distToStart = std::sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
            double distToEnd = std::sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));

            // If the pixel is within the rounded caps region at the start or end
            if ((distToStart <= size || distToEnd <= size)) {

                // Assign new color to pixel
                if ((x >= 0) && (y >= 0) && (x < canvasWidth) && (y < canvasHeight)) {
                    canvas->m_texture->m_image->setPixelColor(x, y, newColor);
                }
            }
    }
    }

    // Update the texture
    canvas->m_texture->markForUpdate();
}

// This function will draw a circle of diameter r centered at x,y
void drawCanvasCircle(double x1, double y1, double r, double width, string incolor) {
    // paint color at tool position
                int canvasWidth = (int)canvas->m_texture->m_image->getWidth();
                int canvasHeight = (int)canvas->m_texture->m_image->getHeight();

                cColorb newColor;
                if (incolor == "blue"){newColor = blue;}
                else if (incolor == "red"){newColor = red;}
                else if (incolor == "yellow"){newColor = yellow;}
                else if (incolor == "green"){newColor = green;}

                double sizeOuter = r + width;
                double sizeInner = r - width;
                for (int x=0; x<canvasWidth; x++)
                {
                    for (int y=0; y<canvasHeight; y++)
                    {                        
                        // compute new color percentage
                        double distance = sqrt((double)(pow(x-(x1),2)+pow(y-(y1),2)));
                        if (distance <= sizeOuter && distance >= sizeInner)
                        {
                            // assign new color to pixel
                            if ((x >= 0) && (y >= 0) && (x < (int)canvas->m_texture->m_image->getWidth()) && (y < (int)canvas->m_texture->m_image->getHeight()))
                            {
                                canvas->m_texture->m_image->setPixelColor(x, y, newColor);
                            }

                            
                        }
                    }
                }

                // update texture
                canvas->m_texture->markForUpdate();
}

//function to store each pixel's color values into an array
void StoreColors(){
    int canvasWidth = (int)canvas->m_texture->m_image->getWidth();
    int canvasHeight = (int)canvas->m_texture->m_image->getHeight();

    for (int x=0; x<canvasWidth; x++)
                {
                    for (int y=0; y<canvasHeight; y++)
                    {
                        cColorb newColor;
                        if ((x >= 0) && (y >= 0) && (x < (int)canvas->m_texture->m_image->getWidth()) && (y < (int)canvas->m_texture->m_image->getHeight()))
                            {
                                canvas->m_texture->m_image->getPixelColor(x, y, newColor);
                            }
                            if (newColor == blue) {newColor = brushblue;}
                            else if (newColor == red) {newColor = brushred;}
                            else if (newColor == green) {newColor = brushgreen;}
                            else if (newColor == yellow) {newColor = brushyellow;}
                            // store each rgb value
                            GLubyte R = newColor.getR();
                            GLubyte G = newColor.getG();
                            GLubyte B = newColor.getB();
                        if (newColor != brushblue & newColor != brushred & newColor != brushyellow & newColor != brushgreen) {
                            if ((x<1000) && (y<1000)) {
                            trueRed[x][y] = 5000;
                            trueGreen[x][y] = 5000;
                            trueBlue[x][y] = 5000;
                        }
                        }
                        else{
                        if ((x<1000) && (y<1000)) {
                            trueRed[x][y] = static_cast<int>(R);
                            trueGreen[x][y] = static_cast<int>(G);
                            trueBlue[x][y] = static_cast<int>(B);
                        }
                        }
                    }
                }
}

//function to grade the accuracy of drawing
void GradeAccuracy() {
    int canvasWidth = (int)canvas->m_texture->m_image->getWidth();
    int canvasHeight = (int)canvas->m_texture->m_image->getHeight();
    accuracy = 0.0;
    int resultRed[1000][1000];
    int resultGreen[1000][1000];
    int resultBlue[1000][1000];
    double totalColored = 0;

    drawTime = elapsedTime;
    timerStarted = 0;

    for (int x=0; x<canvasWidth; x++)
                {
                    for (int y=0; y<canvasHeight; y++)
                    {
                        cColorb newColor;
                        if ((x >= 0) && (y >= 0) && (x < (int)canvas->m_texture->m_image->getWidth()) && (y < (int)canvas->m_texture->m_image->getHeight()))
                            {
                                canvas->m_texture->m_image->getPixelColor(x, y, newColor);
                            }

                            // store each rgb value
                            GLubyte R = newColor.getR();
                            GLubyte G = newColor.getG();
                            GLubyte B = newColor.getB();
                        
                        if ((x<1000) && (y<1000)) {
                            if (newColor != brushblue & newColor != brushred & newColor != brushyellow & newColor != brushgreen) {
                            drawnRed[x][y] = 5000;
                            drawnGreen[x][y] = 5000;
                            drawnBlue[x][y] = 5000;
                            }
                            else {
                            drawnRed[x][y] = static_cast<int>(R);
                            drawnGreen[x][y] = static_cast<int>(G);
                            drawnBlue[x][y] = static_cast<int>(B);
                            }
                            
                        //perform check to see if each pixel is the correct color
                        resultRed[x][y] = drawnRed[x][y] - trueRed[x][y];
                        resultGreen[x][y] = drawnGreen[x][y] - trueGreen[x][y];
                        resultBlue[x][y] = drawnBlue[x][y] - trueBlue[x][y];
                        if (trueRed[x][y] == 5000) {
                            if(resultRed[x][y] != 0 & resultGreen[x][y] != 0 & resultBlue[x][y] != 0) {accuracy = accuracy - 0.5;}
                        }
                        
                        else {
                            if(resultRed[x][y] == 0 & resultGreen[x][y] == 0 & resultBlue[x][y] == 0) {accuracy = accuracy + 1;}
                            totalColored = totalColored + 1;
                            }
                        }
                    }
                }
accuracy = (accuracy/totalColored)*100;

labelAccuracy->setText("Accuracy: " + cStr(accuracy, 0) + "%");

}

void StartTimer() {
    if (!timerStarted) {
        timeStart = chrono::high_resolution_clock::now();
        timerStarted = 1;
    }
}


//==============================================================================
/*
    DEMO:    15-paint.cpp

    This example models a virtual paint brush and allows the operator to select
    a color by touching the color palette, and paint the empty canvas.
    The amount of paint  released is function of the contact force magnitude.
    Finally the image can be saved to file.
*/
//==============================================================================

int main(int argc, char* argv[])
{
    //--------------------------------------------------------------------------
    // INITIALIZATION
    //--------------------------------------------------------------------------

    cout << endl;
    cout << "-----------------------------------" << endl;
    cout << "CHAI3D" << endl;
    cout << "Demo: 15-paint" << endl;
    cout << "Copyright 2003-2024" << endl;
    cout << "-----------------------------------" << endl << endl << endl;
    cout << "Keyboard Options:" << endl << endl;
    cout << "[c] - Clear canvas" << endl;
    cout << "[s] - Save image to file as 'myPicture.jpg'" << endl;
    cout << "[f] - Enable/Disable full screen mode" << endl;
    cout << "[m] - Enable/Disable vertical mirroring" << endl;
    cout << "[q] - Exit application" << endl;
    cout << endl << endl;

    // get current path
    bool fileload;
    string currentpath = cGetCurrentPath();


    //--------------------------------------------------------------------------
    // OPEN GL - WINDOW DISPLAY
    //--------------------------------------------------------------------------

    // initialize GLFW library
    if (!glfwInit())
    {
        cout << "failed initialization" << endl;
        cSleepMs(1000);
        return 1;
    }

    // set GLFW error callback
    glfwSetErrorCallback(onErrorCallback);

    // compute desired size of window
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    windowW = 0.8 * mode->height;
    windowH = 0.5 * mode->height;
    int x = 0.5 * (mode->width - windowW);
    int y = 0.5 * (mode->height - windowH);

    // set OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // enable double buffering
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    // set the desired number of samples to use for multisampling
    glfwWindowHint(GLFW_SAMPLES, 4);

    // specify that window should be resized based on monitor content scale
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    // set active stereo mode
    if (stereoMode == C_STEREO_ACTIVE)
    {
        glfwWindowHint(GLFW_STEREO, GL_TRUE);
    }
    else
    {
        glfwWindowHint(GLFW_STEREO, GL_FALSE);
    }

    // create display context
    window = glfwCreateWindow(windowW, windowH, "CHAI3D", NULL, NULL);
    if (!window)
    {
        cout << "failed to create window" << endl;
        cSleepMs(1000);
        glfwTerminate();
        return 1;
    }

    // set GLFW key callback
    glfwSetKeyCallback(window, onKeyCallback);

    // set GLFW window size callback
    glfwSetWindowSizeCallback(window, onWindowSizeCallback);

    // set GLFW framebuffer size callback
    glfwSetFramebufferSizeCallback(window, onFrameBufferSizeCallback);

    // set GLFW window content scaling callback
    glfwSetWindowContentScaleCallback(window, onWindowContentScaleCallback);

    // get width and height of window
    glfwGetFramebufferSize(window, &framebufferW, &framebufferH);

    // set position of window
    glfwSetWindowPos(window, x, y);

    // set window size
    glfwSetWindowSize(window, windowW, windowH);

    // set GLFW current display context
    glfwMakeContextCurrent(window);

    // set GLFW swap interval for the current display context
    glfwSwapInterval(swapInterval);


#ifdef GLEW_VERSION
    // initialize GLEW library
    if (glewInit() != GLEW_OK)
    {
        cout << "failed to initialize GLEW library" << endl;
        glfwTerminate();
        return 1;
    }
#endif


    //--------------------------------------------------------------------------
    // WORLD - CAMERA - LIGHTING
    //--------------------------------------------------------------------------

    // create a new world.
    world = new cWorld();

    // set the background color of the environment
    world->m_backgroundColor.setWhite();

    // create a camera and insert it into the virtual world
    camera = new cCamera(world);
    world->addChild(camera);

    // position and orient the camera
    camera->set(cVector3d(0.8, 0.0, 0.0),    // camera position (eye) original 0.8, 0.0, 0.0
                cVector3d(0.0, 0.0, 0.0),    // lookat position (target)
                cVector3d(0.0, 0.0, 1.0));   // direction of the (up) vector

    // set the near and far clipping planes of the camera
    // anything in front or behind these clipping planes will not be rendered
    camera->setClippingPlanes(0.01, 10.0);

    // set orthographic camera mode
    if (stereoMode == C_STEREO_DISABLED)
    {
        camera->setOrthographicView(1.3);
    }

    // set stereo mode
    camera->setStereoMode(stereoMode);

    // set stereo eye separation and focal length (applies only if stereo is enabled)
    camera->setStereoEyeSeparation(0.01);
    camera->setStereoFocalLength(1.0);

    // set vertical mirrored display mode
    camera->setMirrorVertical(mirroredDisplay);

    // disable multi-pass rendering to handle transparent objects
    camera->setUseMultipassTransparency(true);

    // create a light source
    light = new cDirectionalLight(world);

    // add light to world
    world->addChild(light);

    // enable light source
    light->setEnabled(true);

    // define the direction of the light beam
    light->setDir(-1.0, 0.0,-0.4);


    //--------------------------------------------------------------------------
    // HAPTIC DEVICES / TOOLS
    //--------------------------------------------------------------------------

    // create a haptic device handler
    handler = new cHapticDeviceHandler();

    // get access to the first available haptic device
    handler->getDevice(hapticDevice, 0);

    // retrieve information about the current haptic device
    cHapticDeviceInfo hapticDeviceInfo = hapticDevice->getSpecifications();

    // create a tool (cursor) and insert into the world
    tool = new cToolCursor(world);
    world->addChild(tool);

    // connect the haptic device to the tool
    tool->setHapticDevice(hapticDevice);

    // define a radius for the tool
    double toolRadius = 0.01;

    // set tool radius
    tool->setRadius(toolRadius);

    // map the physical workspace of the haptic device to a larger virtual workspace.
    tool->setWorkspaceRadius(0.4);

    // haptic forces are enabled only if small forces are first sent to the device;
    // this mode avoids the force spike that occurs when the application starts when 
    // the tool is located inside an object for instance. 
    tool->setWaitForSmallForce(true);

    // start the haptic tool
    tool->start();


    //--------------------------------------------------------------------------
    // CREATE OBJECTS
    //--------------------------------------------------------------------------

    // read the scale factor between the physical workspace of the haptic
    // device and the virtual workspace defined for the tool
    double workspaceScaleFactor = tool->getWorkspaceScaleFactor();

    // maximum stiffness property
    double maxStiffness = hapticDeviceInfo.m_maxLinearStiffness / workspaceScaleFactor;


    /////////////////////////////////////////////////////////////////////////
    // PALETTE: 
    /////////////////////////////////////////////////////////////////////////

    // create a mesh
    palette = new cMesh();

    // create a plane
    cCreatePlane(palette, 0.2, 0.2); //0.5, 0.5 original

    // create collision detector
    palette->createBruteForceCollisionDetector();

    // add object to world
    world->addChild(palette);

    // set the position of the object
    palette->setLocalPos(-0.25, -0.45, 0.0); //-0.25, -0.3, 0.0 is original
    palette->rotateAboutGlobalAxisDeg(cVector3d(0,1,0), 90);
    palette->rotateAboutGlobalAxisRad(cVector3d(1,0,0), cDegToRad(90));

    // create texture map
    cTexture2dPtr texture = cTexture2d::create();
    palette->setTexture(texture);

    // load texture image
    fileload = palette->m_texture->loadFromFile(currentpath + "../resources/images/palette2.jpg"); //palette.png original
    if (!fileload)
    {
        cout << "Error - Texture image 'palette.jpg' failed to load correctly." << endl;
        close();
        return (-1);
    }

    // we disable lighting properties for palette
    palette->setUseMaterial(false);

    // convert palette image from RGB to RGBA
    palette->m_texture->m_image->convert(GL_RGBA);

    // we set the white color (0xff, 0xff, 0xff) of the palette image to transparent (0x00).
    palette->m_texture->m_image->setTransparentColor(0xff, 0xff, 0xff, 0x00);

    // enable mipmaps for adaptive texture size rendering
    palette->m_texture->setUseMipmaps(true);
    
    // enable transparency for this object
    palette->setUseTransparency(true);

    // enable texture mapping
    palette->setUseTexture(true);

    // set haptic properties
    palette->m_material->setStiffness(0.5 * maxStiffness);
    palette->m_material->setStaticFriction(0.6);
    palette->m_material->setDynamicFriction(0.4);

    // initialize a default color for tool brush
    paintColor.setBlueRoyal();
    tool->m_hapticPoint->m_sphereProxy->m_material->setColor(paintColor);
    tool->m_hapticPoint->m_sphereProxy->m_material->setColor(brushblue);


    /////////////////////////////////////////////////////////////////////////
    // CANVAS:
    /////////////////////////////////////////////////////////////////////////

    // create a mesh
    canvas = new cMesh();

    // create a plane
    cCreatePlane(canvas, 0.8, 0.8); //0.5, 0.5 original

    // create collision detector
    canvas->createBruteForceCollisionDetector();

    // add object to world
    world->addChild(canvas);

    // set the position of the object
    canvas->setLocalPos(-0.25, 0.15, 0.0); //-0.25, 0.3, 0.0 original
    canvas->rotateAboutGlobalAxisRad(cVector3d(0,1,0), cDegToRad(90));
    canvas->rotateAboutGlobalAxisRad(cVector3d(1,0,0), cDegToRad(90));

    // create texture map
    canvas->m_texture = cTexture2d::create();

    // load texture image
    fileload = canvas->m_texture->loadFromFile(currentpath + "../resources/images/canvas.jpg");
    if (!fileload)
    {
        cout << "Error - Texture image failed to load correctly." << endl;
        close();
        return (-1);
    }

    // create a copy of canvas so that we can clear page when requested
    canvasOriginal = canvas->m_texture->m_image->copy();

    // we disable lighting properties for canvas
    canvas->setUseMaterial(false);

    // enable texture mapping
    canvas->setUseTexture(true);

    // create normal map from texture data
    cNormalMapPtr normalMap = cNormalMap::create();
    normalMap->createMap(canvas->m_texture);
    canvas->m_normalMap = normalMap;

    // set haptic properties
    canvas->m_material->setStiffness(0.4 * maxStiffness);
    canvas->m_material->setStaticFriction(0.30);
    canvas->m_material->setDynamicFriction(0.30);
    canvas->m_material->setHapticTriangleSides(true, false);
    canvas->m_material->setTextureLevel(0.5);

    //set colors
    blue.set((GLubyte)130, (GLubyte)130, (GLubyte)254);
    red.set((GLubyte)254, (GLubyte)130, (GLubyte)130);
    yellow.set((GLubyte)254, (GLubyte)254, (GLubyte)156);
    green.set((GLubyte)130, (GLubyte)254, (GLubyte)149);
    brushblue.set((GLubyte)30, (GLubyte)30, (GLubyte)254);
    brushred.set((GLubyte)254, (GLubyte)30, (GLubyte)30);
    brushyellow.set((GLubyte)234, (GLubyte)254, (GLubyte)56);
    brushgreen.set((GLubyte)30, (GLubyte)154, (GLubyte)49);
    clearCanvas();


    //--------------------------------------------------------------------------
    // WIDGETS
    //--------------------------------------------------------------------------

    // create a font
    font = NEW_CFONT_CALIBRI_20();
    
    // create a label to display the haptic and graphic rate of the simulation
    labelRates = new cLabel(font);
    camera->m_frontLayer->addChild(labelRates);

    // set font color
    labelRates->m_fontColor.setGrayLevel(0.4);

    // create a label with a small message
    labelMessage = new cLabel(font);
    camera->m_frontLayer->addChild(labelMessage);

    // set font color
    labelMessage->m_fontColor.setGrayLevel(0.4);

    // set text message
    labelMessage->setText("select a color from the palette (left), and paint on the canvas (right)");

    // create a label for the accuracy of the drawing
    labelAccuracy = new cLabel(font);
    camera->m_frontLayer->addChild(labelAccuracy);

    // set font color
    labelAccuracy->m_fontColor.setGrayLevel(0.4);

    // create a label for timer
    labelTimer = new cLabel(font);
    camera->m_frontLayer->addChild(labelTimer);

    // set font color
    labelTimer->m_fontColor.setBlack();

    // create a labelfor win time
    labelDrawTime = new cLabel(font);
    camera->m_frontLayer->addChild(labelDrawTime);

    // set font color
    labelDrawTime->m_fontColor.setBlack();


    // create a background
    background = new cBackground();
    camera->m_backLayer->addChild(background);

    // set background properties
    background->setCornerColors(cColorf(1.00f, 1.00f, 1.00f),
                                cColorf(1.00f, 1.00f, 1.00f),
                                cColorf(0.80f, 0.80f, 0.80f),
                                cColorf(0.80f, 0.80f, 0.80f));


    //--------------------------------------------------------------------------
    // VIEWPORT DISPLAY
    //--------------------------------------------------------------------------

    // get content scale factor
    float contentScaleW, contentScaleH;
    glfwGetWindowContentScale(window, &contentScaleW, &contentScaleH);

    // create a viewport to display the scene.
    viewport = new cViewport(camera, contentScaleW, contentScaleH);


    //--------------------------------------------------------------------------
    // START HAPTIC SIMULATION THREAD
    //--------------------------------------------------------------------------

    // create a thread which starts the main haptics rendering loop
    hapticsThread = new cThread();
    hapticsThread->start(renderHaptics, CTHREAD_PRIORITY_HAPTICS);

    // setup callback when application exits
    atexit(close);


    //--------------------------------------------------------------------------
    // MAIN GRAPHIC LOOP
    //--------------------------------------------------------------------------

    // main graphic loop
    while (!glfwWindowShouldClose(window))
    {
        // render graphics
        renderGraphics();

        // process events
        glfwPollEvents();
    }

    // close window
    glfwDestroyWindow(window);

    // terminate GLFW library
    glfwTerminate();

    // exit
    return (0);
}

//------------------------------------------------------------------------------

void onWindowSizeCallback(GLFWwindow* a_window, int a_width, int a_height)
{
    // update window size
    windowW = a_width;
    windowH = a_height;

    // render scene
    renderGraphics();
}

//------------------------------------------------------------------------------

void onFrameBufferSizeCallback(GLFWwindow* a_window, int a_width, int a_height)
{
    // update frame buffer size
    framebufferW = a_width;
    framebufferH = a_height;
}

//------------------------------------------------------------------------------

void onWindowContentScaleCallback(GLFWwindow* a_window, float a_xscale, float a_yscale)
{
    // update window content scale factor
    viewport->setContentScale(a_xscale, a_yscale);
}

//------------------------------------------------------------------------------

void onErrorCallback(int a_error, const char* a_description)
{
    cout << "Error: " << a_description << endl;
}

//------------------------------------------------------------------------------

void onKeyCallback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods)
{
    // filter calls that only include a key press
    if ((a_action != GLFW_PRESS) && (a_action != GLFW_REPEAT))
    {
        return;
    }

    // option - exit
    else if ((a_key == GLFW_KEY_ESCAPE) || (a_key == GLFW_KEY_Q))
    {
        glfwSetWindowShouldClose(a_window, GLFW_TRUE);
    }

    // option c - clear canvas
    else if (a_key == GLFW_KEY_C)
    {
        // copy original image of canvas to texture
        canvasOriginal->copyTo(canvas->m_texture->m_image);

        // update texture
        canvas->m_texture->markForUpdate();

        // update console message
        cout << "> Canvas has been erased.            \r";
    }

    // option s - save canvas to file
    else if (a_key == GLFW_KEY_S)
    {
        // save current texture image to file
        canvas->m_texture->m_image->convert(GL_RGBA);
        canvas->m_texture->m_image->saveToFile("myPicture.bmp");
        canvas->m_texture->m_image->saveToFile("myPicture.jpg");
        canvas->m_texture->m_image->saveToFile("myPicture.png");
        canvas->m_texture->m_image->saveToFile("myPicture.ppm");
        canvas->m_texture->m_image->saveToFile("myPicture.raw");
        canvas->m_texture->m_image->saveToFile("myPicture.gif");

        // update console message
        cout << "> Canvas has been saved to file.     \r";
    }


    // option - toggle fullscreen
    else if (a_key == GLFW_KEY_F)
    {
        // toggle state variable
        fullscreen = !fullscreen;

        // get handle to monitor
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();

        // get information about monitor
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        // set fullscreen or window mode
        if (fullscreen)
        {
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        }
        else
        {
            int w = 0.8 * mode->height;
            int h = 0.5 * mode->height;
            int x = 0.5 * (mode->width - w);
            int y = 0.5 * (mode->height - h);
            glfwSetWindowMonitor(window, NULL, x, y, w, h, mode->refreshRate);
        }

        // set the desired swap interval and number of samples to use for multisampling
        glfwSwapInterval(swapInterval);
        glfwWindowHint(GLFW_SAMPLES, 4);
    }

    // option - toggle vertical mirroring
    else if (a_key == GLFW_KEY_M)
    {
        mirroredDisplay = !mirroredDisplay;
        camera->setMirrorVertical(mirroredDisplay);
    }

    // draw shape 1
    else if (a_key == GLFW_KEY_1)
    {
        drawing1 = true;
    }
    // draw shape 2
    else if (a_key == GLFW_KEY_2)
    {
        drawing2 = true;
    }
    // draw shape 3
    else if (a_key == GLFW_KEY_3)
    {
        drawing3 = true;
    }
    // draw shape 4
    else if (a_key == GLFW_KEY_4)
    {
        drawing4 = true;
    }
    // draw shape 5
    else if (a_key == GLFW_KEY_5)
    {
        drawing5 = true;
    }
    // draw shape 6
    else if (a_key == GLFW_KEY_6)
    {
        drawing6 = true;
    }
}

//------------------------------------------------------------------------------

void close(void)
{
    // stop the simulation
    simulationRunning = false;

    // wait for graphics and haptics loops to terminate
    while (!simulationFinished) { cSleepMs(100); }

    // close haptic device
    tool->stop();

    // delete resources
    delete hapticsThread;
    delete world;
    delete handler;
}

//------------------------------------------------------------------------------

void renderGraphics(void)
{

    static int first = 1;
    if (first){
    //1/16/25 addition
    cout << "preparing to initialize server on server.cpp"<< endl;     
    server.initializeServer(PORT);
    cout << "server initialized on server.cpp"<< endl;
    first=0;
    }


    // sanity check
    if (viewport == nullptr) { return; }

    /////////////////////////////////////////////////////////////////////
    // UPDATE WIDGETS
    /////////////////////////////////////////////////////////////////////

    // get width and height of CHAI3D internal rendering buffer
    int displayW = viewport->getDisplayWidth();
    int displayH = viewport->getDisplayHeight();

    // update haptic and graphic rate data
    labelRates->setText(cStr(freqCounterGraphics.getFrequency(), 0) + " Hz / " +
                        cStr(freqCounterHaptics.getFrequency(), 0) + " Hz");

    //update timer data
    labelTimer->setText("Time elapsed: " + cStr(elapsedTime.count()) + " Seconds");

    //update win time
    labelDrawTime->setText("Draw Time: " + cStr(drawTime.count()) + " Seconds");

    // update position of label
    labelRates->setLocalPos((int)(0.5 * (displayW - labelRates->getWidth())), 15);

    // update position of label
    labelMessage->setLocalPos((int)(0.5 * (displayW - labelMessage->getWidth())), 40);

    // update position of label Accuracy
    labelAccuracy->setLocalPos((int)(0.5 * (displayW - labelAccuracy->getWidth())), (int)(displayH - 25));

        //update position of timer label
    labelTimer->setLocalPos((int)(0.5 * (displayW - labelTimer->getWidth())), displayH - 50);

    //update position of win time label
    labelDrawTime->setLocalPos((int)(0.5 * (displayW - labelDrawTime->getWidth())), displayH - 75);


    /////////////////////////////////////////////////////////////////////
    // RENDER SCENE
    /////////////////////////////////////////////////////////////////////

    // update shadow maps (if any)
    world->updateShadowMaps(false, mirroredDisplay);

    // render world
    viewport->renderView(framebufferW, framebufferH);

    // wait until all GL commands are completed
    glFinish();

    // check for any OpenGL errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) cout << "Error: " << gluErrorString(error) << endl;

    // swap buffers
    glfwSwapBuffers(window);

    // signal frequency counter
    freqCounterGraphics.signal(1);
}

//------------------------------------------------------------------------------

void renderHaptics(void)
{
    // reset clock
    cPrecisionClock clock;
    clock.reset();

    // simulation in now running
    simulationRunning  = true;
    simulationFinished = false;

        //modified 1/23/25 JML
        // Forces
        cVector3d forces; 
        cVector3d linearVelocity;        
        //modified 1/23/25 JML
        cHapticDeviceInfo hapticDeviceInfo = hapticDevice->getSpecifications();  
        //    


    // main haptic simulation loop
    while(simulationRunning)
    {
        /////////////////////////////////////////////////////////////////////
        // SIMULATION TIME
        /////////////////////////////////////////////////////////////////////

        // stop the simulation clock
        clock.stop();

        // read the time increment in seconds
        double timeInterval = clock.getCurrentTimeSeconds();

        // restart the simulation clock
        clock.reset();
        clock.start();


        //set tool radius as factor of x coordinate
        cVector3d hapticPosition;
        hapticDevice->getPosition(hapticPosition);
        if(hapticPosition.x() > -0.023) {
            tool->setRadius(0.01*((hapticPosition.x()*45+2.1)));
        }

        // calculate time elapsed
        if (timerStarted)
        {
            auto timeEnd = chrono::high_resolution_clock::now();
            elapsedTime = timeEnd - timeStart;
        }


        /////////////////////////////////////////////////////////////////////
        // HAPTIC FORCE COMPUTATION
        /////////////////////////////////////////////////////////////////////





        // compute global reference frames for each object
        world->computeGlobalPositions(true);

        // update position and orientation of tool
        tool->updateFromDevice();

        //1/23/25 modified JML
        // compute interaction forces
        tool->computeInteractionForces();
        forces = tool->computeInteractionForces();

        //1/23/25 modified JML
       // server.sendVector(forces);




        // get interaction forces magnitude
        double force = tool->getDeviceGlobalForce().length();


        //1/23/25 modified JML "forces = tool....just like computeInteractionForces" 
        // send forces to haptic device
        forces = tool->applyToDevice();
        forces = -2*forces;
        if( (forces.x() + forces.y() + forces.z() ) >0){};{

            //modified 1/23/25 JML        
            // read linear velocity 
            hapticDevice->getLinearVelocity(linearVelocity);
            //1/23/25 modified JML
            // compute linear damping force
            double Kv = 1.0 * hapticDeviceInfo.m_maxLinearDamping;
            cVector3d forceDamping = -Kv * linearVelocity;
            forces.add(forceDamping);
            //std::cout << "forceDamping: "<<forceDamping<< std::endl;              
        //server.sendVector(forces);   
        } 


        /////////////////////////////////////////////////////////////////////
        // INTERACTION WITH PALETTE
        /////////////////////////////////////////////////////////////////////

        if (tool->isInContact(palette))
        {
            cCollisionEvent* contact = tool->m_hapticPoint->getCollisionEvent(0);
            if (contact != NULL)
            {
                // retrieve contact information
                cVector3d localPos = contact->m_localPos;
                unsigned int triangleIndex = contact->m_index;
                cTriangleArrayPtr triangles = contact->m_triangles;

                // retrieve texture coordinate
                cVector3d texCoord = triangles->getTexCoordAtPosition(triangleIndex, localPos);

                // retrieve pixel information
                int px, py;
                palette->m_texture->m_image->getPixelLocation(texCoord, px, py);

                // retrieve color information at pixel
                cColorb color;
                palette->m_texture->m_image->getPixelColor(px, py, color);

                // ignore colors approaching white as they corresponds to edges of the palette
                if (!((color.getR() > 180) && (color.getG() > 180) && (color.getB() > 180)))
                {
                    // update color of tool
                    paintColor = color;
                    tool->m_hapticPoint->m_sphereProxy->m_material->setColor(paintColor);
                }
            }
        }


        /////////////////////////////////////////////////////////////////////
        // INTERACTION WITH CANVAS
        /////////////////////////////////////////////////////////////////////

        if (tool->isInContact(canvas))
        {
            cCollisionEvent* contact = tool->m_hapticPoint->getCollisionEvent(0);
            if (contact != NULL)
            {
                // retrieve contact information
                cVector3d localPos = contact->m_localPos;
                unsigned int triangleIndex = contact->m_index;
                cTriangleArrayPtr triangles = contact->m_triangles;

                // retrieve texture coordinate
                cVector3d texCoord = triangles->getTexCoordAtPosition(triangleIndex, localPos);

                // retrieve pixel information
                int px, py;
                canvas->m_texture->m_image->getPixelLocation(texCoord, px, py);

                // paint color at tool position
                const double K_INK = 30;
                const double K_SIZE = 10;
                const int BRUSH_SIZE = 25;

                double size = cClamp((K_SIZE * force), 1.0, (double)(BRUSH_SIZE));
                for (int x=-BRUSH_SIZE; x<BRUSH_SIZE; x++)
                {
                    for (int y=-BRUSH_SIZE; y<BRUSH_SIZE; y++)
                    {                        
                        // compute new color percentage
                        double distance = sqrt((double)(x*x+y*y));
                        if (distance <= size)
                        {
                            // get color at location
                            cColorb color, newColor;
                            canvas->m_texture->m_image->getPixelColor(px+x, py+y, color);
                            
                            // compute color factor based of pixel position and force interaction 
                            //double factor = cClamp(K_INK * timeInterval * cClamp(force, 0.0, 10.0) * cClamp(1 - distance/size, 0.0, 1.0), 0.0, 1.0);
                            // 2/11/25 new color factor
                            double factor = cClamp(1.0, 0.0, 1.0);

                            // compute new color
                            GLubyte R = (GLubyte)cClamp(((1.0 - factor) * (double)color.getR() + factor * (double)paintColor.getR()), 30.0, 254.0);
                            GLubyte G = (GLubyte)cClamp(((1.0 - factor) * (double)color.getG() + factor * (double)paintColor.getG()), 30.0, 254.0);
                            GLubyte B = (GLubyte)cClamp(((1.0 - factor) * (double)color.getB() + factor * (double)paintColor.getB()), 30.0, 254.0);
                            newColor.set(R, G, B);

                            // assign new color to pixel
                            int pixelX = px + x;
                            int pixelY = py + y;
                            if ((pixelX >= 0) && (pixelY >= 0) && (pixelX < (int)canvas->m_texture->m_image->getWidth()) && (pixelY < (int)canvas->m_texture->m_image->getHeight()))
                            {
                                canvas->m_texture->m_image->setPixelColor(pixelX, pixelY, newColor);
                            }
                        }
                    }
                }

                // update texture
                canvas->m_texture->markForUpdate();

                // start timer
                StartTimer();
            }
        }



        /////////////////////////////////////////////////////////////////////
        // DRAW CIRCLE ((TEST))
        /////////////////////////////////////////////////////////////////////

       //bool button0;
        //hapticDevice->getUserSwitch(0, button0);

        if (drawing1) {
        clearCanvas();
        drawCanvasCircle(500, 500, 250, 25, "blue");
        StoreColors();
        drawing1 = false;
        timerStarted = 0;
        }



        ///////////////////////////////////////////////////////
        // END OF TEST AREA
        ///////////////////////////////////////////////////////




        /////////////////////////////////////////////////////////////////////
        // DRAW ARROW ((TEST))
        /////////////////////////////////////////////////////////////////////


        if (drawing2)
        {
            clearCanvas();
            drawCanvasLine(500, 500, 900, 900, 25, "red");
            drawCanvasLine(500, 500, 500, 700, 25, "yellow");
            drawCanvasLine(500, 500, 700, 500, 25, "green");
            drawCanvasCircle(250, 250, 175, 25, "blue");
            StoreColors();
            drawing2 = false;
            timerStarted = 0;
        }
        ///////////////////////////////////////////////////////
        // END OF TEST AREA
        ///////////////////////////////////////////////////////



        /////////////////////////////////////////////////////////////////////
        // DRAW TRIANGLE ((TEST))
        /////////////////////////////////////////////////////////////////////


        if (drawing3)
        {
            clearCanvas();
            drawCanvasLine(200, 200, 500, 800, 25, "red");
            drawCanvasLine(500, 800, 800, 200, 25, "red");
            drawCanvasLine(800, 200, 200, 200, 25, "green");
            StoreColors();
            drawing3 = false;
            timerStarted = 0;
        }
        ///////////////////////////////////////////////////////
        // END OF TEST AREA
        ///////////////////////////////////////////////////////


        /////////////////////////////////////////////////////////////////////
        // DRAW SQUARE ((TEST))
        /////////////////////////////////////////////////////////////////////


        if (drawing4)
        {
            clearCanvas();
            drawCanvasLine(200, 200, 200, 800, 25, "red");
            drawCanvasLine(200, 800, 800, 800, 25, "yellow");
            drawCanvasLine(800, 800, 800, 200, 25, "green");
            drawCanvasLine(800, 200, 200, 200, 25, "blue");
            StoreColors();
            drawing4 = false;
            timerStarted = 0;
        }
        ///////////////////////////////////////////////////////
        // END OF TEST AREA
        ///////////////////////////////////////////////////////





        /////////////////////////////////////////////////////////////////////
        // Grade Accuracy ((TEST))
        /////////////////////////////////////////////////////////////////////

        bool button0;
        hapticDevice->getUserSwitch(0, button0);

        if (button0 && (button0 != b0PreviousState)){
            GradeAccuracy();
            cout << "accuracy: " << accuracy << "%" << endl;
            b0PreviousState = button0;
        }

        if (!button0 && (button0 != b0PreviousState)) {b0PreviousState = button0;}



        ///////////////////////////////////////////////////////
        // END OF TEST AREA
        ///////////////////////////////////////////////////////
        cVector3d position;
        hapticDevice->getPosition(position);

        //std::cout << "preparing to sendVector"<< std::endl;          
        server.sendVector(position);
        //std::cout << "sendVector complete"<< std::endl;  

        // signal frequency counter
        freqCounterHaptics.signal(1);
    }
    
    // exit haptics thread
    simulationFinished = true;
}
//------------------------------------------------------------------------------
