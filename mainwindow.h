#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);
    void LoadFile();
    void DrawTable();
    void SaveFile();
    void GetCords();
    void ClearTable();
    void ReloadTable();
    void AddRow(int);
    void AddColumn(int);
    void RemoveRow();
    void RemoveColumn();
    // void NewTable();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
