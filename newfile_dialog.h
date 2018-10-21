#ifndef NEWFILE_DIALOG_H
#define NEWFILE_DIALOG_H

#include <QDialog>

namespace Ui {
class NewFile_Dialog;
}

class NewFile_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewFile_Dialog(QWidget *parent = nullptr);
    ~NewFile_Dialog();

signals:
    void NucNamechanged(QString str);
    void NucNumchanged(int val);

private:
    Ui::NewFile_Dialog *ui;
    QString NameBuff;
    int NumberBuff;

private slots:
    void onNameChanged(QString input);
    void onNumberChanged(int input);
};

#endif // NEWFILE_DIALOG_H
