#include "VertexInputDialog.h"
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIntValidator>

VertexInputDialog::VertexInputDialog(const QString& algorithmName, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(algorithmName);
    setModal(true);
    setFixedSize(300, 150);

    QLabel *startLabel = new QLabel("Start Vertex ID:", this);
    QLabel *endLabel = new QLabel("End Vertex ID:", this);

    m_startVertexEdit = new QLineEdit(this);
    m_endVertexEdit = new QLineEdit(this);

    QIntValidator *validator = new QIntValidator(1, 9999, this);
    m_startVertexEdit->setValidator(validator);
    m_endVertexEdit->setValidator(validator);

    m_okButton = new QPushButton("OK", this);
    m_cancelButton = new QPushButton("Cancel", this);


    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *startLayout = new QHBoxLayout();
    startLayout->addWidget(startLabel);
    startLayout->addWidget(m_startVertexEdit);

    QHBoxLayout *endLayout = new QHBoxLayout();
    endLayout->addWidget(endLabel);
    endLayout->addWidget(m_endVertexEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_okButton);
    buttonLayout->addWidget(m_cancelButton);

    mainLayout->addLayout(startLayout);
    mainLayout->addLayout(endLayout);
    mainLayout->addLayout(buttonLayout);

    connect(m_okButton, &QPushButton::clicked, this, &VertexInputDialog::onOkClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &VertexInputDialog::onCancelClicked);
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