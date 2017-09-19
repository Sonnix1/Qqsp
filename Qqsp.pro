QT       += core gui widgets

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
QMAKE_CFLAGS += -std=c11 -fshort-wchar

INCLUDEPATH += $$PWD/qsp/qsp/bindings/default/
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
    comtools.cpp

HEADERS += \
    mainwindow.h \
    qsptextbox.h \
    qsplistbox.h \
    qspinputbox.h \
    qspimgcanvas.h \
    qspmsgdlg.h \
    qspinputdlg.h \
    callbacks_gui.h \
    comtools.h

DEFINES += _UNICODE
DEFINES += NOT_RUBY
#DEFINES += EXPORT

SOURCES += \
    qsp/qsp/actions.c \
    qsp/qsp/errors.c \
    qsp/qsp/callbacks.c \
    qsp/qsp/game.c \
    qsp/qsp/codetools.c \
    qsp/qsp/coding.c \
    qsp/qsp/locations.c \
    qsp/qsp/mathops.c \
    qsp/qsp/common.c \
    qsp/qsp/memwatch.c \
    qsp/qsp/objects.c \
    qsp/qsp/playlist.c \
    qsp/qsp/time.c \
    qsp/qsp/towlower.c \
    qsp/qsp/towupper.c \
    qsp/qsp/regexp.c \
    qsp/qsp/statements.c \
    qsp/qsp/variables.c \
    qsp/qsp/variant.c \
    qsp/qsp/menu.c \
    qsp/qsp/text.c \
    qsp/qsp/onig/regcomp.c \
    qsp/qsp/onig/regenc.c \
    qsp/qsp/onig/regerror.c \
    qsp/qsp/onig/regexec.c \
    qsp/qsp/onig/regparse.c \
    qsp/qsp/onig/regtrav.c \
    qsp/qsp/onig/regversion.c \
    qsp/qsp/onig/st.c \
    qsp/qsp/onig/regsyntax.c \
    qsp/qsp/onig/enc/ascii.c \
    qsp/qsp/onig/enc/cp1251.c \
    qsp/qsp/onig/enc/koi8_r.c \
    qsp/qsp/onig/enc/unicode.c \
    qsp/qsp/onig/enc/utf16_be.c \
    qsp/qsp/onig/enc/utf16_le.c \
    qsp/qsp/onig/enc/utf32_be.c \
    qsp/qsp/onig/enc/utf32_le.c \
    qsp/qsp/bindings/default/default_callbacks.c \
    qsp/qsp/bindings/default/default_control.c \
    qsp/qsp/bindings/default/default_platform.c

HEADERS += \
    qsp/qsp/actions.h \
    qsp/qsp/callbacks.h \
    qsp/qsp/errors.h \
    qsp/qsp/game.h \
    qsp/qsp/codetools.h \
    qsp/qsp/locations.h \
    qsp/qsp/coding.h \
    qsp/qsp/mathops.h \
    qsp/qsp/menu.h \
    qsp/qsp/objects.h \
    qsp/qsp/time.h \
    qsp/qsp/playlist.h \
    qsp/qsp/regexp.h \
    qsp/qsp/statements.h \
    qsp/qsp/variables.h \
    qsp/qsp/variant.h \
    qsp/qsp/common.h \
    qsp/qsp/declarations.h \
    qsp/qsp/memwatch.h \
    qsp/qsp/text.h \
    qsp/qsp/onig/config.h \
    qsp/qsp/onig/oniguruma.h \
    qsp/qsp/onig/regenc.h \
    qsp/qsp/onig/regint.h \
    qsp/qsp/onig/regparse.h \
    qsp/qsp/onig/st.h \
    qsp/qsp/bindings/default/qsp_default.h

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
}
macx {
    DEPLOY_COMMAND = macdeployqt
}

CONFIG( debug, debug|release ) {
    # debug
    DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/debug/$${TARGET}$${TARGET_CUSTOM_EXT}))
} else {
    # release
    DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/release/$${TARGET}$${TARGET_CUSTOM_EXT}))
}

#  # Uncomment the following line to help debug the deploy command when running qmake
#  warning($${DEPLOY_COMMAND} $${DEPLOY_TARGET})

win32|macx {
    QMAKE_POST_LINK = $${DEPLOY_COMMAND} $${DEPLOY_TARGET}
}

win32:RC_ICONS += qsp/players/classic/qspgui/misc/icons/logo.ico
