#pragma once
#include "book.h"
#include "user.h"
#include "loan.h"
#include "reservation.h"
#include <QVector>

class LibraryData {
public:
    QVector<Book> books;
    QVector<User> users;
    QVector<Loan> loans;
    QVector<Reservation> reservations;
};
