#ifndef GFILE_H
#define GFILE_H

#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QtEvents>
#include <QFileDevice>
#include "gdatabase.h"
#include "gsetting.h"

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

private:
    bool isNewFile;
    Spectral *UserSpectral;
    QString UsedName;
    QWidget *WidgetBuff;

};

#endif // GFILE_H
