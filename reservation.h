#pragma once
#include <QDate>

class Reservation {
private:
    int reservationId;
    int bookId;
    int userId;
    QDate reservationDate;
public:
    Reservation(int reservationId, int bookId, int userId, QDate reservationDate);
    int getReservationId() const;
    int getBookId() const;
    int getUserId() const;
    QDate getReservationDate() const;
};
