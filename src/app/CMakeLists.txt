string(CONCAT APP_SOURCES
)

add_executable(app
        main.cpp
        ${APP_SOURCES}
)


target_include_directories(app
        PRIVATE
        ${DOCTEST_INCLUDE_DIRS}
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
    # -fsanitize=address 
    -O2 

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
    -Wmissing-declarations 
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
    # -fsanitize=address 
)


target_compile_definitions(app 
    PRIVATE 
    DOCTEST_CONFIG_DISABLE
)


add_executable(app_test
        TestRunner.cpp
        ${APP_SOURCES}
)


target_include_directories(app_test
        PRIVATE
        ${DOCTEST_INCLUDE_DIRS}
)

