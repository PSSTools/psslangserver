cmake_minimum_required(VERSION 2.8)

project(psslangserver)

include(ExternalProject)

if (NOT PACKAGES_DIR)
  set(PACKAGES_DIR ${CMAKE_SOURCE_DIR}/packages)
endif()

ExternalProject_Add(PSSPARSER
	PREFIX pssparser
	SOURCE_DIR "${PACKAGES_DIR}/pssparser"
	CMAKE_CACHE_ARGS
	  -DPACKAGES_DIR:PATH=${PACKAGES_DIR}
	  -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/pssparser
)
	
ExternalProject_Add(LIBLANGSERVER
	PREFIX liblangserver
	SOURCE_DIR "${PACKAGES_DIR}/liblangserver"
	CMAKE_CACHE_ARGS
	  -DPACKAGES_DIR:PATH=${PACKAGES_DIR}
	  -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/liblangserver
)

file(GLOB psslangserver_SRC
  src/*.cpp
  src/*.h)
  
link_directories(
  ${CMAKE_BINARY_DIR}/liblangserver/lib
  ${CMAKE_BINARY_DIR}/pssparser/lib
  ${CMAKE_BINARY_DIR}/pssparser/src/PSSPARSER-build/antlr4/lib
  )

add_executable(psslangserver ${psslangserver_SRC})

target_link_libraries(psslangserver
  liblangserver
  pssparser
  antlr4-runtime
  pthread)
  
target_include_directories(psslangserver PRIVATE
	${PACKAGES_DIR}/liblangserver/src
	${PACKAGES_DIR}/json/include
	${PACKAGES_DIR}/pssparser/src/parser
	)
	
add_dependencies(psslangserver
	LIBLANGSERVER
	)


  