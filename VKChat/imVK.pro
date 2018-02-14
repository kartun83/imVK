APP_NAME = imVK

CONFIG += qt warn_on cascades10 
QT += network
LIBS += -lbbdata -lbbnetwork -lbbplatform -lbbpim -lbbsystem -lbb -lbbcascadespickers -lbbdevice -lbbmultimedia -lQtLocationSubset -lbbcascadesplaces

include(config.pri)
