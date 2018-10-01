#include "gfile.h"

FileProcessor::FileProcessor(QWidget *parent,
                             Spectral *inputSpec):
    QWidget(parent)
  , isNewFile(true)
  , UserSpectral(inputSpec){}

FileProcessor::~FileProcessor(){}

void FileProcessor::creatFile()
{
    isNewFile = true;
    cleanFile();
}

void FileProcessor::cleanFile(){
    UserSpectral->Reset();
}

void FileProcessor::saveFile()
{
    if(isNewFile)
    {
        QFileDialog fileDialog;
        QString str = fileDialog.
                getSaveFileName(this,tr("Save File"),"/New Spectral",
                                     tr("Text File(*.txt)"));
        if(str.isEmpty())
            return;
        QFile filename(str);
        if(!filename.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(
                        this,tr("错误"),tr("打开文件失败"),
                        QMessageBox::Ok);
            return;
        }
        else
        {
            QTextStream textStream(&filename);
            textStream<<FileInfo<<"\n";
            textStream<<"Spectral Name: ";
            textStream<<UserSpectral->Element<<UserSpectral->NucleonNum<<"\n";
            for(int counter = 0; counter < ChannelNum; counter++)
                textStream<<UserSpectral->SignleOutput(counter)<<" ";
            QMessageBox::information(this,tr("保存文件"),tr("保存文件成功"),
                                     QMessageBox::Ok);
        }

        filename.close();
        isNewFile = false;
        UsedName = str;
    }
    else
    {
        QFile file(UsedName);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(this,tr("警告"),tr("打开文件失败"));
            return;
        }
        else
        {
            QTextStream textStream(&file);
            textStream<<FileInfo<<"\n";
            textStream<<"Spectral Name:";
            textStream<<UserSpectral->Element<<UserSpectral->NucleonNum<<"\n";
            for(int counter = 0; counter < ChannelNum; counter++)
                textStream<<UserSpectral->SignleOutput(counter)<<" ";
            QMessageBox::information(this,tr("提示"),tr("保存文件成功"));
        }
        file.close();
    }
}

void FileProcessor::saveFile_as()
{
    QFileDialog fileDialog;
    QString str = fileDialog.
            getSaveFileName(this,tr("Save File as"),"/New Spectral",
                                 tr("Text File(*.txt)"));
    if(str.isEmpty())
        return;
    QFile filename(str);
    if(!filename.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(
                    this,tr("错误"),tr("打开文件失败"),
                    QMessageBox::Ok);
        return;
    }
    else
    {
        QTextStream textStream(&filename);
        textStream<<FileInfo<<"\n";
        textStream<<"Spectral Name: ";
        textStream<<UserSpectral->Element<<UserSpectral->NucleonNum<<"\n";
        for(int counter = 0; counter < ChannelNum; counter++)
            textStream<<UserSpectral->SignleOutput(counter)<<" ";
        QMessageBox::information(this,tr("保存文件"),tr("保存文件成功"),
                                 QMessageBox::Ok);
    }

    filename.close();
}

void FileProcessor::openFile()
{
    QString str = QFileDialog::getOpenFileName(
                this, tr("Open File"), "/", tr("Text File(*.txt)"));
    if(str.isEmpty())
        return;
    QFile filename(str);
    if(!filename.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(
                    this,tr("错误"),tr("打开文件失败"),
                    QMessageBox::Ok);
        return;
    }
    else
    {
        QTextStream textStream(&filename);
        QString stringBuff;
        stringBuff = textStream.readLine();
        if(stringBuff != FileInfo)
            qDebug()<<"Not Match 1";
        stringBuff = textStream.readLine();
        if(stringBuff.left(14) != "Spectral Name:")
            qDebug()<<"Not Match 2";
        QStringList stringListBuff;
        int counter = 0;
        int lengthBuff = 0;
        while(!textStream.atEnd())
        {
            stringBuff = textStream.readLine();
            stringListBuff = stringBuff.split(" ");
            while(stringListBuff.length() + lengthBuff > counter)
            {
                UserSpectral->setChannel(counter, uint64_t(
                                             stringListBuff.at(counter).toLongLong()));
                counter++;
            }
            lengthBuff += stringListBuff.length();
        }
        qDebug()<<lengthBuff<<"'"<<counter;
    }
    filename.close();

}

