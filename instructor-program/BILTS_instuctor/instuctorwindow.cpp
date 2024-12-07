#include "instuctorwindow.h"
#include "ui_instuctorwindow.h"

InstuctorWindow::InstuctorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::InstuctorWindow)
{
    ui->setupUi(this);
//    hide_frames();
}

InstuctorWindow::~InstuctorWindow()
{
    delete ui;
}


void InstuctorWindow::on_actionExit_triggered()
{
    exit(0);
}

//void InstuctorWindow::hide_frames() {
//    // List of frame pointers
//    QFrame* frames[] = {
//        ui->heartFrame,
//    };

//    // Number of frames in the array
//    int frameCount = sizeof(frames) / sizeof(frames[0]);

//    // Iterate over the array and hide each frame
//    for (int i = 0; i < frameCount; ++i) {
//        frames[i]->hide();
//    }
//}

void InstuctorWindow::on_actionHart_triggered()
{

    // Show the heart frame
//    ui->heartFrame->show();
}
