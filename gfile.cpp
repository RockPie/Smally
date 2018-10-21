#include "gfile.h"

FileProcessor::FileProcessor(QWidget *parent,
                             Spectral *inputSpec):
    QObject(parent)
  , isNewFile(true)
  , UserSpectral(inputSpec){
    WidgetBuff = parent;
}

FileProcessor::~FileProcessor(){}

void FileProcessor::creatFile()
{
    isNewFile = true;
    cleanFile();
    NewFile_Dialog *NewFileWindow = new NewFile_Dialog(WidgetBuff);
    connect(NewFileWindow,      &NewFile_Dialog::NucNamechanged,
            this,               &FileProcessor::setNucName);
    connect(NewFileWindow,      &NewFile_Dialog::NucNumchanged,
            this,               &FileProcessor::setNucNum);
    NewFileWindow->show();
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
                getSaveFileName(WidgetBuff, tr("Save File"),"/New Spectral",
                                     tr("Text File(*.txt)"));
        if(str.isEmpty())
            return;
        QFile filename(str);
        if(!filename.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(
                        WidgetBuff, tr("错误"),tr("打开文件失败"),
                        QMessageBox::Ok);
            return;
        }
        else
        {
            QTextStream textStream(&filename);
            textStream<<FileInfo<<"\n";
            textStream<<"Spectral Name: ";
            textStream<<UserSpectral->Element<<" "<<UserSpectral->NucleonNum<<"\n";
            for(int counter = 0; counter < ChannelNum - 1; counter++)
                textStream<<UserSpectral->SignalOutput(counter)<<" ";
            textStream<<UserSpectral->SignalOutput(ChannelNum - 1);
            QMessageBox::information(WidgetBuff, tr("保存文件"),tr("保存文件成功"),
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
            QMessageBox::warning(WidgetBuff,tr("警告"),tr("打开文件失败"));
            return;
        }
        else
        {
            QTextStream textStream(&file);
            textStream<<FileInfo<<"\n";
            textStream<<"Spectral Name:";
            textStream<<UserSpectral->Element<<" "<<UserSpectral->NucleonNum<<"\n";
            for(int counter = 0; counter < ChannelNum - 1; counter++)
                textStream<<UserSpectral->SignalOutput(counter)<<" ";
            textStream<<UserSpectral->SignalOutput(ChannelNum - 1);
            QMessageBox::information(WidgetBuff,tr("提示"),tr("保存文件成功"));
        }
        file.close();
    }
}

void FileProcessor::saveFile_as()
{
    QFileDialog fileDialog;
    QString str = fileDialog.
            getSaveFileName(WidgetBuff, tr("Save File as"),"/New Spectral",
                                 tr("Text File(*.txt)"));
    if(str.isEmpty())
        return;
    QFile filename(str);
    if(!filename.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(
                    WidgetBuff, tr("错误"),tr("打开文件失败"),
                    QMessageBox::Ok);
        return;
    }
    else
    {
        QTextStream textStream(&filename);
        textStream<<FileInfo<<"\n";
        textStream<<"Spectral Name: ";
        textStream<<UserSpectral->Element<<" "<<UserSpectral->NucleonNum<<"\n";
        for(int counter = 0; counter < ChannelNum - 1; counter++)
            textStream<<UserSpectral->SignalOutput(counter)<<" ";
        textStream<<UserSpectral->SignalOutput(ChannelNum - 1);
        QMessageBox::information(WidgetBuff, tr("保存文件"),tr("保存文件成功"),
                                 QMessageBox::Ok);
    }

    filename.close();
}

void FileProcessor::openFile()
{
    QString str = QFileDialog::getOpenFileName(
                WidgetBuff, tr("Open File"), "/", tr("Text File(*.txt)"));
    if(str.isEmpty())
        return;
    QFile filename(str);
    if(!filename.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(
                    WidgetBuff, tr("错误"),tr("打开文件失败"),
                    QMessageBox::Ok);
        return;
    }
    else
    {
        QTextStream textStream(&filename);
        QString stringBuff;
        stringBuff = textStream.readLine();
        //Check first line content
        if(stringBuff != FileInfo)
            qDebug()<<"Not Match 1";
        stringBuff = textStream.readLine();
        //Check second line content
        if(stringBuff.left(14) != "Spectral Name:")
            qDebug()<<"Not Match 2";
        else
        {
            UserSpectral->setElement(stringBuff.split(" ").at(2));
            UserSpectral->setNucNum(stringBuff.split(" ").at(3).toUInt());
        }
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
    UsedName = str;
    isNewFile = false;
    emit NewDataLoaded();
}

void FileProcessor::setNucName(QString input){
    UserSpectral->setElement(input);
}

void FileProcessor::setNucNum(int input){
    UserSpectral->setNucNum(uint(input));
}

