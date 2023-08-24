 
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
    $<INSTALL_INTERFACE:include>  # <prefix>/core/include
)


target_compile_features(ntuples INTERFACE cxx_std_17)




add_executable(test1  tests/test1.cc)
target_link_libraries(test1 PRIVATE ntuples)


set(version_config ${PROJECT_BINARY_DIR}/ntuples-config-version.cmake)
set(project_config ${PROJECT_BINARY_DIR}/ntuples-config.cmake)


set(targets_export_name ntuples-targets)
set(INSTALL_TARGETS ntuples)

set(ntuples_CMAKE_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/ntuples )

install(TARGETS ${INSTALL_TARGETS} EXPORT ${targets_export_name})
export(TARGETS ${INSTALL_TARGETS} NAMESPACE ntuples::       FILE ${PROJECT_BINARY_DIR}/${targets_export_name}.cmake           )

install(DIRECTORY core/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES
  cmake/ntuples-config.cmake
  DESTINATION share/ntuples)


install(EXPORT ${targets_export_name}
  
	NAMESPACE ntuples::
	DESTINATION ${CMAKE_INSTALL_DATADIR}/ntuples
  )

