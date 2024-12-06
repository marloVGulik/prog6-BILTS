#include "instuctorwindow.h"
#include "ui_instuctorwindow.h"

InstuctorWindow::InstuctorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::InstuctorWindow)
{
    ui->setupUi(this);
}

InstuctorWindow::~InstuctorWindow()
{
    delete ui;
}

