#include "user.h"

User::User(int id, const QString& name, const QString& email, const QString& role)
    : id(id), name(name), email(email), role(role) {}

int User::getId() const { return id; }
QString User::getName() const { return name; }
QString User::getEmail() const { return email; }
QString User::getRole() const { return role; }
void User::setRole(const QString& r) { role = r; }
