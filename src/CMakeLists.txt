if(NOT DEFINED ENV{XROOTD_ROOT})
message(FATAL_ERROR "XROOTD_ROOT environment variable should point to your XRootD install dir")
else()
SET(XROOTD_ROOT $ENV{XROOTD_ROOT})
endif()

if(NOT DEFINED ENV{LUSTRE_ROOT})
message(FATAL_ERROR "LUSTRE_ROOT environment variable should point to your Lustre(-Utils) install dir")
else()
SET(LUSTRE_ROOT $ENV{LUSTRE_ROOT})
endif()
include_directories( ${LUSTRE_ROOT}/include )
include_directories( ${XROOTD_ROOT}/include/xrootd )
add_library (Lust STATIC lfs.c qsStruct.h )
add_library( LustreOss SHARED
             LustreOss.cc LustreOss.hh qsStruct.h )
         

add_definitions( -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 )

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    if( Linux )
    target_link_libraries( LustreOss  Lust)
    set_target_properties( LustreOss PROPERTIES
        VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
        SOVERSION ${VERSION_MAJOR}
        CLEAN_DIRECT_OUTPUT 1
    )
    endif( Linux )

    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -g -Wall -g -O2 -Werror ")
    SET(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -g -Wall -g -O2 -Werror ")
    INSTALL( TARGETS LustreOss LIBRARY DESTINATION lib/${PROJECT_NAME} )
