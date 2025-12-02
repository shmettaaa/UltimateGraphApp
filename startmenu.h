#ifndef STARTMENU_H
#define STARTMENU_H

#include <QWidget>

class QPushButton;

class StartMenu : public QWidget
{
    Q_OBJECT

public:
    explicit StartMenu(QWidget *parent = nullptr);

    signals:
        void newProjectClicked();
    void openProjectClicked();
    void exitClicked();

private:
    QPushButton *m_newProjectButton;
    QPushButton *m_openProjectButton;
    QPushButton *m_exitButton;
};

#endif