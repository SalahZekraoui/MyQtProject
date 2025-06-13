#pragma once
#include <QString>

class User {
private:
    int id;
    QString name;
    QString email;
    QString role; // Add this line
public:
    User(int id, const QString& name, const QString& email, const QString& role); // Update constructor
    int getId() const;
    QString getName() const;
    QString getEmail() const;
    QString getRole() const; // Add this getter
    void setRole(const QString& role); // Add this setter
};
