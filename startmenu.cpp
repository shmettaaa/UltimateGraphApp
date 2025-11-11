#include "StartMenu.h"
#include <QPushButton>
#include <QVBoxLayout>

StartMenu::StartMenu(QWidget *parent) : QWidget(parent)
    , m_newProjectButton(nullptr)
{
    m_newProjectButton = new QPushButton("New Project", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_newProjectButton);

    setWindowTitle("Graph Application - Start Menu");
    resize(300, 200);

    connect(m_newProjectButton, &QPushButton::clicked,
            this, &StartMenu::newProjectClicked);
}