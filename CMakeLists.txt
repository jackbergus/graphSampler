cmake_minimum_required(VERSION 3.6)
project(graphSampler)

FIND_PACKAGE( Boost 1.62 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

set(THREADS_PREFER_PTHREAD_FLAG ON)
#find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(SOURCE_FILES unordered.cpp)
#set(SOURCE_FILES main.cpp rbtree/Node.h rbtree/enums.h rbtree/rbtree.h random/RanHash.cpp random/RanHash.h future/forward/optional.h future/unveil.h numeric/compares.cpp algorithm/SamplingAlgorithm.cpp algorithm/SamplingAlgorithm.h algorithm/SamplingAlgorithmCache.cpp algorithm/SamplingAlgorithmCache.h random/RandomFileLine.cpp random/RandomFileLine.h random/RandomGMarkFile.cpp random/RandomGMarkFile.h algorithm/quicksortFile.cpp algorithm/quicksortFile.h printVertex.cpp printVertex.h algorithm/argsSampling.cpp algorithm/argsSampling.h)

add_executable(graphSampler ${SOURCE_FILES})
#target_link_libraries(graphSampler Threads::Threads)
TARGET_LINK_LIBRARIES( graphSampler LINK_PUBLIC ${Boost_LIBRARIES} )
