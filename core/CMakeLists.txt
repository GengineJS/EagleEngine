IF(APPLE)
        set_source_files_properties(./EagleApplication.cpp PROPERTIES
        LANGUAGE OBJCXX
        )
ENDIF(APPLE)
file(GLOB BASE_SRC "*.cpp" "*.hpp" "*.h" "../external/imgui/*.cpp")
file(GLOB BASE_HEADERS "*.hpp" "*.h")
file(GLOB GRAPHICS "graphics/*.cpp"
        "graphics/*.hpp"
        "graphics/*.h"
        "graphics/vulkan/*.cpp"
        "graphics/vulkan/*.hpp"
        "graphics/vulkan/*.h")
file(GLOB UTILS "utils/*.cpp"
        "utils/*.hpp"
        "utils/*.h")
file(GLOB PIPELINE "pipeline/*.cpp"
        "pipeline/*.hpp"
        "pipeline/*.h"
        "pipeline/render-graph/*.cpp"
        "pipeline/render-graph/*.hpp"
        "pipeline/render-graph/*.h")
set(KTX_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../external/ktx)
set(KTX_SOURCES
    ${KTX_DIR}/lib/texture.c
    ${KTX_DIR}/lib/hashlist.c
    ${KTX_DIR}/lib/checkheader.c
    ${KTX_DIR}/lib/swap.c
    ${KTX_DIR}/lib/memstream.c
    ${KTX_DIR}/lib/filestream.c)

add_library(core STATIC ${BASE_SRC} ${GRAPHICS} ${UTILS} ${PIPELINE} ${KTX_SOURCES})
if(WIN32)
    target_link_libraries(core ${Vulkan_LIBRARY} ${WINLIBS})
 else(WIN32)
    target_link_libraries(core ${Vulkan_LIBRARY} ${XCB_LIBRARIES} ${WAYLAND_CLIENT_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endif(WIN32)