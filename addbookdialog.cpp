#include "addbookdialog.h"
#include "ui_addbookdialog.h"

AddBookDialog::AddBookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBookDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

AddBookDialog::~AddBookDialog()
{
    delete ui;
}

QString AddBookDialog::getTitle() const {
    return ui->lineEditTitle->text();
}

QString AddBookDialog::getAuthor() const {
    return ui->lineEditAuthor->text();
}

int AddBookDialog::getYear() const {
    return ui->lineEditYear->text().toInt();
}
