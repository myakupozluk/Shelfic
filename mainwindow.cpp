#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QSqlQuery>
#include <QHeaderView>
#include <QMessageBox>
#include <QDateTime>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Shelfic");

    connectDatabase();
    loadBooks();
    loadMembers();
    loadBorrowedBooks();

    connect(ui->addBookButton, &QPushButton::clicked, this, &MainWindow::on_addBookButton_clicked);
    connect(ui->deleteBookButton, &QPushButton::clicked, this, &MainWindow::on_deleteBookButton_clicked);
    connect(ui->addMemberButton, &QPushButton::clicked, this, &MainWindow::on_addMemberButton_clicked);
    connect(ui->deleteMemberButton, &QPushButton::clicked, this, &MainWindow::on_deleteMemberButton_clicked);
    connect(ui->borrowButton, &QPushButton::clicked, this, &MainWindow::on_borrowButton_clicked);
    connect(ui->returnBookButton, &QPushButton::clicked, this, &MainWindow::on_returnBookButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("library.db");

    if (db.open()) {
        QSqlQuery query;
        query.exec("CREATE TABLE IF NOT EXISTS books (id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT, author TEXT)");
        query.exec("CREATE TABLE IF NOT EXISTS members (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, surname TEXT)");
        query.exec("CREATE TABLE IF NOT EXISTS borrow (id INTEGER PRIMARY KEY AUTOINCREMENT, book_id INTEGER, member_id INTEGER, borrow_date TEXT)");
    }
}

void MainWindow::on_addBookButton_clicked(){
    QString idText = ui->idLineEdit->text().trimmed();
    QString bookName = ui->bookLineEdit->text().trimmed();
    QString authorName = ui->writerLineEdit->text().trimmed();

    if (bookName.isEmpty() || authorName.isEmpty()) return;

    QSqlQuery query;
    if (!idText.isEmpty()) {
        query.prepare("INSERT INTO books (id, title, author) VALUES (:id, :title, :author)");
        query.bindValue(":id", idText.toInt());
    } else {
        query.prepare("INSERT INTO books (title, author) VALUES (:title, :author)");
    }

    query.bindValue(":title", bookName);
    query.bindValue(":author", authorName);

    if (query.exec()) {
        ui->idLineEdit->clear();
        ui->bookLineEdit->clear();
        ui->writerLineEdit->clear();
        loadBooks();
    }
}

void MainWindow::on_deleteBookButton_clicked() {
    QItemSelectionModel *select = ui->bookList->selectionModel();
    if (!select->hasSelection()) return;

    QModelIndexList selectedRows = select->selectedRows();
    std::sort(selectedRows.begin(), selectedRows.end(), [](const QModelIndex &a, const QModelIndex &b) {
        return a.row() > b.row();
    });

    for (const QModelIndex &index : selectedRows) {
        model->removeRow(index.row());
    }
    model->submitAll();
    loadBooks();
}

void MainWindow::loadBooks() {
    model = new QSqlTableModel(this, db);
    model->setTable("books");
    model->select();
    ui->bookList->setModel(model);
    ui->bookList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->bookList->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void MainWindow::on_addMemberButton_clicked() {
    QString idText = ui->addMemberIdLineEdit->text().trimmed();
    QString name = ui->addNameLineEdit->text().trimmed();
    QString surname = ui->addSurnameLineEdit->text().trimmed();

    if (name.isEmpty() || surname.isEmpty()) return;

    QSqlQuery query;
    if (!idText.isEmpty()) {
        query.prepare("INSERT INTO members (id, name, surname) VALUES (:id, :name, :surname)");
        query.bindValue(":id", idText.toInt());
    } else {
        query.prepare("INSERT INTO members (name, surname) VALUES (:name, :surname)");
    }

    query.bindValue(":name", name);
    query.bindValue(":surname", surname);

    if (query.exec()) {
        ui->addMemberIdLineEdit->clear();
        ui->addNameLineEdit->clear();
        ui->addSurnameLineEdit->clear();
        loadMembers();
    }
}

void MainWindow::on_deleteMemberButton_clicked() {
    QItemSelectionModel *select = ui->memberList->selectionModel();
    if (!select->hasSelection()) return;

    QModelIndexList selectedRows = select->selectedRows();
    std::sort(selectedRows.begin(), selectedRows.end(), [](const QModelIndex &a, const QModelIndex &b) {
        return a.row() > b.row();
    });

    for (const QModelIndex &index : selectedRows) {
        memberModel->removeRow(index.row());
    }
    memberModel->submitAll();
    loadMembers();
}

void MainWindow::loadMembers() {
    memberModel = new QSqlTableModel(this, db);
    memberModel->setTable("members");
    memberModel->select();
    ui->memberList->setModel(memberModel);
    ui->memberList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->memberList->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void MainWindow::on_borrowButton_clicked() {
    QString bookId = ui->borrowBookIdLineEdit->text().trimmed();
    QString memberId = ui->borrowMemberIdLineEdit->text().trimmed();

    if (bookId.isEmpty() || memberId.isEmpty()) return;

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT id FROM borrow WHERE book_id = :bId");
    checkQuery.bindValue(":bId", bookId.toInt());
    checkQuery.exec();

    if (checkQuery.next()) {
        QMessageBox::warning(this, "Hata", "Bu kitap zaten ödünç verilmiş!");
        return;
    }

    QSqlQuery existQuery;
    existQuery.prepare("SELECT id FROM books WHERE id = :bId");
    existQuery.bindValue(":bId", bookId.toInt());
    existQuery.exec();
    if (!existQuery.next()) {
        QMessageBox::warning(this, "Hata", "Kitap ID bulunamadı!");
        return;
    }

    QString date = QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm");
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO borrow (book_id, member_id, borrow_date) VALUES (:b, :m, :d)");
    insertQuery.bindValue(":b", bookId.toInt());
    insertQuery.bindValue(":m", memberId.toInt());
    insertQuery.bindValue(":d", date);

    if (insertQuery.exec()) {
        ui->borrowBookIdLineEdit->clear();
        ui->borrowMemberIdLineEdit->clear();
        loadBorrowedBooks();
    }
}

void MainWindow::on_returnBookButton_clicked() {
    QItemSelectionModel *select = ui->borrowList->selectionModel();
    if (!select->hasSelection()) return;

    QModelIndexList selectedRows = select->selectedRows();
    for (const QModelIndex &index : selectedRows) {
        borrowModel->removeRow(index.row());
    }
    borrowModel->submitAll();
    loadBorrowedBooks();
}

void MainWindow::loadBorrowedBooks() {
    borrowModel = new QSqlTableModel(this, db);
    borrowModel->setTable("borrow");
    borrowModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    borrowModel->select();

    borrowModel->setHeaderData(1, Qt::Horizontal, "Kitap ID");
    borrowModel->setHeaderData(2, Qt::Horizontal, "Üye ID");
    borrowModel->setHeaderData(3, Qt::Horizontal, "Tarih");

    ui->borrowList->setModel(borrowModel);
    ui->borrowList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->borrowList->setSelectionBehavior(QAbstractItemView::SelectRows);
}