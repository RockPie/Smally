#include "newfile_dialog.h"
#include "ui_newfile_dialog.h"

NewFile_Dialog::NewFile_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewFile_Dialog),
    NameBuff("Unknown"),
    NumberBuff(0)
{
    ui->setupUi(this);
    this->setWindowTitle("设置新谱图名");
    connect(ui->NucNamelineEdit,        &QLineEdit::textChanged,
            this,                       &NewFile_Dialog::onNameChanged);
    connect(ui->NucNumspinBox,          SIGNAL(valueChanged(int)),
            this,                       SLOT(onNumberChanged(int)));
}

NewFile_Dialog::~NewFile_Dialog()
{
    delete ui;
}

void NewFile_Dialog::onNameChanged(QString input){
    NameBuff = input;
    emit NucNamechanged(NameBuff);
}

void NewFile_Dialog::onNumberChanged(int input){
    NumberBuff = input;
    emit NucNumchanged(NumberBuff);
}
