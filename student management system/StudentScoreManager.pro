QT       += core gui sql widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = StudentScoreManager
TEMPLATE = app

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    StudentManager.cpp

HEADERS += \
    Student.h \
    MainWindow.h \
    StudentManager.h

QMAKE_CXXFLAGS += -finput-charset=UTF-8
QMAKE_CXXFLAGS += -fexec-charset=UTF-8