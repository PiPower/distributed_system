file(GLOB networksrc network_module/*.cpp)
file(GLOB clientsrc client_module/*.cpp)

add_executable(network_simulator ${networksrc})
add_library(client STATIC ${clientsrc})