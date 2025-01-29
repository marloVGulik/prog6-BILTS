#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ecgsimulator.h"
#include <qpainter.h>
#include <cmath>  // for sin()
#include <QPushButton>
#include <QTimer>
#include <QElapsedTimer>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ecgSimulator(new ECGsimulator())
    , waveformPhase(0.0)
{
    ui->setupUi(this);

    // Create an instance of ECGsimulator here
    ECGsimulator ecgSimulator;
    ui->Heartrateslider->setRange(0, 300);
    ui->Heartrateslider->setValue(ecgSimulator.heartrate);

    // Set up timer for waveform updates
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateWaveform);
    timer->start(50); // Update every 50 ms
}

MainWindow::~MainWindow()
{
    delete ui;
    delete ecgSimulator;
}

void MainWindow::on_Heartrateslider_valueChanged(int value)
{
    ecgSimulator->heartrate = value;
    ui->LCDBPM->display(value);
}

// Update the waveform update function for smoother animation
void MainWindow::updateWaveform(){
    // Calculate the time offset based on the PRbpm (i.e., the phase shift)
    double offsetFactor = ecgSimulator->heartrate / 60.0;  // Convert PRbpm to a fraction (0 to 1)

    // Instead of changing frequency, we adjust the phase offset based on the PRbpm
    waveformPhase += offsetFactor * 0.03;  // Adjust phase increment to shift the waveform
    if (waveformPhase > 1.0) {
        waveformPhase -= 1.0;  // Keep the phase within the range [0, 1]
    }

    // Trigger the repaint of the waveform widget
    ui->Graph->update();
}

void MainWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    ECGsimulator simulator(75);
    int width = this->width();
    int height = this->height();
    int centerY = height / 2;

    // Make amplitude depend on SpO2 value
    int amplitude = centerY;

    QPen pen(QColor(0, 128, 0));
    pen.setWidth(3);
    painter.setPen(pen);

    QPolygon wavePolygon;
    for (int x = 0; x < width; x += 2) {
        double t = (waveformPhase + (double)x / (width * 0.5));
        double y = 0.0;
        y = amplitude * simulator.GenerateWave(t);

        wavePolygon << QPoint(x, centerY - y);
    }
    painter.drawPolyline(wavePolygon);
}

