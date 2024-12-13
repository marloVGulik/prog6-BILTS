#include "instuctorwindow.h"
#include "ui_instuctorwindow.h"

InstuctorWindow::InstuctorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::InstuctorWindow)
{
    ui->setupUi(this);

    // Initialize the MQTT client once with the broker address and topic
    std::string brokerAddress = "localhost";
    std::string topic = "test/topic";
    mqttClient = new MqttClient(brokerAddress, topic);
}

InstuctorWindow::~InstuctorWindow()
{
    delete mqttClient; // Clean up the MQTT client
    delete ui;
}

// Menu navigation
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

void InstuctorWindow::on_pushButton_clicked()
{
    // Message to publish
    std::string message = "Hello from MQTT Client!";

    // Publish the message using the initialized MQTT client
    if (mqttClient->publishMessage(message)) {
        std::cout << "Message published successfully!" << std::endl;
    } else {
        std::cerr << "Failed to publish the message." << std::endl;
    }
}
