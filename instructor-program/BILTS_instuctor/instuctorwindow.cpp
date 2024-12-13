#include "instuctorwindow.h"
#include "ui_instuctorwindow.h"

InstuctorWindow::InstuctorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::InstuctorWindow)
{
    ui->setupUi(this);
    createBackgroundLabel(); // creates background picture heartpage
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

// create hartpage sine
void InstuctorWindow::createBackgroundLabel() {
    // Get the page from the stacked widget
    QWidget* hartwindow = ui->pages->findChild<QWidget*>("HartPage");
    if (!hartwindow) {
        qWarning("Hartwindow page not found in the stacked widget.");
        return;
    }

    // Create the label and set it as a child of the hartwindow
    QLabel* backgroundLabel = new QLabel(hartwindow);

    // Load the background image
    QPixmap pixmap("../BILTS_instuctor/Heartsine.png");

    // Set the pixmap and scale it to fit the page size
    backgroundLabel->setPixmap(pixmap.scaled(hartwindow->size(), Qt::KeepAspectRatioByExpanding));
    backgroundLabel->setScaledContents(true); // Optional: Ensures the image scales with the label

    // Resize the label to cover the entire hartwindow
    backgroundLabel->resize(350, 350);

    // Center the label in the hartwindow
    int x = (hartwindow->width() - backgroundLabel->width()) / 2;
    int y = (hartwindow->height() - backgroundLabel->height()) / 2;
    backgroundLabel->move(x, y);
}
