# Reglas para compilar la librería img

include $(CPP_ENVRULES)


# Variables genéricas de compilación del proyecto
PROJ_CXXFLAGS=-I$(CPP_INCLUDE)/alp
PROJ_LDFLAGS=-L$(INSTALL_LIB) -lalp

include $(CPP_GENRULES)


