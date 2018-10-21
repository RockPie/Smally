#ifndef GFILE_H
#define GFILE_H

#include <QWidget>
#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QtEvents>
#include <QFileDevice>
#include "gdatabase.h"
#include "gsetting.h"
#include "newfile_dialog.h"

class FileProcessor: public QObject
{
    Q_OBJECT
public:
    explicit FileProcessor(QWidget *parent,
                           Spectral *inputSpec);
    ~FileProcessor();

public slots:
    void saveFile();
    void creatFile();
    void cleanFile();
    void saveFile_as();
    void openFile();

signals:
    void NewDataLoaded();

private:
    bool isNewFile;
    Spectral *UserSpectral;
    QString UsedName;
    QWidget *WidgetBuff;

private slots:
    void setNucName(QString input);
    void setNucNum(int input);

};

#endif // GFILE_H
