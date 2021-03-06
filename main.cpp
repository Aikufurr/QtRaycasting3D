#include "window.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QSurfaceFormat fmt;
    fmt.setSamples(4);
    QSurfaceFormat::setDefaultFormat(fmt);

    // Display the main window
    Window window;
    window.show();
    return app.exec();
}
