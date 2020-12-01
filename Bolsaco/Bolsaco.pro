QT       += core gui
QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ActividadPorEmpleadoOptions.cpp \
    Admin.cpp \
    CargaTareas.cpp \
    Cortadora.cpp \
    DataBaseUtils.cpp \
    Extrusora.cpp \
    Filtradora.cpp \
    InicioSesion.cpp \
    Lavadora.cpp \
    NotificationSender.cpp \
    PrintPreview.cpp \
    ProduccionPorMaquinaOptions.cpp \
    Rebobinadora.cpp \
    SimpleCrypt.cpp \
    TrazabilidadPorBobinaOptions.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ActividadPorEmpleadoOptions.h \
    Admin.h \
    CargaTareas.h \
    Cortadora.h \
    DataBaseData.h \
    DataBaseUtils.h \
    Extrusora.h \
    Filtradora.h \
    InicioSesion.h \
    Lavadora.h \
    NotificationSender.h \
    PrintPreview.h \
    ProduccionPorMaquinaOptions.h \
    Rebobinadora.h \
    Result.h \
    SimpleCrypt.h \
    TrazabilidadPorBobinaOptions.h \
    mainwindow.h

FORMS += \
    ActividadPorEmpleadoOptions.ui \
    Admin.ui \
    CargaTareas.ui \
    Cortadora.ui \
    Extrusora.ui \
    Filtradora.ui \
    InicioSesion.ui \
    Lavadora.ui \
    PrintPreview.ui \
    ProduccionPorMaquinaOptions.ui \
    Rebobinadora.ui \
    TrazabilidadPorBobinaOptions.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Images/Correct.png \
    Images/Incorrect.png

RESOURCES += \
    Resources.qrc
