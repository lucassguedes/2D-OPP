cmake_minimum_required(VERSION 3.12)


project(2DOPP
		VERSION 1.0.0
		DESCRIPTION "2D-OPP"
		HOMEPAGE_URL "lucassguedes.github.io"
		)



set(CPLEXDIR "/opt/ibm/ILOG/CPLEX_Studio201/cplex")
set(CONCERTDIR "/opt/ibm/ILOG/CPLEX_Studio201/concert")
set(CPLEXLIBDIR "${CPLEXDIR}/lib/x86-64_linux/static_pic")
set(CONCERTLIBDIR "${CONCERTDIR}/lib/x86-64_linux/static_pic")

set(CONCERTINCDIR "${CONCERTDIR}/include")
set(CPLEXINCDIR "${CPLEXDIR}/include")

find_package(OpenGL REQUIRED)

find_package(GLEW REQUIRED)

include_directories("/usr/include/GLFW/"
					"/usr/include/GL"
					"/usr/include/glm/"
					${CPLEXINCDIR}
					${CONCERTINCDIR}
					)


file(GLOB CONCERT_LIBS "${CONCERTLIBDIR}/*.a")
file(GLOB CPLEX_LIBS "${CPLEXLIBDIR}/*.a")

message("Concert libs: ${CONCERT_LIBS}")
message("Cplex libs: ${CPLEX_LIBS}")

					

link_libraries(glfw
				"/lib/x86_64-linux-gnu/libGLEW.so"
				"/lib/x86_64-linux-gnu/libOpenGL.so"
				"/lib/x86_64-linux-gnu/libglfw.so"
				"/lib/x86_64-linux-gnu/libglut.so"
				
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW
)
		

set(CMAKE_C_STANDARD 17) # Set C++11 as the standard version
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./bin")
add_compile_options(-g -Wall -lGL -lGLU -lglut -lGLEW -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# add_compile_options(-L${CPLEXLIBDIR} -lilocplex -lcplex -L${CONCERTLIBDIR} -lconcert -lm -ldl)

# add_compile_options(-lilocplex -lcplex -lconcert -lm -ldl -DNDEBUG -DIL_STD)


# add_compile_options(-I${CPLEXINCDIR} -I${CONCERTINCDIR})

# add_compile_options(-DPERFORMANCE_REPORT=ON)

file(GLOB SOURCES src/*.c)

add_executable(2DOPP ${SOURCES})

target_link_libraries(2DOPP 
					-L${CONCERTLIBDIR} 
					 -L${CPLEXLIBDIR})					

target_link_libraries(2DOPP -lilocplex -lcplex -lconcert -lm -ldl)

# link_directories(${CONCERT_LIBS}
# 					  ${CPLEX_LIBS})

target_include_directories(2DOPP PUBLIC
						   include
						   "include/data_structures"
)

