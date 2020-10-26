cmake_minimum_required(VERSION 2.8)

project(psslangserver)

include(ExternalProject)

if (NOT PACKAGES_DIR)
  set(PACKAGES_DIR ${CMAKE_SOURCE_DIR}/packages)
endif()

message("CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")

ExternalProject_Add(PSSPARSER
	PREFIX pssparser
	SOURCE_DIR "${PACKAGES_DIR}/pssparser"
	CMAKE_CACHE_ARGS
	  -DPACKAGES_DIR:PATH=${PACKAGES_DIR}
	  -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/pssparser
	  -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
	  -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
)
	
ExternalProject_Add(LIBLANGSERVER
	PREFIX liblangserver
	SOURCE_DIR "${PACKAGES_DIR}/liblangserver"
	CMAKE_CACHE_ARGS
	  -DPACKAGES_DIR:PATH=${PACKAGES_DIR}
	  -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/liblangserver
	  -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
	  -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
)

set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
foreach(CompilerFlag ${CompilerFlags})
  string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
  string(REPLACE "/MDd" "/MTd" ${CompilerFlag} "${${CompilerFlag}}")
endforeach()

file(GLOB psslangserver_SRC
  src/*.cpp
  src/*.h)
  
link_directories(
  ${CMAKE_BINARY_DIR}/liblangserver/lib
  ${CMAKE_BINARY_DIR}/pssparser/lib
  ${CMAKE_BINARY_DIR}/pssparser/src/PSSPARSER-build/antlr4/lib
  ${CMAKE_BINARY_DIR}/pssparser/src/PSSPARSER-build/pssast/lib
  )

add_executable(psslangserver ${psslangserver_SRC})
set_property(TARGET psslangserver PROPERTY
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

list(APPEND PSSLANGSERVER_LIBS liblangserver pssparser pssast)

if (WIN32)
list(APPEND PSSLANGSERVER_LIBS antlr4-runtime-static ws2_32)
#list(APPEND PSSLANGSERVER_LIBS antlr4-runtime ws2_32)
else()
list(APPEND PSSLANGSERVER_LIBS antlr4-runtime pthread)
endif()

target_link_libraries(psslangserver ${PSSLANGSERVER_LIBS})
  
target_include_directories(psslangserver PRIVATE
	${PACKAGES_DIR}/liblangserver/src
	${PACKAGES_DIR}/json/include
	${PACKAGES_DIR}/pssparser/src
	${CMAKE_BINARY_DIR}/pssparser/src/PSSPARSER-build/pssast/src
	)
	
add_dependencies(psslangserver
	LIBLANGSERVER
	PSSPARSER
	)


 


