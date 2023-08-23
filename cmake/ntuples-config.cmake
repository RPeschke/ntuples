include("${CMAKE_CURRENT_LIST_DIR}/ntuples-targets.cmake")

if (TARGET ntuples::ntuples)
  return()
endif()

add_library(ntuples::ntuples INTERFACE IMPORTED)