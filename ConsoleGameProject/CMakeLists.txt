add_executable (ConsoleGameProject "ConsoleGameProject.cpp" "ConsoleGameProject.h" "Maps.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ConsoleGameProject PROPERTY CXX_STANDARD 20)
endif()
