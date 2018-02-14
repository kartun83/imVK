APP_NAME = VKChatHeadlessService

CONFIG += qt warn_on

include(config.pri)

QT += network
LIBS   += -lbbsystem -lbb -lbbpim -lunifieddatasourcec -lbbdata -lbbplatform -lbbnetwork -lbps
