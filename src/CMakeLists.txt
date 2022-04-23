add_executable(test
        TestRunner.cpp
)

target_include_directories(test
        PRIVATE
        ../deps/doctest/doctest
)
