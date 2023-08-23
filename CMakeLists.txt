 
cmake_minimum_required(VERSION 3.15)

set(ntuple_VERSION 1.0.1)

project(ntuples
	VERSION ${ntuple_VERSION}
	DESCRIPTION "A simple json parsing and stringifying library"
	HOMEPAGE_URL "https://github.com/northwindtraders/ntuples"
	LANGUAGES CXX)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)



add_library(ntuples INTERFACE core/include/ntuples/ntuples.hh)
add_library(ntuples::ntuples ALIAS ntuples)


target_include_directories(ntuples INTERFACE  
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/core/include>  
    $<INSTALL_INTERFACE:core/include>  # <prefix>/core/include
)


target_compile_features(ntuples INTERFACE cxx_std_17)




add_executable(test1  tests/test1.cc)
target_link_libraries(test1 PRIVATE ntuples)



install(TARGETS ntuples EXPORT ntuples-targets)
install(DIRECTORY core/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT ntuples-targets
	NAMESPACE ntuples::
	DESTINATION ${CMAKE_INSTALL_DATADIR}/ntuples)