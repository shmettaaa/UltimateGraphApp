#include <QApplication>
#include "StartMenu.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    StartMenu startMenu;
    MainWindow *mainWindow = nullptr;

    QObject::connect(&startMenu, &StartMenu::newProjectClicked, [&]() {
        mainWindow = new MainWindow();
        mainWindow->show();
        startMenu.close();
    });

    startMenu.show();

    return app.exec();
}