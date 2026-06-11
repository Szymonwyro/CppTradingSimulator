cmake_minimum_required(VERSION 3.15)
project(OrderbookSim CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include headers
include_directories(include)

# Collect all source files
file(GLOB_RECURSE ENGINE_SOURCES     "src/engine/*.cpp")
file(GLOB_RECURSE AGENT_SOURCES      "src/agents/*.cpp")
file(GLOB_RECURSE ORDERBOOK_SOURCES  "src/orderbook/*.cpp")
file(GLOB_RECURSE STRATEGY_SOURCES     "src/strategies/*.cpp")

set(LIB_SOURCES
    ${ENGINE_SOURCES}
    ${AGENT_SOURCES}
    ${ORDERBOOK_SOURCES}
    ${STRATEGY_SOURCES}
)

# Tests / executables
add_executable(price_sim            src/tests/PriceSim.cpp          ${LIB_SOURCES})
add_executable(as_test              src/tests/AS_Test.cpp            ${LIB_SOURCES})
add_executable(momentum_trader_test src/tests/MomentumTraderTest.cpp ${LIB_SOURCES})
add_executable(random_trader_test   src/tests/RandomTraderTest.cpp   ${LIB_SOURCES})