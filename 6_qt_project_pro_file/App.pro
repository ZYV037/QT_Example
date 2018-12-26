#-------------------------------------------------
#
# Project created by QtCreator 2017-03-01T09:43:23
#
#-------------------------------------------------

QT       += core network

TARGET = RMS
TEMPLATE = app

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CommonDir = ../../../../../

DESTDIR = $$PWD/$$CommonDir/Bin/wmps\RMS
OBJECTS_DIR = $$PWD/$$CommonDir/Obj/RMS/.obj
MOC_DIR = $$PWD/$$CommonDir/Obj/RMS/.moc

CONFIG(release, debug|release){
    DEFINES+= QT_NO_DEBUG_OUTPUT
    Configuration = release
}else
{
    Configuration = debug
}

INCLUDEPATH += \
     $$PWD/$$CommonDir/Common/CommonInc  \
     $$PWD/$$CommonDir/ThirdParty/Log4cpp/include   \
     $$PWD/$$CommonDir/Common/  \
     $$PWD/../../../Common/ \
     $$PWD/../../../Common/UI/  \
     $$PWD/../../../RMS \
     $$PWD/../Src

LIBS += -L$$PWD/$$CommonDir/Lib/ -lCommonInc
LIBS += -L$$PWD/$$CommonDir/Lib/ -lModuleManager
LIBS += -L$$PWD/$$CommonDir/Lib/ -lModuleEncryptCfgManager
LIBS += -L$$PWD/$$CommonDir/Lib/ -lRmsFlowCtrl
LIBS += -L$$PWD/$$CommonDir/ThirdParty/PlatformLib/

win32 {
    LIBS += -L$$PWD/$$CommonDir/ThirdParty/Log4cpp/lib/$$QT_ARCH/$$Configuration   -llog4cpp
    LIBS += -L$$PWD/$$CommonDir/ThirdParty/EnDecryption/lib/$$QT_ARCH/$$Configuration/
    LIBS += -L$$PWD/$$CommonDir/ThirdParty/Botan/lib/$$QT_ARCH/$$Configuration/
    LIBS += -L$$PWD/$$CommonDir/ThirdParty/Json/lib/$$QT_ARCH/$$Configuration/
    LIBS += -L$$PWD/$$CommonDir/ThirdParty/Curl/lib/$$QT_ARCH/
    LIBS += -L$$PWD/$$CommonDir/ThirdParty/Alg/lib/$$QT_ARCH/
    LIBS += -L$$PWD/$$CommonDir/ThirdParty/Base64/lib/$$QT_ARCH/$$Configuration/
    LIBS += -L$$PWD/$$CommonDir/ThirdParty/Dxf/lib/$$QT_ARCH/$$Configuration
    LIBS += -L$$PWD/$$CommonDir/ThirdParty/Osg/lib/$$QT_ARCH/$$Configuration/

    RC_FILE += app.rc
}

unix {
    LIBS += -L$$PWD/$$CommonDir/ThirdParty/Log4cpp/lib/linux   -llog4cpp
    QMAKE_RPATHDIR += :$$PWD/$$CommonDir/ThirdParty/EnDecryption/lib/linux/
    QMAKE_RPATHDIR += :$$PWD/$$CommonDir/ThirdParty/Botan/lib/linux/
    QMAKE_RPATHDIR += :$$PWD/$$CommonDir/ThirdParty/Json/lib/linux/
    QMAKE_RPATHDIR += :$$PWD/$$CommonDir/ThirdParty/Curl/lib/linux/
    QMAKE_RPATHDIR += :$$PWD/$$CommonDir/ThirdParty/Alg/lib/linux/
    QMAKE_RPATHDIR += :$$PWD/$$CommonDir/ThirdParty/Osg/lib/linux/
    QMAKE_RPATHDIR += :$$PWD/$$CommonDir/ThirdParty/Dxf/lib/linux/

    QMAKE_RPATHDIR += :$$PWD/$$CommonDir/Lib/
    QMAKE_RPATHDIR += :$$PWD/$$CommonDir/ThirdParty/Libusb/lib/linux/
    QMAKE_RPATHDIR += :$$PWD/$$CommonDir/ThirdParty/Base64/lib/linux

    CONFIG(release, debug|release){
        DEFINES += LIB_PATH=\\\"./\\\"
    }
}

SOURCES += main.cpp\
    apploader.cpp \
    ModuleRefMacro.cpp

HEADERS  += apploader.h \
    ModuleRefMacro.h


RESOURCES += \
    ../../../Resource/wtgeo.qrc

DISTFILES += \
    app.rc  \
    $$PWD/$$CommonDir/System/WMPS/Common/WmpsInc/wmpsUIString.h

