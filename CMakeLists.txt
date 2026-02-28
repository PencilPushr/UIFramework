# CMakeList.txt : CMake project for UIFramework, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("UIFramework")

# Set C++20
set(CMAKE_CXX_STANDARD 20)
    

# Enable Hot Reload for MSVC compilers if supported.
if(POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:
    $<C_COMPILER_ID:MSVC>,
    $<CXX_COMPILER_ID:MSVC>>,
    $<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,
    $<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

file (GLOB_RECURSE SRC_FILES
	"src/*.cpp"
	"src/*.h"
)

# Add source to this project's executable.
# add_executable (${PROJECT_NAME} "main.cpp" ${SRC_FILES})
add_executable (${PROJECT_NAME} "main.cpp" "src/ui/ImGui.h" "src/ui/ImGui.cpp")

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core"
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    d3d11 
    d3dcompiler.lib 
    dxgi
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
