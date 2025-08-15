# cmake/ntuples-config.cmake
# Minimal, relocatable config for a header-only INTERFACE target.

if (TARGET ntuples::ntuples)
  return()
endif()

add_library(ntuples::ntuples INTERFACE IMPORTED)

# Point to the installed include/ relative to this config file.
# In vcpkg, this file ends up in: <installed>/share/ntuples/ntuples-config.cmake
# So include dir is two levels up, then /include.
get_filename_component(_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)

target_include_directories(ntuples::ntuples
  INTERFACE
    "${_IMPORT_PREFIX}/include"
)

# Optional: if you require a certain standard
# target_compile_features(ntuples::ntuples INTERFACE cxx_std_20)
