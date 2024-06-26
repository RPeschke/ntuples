 
cmake_minimum_required(VERSION 3.15)

set(ntuple_VERSION 1.0.1)

project(ntuples
	VERSION ${ntuple_VERSION}
	DESCRIPTION "A header-only library for n-tuple operations"
	HOMEPAGE_URL "https://github.com/RPeschke/ntuples"
	LANGUAGES CXX)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ntuples INTERFACE core/include/ntuples/ntuples.hh)
add_library(ntuples::ntuples ALIAS ntuples)


target_include_directories(ntuples INTERFACE  
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/core/include>  
    $<INSTALL_INTERFACE:include>  
)


target_compile_features(ntuples INTERFACE cxx_std_17)




add_executable(test1  tests/test1.cc)
target_link_libraries(test1 PRIVATE ntuples)











install(
  TARGETS ntuples 
  EXPORT ntuples-targets
  )

install(
  EXPORT ntuples-targets
	NAMESPACE ntuples::
	DESTINATION ${CMAKE_INSTALL_DATADIR}/ntuples
  )

install(
  DIRECTORY core/include/ 
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

install(
  FILES cmake/ntuples-config.cmake
  DESTINATION ${CMAKE_INSTALL_DATADIR}/ntuples
  )




