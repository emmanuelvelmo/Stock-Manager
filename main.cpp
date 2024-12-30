#include <QApplication>
#include <QCoreApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    // Crear una instancia de QApplication
    QApplication qt_app(argc, argv);
    // Establecer la versión de la aplicación
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    
    // Crear una instancia de MainWindow
    MainWindow vent_prin;
    // Mostrar la ventana principal de la aplicación
    vent_prin.show();
    
    // Ejecutar el bucle principal de la aplicación
    return qt_app.exec();
}