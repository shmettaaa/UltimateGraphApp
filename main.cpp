#include <QApplication>
#include "StartMenu.h"
#include "MainWindow.h"
#include <QTimer>
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

    QObject::connect(&startMenu, &StartMenu::openProjectClicked, [&]() {
        mainWindow = new MainWindow();
        mainWindow->show();
        startMenu.close();

        QTimer::singleShot(100, mainWindow, &MainWindow::onOpen);
    });

    QObject::connect(&startMenu, &StartMenu::exitClicked, [&]() {
        app.quit();
    });

    startMenu.show();

    return app.exec();
}