#include "VertexInputDialog.h"
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QFont>

VertexInputDialog::VertexInputDialog(const QString& algorithmName, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(algorithmName);
    setModal(true);
    setFixedSize(350, 180);

    setStyleSheet(
        "QDialog {"
        "   background-color: #f0f0f0;"
        "}"
    );

    QLabel *titleLabel = new QLabel(algorithmName, this);
    QFont titleFont("Arial", 14, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "QLabel {"
        "   color: #2E7D32;"
        "   padding: 5px;"
        "}"
    );

    QLabel *startLabel = new QLabel("Start Vertex ID:", this);
    QLabel *endLabel = new QLabel("End Vertex ID:", this);

    QFont labelFont("Arial", 10);
    startLabel->setFont(labelFont);
    endLabel->setFont(labelFont);

    startLabel->setStyleSheet("QLabel { color: #333333; }");
    endLabel->setStyleSheet("QLabel { color: #333333; }");

    m_startVertexEdit = new QLineEdit(this);
    m_endVertexEdit = new QLineEdit(this);

    QString lineEditStyle =
        "QLineEdit {"
        "   background-color: white;"
        "   border: 2px solid #4CAF50;"
        "   border-radius: 4px;"
        "   padding: 5px;"
        "   font-size: 11px;"
        "}"
        "QLineEdit:focus {"
        "   border: 2px solid #2E7D32;"
        "}";

    m_startVertexEdit->setStyleSheet(lineEditStyle);
    m_endVertexEdit->setStyleSheet(lineEditStyle);

    QIntValidator *validator = new QIntValidator(1, 9999, this);
    m_startVertexEdit->setValidator(validator);
    m_endVertexEdit->setValidator(validator);

    m_okButton = new QPushButton("OK", this);
    m_cancelButton = new QPushButton("Cancel", this);

    QString buttonStyle =
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   border: none;"
        "   color: white;"
        "   padding: 8px 20px;"
        "   text-align: center;"
        "   text-decoration: none;"
        "   border-radius: 6px;"
        "   font-weight: bold;"
        "   min-width: 80px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #3d8b40;"
        "}"
        "QPushButton:disabled {"
        "   background-color: #cccccc;"
        "}";

    m_okButton->setStyleSheet(buttonStyle);
    m_cancelButton->setStyleSheet(buttonStyle);

    QFont buttonFont("Arial", 10, QFont::Bold);
    m_okButton->setFont(buttonFont);
    m_cancelButton->setFont(buttonFont);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 15, 20, 15);
    mainLayout->setSpacing(15);

    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(10);

    QHBoxLayout *startLayout = new QHBoxLayout();
    startLayout->addWidget(startLabel);
    startLayout->addWidget(m_startVertexEdit);
    startLayout->setSpacing(10);

    QHBoxLayout *endLayout = new QHBoxLayout();
    endLayout->addWidget(endLabel);
    endLayout->addWidget(m_endVertexEdit);
    endLayout->setSpacing(10);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(m_okButton);
    buttonLayout->addSpacing(10);
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addStretch(1);
    buttonLayout->setSpacing(10);

    mainLayout->addLayout(startLayout);
    mainLayout->addLayout(endLayout);
    mainLayout->addStretch(1);
    mainLayout->addLayout(buttonLayout);

    connect(m_okButton, &QPushButton::clicked, this, &VertexInputDialog::onOkClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &VertexInputDialog::onCancelClicked);

    m_startVertexEdit->setFocus();
}

int VertexInputDialog::getStartVertexId() const
{
    return m_startVertexEdit->text().toInt();
}

int VertexInputDialog::getEndVertexId() const
{
    return m_endVertexEdit->text().toInt();
}

void VertexInputDialog::onOkClicked()
{
    if (!m_startVertexEdit->text().isEmpty() && !m_endVertexEdit->text().isEmpty()) {
        accept();
    }
}

void VertexInputDialog::onCancelClicked()
{
    reject();
}