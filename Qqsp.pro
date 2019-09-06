QT       += core gui widgets multimedia
#QT += multimedia-private

TARGET = Qqsp
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14
#CONFIG += c11
#QMAKE_CXXFLAGS+= -std=c++14
QMAKE_CFLAGS += -std=c11

INCLUDEPATH += $$PWD/qsp/bindings/default/
#INCLUDEPATH += $$PWD/qsp/qsp/
#INCLUDEPATH += $$PWD/qsp/qsp/onig/

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    qsptextbox.cpp \
    qsplistbox.cpp \
    qspinputbox.cpp \
    qspimgcanvas.cpp \
    qspmsgdlg.cpp \
    qspinputdlg.cpp \
    callbacks_gui.cpp \
    comtools.cpp \
    optionsdialog.cpp \
    videoframeprocessor.cpp \
    videolabel.cpp

HEADERS += \
    mainwindow.h \
    qsptextbox.h \
    qsplistbox.h \
    qspinputbox.h \
    qspimgcanvas.h \
    qspmsgdlg.h \
    qspinputdlg.h \
    callbacks_gui.h \
    comtools.h \
    optionsdialog.h \
    videoframeprocessor.h \
    videolabel.h

FORMS += \
    optionsdialog.ui

TRANSLATIONS = Qqsp.en.ts \
    Qqsp.ru.ts

DEFINES += _UNICODE
DEFINES += NOT_RUBY
#DEFINES += EXPORT

SOURCES += \
    qsp/actions.c \
    qsp/callbacks.c \
    qsp/codetools.c \
    qsp/coding.c \
    qsp/common.c \
    qsp/errors.c \
    qsp/game.c \
    qsp/locations.c \
    qsp/mathops.c \
    qsp/menu.c \
    qsp/objects.c \
    qsp/playlist.c \
    qsp/statements.c \
    qsp/text.c \
    qsp/time_qsp.c \
    qsp/towlower.c \
    qsp/towupper.c \
    qsp/variables.c \
    qsp/variant.c \
    qsp/onig/regcomp.c \
    qsp/onig/regenc.c \
    qsp/onig/regerror.c \
    qsp/onig/regexec.c \
    qsp/onig/regparse.c \
    qsp/onig/regsyntax.c \
    qsp/onig/regtrav.c \
    qsp/onig/regversion.c \
    qsp/onig/st.c \
    qsp/onig/enc/ascii.c \
    qsp/onig/enc/cp1251.c \
    qsp/onig/enc/koi8_r.c \
    qsp/onig/enc/unicode.c \
    qsp/onig/enc/utf16_be.c \
    qsp/onig/enc/utf16_le.c \
    qsp/onig/enc/utf32_be.c \
    qsp/onig/enc/utf32_le.c \
    qsp/bindings/default/default_callbacks.c \
    qsp/bindings/default/default_coding.c \
    qsp/bindings/default/default_control.c
#    t2g_locations.c \
#    txt2gam.c

HEADERS += \
    qsp/actions.h \
    qsp/callbacks.h \
    qsp/codetools.h \
    qsp/coding.h \
    qsp/common.h \
    qsp/declarations.h \
    qsp/errors.h \
    qsp/game.h \
    qsp/locations.h \
    qsp/mathops.h \
    qsp/menu.h \
    qsp/objects.h \
    qsp/playlist.h \
    qsp/qsp.h \
    qsp/statements.h \
    qsp/text.h \
    qsp/time_qsp.h \
    qsp/variables.h \
    qsp/variant.h \
    qsp/onig/config.h \
    qsp/onig/oniguruma.h \
    qsp/onig/regenc.h \
    qsp/onig/regint.h \
    qsp/onig/regparse.h \
    qsp/onig/st.h \
    qsp/bindings/bindings_config.h \
    qsp/bindings/default/qsp_default.h
#    t2g_locations.h \
#    txt2gam.h

RESOURCES += \
    qqsp.qrc

#CONFIG += enable-debug

enable-debug {
  DEFINES += _DEBUGMW
  SOURCES += qsp/memwatch.c
  HEADERS += qsp/memwatch.h
}

CONFIG += enable-webbox

enable-webbox {
  DEFINES += _WEBBOX
  DEFINES += _WEBBOX_COMMON
  QT += webengine webenginewidgets webchannel
  SOURCES += qspwebbox.cpp \
    qspwebengineurlrequestinterceptor.cpp \
    qspwebengineurlschemehandler.cpp \
    qspexecwebengineurlschemehandler.cpp \
    url_schemes.cpp \
    qspwebchannel.cpp
  HEADERS += qspwebbox.h \
    qspwebengineurlrequestinterceptor.h \
    qspwebengineurlschemehandler.h \
    qspexecwebengineurlschemehandler.h \
    url_schemes.h \
    qspwebchannel.h
}

#CONFIG += enable-webbox-webkit

enable-webbox-webkit {
  DEFINES += _WEBBOX_WEBKIT
  DEFINES += _WEBBOX_COMMON
  QT += webkitwidgets
  SOURCES += qspwebbox_webkit.cpp \
    qspnetworkaccessmanager.cpp \
    qspreply.cpp
  HEADERS += qspwebbox_webkit.h \
    qspnetworkaccessmanager.h \
    qspreply.h
}

unix:!macx {
    CONFIG += disable-nativedialog
}

disable-nativedialog {
  DEFINES += _NONATIVEDIALOG
}

#CONFIG += enable-android

enable-android {
  DEFINES += _ANDROIDQT
  QT += androidextras
  SOURCES += androidfiledialog.cpp
  HEADERS += androidfiledialog.h
}

ICON = icons/qsp-logo.icns

isEmpty(TARGET_EXT) {
    win32 {
        TARGET_CUSTOM_EXT = .exe
    }
    macx {
        TARGET_CUSTOM_EXT = .app
    }
} else {
    TARGET_CUSTOM_EXT = $${TARGET_EXT}
}

win32 {
    DEPLOY_COMMAND = windeployqt
    CONFIG( debug, debug|release ) {
        # debug
        DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/debug/$${TARGET}$${TARGET_CUSTOM_EXT}))
    } else {
        # release
        DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/release/$${TARGET}$${TARGET_CUSTOM_EXT}))
    }
}
macx {
    DEPLOY_COMMAND = macdeployqt
    CONFIG( debug, debug|release ) {
        # debug
        DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/$${TARGET}$${TARGET_CUSTOM_EXT}))
    } else {
        # release
        DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/$${TARGET}$${TARGET_CUSTOM_EXT}))
    }
}

#  # Uncomment the following line to help debug the deploy command when running qmake
#  warning($${DEPLOY_COMMAND} $${DEPLOY_TARGET})

win32|macx {
    QMAKE_POST_LINK = $${DEPLOY_COMMAND} $${DEPLOY_TARGET}
}

VERSION = 1.9.0.0

win32:RC_ICONS += icons/logo.ico

win32:QMAKE_TARGET_COMPANY = Qqsp
win32:QMAKE_TARGET_DESCRIPTION = Qqsp
win32:QMAKE_TARGET_PRODUCT = Qqsp
