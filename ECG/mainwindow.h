#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ecgsimulator.h"
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

private slots:
    void on_Heartrateslider_valueChanged(int value);
    void updateWaveform();

private:
    Ui::MainWindow *ui;
    ECGsimulator* ecgSimulator;
    double waveformPhase;
    QTimer * timer;

protected:
    void paintEvent(QPaintEvent *event) override;
};
#endif // MAINWINDOW_H
