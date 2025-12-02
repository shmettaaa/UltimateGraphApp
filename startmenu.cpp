#include "StartMenu.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QFont>
#include <QLabel>

StartMenu::StartMenu(QWidget *parent) : QWidget(parent)
    , m_newProjectButton(nullptr)
    , m_openProjectButton(nullptr)
    , m_exitButton(nullptr)
{
    QLabel *titleLabel = new QLabel("Graph Application", this);
    QFont titleFont("Arial", 20, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "QLabel {"
        "   color: #2E7D32;"
        "   padding: 10px;"
        "}"
    );

    m_newProjectButton = new QPushButton("New Project", this);
    m_openProjectButton = new QPushButton("Open Project", this);
    m_exitButton = new QPushButton("Exit", this);

    QFont buttonFont("Arial", 12, QFont::Bold);
    m_newProjectButton->setFont(buttonFont);
    m_openProjectButton->setFont(buttonFont);
    m_exitButton->setFont(buttonFont);

    m_newProjectButton->setMinimumSize(200, 50);
    m_openProjectButton->setMinimumSize(200, 50);
    m_exitButton->setMinimumSize(200, 50);

    QString buttonStyle =
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   border: none;"
        "   color: white;"
        "   padding: 10px;"
        "   text-align: center;"
        "   text-decoration: none;"
        "   border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #3d8b40;"
        "}";

    m_newProjectButton->setStyleSheet(buttonStyle);
    m_openProjectButton->setStyleSheet(buttonStyle);
    m_exitButton->setStyleSheet(buttonStyle);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mainLayout->addStretch(1);
    mainLayout->addWidget(titleLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(40);

    mainLayout->addWidget(m_newProjectButton, 0, Qt::AlignCenter);
    mainLayout->addSpacing(20);

    mainLayout->addWidget(m_openProjectButton, 0, Qt::AlignCenter);
    mainLayout->addSpacing(20);

    mainLayout->addWidget(m_exitButton, 0, Qt::AlignCenter);

    mainLayout->addStretch(1);

    mainLayout->setContentsMargins(40, 40, 40, 40);

    setWindowTitle("Graph Application - Start Menu");
    setFixedSize(450, 450);

    connect(m_newProjectButton, &QPushButton::clicked,
            this, &StartMenu::newProjectClicked);

    connect(m_openProjectButton, &QPushButton::clicked,
            this, &StartMenu::openProjectClicked);

    connect(m_exitButton, &QPushButton::clicked,
            this, &StartMenu::exitClicked);
}