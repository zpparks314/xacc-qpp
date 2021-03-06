set (PACKAGE_NAME "QPP XACC Accelerator")
set (PACKAGE_DESCIPTION "QPP XACC Accelerator Framework")
set (LIBRARY_NAME xacc-qpp-accelerator)

file (GLOB_RECURSE HEADERS *.hpp)
file (GLOB SRC *.cpp)

# Set up dependencies to resources to track changes
usFunctionGetResourceSource(TARGET ${LIBRARY_NAME} OUT SRC)
# Generate bundle initialization code
usFunctionGenerateBundleInit(TARGET ${LIBRARY_NAME} OUT SRC)

add_library(${LIBRARY_NAME} SHARED ${SRC})

set(_bundle_name xacc_qpp_accelerator)

set_target_properties(${LIBRARY_NAME} PROPERTIES
  # This is required for every bundle
  COMPILE_DEFINITIONS US_BUNDLE_NAME=${_bundle_name}
  # This is for convenience, used by other CMake functions
  US_BUNDLE_NAME ${_bundle_name}
)


# Embed meta-data from a manifest.json file
usFunctionEmbedResources(TARGET ${LIBRARY_NAME}
  WORKING_DIRECTORY
  ${CMAKE_CURRENT_SOURCE_DIR}
  FILES
  manifest.json
)

if(APPLE)
   set_target_properties(${LIBRARY_NAME} PROPERTIES INSTALL_RPATH "@loader_path/../lib")
   set_target_properties(${LIBRARY_NAME} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
else()
   set_target_properties(${LIBRARY_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")
   set_target_properties(${LIBRARY_NAME} PROPERTIES LINK_FLAGS "-shared")
endif()


install(TARGETS ${LIBRARY_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/plugins)
