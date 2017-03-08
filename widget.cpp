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

    process = new QProcess(this);
    connect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(OnProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_EditPath_clicked()
{
    fileList = QFileDialog::getOpenFileNames(this,"Select one or more files to upload","/output","JSON (*.json)");
    qDebug()<<fileList;

    QString display="";

    //For each file path selected
    for(int i=0;i<fileList.size();i++)
    {
        if(i!=0)
            display+="; ";
        QString file = fileList[i];
        QFileInfo info = QFileInfo(file);
        display+=info.fileName();
    }

    //Set the display of the file names
    ui->lineEdit_Files->setText(display);
}

void Widget::on_pushButton_Go_clicked()
{
    currentId = 0;
    output="";

    settings->setValue("Endpoint",ui->lineEdit_Endpoint->text());

    StartProcess();
}

void Widget::StartProcess()
{
    if(currentId>=fileList.size())
        return;

    //Execute command like
    //curl -XPOST 'http://search-movies-4f3nw7eiia2xiynjr55a2nao2y.us-west-1.es.amazonaws.com/_bulk' --data-binary @bulk_movies.json
    process->start("curl -XPOST '"+ui->lineEdit_Endpoint->text()+"_bulk' --data-binary @"+fileList[currentId]);
}

void Widget::OnProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    output+=fileList[currentId]+" :\n";
    if(exitCode==-1)
    {
        output+=process->readAllStandardError();
        QMessageBox::critical(this,"Output",output);
    }
    else
    {
        output+=process->readAllStandardOutput();
        output+="\n\n";

        currentId++;
        if(currentId<fileList.size())
            StartProcess();
        else
        {
            output+="\n\nDone !";
            QMessageBox::information(this,"Output",output);
        }
    }
}
