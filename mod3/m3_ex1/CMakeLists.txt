set(
    Graphs
    src/CListGraph.cpp
    src/CMatrixGraph.cpp
    src/CSetGraph.cpp
    src/CArcGraph.cpp
    src/GraphTraversal.cpp
    include/IGraph.h
    include/CListGraph.h
    include/CMatrixGraph.h
    include/CSetGraph.h
    include/CArcGraph.h
    include/GraphTraversal.h
)

add_executable(m3_ex1 m3_ex1.cpp ${Graphs})
target_include_directories(m3_ex1 PRIVATE include)