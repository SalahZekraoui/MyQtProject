#include "scheduledialog.h"
#include <QVBoxLayout>
#include <QLabel>

ScheduleDialog::ScheduleDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Library Schedule & Events");
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *hours = new QLabel(
        "<b>Library Hours:</b><br>"
        "Monday - Friday: 8:00 AM - 8:00 PM<br>"
        "Saturday: 9:00 AM - 5:00 PM<br>"
        "Sunday: Closed<br><br>"
        "<b>Upcoming Events:</b><br>"
        "June 20: Book Club Meeting<br>"
        "June 25: Research Workshop<br>"
        "July 2: Author Visit<br>"
        );
    hours->setTextFormat(Qt::RichText);
    layout->addWidget(hours);
    setLayout(layout);
}
