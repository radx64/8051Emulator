set(GMOCK_DIR "gmock"
    CACHE PATH "The path to the GoogleMock test framework.")

set(SRC_DIR "../src"
    CACHE PATH "The path to the source files of emulator.")

add_subdirectory(${GMOCK_DIR} ${CMAKE_BINARY_DIR}/gmock)

include_directories(SYSTEM 
					${SRC_DIR}
					${GMOCK_DIR}/gtest/include
                    ${GMOCK_DIR}/include)

file(GLOB TESTS
"testCases/*.cpp"
)


add_executable(EmulatorTests ${SRC} ${TESTS})
target_link_libraries(EmulatorTests gmock_main emulatorCore)

add_test(EmulatorTests EmulatorTests)