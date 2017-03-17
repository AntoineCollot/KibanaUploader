#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QStringList>
#include <QFileInfo>
#include <QProcess>
#include <QMessageBox>
#include <QSettings>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_EditPath_clicked();

    void on_pushButton_Go_clicked();

private:
    Ui::Widget *ui;

    QStringList fileList;
    QSettings* settings;
    QString path;
};

#endif // WIDGET_H
