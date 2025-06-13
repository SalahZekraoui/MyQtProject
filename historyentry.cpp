#include "historyentry.h"

HistoryEntry::HistoryEntry() {}

HistoryEntry::HistoryEntry(const QString& resourceTitle, const QString& user, const QString& action, const QDateTime& dateTime)
    : resourceTitle(resourceTitle), user(user), action(action), dateTime(dateTime) {}

QJsonObject HistoryEntry::toJson() const {
    QJsonObject obj;
    obj["resourceTitle"] = resourceTitle;
    obj["user"] = user;
    obj["action"] = action;
    obj["dateTime"] = dateTime.toString(Qt::ISODate);
    return obj;
}

HistoryEntry HistoryEntry::fromJson(const QJsonObject& obj) {
    return HistoryEntry(
        obj["resourceTitle"].toString(),
        obj["user"].toString(),
        obj["action"].toString(),
        QDateTime::fromString(obj["dateTime"].toString(), Qt::ISODate)
        );
}
