#-------------------------------------------------
#
# Project created by QtCreator 2018-06-24T20:45:44
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += sql
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = l110
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += D:\Documents\CameroNet
INCLUDEPATH += D:\Documents\Screen
INCLUDEPATH += D:\Documents\player
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    z1801.cpp \
    wrapper.cpp \
    cmenubar.cpp \
    alarmwidget.cpp \
    csqltablemode.cpp \
    style/currentstyle.cpp \
    ruleWidget/rulewidget.cpp \
    ruleWidget/rulemodel.cpp \
    ruleWidget/voicedelegate.cpp \
    ruleWidget/myedit.cpp \
    preview/preview.cpp \
    preview/screenbar.cpp \
    preview/devicetreemode.cpp \
    playback/playback.cpp \
    areamap.cpp \
    customContro/switchbutton.cpp \
    style/norwegianwoodstyle.cpp \
    devicemodel/devicemodel.cpp \
    devicemodel/imforparse.cpp \
    devicemodel/node.cpp \
    devicemodel/nodedelegate.cpp \
    areaMapEditDiaLog/addareamap.cpp \
    areaMapEditDiaLog/mapscence.cpp \
    areaMapEditDiaLog/mylistview.cpp \
    areaMapEditDiaLog/mymodel.cpp \
    areaMapEditDiaLog/pixmapitem.cpp \
    globdata.cpp \
    preview/ptzcontrol.cpp \
    preview/sqldata.cpp \
    playback/cplayer.cpp \
    messageRecord/messagerecord.cpp \
    messageRecord/submitwidget.cpp \
    messageRecord/messageview.cpp \
    style/customtabwidgetstyle.cpp \
    editWidget/systemeditdialog.cpp \
    customContro/cpushbutton.cpp \
    contralpanel.cpp \
    editWidget/devicesetwidget.cpp \
    devicesetmodel.cpp \
    devicesetitemdelegate.cpp \
    dataEntry/dataentrydialog.cpp \
    dataEntry/entryform.cpp \
    VedioLink/vediolinkconfigdialog.cpp \
    VedioLink/intlistmodle.cpp \
    VedioLink/ctablewidget.cpp \
    bombScreen/bombscreen.cpp \
    controlwidget.cpp \
    mytimer.cpp \
    messageRecord/viewitemdelege.cpp \
    bottomstatu.cpp \
    csqlquerymodel.cpp \
    serchedit.cpp \
    preview/addandeditedialog.cpp \
    preview/deviceform.cpp \
    preview/listitemdelege.cpp \
    preview/deviceimforsqltablemodel.cpp \
    handledialog.cpp \
    preview/channeleditdialog.cpp \
    cstatubar.cpp \
    singleapplication.cpp \
    logwidget.cpp \
    editWidget/statubutton.cpp \
    aboutdialog.cpp \
    changeaccountdidalog.cpp


HEADERS  += \
    z1801p.h \
    wrapper.h \
    cmenubar.h \
    alarmwidget.h \
    csqltablemode.h \
    style/currentstyle.h \
    ruleWidget/rulewidget.h \
    ruleWidget/rulemodel.h \
    ruleWidget/voicedelegate.h \
    ruleWidget/myedit.h \
    preview/preview.h \
    preview/jcsdk.h \
    playback/playback.h \
    areamap.h \
    customContro/switchbutton.h \
    style/norwegianwoodstyle.h \
    devicemodel/devicemodel.h \
    devicemodel/imforparse.h \
    devicemodel/node.h \
    devicemodel/nodedelegate.h \
    areaMapEditDiaLog/addareamap.h \
    areaMapEditDiaLog/mapscence.h \
    areaMapEditDiaLog/mylistview.h \
    areaMapEditDiaLog/mymodel.h \
    areaMapEditDiaLog/pixmapitem.h \
    globdata.h \
    preview/ptzcontrol.h \
    preview/sqldata.h \
    playback/cplayer.h \
    messageRecord/messagerecord.h \
    messageRecord/submitwidget.h \
    messageRecord/messageview.h \
    cameronet/include/cameronet.h \
    preview/screenbar.h \
    style/customtabwidgetstyle.h \
    editWidget/systemeditdialog.h \
    customdata.h \
    customContro/cpushbutton.h \
    z1801.h \
    contralpanel.h \
    editWidget/devicesetwidget.h \
    devicesetmodel.h \
    devicesetitemdelegate.h \
    dataEntry/dataentrydialog.h \
    dataEntry/entryform.h \
    VedioLink/vediolinkconfigdialog.h \
    VedioLink/intlistmodle.h \
    VedioLink/ctablewidget.h \
    bombScreen/bombscreen.h \
    controlwidget.h \
    mytimer.h \
    messageRecord/viewitemdelege.h \
    bottomstatu.h \
    csqlquerymodel.h \
    preview/devicetreemode.h \
    serchedit.h \
    preview/addandeditedialog.h \
    preview/deviceform.h \
    preview/listitemdelege.h \
    preview/deviceimforsqltablemodel.h \
    handledialog.h \
    preview/channeleditdialog.h \
    cstatubar.h \
    singleapplication.h \
    logwidget.h \
    editWidget/statubutton.h \
    ../build-l110-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ui_bombscreen.h \
    aboutdialog.h \
    changeaccountdidalog.h


RESOURCES += \
    qss.qrc

FORMS += \
    ruleWidget/rulewidget.ui \
    preview/screentoolbar.ui \
    preview/ptzcontrol.ui \
    messageRecord/submitwidget.ui \
    editWidget/systemeditdialog.ui \
    editWidget/devicesetwidget.ui \
    dataEntry/dataentrydialog.ui \
    dataEntry/entryform.ui \
    VedioLink/vediolinkconfigdialog.ui \
    VedioLink/ctablewidget.ui \
    bombScreen/bombscreen.ui \
    controlwidget.ui \
    bottomstatu.ui \
    preview/addandeditedialog.ui \
    preview/deviceform.ui \
    handledialog.ui \
    preview/channeleditdialog.ui \
    logwidget.ui \
    aboutdialog.ui \
    changeaccountdidalog.ui


LIBS += -L../l110/lib -ljcsdk
LIBS += -L../l110/lib -llibScreen -llibCameroNet -llibplayer


# install
target.path = C:/Users/Administrator/Desktop/l110
INSTALLS += target

SUBDIRS += \
    areaMapEditDiaLog/DragDrop.pro

DISTFILES += \
    image/play_hover.png \
    image/viewPlay.png


RC_FILE = appIcon.rc
