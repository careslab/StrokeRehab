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
//------------------------------------------------------------------------------
#include <GLFW/glfw3.h>
#include <chrono>
#include <fstream>
#include <ctime>
//1/17/25 addition
//#include "falconClientServer/falconClientServer.h"
//------------------------------------------------------------------------------
using namespace chai3d;
using namespace std;
//------------------------------------------------------------------------------

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
// DECLARED CONSTANTS
//------------------------------------------------------------------------------

// number of spheres in the scene
const int NUM_SPHERES = 16;

// radius of each sphere
const double SPHERE_RADIUS = 0.007;

// radius and height of cylinder
const double CYLINDER_RADIUS = 0.021;
const double CYLINDER_HEIGHT = 0.5;



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
cSpotLight *light;

// a haptic device handler
cHapticDeviceHandler* handler;

// a pointer to the current haptic device
cGenericHapticDevicePtr hapticDevice;

// highest stiffness the current haptic device can render
double hapticDeviceMaxStiffness;

// sphere objects
cShapeSphere* spheres[NUM_SPHERES];

// linear velocity of each sphere
cVector3d sphereVel[NUM_SPHERES];

// number of spheres over the line
int spheresOverLine = 0;
int spheresOverLineLoop = 0;

// Cylinder Object
cShapeCylinder* cylinder;

// a colored background
cBackground* background;

// a font for rendering text
cFontPtr font;

// a label to display the rate [Hz] at which the simulation is running
cLabel* labelRates;

// a label to explain what is happening
cLabel* labelMessage;

// a label to display number of spheres over the line
cLabel* labelSpheres;

// a label to display the timer
cLabel* labelTimer;

// a label for the win time
cLabel* labelWinTime;

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

// Timer Started Variable
int timerStarted = 0;
chrono::high_resolution_clock::time_point timeStart = chrono::high_resolution_clock::now();

// Elapsed Time Variable
chrono::duration<double> elapsedTime = chrono::seconds(0);

// Win Time Variable
chrono::duration<double> winTime = chrono::seconds(0);

// Reset Timer Variables
chrono::high_resolution_clock::time_point resetTimeStart = chrono::high_resolution_clock::now();
chrono::duration<double> resetElapsedTime = chrono::seconds(0);
int Resetting = 0;

//clientServerSoftware class test variable
//1/17/25 addition
//SocketClientServer server;


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


//==============================================================================
/*
    DEMO:   09-magnets.cpp

    This example illustrates how to create a simple dynamic simulation using
    small sphere shape primitives. All dynamics and collisions are computed
    in the haptics thread.
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
    cout << "Demo: 09-magnets" << endl;
    cout << "Copyright 2003-2024" << endl;
    cout << "-----------------------------------" << endl << endl << endl;
    cout << "Keyboard Options:" << endl << endl;
    cout << "[f] - Enable/Disable full screen mode" << endl;
    cout << "[m] - Enable/Disable vertical mirroring" << endl;
    cout << "[q] - Exit application" << endl;
    cout << endl << endl;


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
    camera->set(cVector3d(0.3, 0.04, 0.15),    // camera position (eye)
                cVector3d(0.00, 0.04, 0.05),    // lookat position (target)
                cVector3d(0.00, 0.00, 1.00));   // direction of the (up) vector

    // set the near and far clipping planes of the camera
    // anything in front or behind these clipping planes will not be rendered
    camera->setClippingPlanes(0.01, 10.0);

    // set stereo mode
    camera->setStereoMode(stereoMode);

    // set stereo eye separation and focal length (applies only if stereo is enabled)
    camera->setStereoEyeSeparation(0.03);
    camera->setStereoFocalLength(1.8);

    // set vertical mirrored display mode
    camera->setMirrorVertical(mirroredDisplay);

    // create a light source
    light = new cSpotLight(world);

    // attach light to camera
    world->addChild(light);

    // enable light source
    light->setEnabled(true);

    // position the light source
    light->setLocalPos(0.0, 0.3, 0.4);

    // define the direction of the light beam
    light->setDir(0.0,-0.25, -0.4);

    // enable this light source to generate shadows
    light->setShadowMapEnabled(false);

    // set the resolution of the shadow map
    //light->m_shadowMap->setQualityLow();
    light->m_shadowMap->setQualityHigh();

    // set shadow factor
    world->setShadowIntensity(0.3);

    // set light cone half angle
    light->setCutOffAngleDeg(30);


    //--------------------------------------------------------------------------
    // HAPTIC DEVICES / TOOLS
    //--------------------------------------------------------------------------

    // create a haptic device handler
    handler = new cHapticDeviceHandler();

    // get access to the first available haptic device
    handler->getDevice(hapticDevice, 0);

    // retrieve information about the current haptic device
    cHapticDeviceInfo hapticDeviceInfo = hapticDevice->getSpecifications();

    // retrieve the highest stiffness this device can render
    hapticDeviceMaxStiffness = hapticDeviceInfo.m_maxLinearStiffness;

    // if the haptic devices carries a gripper, enable it to behave like a user switch
    hapticDevice->setEnableGripperUserSwitch(true);


    //--------------------------------------------------------------------------
    // CREATE PLANE
    //--------------------------------------------------------------------------

    // create mesh
    cMesh* plane = new cMesh();

    // add mesh to world
    world->addChild(plane);

    // create plane primitive
    cCreateMap(plane, 0.3, 1.0, 20, 20);

    // compile object
    plane->setUseDisplayList(true);

    // set color properties
    plane->m_material->setWhite();


    //--------------------------------------------------------------------------
    // CREATE WALLS AND CEILINGS
    //--------------------------------------------------------------------------

    // create meshes
    cMesh* wallBack = new cMesh();
    cMesh* wallLeft = new cMesh();
    cMesh* wallRight = new cMesh();
    cMesh* ceiling = new cMesh();

    // add meshes to world
    world->addChild(wallBack);
    world->addChild(wallLeft);
    world->addChild(wallRight);
    world->addChild(ceiling);

    //create maps
    //back wall
    cVector3d mapPos = cVector3d(-0.08, 0.03, 0.3);
    cVector3d mapRotAxis = cVector3d(0,1,0);
    cMatrix3d mapRot = cMatrix3d();
    mapRot.setAxisAngleRotationDeg(mapRotAxis, 90);
    cCreateMap(wallBack, 0.6, 0.26, 20, 20, mapPos, mapRot);

    //left wall
    mapPos = cVector3d(0, -0.1, 0.3);
    mapRotAxis = cVector3d(1,0,0);
    mapRot = cMatrix3d();
    mapRot.setAxisAngleRotationDeg(mapRotAxis, 90);
    cCreateMap(wallLeft, 0.4, 0.6, 20, 20, mapPos, mapRot);

    //right wall
    mapPos = cVector3d(0, 0.16, 0.3);
    mapRotAxis = cVector3d(1,0,0);
    mapRot = cMatrix3d();
    mapRot.setAxisAngleRotationDeg(mapRotAxis, 90);
    cCreateMap(wallRight, 0.4, 0.6, 20, 20, mapPos, mapRot);

    // compile objects
    wallBack->setUseDisplayList(true);
    wallLeft->setUseDisplayList(true);
    wallRight->setUseDisplayList(true);

    // set color properties
    wallBack->m_material->setGrayDark();
    wallLeft->m_material->setGrayDark();
    wallRight->m_material->setGrayDark();


    //--------------------------------------------------------------------------
    // CREATE SPHERES
    //--------------------------------------------------------------------------

    // get current path
    bool fileload;
    string currentpath = cGetCurrentPath();

    // create texture
    cTexture2dPtr texture = cTexture2d::create();

    // load texture file
    fileload = texture->loadFromFile(currentpath + "../resources/images/spheremap-3.jpg");
    if (!fileload)
    {
        cout << "Error - Texture image failed to load correctly." << endl;
        close();
        return (-1);
    }

    // create spheres
    for (int i=0; i<NUM_SPHERES; i++)
    {
        // create a sphere and define its radius
        cShapeSphere* sphere = new cShapeSphere(SPHERE_RADIUS);

        // store pointer to sphere primitive
        spheres[i] = sphere;

        // add sphere primitive to world
        world->addChild(sphere);

        // set the position of the object at the center of the world
        //sphere->setLocalPos((0.8 * SPHERE_RADIUS * (double)(i+2) * cos(1.0 * (double)(i))), 
        //                    (0.8 * SPHERE_RADIUS * (double)(i+2) * sin(1.0 * (double)(i)))+0.06, 
        //                    SPHERE_RADIUS);

        // set the position of the object at the right of the wall
        sphere->setLocalPos((4 * SPHERE_RADIUS * -(i/4))+0.06, 
                            (4 * SPHERE_RADIUS * (i%4))+0.06, 
                            SPHERE_RADIUS);

        // set graphic properties of sphere
        sphere->setTexture(texture);
        sphere->m_texture->setSphericalMappingEnabled(true);
        sphere->setUseTexture(true);
        sphere->m_material->setWhite();
    }


    //--------------------------------------------------------------------------
    // CREATE CYLINDER
    //--------------------------------------------------------------------------

    currentpath = cGetCurrentPath();

    // create texture
    cTexture2dPtr textureCyl = cTexture2d::create();

    // load texture file
    fileload = textureCyl->loadFromFile(currentpath + "../resources/images/stonebricks.png");
    if (!fileload)
    {
        cout << "Error - TextureCyl image failed to load correctly." << endl;
        close();
        return (-1);
    }

    // create cylinder object
    cylinder = new cShapeCylinder(CYLINDER_RADIUS, CYLINDER_RADIUS, CYLINDER_HEIGHT);

    // add cylinder primitive to world
    world->addChild(cylinder);

    // set the position of the object at the center of the world
    cylinder->setLocalPos(-0.125,0,0);
    cylinder->rotateAboutGlobalAxisDeg(0,1,0,90);

    // set graphic properties of sphere
    cylinder->setTexture(textureCyl);
    //cylinder->m_texture->setSphericalMappingEnabled(true);
    cylinder->setUseTexture(true);
    cylinder->m_material->setGrayLight();


    //--------------------------------------------------------------------------
    // WIDGETS
    //--------------------------------------------------------------------------

    // create a font
    font = NEW_CFONT_CALIBRI_20();
    
    // create a label to display the haptic and graphic rate of the simulation
    labelRates = new cLabel(font);
    labelRates->m_fontColor.setBlack();
    camera->m_frontLayer->addChild(labelRates);

    // create a label with a small message
    labelMessage = new cLabel(font);
    camera->m_frontLayer->addChild(labelMessage);

    // set font color
    labelMessage->m_fontColor.setBlack();

    // set text message
    labelMessage->setText("Press user switch to pick up sphere\nMove all spheres to the left side of the wall");

    // create a label for spheres over line
    labelSpheres = new cLabel(font);
    camera->m_frontLayer->addChild(labelSpheres);

    // set font color
    labelSpheres->m_fontColor.setBlack();

    // create a label for timer
    labelTimer = new cLabel(font);
    camera->m_frontLayer->addChild(labelTimer);

    // set font color
    labelTimer->m_fontColor.setBlack();

    // create a labelfor win time
    labelWinTime = new cLabel(font);
    camera->m_frontLayer->addChild(labelWinTime);

    // set font color
    labelWinTime->m_fontColor.setBlack();

    // create a background
    background = new cBackground();
    camera->m_backLayer->addChild(background);

    // set aspect ration of background image a constant
    background->setFixedAspectRatio(true);

    // load background image
    fileload = background->loadFromFile(currentpath + "../resources/images/background.png");
    if (!fileload)
    {
        cout << "Error - Image failed to load correctly." << endl;
        close();
        return (-1);
    }


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
    return 0;
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
}

//------------------------------------------------------------------------------

void close(void)
{
    // stop the simulation
    simulationRunning = false;

    // wait for graphics and haptics loops to terminate
    while (!simulationFinished) { cSleepMs(100); }

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
    //cout << "preparing to initialize server on server.cpp"<< endl;     
    //server.initializeServer(PORT);
    //cout << "server initialized on server.cpp"<< endl;
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

    //update sphere over line data
    labelSpheres->setText(cStr(spheresOverLine) + " Spheres in the goal");

    //update timer data
    labelTimer->setText("Time elapsed: " + cStr(elapsedTime.count()) + " Seconds");

    //update win time
    labelWinTime->setText("Win Time: " + cStr(winTime.count()) + " Seconds");

    // update position of label
    labelRates->setLocalPos((int)(0.5 * (displayW - labelRates->getWidth())), 15);

    // update position of message label
    labelMessage->setLocalPos((int)(0 * (displayW - labelMessage->getWidth())), 50);

    //update position of spheres label
    labelSpheres->setLocalPos((int)(0.5 * (displayW - labelSpheres->getWidth())), displayH - 25);

    //update position of timer label
    labelTimer->setLocalPos((int)(0.5 * (displayW - labelTimer->getWidth())), displayH - 50);

    //update position of win time label
    labelWinTime->setLocalPos((int)(0.5 * (displayW - labelWinTime->getWidth())), displayH - 75);




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


    // simulation in now running
    simulationRunning  = true;
    simulationFinished = false;

    // a flag to indicate if haptic forces are active
    bool flagHapticsEnabled = false;

    // reset clock
    cPrecisionClock clock;
    clock.reset();

    // open a connection to haptic device
    hapticDevice->open();

    // calibrate device (if necessary)
    hapticDevice->calibrate();

    // main haptic simulation loop
    while(simulationRunning)
    {
        /////////////////////////////////////////////////////////////////////
        // SIMULATION TIME
        /////////////////////////////////////////////////////////////////////

        // stop the simulation clock
        clock.stop();

        // read the time increment in seconds
        double timeInterval = cMin(0.001, clock.getCurrentTimeSeconds());

        // restart the simulation clock
        clock.reset();
        clock.start();

        // signal frequency counter
        freqCounterHaptics.signal(1);


        /////////////////////////////////////////////////////////////////////////
        // READ HAPTIC DEVICE
        /////////////////////////////////////////////////////////////////////////

        // read position 
        cVector3d position;
        hapticDevice->getPosition(position);

        // read user-switch status (button 0)
        bool button;
        hapticDevice->getUserSwitch(0, button);

        // start timer if button pressed
        if (button && !timerStarted && !Resetting)
        {
            timeStart = chrono::high_resolution_clock::now();
            timerStarted = 1;
        }

        // calculate time elapsed
        if (timerStarted && !Resetting)
        {
            auto timeEnd = chrono::high_resolution_clock::now();
            elapsedTime = timeEnd - timeStart;
        }

        // win conditions and reset
        if (spheresOverLine == 16)
        {
            winTime = elapsedTime;
            Resetting = 1;
            resetTimeStart = chrono::high_resolution_clock::now();
            timerStarted = 0;
        }

        if (Resetting)
        {
            auto resetTimeEnd = chrono::high_resolution_clock::now();
            resetElapsedTime = resetTimeEnd - resetTimeStart;
        }

        //reset delay lasts 5 seconds, then perform reset
        if (resetElapsedTime.count() > 5 && Resetting)
        {
            Resetting = 0;
            for(int i=1; i<NUM_SPHERES; i++)
            {
            spheres[i]->setLocalPos((4 * SPHERE_RADIUS * -(i/4))+0.06, 
                          (4 * SPHERE_RADIUS * (i%4))+0.06, 
                         SPHERE_RADIUS);
            }

            //output win time to txt file
            time_t currentTime = time(0);
            tm* localCurrentTime = localtime(&currentTime);
            char buffer[80];
            strftime(buffer, sizeof(buffer), "%c", localCurrentTime);

            auto currentpath = cGetCurrentPath();
            ofstream outputFile(currentpath + "../WinText/magnetswins.txt", ios::app);

            if (outputFile.is_open())
            {
                outputFile << "\nDate and Time: " << buffer << "\n" << "Win Time: " << winTime.count() << " Seconds\n";
                outputFile.close();
            }
            else
            {
                cout << "Error opening file" << endl;
            }
        }

        /////////////////////////////////////////////////////////////////////////
        // UPDATE SIMULATION
        /////////////////////////////////////////////////////////////////////////

        // position of walls and ground
        const double WALL_GROUND        = 0.0 + SPHERE_RADIUS;
        const double WALL_LEFT          =-0.1;
        const double WALL_RIGHT         = 0.16;
        const double WALL_FRONT         = 0.08;
        const double WALL_BACK          =-0.08;
        const double SPHERE_STIFFNESS   = 1000.0;
        const double SPHERE_MASS        = 0.32; // original: 0.04
        const double K_DAMPING          = 0.996;
        const double K_MAGNET           = 4000.0; // original: 500.0
        const double HAPTIC_STIFFNESS   = 500.0;

        // clear forces for all spheres
        cVector3d sphereFce[NUM_SPHERES];

        for (int i=0; i<NUM_SPHERES; i++)
        {
            sphereFce[i].zero();
        }

        // compute forces for all spheres
        for (int i=0; i<NUM_SPHERES; i++)
        {
            cVector3d force;
            cVector3d pos0 = spheres[i]->getLocalPos();

            // check forces with all other spheres
            for (int j=i+1; j<NUM_SPHERES; j++)
            {
                // init force
                force.zero();
                
                // get position of sphere
                cVector3d pos1 = spheres[j]->getLocalPos();

                // compute direction vector from sphere 0 to 1
                cVector3d dir01 = cNormalize(pos1 - pos0);

                // compute distance between both spheres
                double distance = cDistance(pos0, pos1);

                //compute distance between sphere 0 and all other spheres
                int closestSphere = 0;
                double closestSphereDist = 0;
                if (i == 0) 
                {
                    for (int x=i+1; x<NUM_SPHERES; x++)
                    {
                        //get position of sphere
                        cVector3d pos2 = spheres[x]->getLocalPos();

                        //compute distance from sphere 0 to x
                        double distancex = cDistance(pos0, pos2);
                        if((closestSphereDist == 0) || (distancex < closestSphereDist))
                        {
                            closestSphere = x;
                            closestSphereDist = distancex;
                        }
                    }
                }

                // compute magnetic force
                if ((button) && (i == 0) && (j == closestSphere))
                {
                    if ((distance < 2.5 * SPHERE_RADIUS) && (distance > 2.0 * SPHERE_RADIUS))
                    {
                        force.add(-K_MAGNET * (distance - 2.5 * SPHERE_RADIUS) * dir01);
                    }
                }

                // compute contact force
                if (distance < 2.0 * SPHERE_RADIUS)
                {
                    force.add(SPHERE_STIFFNESS * (distance - 2.0 * SPHERE_RADIUS) * dir01);
                }

                // add force to each sphere
                sphereFce[i].add(force);
                sphereFce[j].add(-force);
            }

            // check forces with ground and walls
            if (pos0.z() < WALL_GROUND)
            {
                sphereFce[i].add(cVector3d(0.0, 0.0, SPHERE_STIFFNESS * (WALL_GROUND - pos0.z())));
            }
            if (pos0.y() < WALL_LEFT)
            {
                sphereFce[i].add(cVector3d(0.0, SPHERE_STIFFNESS * (WALL_LEFT - pos0.y()), 0.0));
            }
            if (pos0.y() > WALL_RIGHT)
            {
                sphereFce[i].add(cVector3d(0.0, SPHERE_STIFFNESS * (WALL_RIGHT - pos0.y()), 0.0));
            }
            if (pos0.x() < WALL_BACK)
            {
                sphereFce[i].add(cVector3d(SPHERE_STIFFNESS * (WALL_BACK - pos0.x()), 0.0, 0.0));
            }
            if (pos0.x() > WALL_FRONT)
            {
                sphereFce[i].add(cVector3d(SPHERE_STIFFNESS * (WALL_FRONT - pos0.x()), 0.0, 0.0));
            }

            //check force with cylinder
            cVector3d newSphereFce = cVector3d(0,pos0.y(),pos0.z());
            if (newSphereFce.length() < SPHERE_RADIUS + CYLINDER_RADIUS)
            {
                newSphereFce.mul(SPHERE_STIFFNESS * 60 * (CYLINDER_RADIUS + SPHERE_RADIUS - newSphereFce.length()));
                sphereFce[i].add(newSphereFce);
            }

            //check if sphere is over line

            if (pos0.y() < 0)
            {
                spheresOverLineLoop ++;
            }
        }
        spheresOverLine = spheresOverLineLoop;
        spheresOverLineLoop = 0;


        // compute haptic force
        cVector3d dirHapticSphere = spheres[0]->getLocalPos() - position*3;
        cVector3d forceSphere =-SPHERE_STIFFNESS * dirHapticSphere;
        cVector3d forceHaptic = HAPTIC_STIFFNESS * dirHapticSphere;
        sphereFce[0].add(forceSphere);

        // update velocity and position of all spheres
        for (int i=0; i<NUM_SPHERES; i++)
        {
            // compute acceleration
            cVector3d sphereAcc = (sphereFce[i] / SPHERE_MASS) + cVector3d(0.0, 0.0, -9.81);

            // compute velocity
            sphereVel[i] = K_DAMPING * (sphereVel[i] + timeInterval * sphereAcc);

            // compute position
            cVector3d spherePos = spheres[i]->getLocalPos() + timeInterval * sphereVel[i] + cSqr(timeInterval) * sphereAcc;

            // update value to sphere object
            spheres[i]->setLocalPos(spherePos);
        }


        /////////////////////////////////////////////////////////////////////////
        // APPLY FORCES
        /////////////////////////////////////////////////////////////////////////

        // haptic forces are only enabled if a small value is first sent to the device
        if (!flagHapticsEnabled)
        {
            // check for small force
            if (forceHaptic.length() < 1.0)
            {
                flagHapticsEnabled = true;
            }
            else
            {
                forceHaptic.zero();
            }
        }

        // scale the force according to the max stiffness the device can render
        double stiffnessRatio = 1.0;
        if (hapticDeviceMaxStiffness < HAPTIC_STIFFNESS)
        {
            stiffnessRatio = hapticDeviceMaxStiffness / HAPTIC_STIFFNESS;
        }

        // send computed force to haptic device
        hapticDevice->setForce(stiffnessRatio * forceHaptic);
        //1/17/25 addition
        //std::cout << "preparing to sendVector"<< std::endl;          
        //server.sendVector(stiffnessRatio * forceHaptic);
        //std::cout << "sendVector complete"<< std::endl;        
    }

    // close  connection to haptic device
    hapticDevice->close();

    // exit haptics thread
    simulationFinished = true;
}

//------------------------------------------------------------------------------
