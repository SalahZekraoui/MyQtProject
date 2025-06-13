#ifndef HISTORYENTRY_H
#define HISTORYENTRY_H

#include <QString>
#include <QDateTime>
#include <QJsonObject>

class HistoryEntry {
public:
    HistoryEntry();
    HistoryEntry(const QString& resourceTitle, const QString& user, const QString& action, const QDateTime& dateTime);

    QString resourceTitle;
    QString user;
    QString action; // "Borrowed" or "Returned"
    QDateTime dateTime;

    QJsonObject toJson() const;
    static HistoryEntry fromJson(const QJsonObject& obj);
};

#endif // HISTORYENTRY_H
