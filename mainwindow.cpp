/**
        CSVsmith
------------------------
* - Name           CSVsmith
* - Description    This is a simple CSV file viewer
* - Author         Youssef Ahmed Mahmoud Elebiary
* - Version        1.0
*/



//////////////////////////////
//     FEATURES MISSING     //
//  1) Creating a new table //
//  2) Go to a certain cell //
//////////////////////////////



#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFileInfo>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <QFont>
#include <QInputDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <iostream>



// Global Variables
QString filepath;
std::ifstream fin;
bool isTableLoaded;
bool isSaved;
bool isNewFile = false;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Connecting the actions
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::LoadFile);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::SaveFile);
    connect(ui->actionClose, &QAction::triggered, this, &MainWindow::ClearTable);
    connect(ui->actionReload, &QAction::triggered, this, &MainWindow::ReloadTable);
    connect(ui->tableWidget, &QTableWidget::currentCellChanged, this, &MainWindow::GetCords);
    connect(ui->actionAppend_Row, &QAction::triggered, this, [this](){MainWindow::AddRow(-1);});
    connect(ui->actionAppend_Column, &QAction::triggered, this, [this](){MainWindow::AddColumn(-1);});
    connect(ui->actionInsert_Row, &QAction::triggered, this, [this](){MainWindow::AddRow(1);});
    connect(ui->actionInsert_Column, &QAction::triggered, this, [this](){MainWindow::AddColumn(1);});
    connect(ui->actionRemove_Row, &QAction::triggered, this, &MainWindow::RemoveRow);
    connect(ui->actionRemove_Column, &QAction::triggered, this, &MainWindow::RemoveColumn);
    connect(ui->tableWidget, &QTableWidget::cellChanged, this, [](){isSaved = false;});
    // Setting the global variables
    isTableLoaded = false;
    isSaved = true;
}



void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!isSaved)
    {
        int choice = QMessageBox::question(this, "Save Changes", "Do you want to save the changes?", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (choice == QMessageBox::Save)
        {
            SaveFile();
        }
        else if (choice == QMessageBox::Discard)
        {
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }
    else
    {
        event->accept();
    }
}



MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::LoadFile()
{
    if (isTableLoaded == true)
    {
        ClearTable();
    }
    if (!isSaved)
    {
        int choice = QMessageBox::question(this, "Save Changes", "Do you want to save the changes?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (choice == QMessageBox::Yes)
        {
            SaveFile();
        }
        else if (choice == QMessageBox::Cancel)
        {
            return;
        }
    }
    // Getting the file path
    filepath = QFileDialog::getOpenFileName(this, "Open a File", "", "*.csv");
    std::string filepath_std = filepath.toStdString();
    // Opening the file
    fin.open(filepath_std);
    // Getting the file name
    QFileInfo fileInfo(filepath);
    QString fileName = fileInfo.fileName();
    std::string std_fileName = fileName.toStdString();
    std_fileName = std_fileName.substr(0, std_fileName.find(".csv"));
    // Updating the file name label
    ui->filename->setText(QString::fromStdString(std_fileName));
    // Drawing the table
    DrawTable();
    isTableLoaded = true;
    isSaved = true;
    fin.close();
}



void MainWindow::DrawTable()
{
    int rows_num = 1;
    // Getting the Header
    std::string ColomnHeader;
    std::getline(fin, ColomnHeader);
    // Getting the number of colomns
    int colomns_num = std::count(ColomnHeader.begin(), ColomnHeader.end(), ',') + 1;
    std::vector<std::string> headers;
    // Inserting the headers into the vector
    std::stringstream sin(ColomnHeader);
    std::string header;
    while (std::getline(sin, header, ','))
    {
        headers.push_back(header);
    }
    // Getting all the rows
    std::vector<std::string> data_raw;
    std::string row;
    while (std::getline(fin, row))
    {
        data_raw.push_back(row);
        ++rows_num;
    }
    // Setting the table dimensions
    ui->tableWidget->setColumnCount(colomns_num);
    ui->tableWidget->setRowCount(rows_num);
    // Adding the rows
    int current_row = 1;
    for (int ss = 0; ss < data_raw.size(); ++ss)
    {
        std::vector<std::string> data;
        std::stringstream sin2(data_raw[ss]);
        std::string cell;
        // Splitting the string
        while (std::getline(sin2, cell, ','))
        {
            data.push_back(cell);
        }
        // Inserting the rows
        for (int i = 0; i < colomns_num; ++i)
        {
            QTableWidgetItem* item2 = new QTableWidgetItem(QString::fromStdString(data[i]));
            item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->tableWidget->setItem(current_row, i, item2);
        }
        data.clear();
        ++current_row;
    }
    // Inserting the colomn headers
    for (int i = 0; i < colomns_num; ++i)
    {
        QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(headers[i]));
        item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        item->setBackground(Qt::lightGray);
        QFont font = item->font();
        font.setBold(true);
        font.setItalic(true);
        font.setPointSize(12);
        item->setFont(font);
        ui->tableWidget->setItem(0, i, item);
    }
    // Updating the table
    ui->tableWidget->update();
}



void MainWindow::SaveFile()
{
    std::ofstream fout(filepath.toStdString());
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
        std::string data = ui->tableWidget->item(i, 0)->text().toStdString();
        for (int j = 1; j < ui->tableWidget->columnCount(); ++j)
        {
            data = data + std::string(1, ',') + ui->tableWidget->item(i, j)->text().toStdString();
        }
        fout << data << '\n';
    }
    isSaved = true;
}



void MainWindow::GetCords()
{
    std::stringstream ss, ss2;
    int current_row = ui->tableWidget->currentRow() + 1;
    int current_col = ui->tableWidget->currentColumn() + 1;
    std::string c_row, c_col;
    ss << "Row: " << current_row;
    c_row = ss.str();
    ss2 << "Colomn: " << current_col;
    c_col = ss2.str();
    QLabel* col_lbl = this->findChild<QLabel*>("col_lbl");
    QLabel* row_lbl = this->findChild<QLabel*>("row_lbl");
    col_lbl->setText(QString::fromStdString(c_col));
    row_lbl->setText(QString::fromStdString(c_row));
}



void MainWindow::ClearTable()
{
    if (!isSaved)
    {
        int choice = QMessageBox::question(this, "Save Changes", "Do you want to save the changes?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (choice == QMessageBox::Yes)
        {
            SaveFile();
        }
        else if (choice == QMessageBox::Cancel)
        {
            return;
        }
    }
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    isTableLoaded = false;
    isSaved = true;
    filepath = "";
}



void MainWindow::ReloadTable()
{
    std::string filepath_std = filepath.toStdString();
    // Reopening the file
    fin.open(filepath_std);
    // Reloading the table
    DrawTable();
    fin.close();
}



void MainWindow::AddRow(int pos)
{
    // Adding the row
    int c_row = ui->tableWidget->rowCount();
    if (pos == -1)
    {
       ui->tableWidget->insertRow(c_row);
    }
    else
    {
        int pos = QInputDialog::getInt(this, "Row Position", "Enter the row's position", 0, 1, c_row+1) - 1;
        ui->tableWidget->insertRow(pos);
    }
    isSaved = false;
}



void MainWindow::AddColumn(int pos)
{
    // Adding the column
    int c_column = ui->tableWidget->columnCount();
    if (pos == -1)
    {
        pos = c_column;
        ui->tableWidget->insertColumn(c_column);
    }
    else
    {
        pos = QInputDialog::getInt(this, "Column Position", "Enter the column's position", 0, 1, c_column+1) - 1;
        ui->tableWidget->insertColumn(pos);
    }
    // Formatting the header
    QTableWidgetItem* item = new QTableWidgetItem();
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item->setBackground(Qt::lightGray);
    QFont font = item->font();
    font.setBold(true);
    font.setItalic(true);
    font.setPointSize(12);
    item->setFont(font);
    ui->tableWidget->setItem(0, pos, item);
    isSaved = false;
}



void MainWindow::RemoveRow()
{
    int max_row = ui->tableWidget->rowCount();
    int n_row = QInputDialog::getInt(this, "Remove Row", "Enter the row number to remove", 0, 0, max_row);
    ui->tableWidget->removeRow(n_row);
    isSaved = false;
}



void MainWindow::RemoveColumn()
{
    int max_column = ui->tableWidget->columnCount();
    int n_column = QInputDialog::getInt(this, "Remove Column", "Enter the column number to remove", 0, 0, max_column);
    ui->tableWidget->removeColumn(n_column);
    isSaved = false;
}
