AUX_SOURCE_DIRECTORY(./ SRC)

INCLUDE_DIRECTORIES("../../../depends/RakNet/include/raknet")

LINK_DIRECTORIES("../../../depends/RakNet/Lib/DLL")

ADD_EXECUTABLE(server ${SRC})

TARGET_LINK_LIBRARIES(server pthread RakNetDLL)
