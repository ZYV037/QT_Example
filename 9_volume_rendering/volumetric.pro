QT += widgets
QT += xml
QT += datavisualization

SUBDIRS += \
    volumetric.pro

DISTFILES += \
    volumetric.pro.user

HEADERS += \
    histogram.h \
    indexmapper.h \
    inversion3drightdockwidget.h \
    inversion3dwidget.h \
    rangecolorcontrolpointconnection.h \
    rangecolordefine.h \
    rangecoloredit.h \
    rangecoloreditcontrolpoint.h \
    rangecoloreditview.h \
    rangecolorsetdialog.h \
    trtparser.h \
    typedef.h \
    unity.h \
    volumeramhistogram.h \
    volumetric.h \
    wdd_ts_string.h \
    wtgeodockwidget.h \
    wtgeodockwidgettitlebar.h

SOURCES += \
    histogram.cpp \
    inversion3drightdockwidget.cpp \
    inversion3dwidget.cpp \
    main.cpp \
    rangecolorcontrolpointconnection.cpp \
    rangecoloredit.cpp \
    rangecoloreditcontrolpoint.cpp \
    rangecoloreditview.cpp \
    rangecolorsetdialog.cpp \
    trtparser.cpp \
    unity.cpp \
    volumetric.cpp \
    wtgeodockwidget.cpp \
    wtgeodockwidgettitlebar.cpp

