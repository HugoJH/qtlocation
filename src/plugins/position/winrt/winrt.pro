TARGET = qtposition_winrt
QT = core core-private positioning

PLUGIN_TYPE = position
PLUGIN_CLASS_NAME = QGeoPositionInfoSourceFactoryWinRT
load(qt_plugin)

SOURCES += qgeopositioninfosource_winrt.cpp \
    qgeopositioninfosourcefactory_winrt.cpp
HEADERS += qgeopositioninfosource_winrt_p.h \
    qgeopositioninfosourcefactory_winrt.h

OTHER_FILES += \
    plugin.json
