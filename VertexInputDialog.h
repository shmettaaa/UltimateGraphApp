#ifndef VERTEXINPUTDIALOG_H
#define VERTEXINPUTDIALOG_H

#include <QDialog>

class QLineEdit;
class QLabel;
class QPushButton;

class VertexInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VertexInputDialog(const QString& algorithmName, QWidget *parent = nullptr);

    int getStartVertexId() const;
    int getEndVertexId() const;

private slots:
    void onOkClicked();
    void onCancelClicked();

private:
    QLineEdit *m_startVertexEdit;
    QLineEdit *m_endVertexEdit;
    QPushButton *m_okButton;
    QPushButton *m_cancelButton;
};

#endif