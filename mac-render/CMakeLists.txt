cmake_minimum_required(VERSION 3.20)

project(XXdRender)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
# add_compile_options(-fobjc-arc)
# add_compile_options(-x objective-c++)

# target render
set(XXD_CORE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/xxd-core)

aux_source_directory(${XXD_CORE_SOURCE_DIR}/xxd-log/. XXD_LOG_SOURCE_FILES)
aux_source_directory(${XXD_CORE_SOURCE_DIR}/xxd-render/. XXD_RENDER_SOURCE_FILES)
aux_source_directory(${XXD_CORE_SOURCE_DIR}/xxd-math/. XXD_MATH_SOURCE_FILES)

add_library(XXdMetal SHARED ${XXD_LOG_SOURCE_FILES} ${XXD_RENDER_SOURCE_FILES} ${XXD_MATH_SOURCE_FILES})
set_target_properties(XXdMetal PROPERTIES
    FRAMEWORK TRUE
	FRAMEWORK_VERSION C
	MACOSX_FRAMEWORK_IDENTIFIER YGXXD.XXdMetal 
	XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER YGXXD.XXdMetal
)
target_link_libraries(XXdMetal PUBLIC "-framework Metal" "-framework Foundation" "-framework QuartzCore")

target_include_directories(XXdMetal PUBLIC ${XXD_CORE_SOURCE_DIR}/xxd-log)
target_include_directories(XXdMetal PUBLIC ${XXD_CORE_SOURCE_DIR}/xxd-math)
target_include_directories(XXdMetal PUBLIC ${XXD_CORE_SOURCE_DIR}/xxd-render)
target_include_directories(XXdMetal PUBLIC ${XXD_CORE_SOURCE_DIR}/xxd-template)
target_include_directories(XXdMetal PUBLIC ${XXD_CORE_SOURCE_DIR}/metal-cpp)

# target shader
find_program(XCRUN xcrun)
execute_process(
    COMMAND ${XCRUN} --sdk macosx --find metal
    OUTPUT_VARIABLE METAL
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND ${XCRUN} --sdk macosx --find metallib
	OUTPUT_VARIABLE METALLIB
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(SHADER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/xxd-shader)
file(GLOB SHADER_SOURCE_FILES ${SHADER_SOURCE_DIR}/*.metal)

foreach(SHADER IN LISTS SHADER_SOURCE_FILES) 
	get_filename_component(FILENAME ${SHADER} NAME)
	add_custom_command(
		OUTPUT ${SHADER_SOURCE_DIR}/${FILENAME}.air
		COMMAND ${METAL} ${SHADER} -c -o ${SHADER_SOURCE_DIR}/${FILENAME}.air
		DEPENDS ${SHADER}
		COMMENT "Compling ${FILENAME} To ${FILENAME}.air"
	)
   	list(APPEND METAL_OUTS ${SHADER_SOURCE_DIR}/${FILENAME}.air)
endforeach(SHADER IN LISTS SOURCE_SHADER_FILES)

add_custom_target(XXdShader 
	COMMAND ${METALLIB} ${METAL_OUTS} -o ${SHADER_SOURCE_DIR}/XXdShader.metallib
	DEPENDS ${METAL_OUTS}
	COMMENT "Generating XXdShader.metallib"

)

# link
set(RESOURCE_FILES ${SHADER_SOURCE_DIR}/XXdShader.metallib)
set_source_files_properties(${RESOURCE_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/xxd-test/. CPP_TEST_SOURCE_FILES)
add_executable(${PROJECT_NAME} ${CPP_TEST_SOURCE_FILES} ${RESOURCE_FILES}) 

target_link_libraries(${PROJECT_NAME} "-framework MetalKit" "-framework AppKit")
target_link_libraries(${PROJECT_NAME} XXdMetal) 
add_dependencies(${PROJECT_NAME} XXdShader)

set_target_properties(${PROJECT_NAME} PROPERTIES
	MACOSX_BUNDLE ON
	MACOSX_BUNDLE_BUNDLE_VERSION C
	MACOSX_FRAMEWORK_IDENTIFIER YGXXD.${PROJECT_NAME}
	XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER YGXXD.${PROJECT_NAME}
#	XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC "YES"
#   XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_WEAK "YES"
)

