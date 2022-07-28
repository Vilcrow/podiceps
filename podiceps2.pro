QT += widgets
requires(qtConfig(listview))

SOURCES = dictionarywidget.cpp \
		  main.cpp \
		  mainwindow.cpp \
		  tablemodel.cpp \
		  savedialog.cpp

HEADERS = dictionarywidget.h \
		  mainwindow.h \
		  tablemodel.h \
		  savedialog.h

QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O0
