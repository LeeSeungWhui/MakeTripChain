#최소 버젼
cmake_minimum_required(VERSION 2.8)

#프로젝트 이름
project(MakeTripChain)

# 컴파일플래그
set(CMAKE_CXX_FLAGS "-std=c++0x")

#컴파일러
set(CMAKE_C_COMPILER "g++")

# 인클루드 디렉토리
INCLUDE_DIRECTORIES(/usr/include)

# 라이브러리
LINK_LIBRARIES(mysqlcppconn-static mysqlclient)

# 라이브러리 디렉토리
LINK_DIRECTORIES(/usr/lib)

# 컴파일 옵션
ADD_COMPILE_OPTIONS(-g -Wall)

# 소스파일
set(SOURCE_FILES ../src/Main.cpp ../src/TripChain/TripChain.cpp ../src/TripChain/TripChainData.cpp ../src/DbConnection/DbConnection.cpp ../src/Config/ConfigFile.cpp)

# 출력 디렉토리
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

# 바이너리
add_executable(MakeTripChain ${SOURCE_FILES})

# 라이브러리 연결
target_link_libraries(MakeTripChain dl)
target_link_libraries(MakeTripChain pthread)
target_link_libraries(MakeTripChain mysqlclient)
target_link_libraries(MakeTripChain mysqlcppconn-static)

# Install 디렉토리
install(FILES ../bin/MakeTripChain DESTINATION ~/MakeTripChain)
