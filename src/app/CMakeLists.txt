set(APP_SOURCES
    EventHandler.cpp
)

add_executable(app
    main.cpp
    ${APP_SOURCES}
)


target_include_directories(app
    PRIVATE
    ${SOURCE_DIR}
)


target_link_libraries(app
    PRIVATE
    gui
)


target_compile_options(app 
    PRIVATE

    #FEATURES
    -std=c++20
    -fcoroutines  
    -fconcepts 
    -fconcepts-diagnostics-depth=2 
    # -O2 
    -ggdb

    # WARNINGS
    -Wpedantic  
    -Wall 
    -Wextra 
    -Werror 
    -Winit-self 
    -Wcast-align 
    -Wcast-qual 
    -Wdisabled-optimization 
    -Wlogical-op 
    -Wmissing-include-dirs 
    -Woverloaded-virtual 
    -Wstrict-null-sentinel 
    -Wctor-dtor-privacy 
    -Wfloat-equal 
    -Wredundant-decls   
    -Wsign-conversion 
    -Wsign-promo  
    -Wstrict-overflow=5 
    -Wswitch-default 
)


target_link_options(app
    PRIVATE
)


target_compile_definitions(app 
    PRIVATE 
)
