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
