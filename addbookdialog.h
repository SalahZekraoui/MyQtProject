#pragma once
#include <QDialog>

namespace Ui {
class AddBookDialog;
}

class AddBookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddBookDialog(QWidget *parent = nullptr);
    ~AddBookDialog();

    QString getTitle() const;
    QString getAuthor() const;
    int getYear() const;

private:
    Ui::AddBookDialog *ui;
};
