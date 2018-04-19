cmake_minimum_required(VERSION 3.0)
project(Polynomial_Root_Finding_pGA)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        World/Individual.cpp
        World/Individual.h
        World/Population.cpp
        World/Population.h
        World.h
        Local.h
        main.cpp
        Local.cpp
        Processes/Process.cpp
        Processes/Process.h
        Processes/Worker.cpp
        Processes/Worker.h
        Processes/Master.cpp
        Processes/Master.h
        World/Polynomial.cpp
        World/Polynomial.h)

add_executable(Polynomial_Root_Finding_pGA ${SOURCE_FILES})


#require mpi
find_package(MPI REQUIRED)
if (MPI_FOUND)
    set(CMAKE_CXX_COMPILER /usr/bin/mpic++)
    set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
    set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
    include_directories(MPI_INCLUDE_PATH)
    target_link_libraries(Polynomial_Root_Finding_pGA ${MPI_LIBRARIES})
else (MPI_FOUND)
    message(SEND_ERROR "This application cannot compile without MPI")
endif (MPI_FOUND)





