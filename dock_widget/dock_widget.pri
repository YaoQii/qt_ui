INCLUDEPATH += $$PROJECT_SRC_DIR/dock_widget
DEPENDPATH += $$PROJECT_SRC_DIR/dock_widget

#HEADERS += \
#    grid_renderer.h \
#    viewer.h \
#    car_renderer.h \
#    rwx.h \
#    dgcviewer.h \
#    tokenize.h\
#    $$PWD/gl_color.h \
#    $$PWD/hd_map_renderer.h \
#    $$PWD/intensity_map.h \
#    console_renderer.h \
#    $$PWD/adu_renderer.h \
#    $$PWD/qcustomplot.h \
#    $$PWD/MDockAncestor.h \
#    $$PWD/curvature_dock.h

#SOURCES += \
##    renderer_grid.c \
#    viewer.c \
##    car_renderer.c \
#    rwx.c \
#    tokenize.c \
#    $$PWD/hd_map_renderer.cpp \
#    $$PWD/qcustomplot.cpp \
#    $$PWD/MDockAncestor.cpp \
#    $$PWD/curvature_dock.cpp

#FORMS += \
#    $$PWD/CurvatureForm.ui

DISTFILES += \
    $$PWD/dock_widget.pri

FORMS += \
    $$PWD/CurvatureForm.ui

HEADERS += \
    $$PWD/qcustomplot.h \
    $$PWD/MDockAncestor.h \
    $$PWD/curvature_dock.h

SOURCES += \
    $$PWD/qcustomplot.cpp \
    $$PWD/MDockAncestor.cpp \
    $$PWD/curvature_dock.cpp

