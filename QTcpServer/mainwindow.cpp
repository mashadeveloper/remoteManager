#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QRegExp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _ledInstance = new Led();
}

MainWindow::~MainWindow()
{
    delete ui;
    server_status=0;
}

bool MainWindow::isDec(const QString &str)
{
    QRegExp reg("[0-9]+");
    return reg.indexIn(str) != -1;
}

void MainWindow::on_starting_clicked()
{
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newuser()));
    if (!tcpServer->listen(QHostAddress::Any, 33333) && server_status==0) {
        qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(tcpServer->errorString());
        ui->textinfo->append(tcpServer->errorString());
    } else {
        server_status=1;
        qDebug() << tcpServer->isListening() << "TCPSocket listen on port";
        ui->textinfo->append(QString::fromUtf8("Сервер запущен!"));
        qDebug() << QString::fromUtf8("Сервер запущен!");
    }
}

void MainWindow::on_stoping_clicked()
{
    if(server_status==1){
        foreach(int i,SClients.keys()){
            QTextStream os(SClients[i]);
            os.setAutoDetectUnicode(true);
            os << QDateTime::currentDateTime().toString() << "\n";
            SClients[i]->close();
            SClients.remove(i);
        }
        tcpServer->close();
        ui->textinfo->append(QString::fromUtf8("Сервер остановлен!"));
        qDebug() << QString::fromUtf8("Сервер остановлен!");
        server_status=0;
    }
}


void MainWindow::newuser()
{
    if(server_status==1){
        qDebug() << QString::fromUtf8("У нас новое соединение!");
        ui->textinfo->append(QString::fromUtf8("У нас новое соединение!"));
        QTcpSocket* clientSocket=tcpServer->nextPendingConnection();
        int idusersocs=clientSocket->socketDescriptor();
        SClients[idusersocs]=clientSocket;
        connect(SClients[idusersocs],SIGNAL(readyRead()),this, SLOT(slotReadClient()));
    }
}

void MainWindow::slotReadClient()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    int idusersocs=clientSocket->socketDescriptor();
    QTextStream os;
    os.setDevice(clientSocket);
    os.setAutoDetectUnicode(true);

    QByteArray request;
    request.append(clientSocket->readAll());
    ui->textinfo->append("ReadClient:"+request+"\n\r");

    int beginStr = request.indexOf("{\"command\"");
    int endStr = request.indexOf("\"}");
    int resStr = request.size() - endStr - 2;
    request.remove(0, beginStr);
    request.chop(resStr);

    QJsonDocument jsonDoc;
    jsonDoc = QJsonDocument::fromJson(request);
    QJsonObject root = jsonDoc.object();
    QJsonValue commandVal = root.value("command");
    QJsonValue argumentVal = root.value("argument");

    if (commandVal.toString() == "set-led-state")
    {
        if (argumentVal.toString() == "on" || argumentVal.toString() == "off")
        {
            _ledInstance->setLedState(argumentVal.toString());
            os << "OK. Led state set " << "\"" << _ledInstance->ledState() << "\".\n";
        }
        else
        {
            os << "Failed. Error:Argument can take values: \"on\", \"off\"\n";
        }
    }
    else if (commandVal.toString() == "get-led-state")
    {
        os << "OK " << _ledInstance->ledState() << "\n";
    }
    else if (commandVal.toString() == "set-led-color")
    {
        bool check = false;
        for (QList<QString>::iterator itr = _ledInstance->colorList().begin(); itr != _ledInstance->colorList().end(); itr++)
        {
            if (argumentVal.toString() == *itr)
            {
                _ledInstance->setLedColor(argumentVal.toString());
                check = true;
                break;
            }
        }
        if (check)
        {
            os << "OK. Led color set " << _ledInstance->ledColor() << ".\n";
        }
        else
        {
            os << "Failed. Error:Invalid argument value.\n";
        }
    }
    else if (commandVal.toString() == "get-led-color")
    {
        os << "OK " << _ledInstance->ledColor() << "\n";
    }
    else if (commandVal.toString() == "set-led-rate")
    {
        QString str;
        str = argumentVal.toString();
        if (isDec(str))
        {
            int intStr = str.toInt();
            if (intStr >= 0 && intStr <= 5)
            {
                _ledInstance->setLedRate(intStr);
                os << "OK. Led rate set on " << intStr << ".\n";
            }
            else
            {
                os << "Failed. Error:Invalid argument value.\n";
            }
        }
        else
        {
            os << "Failed. Error:Invalid argument value.\n";
        }
    }
    else if (commandVal.toString() == "get-led-rate")
    {
        QString str = QString::number(_ledInstance->ledRate());
        os << "OK. Led rate is " << str << ".\n";
    }
    else
    {
        os << "Failed. Invalid command.\n";
    }
    // Если нужно закрыть сокет
    clientSocket->close();
    SClients.remove(idusersocs);
}
