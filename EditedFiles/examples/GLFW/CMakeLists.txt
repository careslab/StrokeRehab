################################################################################
#
#  Software License Agreement (BSD License)
#  Copyright (c) 2003-2024, CHAI3D
#  (www.chai3d.org)
#
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#
#  * Redistributions of source code must retain the above copyright
#  notice, this list of conditions and the following disclaimer.
#
#  * Redistributions in binary form must reproduce the above
#  copyright notice, this list of conditions and the following
#  disclaimer in the documentation and/or other materials provided
#  with the distribution.
#
#  * Neither the name of CHAI3D nor the names of its contributors may
#  be used to endorse or promote products derived from this software
#  without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
#  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
#  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
#  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
#  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
#  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
#  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#  POSSIBILITY OF SUCH DAMAGE.
#
################################################################################

# Common settings required to build examples
include_directories (${GLFW_INCLUDE_DIRS})

# Build all targets.
foreach (example 09-magnets 15-paint client 15-paint-nogui)

    # Collect the UI file (only if it exists)
    set(UI_FILES ${example}/mainwindow.ui)
    if(EXISTS ${UI_FILES})
        message(STATUS "Processing UI files: ${UI_FILES}")
        qt5_wrap_ui(UI_HEADERS ${UI_FILES})
        message(STATUS "Generated UI headers: ${UI_HEADERS}")

        # Collect all source files
        file(GLOB SOURCE ${example}/*.cpp ${example}/*.h)

        # Collect all headers that need MOC processing
        set(MOC_HEADERS)  # Create an empty list for MOC headers
        foreach(HEADER ${SOURCE})
            if(${HEADER} MATCHES ".*\\.h$")
                list(APPEND MOC_HEADERS ${HEADER})
            endif()
        endforeach()

        # Process MOC files (generate moc_ files)
        qt5_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

        # Combine source files, UI headers, and MOC sources
        list(APPEND SOURCE ${MOC_SOURCES} ${UI_HEADERS})

        # Create executable
        add_executable(${example} ${SOURCE})

        # Link the necessary libraries
        target_link_libraries(${example} ${CHAI3D_LIBRARIES} ${GLFW_LIBRARIES} ${QT_LIBRARIES})
    else()
        # For examples without mainwindow.ui, just collect the source files without MOC
        file(GLOB SOURCE ${example}/*.cpp ${example}/*.h)

        # Create executable for examples without mainwindow.ui
        add_executable(${example} ${SOURCE})

        # Link the necessary libraries
        target_link_libraries(${example} ${CHAI3D_LIBRARIES} ${GLFW_LIBRARIES} ${QT_LIBRARIES})
    endif()

    # OS specific adjustments (for macOS)
    if (${CMAKE_SYSTEM_NAME} MATCHES Darwin)
        add_custom_command (TARGET ${example} POST_BUILD COMMAND Rez -append ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../resources/icons/chai3d.rsrc -o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${example} COMMAND SetFile -a C ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${example} VERBATIM)
    endif ()

endforeach()
