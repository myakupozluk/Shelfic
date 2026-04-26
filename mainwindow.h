#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;

    void connectDatabase();

    QSqlTableModel *model;
    QSqlTableModel *memberModel;
    QSqlTableModel *borrowModel;

    void loadBooks();
    void loadMembers();
    void loadBorrowedBooks();

    void on_addBookButton_clicked();
    void on_deleteBookButton_clicked();
    void on_addMemberButton_clicked();
    void on_deleteMemberButton_clicked();
    void on_borrowButton_clicked();
    void on_returnBookButton_clicked();
};

#endif