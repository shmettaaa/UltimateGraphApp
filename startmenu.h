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

private:
    QPushButton *m_newProjectButton;
};

#endif