# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\SolarSystem_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\SolarSystem_autogen.dir\\ParseCache.txt"
  "SolarSystem_autogen"
  )
endif()
