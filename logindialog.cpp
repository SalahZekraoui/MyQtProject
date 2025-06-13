#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    // Add roles to the combo box
    ui->comboBoxRole->addItems({"Student", "Teacher", "Library Administrator", "Library Employee"});

    // Validate input before accepting
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [this]() {
        if (ui->lineEditName->text().trimmed().isEmpty() ||
            ui->lineEditEmail->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Name and Email cannot be empty.");
            return;
        }
        accept();
    });
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

User LoginDialog::getUser() const
{
    return User(
        0, // ID can be set later if needed
        ui->lineEditName->text().trimmed(),
        ui->lineEditEmail->text().trimmed(),
        ui->comboBoxRole->currentText()
        );
}
