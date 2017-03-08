#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle("Kibana Uploader");

    settings = new QSettings("LonelyYorsh","KibanaUploader",this);
    ui->lineEdit_Endpoint->setText(settings->value("Endpoint","").toString());
    path = settings->value("Path","").toString();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_EditPath_clicked()
{
    fileList = QFileDialog::getOpenFileNames(this,"Select one or more files to upload",path,"JSON (*.json)");
    qDebug()<<fileList;

    QString display="";

    //For each file path selected
    for(int i=0;i<fileList.size();i++)
    {
        if(i!=0)
            display+="; ";
        QString file = fileList[i];
        QFileInfo info = QFileInfo(file);
        path = info.dir().path();
        display+=info.fileName();
    }

    //Set the display of the file names
    ui->lineEdit_Files->setText(display);
}

void Widget::on_pushButton_Go_clicked()
{
    QString output="";

    settings->setValue("Endpoint",ui->lineEdit_Endpoint->text());
    settings->setValue("Path",path);

    for(int i=0;i<fileList.size();i++)
    {
        QProcess process;
        //Execute command like
        //curl -XPOST 'http://search-movies-4f3nw7eiia2xiynjr55a2nao2y.us-west-1.es.amazonaws.com/_bulk' --data-binary @bulk_movies.json
        process.setProcessChannelMode(QProcess::MergedChannels);
        QString command = "curl -XPOST '"+ui->lineEdit_Endpoint->text()+"_bulk' --data-binary @"+fileList[i];
        process.start(command.toStdString().c_str());

        // Wait for it to start
        if(!process.waitForStarted())
        {
            QMessageBox::critical(this,"Error","Command could not be started :\n"+command);
            return;
        }

        // Continue reading the data until EOF reached
        QByteArray data;

        while(process.waitForReadyRead())
            data.append(process.readAll());

        process.waitForFinished();

        output+="\n\n"+fileList[i]+" :\n\n";
        output +=data;
    }

    ui->textEdit_Output->setPlainText(output);
}
