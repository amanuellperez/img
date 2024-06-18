# -------------------------
# Estructura de directorios
# -------------------------
IMG_ROOT=${MY_HOME}/prj/git/img		; export IMG_ROOT
IMG_SRC=${IMG_ROOT}/src				; export IMG_SRC
IMG_MK=${IMG_ROOT}/mk	    ; export IMG_MK

# ---------------------
# Reglas de compilación
# ---------------------
# Reglas de compilación de cada entorno
IMG_COMPRULES=${IMG_MK}/img.mk	; export IMG_COMPRULES


# ------------------------------------------------------
# CPATH = es donde busca g++ los archivos .h (opción -I)
# Movido a cpp/entorno.sh
# ------------------------------------------------------
#CPATH=${CPATH}:${MY_HOME}/include
#export CPATH

# --------------------------------------------------------
# LIBRARY_PATH = donde busca g++ las librerias (opción -L)
# Movido a cpp/entorno.sh
# --------------------------------------------------------
#LIBRARY_PATH=${MY_HOME}/lib
#export LIBRARY_PATH 


# Alias
alias s='cd ${IMG_SRC}'

cd ${IMG_SRC}
