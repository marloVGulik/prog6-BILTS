#ifndef INSTUCTORWINDOW_H
#define INSTUCTORWINDOW_H

#include <QMainWindow>
#include <windows.h>
#include <iostream>
#include <QLabel>
#include <QPixmap>


QT_BEGIN_NAMESPACE
namespace Ui { class InstuctorWindow; }
QT_END_NAMESPACE

class InstuctorWindow : public QMainWindow
{
    Q_OBJECT

public:
    InstuctorWindow(QWidget *parent = nullptr);
    ~InstuctorWindow();

private slots:
    void on_actionExit_triggered();
    void on_actionHart_triggered();

    void on_actionIP_Configuration_triggered();

    void on_actionBloed_druk_triggered();

    void on_actiontempratuur_triggered();

    void on_actionSPO2_triggered();
    void createBackgroundLabel();
private:
    Ui::InstuctorWindow *ui;
};
#endif // INSTUCTORWINDOW_H
