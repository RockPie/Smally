/**********************************************************
 * 代码编写：贾世海
 * Coding: Geoffrey Jia
 * 清华大学工程物理系
 * Department of Engineering Physics, Tsinghua University
 * 开发开始日期：2018年09月18日
 * Development starting date: Sep./18/2018
 * 开发平台：
 *          操作系统：Microsoft Windows 10 Home
 *          IDE:Qt Creator 4.7.0
 *          开发框架：Qt 5.11.1(MSVC 2015)
 *          第三方库：Qwt-6.1.3
 * 软件仅作学习用途，禁止任何形式的商业使用
 * The software is for learning purposes only
 * Any form of commercial use is prohibited
 * e-mail: jsh15@mails.tsinghua.edu.cn
 **********************************************************/

#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //Set splash screen
    QPixmap SmallyPixmap(":/png/SplashScreen.png");
    QSplashScreen *SmallySplash = new QSplashScreen(SmallyPixmap);
    SmallySplash->show();
    app.processEvents();

    MainWindow win;
    win.show();
    SmallySplash->finish(&win);
    delete SmallySplash;

    return app.exec();
}
