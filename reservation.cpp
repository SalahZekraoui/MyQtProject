#include "reservation.h"

Reservation::Reservation(int reservationId, int bookId, int userId, QDate reservationDate)
    : reservationId(reservationId), bookId(bookId), userId(userId), reservationDate(reservationDate) {}

int Reservation::getReservationId() const { return reservationId; }
int Reservation::getBookId() const { return bookId; }
int Reservation::getUserId() const { return userId; }
QDate Reservation::getReservationDate() const { return reservationDate; }
