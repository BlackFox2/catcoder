cmake_minimum_required(VERSION 3.1)
project(catcoder)



#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "-std=c99 -lm")

#set(SOURCE_FILES bowling.c)
#add_executable(catcoder ${SOURCE_FILES} bowling.c auction_bidding.c)

set(SOURCE_FILES auction_bidding.c)
add_executable(auction_bidding auction_bidding.c)
target_link_libraries(auction_bidding m)