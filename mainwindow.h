#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "resource.h"
#include "historyentry.h"
#include "user.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent, const User& user); // Modified constructor
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QVector<Resource*> resources;
    QVector<HistoryEntry> history;
    QVector<User> users;
    User currentUser; // Stores the currently logged-in user

private slots:
    void onAddResourceClicked();
    void onRemoveResourceClicked();
    void onEditResourceClicked();
    void onSearchClicked();
    void onBorrowResourceClicked();
    void onReserveResourceClicked();
    void onReturnResourceClicked();
    void onRenewResourceClicked();
    void onViewHistoryClicked();
    void onScheduleClicked();

private:
    void saveResourcesToFile(const QString& filename);
    void loadResourcesFromFile(const QString& filename);
    void saveHistoryToFile(const QString& filename);
    void loadHistoryFromFile(const QString& filename);
    void checkNotifications();
    void saveUsersToFile(const QString& filename);
    void loadUsersFromFile(const QString& filename);
};

#endif // MAINWINDOW_H
