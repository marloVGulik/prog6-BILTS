#ifndef INSTUCTORWINDOW_H
#define INSTUCTORWINDOW_H

#include <QMainWindow>

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

private:
    Ui::InstuctorWindow *ui;
};
#endif // INSTUCTORWINDOW_H
