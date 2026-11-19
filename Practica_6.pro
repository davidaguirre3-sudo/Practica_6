QT += widgets
QT += multimedia

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    juego.cpp\
    ventanaprincipal.cpp\
    jugador.cpp\
    caja.cpp\
    canon.cpp\
    colisiones.cpp\
    estructura.cpp\
    obstaculo.cpp\
    particula.cpp\
    proyectil.cpp\
    representante.cpp


HEADERS += \
    juego.h\
    ventanaprincipal.h\
    jugador.h\
    caja.h\
    canon.h\
    colisiones.h\
    estructura.h\
    obstaculo.h\
    particula.h\
    proyectil.h\
    representante.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    recursos.qrc

