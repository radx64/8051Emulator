mkdir valgrindResults
cd build
valgrind --leak-check=yes ./emulator8051 2> ../valgrindResults/binary.txt
cd test
valgrind --leak-check=yes ./EmulatorTests 2> ../../valgrindResults/tests.txt