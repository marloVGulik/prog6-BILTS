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


//menu navigation

void InstuctorWindow::on_actionExit_triggered()
{
    exit(0);
}

void InstuctorWindow::on_actionHart_triggered()
{
    ui->pages->setCurrentWidget(ui->HartPage);
}

void InstuctorWindow::on_actionIP_Configuration_triggered()
{
    ui->pages->setCurrentWidget(ui->SettingsPage);
}


void InstuctorWindow::on_actionBloed_druk_triggered()
{
    ui->pages->setCurrentWidget(ui->BD_page);
}


void InstuctorWindow::on_actiontempratuur_triggered()
{
    // Switch to the TempPage
    ui->pages->setCurrentWidget(ui->TempPage);
}

void InstuctorWindow::on_actionSPO2_triggered()
{
    ui->pages->setCurrentWidget(ui->SPO2_Page);
}

