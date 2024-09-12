cmake_minimum_required(VERSION 3.10)

# 项目名称
project(DeepTask)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含头文件目录
include_directories(Headers)

# 设置vcpkg工具链文件路径
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
else()
    message(FATAL_ERROR "VCPKG_ROOT environment variable is not set")
endif()

# 查找Eigen库
find_package(Eigen3 CONFIG REQUIRED)
# 查找boost库
find_package(Boost REQUIRED)

# 添加boost库的include目录
include_directories(${Boost_INCLUDE_DIRS})


# 设置SFML库的路径
set(SFML_DIR "${CMAKE_SOURCE_DIR}/sdk/SFML")

# 添加SFML的include目录
include_directories("${SFML_DIR}/include")

# 查找SFML库
find_library(SFML_GRAPHICS sfml-graphics PATHS "${SFML_DIR}/lib")
find_library(SFML_WINDOW sfml-window PATHS "${SFML_DIR}/lib")
find_library(SFML_SYSTEM sfml-system PATHS "${SFML_DIR}/lib")

# 如果找不到库，给出错误提示
if(NOT SFML_GRAPHICS OR NOT SFML_WINDOW OR NOT SFML_SYSTEM)
    message(FATAL_ERROR "Could not find SFML libraries")
endif()

# 添加源文件
file(GLOB SOURCES "Source/*.cpp")

# 创建库
# add_library(DeepTaskLib ${SOURCES})
# target_link_libraries(DeepTaskLib PRIVATE Eigen3::Eigen)

# 查找所有测试文件
file(GLOB TEST_SOURCES "Testers/*.cpp")

# 为每个测试文件创建一个可执行文件
foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    if(${TEST_NAME} STREQUAL "CKF")
        add_executable(${TEST_NAME} ${TEST_SOURCE})
        target_link_libraries(${TEST_NAME} PRIVATE Eigen3::Eigen  ${SFML_GRAPHICS} ${SFML_WINDOW} ${SFML_SYSTEM})
        set_target_properties(${TEST_NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/sdk/SFML/bin"
        )
    else()
        add_executable(${TEST_NAME} ${TEST_SOURCE})
        target_link_libraries(${TEST_NAME} PRIVATE ${Boost_LIBRAYIES} Eigen3::Eigen)
        set_target_properties(${TEST_NAME} PROPERTIES
			RUNTIME_OUTPUT_DIRECTORY "bin"
		)
    endif()
endforeach()

