pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)


add_library(gui
    Application.cpp
)

target_include_directories(gui
    PRIVATE
    ${GTKMM_INCLUDE_DIRS}
    ${SOURCE_DIR}
)

target_link_libraries(gui
    PRIVATE
    ${GTKMM_LIBRARIES}
)

target_link_directories(gui 
    PRIVATE 
    ${GTKMM_LIBRARY_DIRS}
)

target_compile_options(gui 
    PRIVATE 
    ${GTKMM_CFLAGS_OTHER}
    -std=c++17
    -O2 
)

