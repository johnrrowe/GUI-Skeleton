pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)


add_library(gui
    GUI.cpp
)

target_include_directories(gui
    PRIVATE
    ${GTKMM_INCLUDE_DIRS}
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
    -O2 
)

