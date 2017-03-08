#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QStringList>
#include <QFileInfo>
#include <QProcess>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void StartProcess();

private slots:
    void on_pushButton_EditPath_clicked();

    void on_pushButton_Go_clicked();

    void OnProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    Ui::Widget *ui;

    QStringList fileList;
    int currentId;
    QString output;
    QProcess* process;
    QSettings* settings;
};

#endif // WIDGET_H
