/********************************************************************************
** Form generated from reading UI file 'QtGuiApplication1.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTGUIAPPLICATION1_H
#define UI_QTGUIAPPLICATION1_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtGuiApplication1Class
{
public:
    QAction *start;
    QAction *close;
    QAction *actionQuit;
    QWidget *centralWidget;
    QPushButton *OKbutton;
    QLineEdit *ip1;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *ip2;
    QLabel *label_4;
    QLineEdit *ip3;
    QLineEdit *ip4;
    QLabel *label_5;
    QLabel *label_6;
    QLineEdit *port1;
    QLabel *label_7;
    QLineEdit *homepage;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QLabel *outputLabel;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents2;
    QLabel *outputLabel2;
    QPushButton *Start;
    QPushButton *Close;
    QScrollArea *scrollArea3;
    QWidget *scrollAreaWidgetContents2_3;
    QLabel *outputLabel3;
    QScrollArea *scrollArea4;
    QWidget *scrollAreaWidgetContents4;
    QLabel *outputLabel4;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtGuiApplication1Class)
    {
        if (QtGuiApplication1Class->objectName().isEmpty())
            QtGuiApplication1Class->setObjectName(QStringLiteral("QtGuiApplication1Class"));
        QtGuiApplication1Class->resize(1057, 1197);
        start = new QAction(QtGuiApplication1Class);
        start->setObjectName(QStringLiteral("start"));
        start->setCheckable(false);
        close = new QAction(QtGuiApplication1Class);
        close->setObjectName(QStringLiteral("close"));
        actionQuit = new QAction(QtGuiApplication1Class);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        centralWidget = new QWidget(QtGuiApplication1Class);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        OKbutton = new QPushButton(centralWidget);
        OKbutton->setObjectName(QStringLiteral("OKbutton"));
        OKbutton->setGeometry(QRect(920, 100, 101, 31));
        ip1 = new QLineEdit(centralWidget);
        ip1->setObjectName(QStringLiteral("ip1"));
        ip1->setGeometry(QRect(130, 90, 101, 61));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(240, 130, 61, 20));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(16);
        label_2->setFont(font);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(360, 130, 61, 20));
        label_3->setFont(font);
        ip2 = new QLineEdit(centralWidget);
        ip2->setObjectName(QStringLiteral("ip2"));
        ip2->setGeometry(QRect(250, 90, 101, 61));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(480, 130, 61, 20));
        label_4->setFont(font);
        ip3 = new QLineEdit(centralWidget);
        ip3->setObjectName(QStringLiteral("ip3"));
        ip3->setGeometry(QRect(370, 90, 101, 61));
        ip4 = new QLineEdit(centralWidget);
        ip4->setObjectName(QStringLiteral("ip4"));
        ip4->setGeometry(QRect(490, 90, 101, 61));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(50, 100, 101, 41));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(18);
        label_5->setFont(font1);
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(620, 100, 191, 41));
        label_6->setFont(font1);
        port1 = new QLineEdit(centralWidget);
        port1->setObjectName(QStringLiteral("port1"));
        port1->setGeometry(QRect(790, 90, 101, 61));
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(50, 20, 261, 61));
        label_7->setFont(font1);
        homepage = new QLineEdit(centralWidget);
        homepage->setObjectName(QStringLiteral("homepage"));
        homepage->setGeometry(QRect(340, 20, 551, 61));
        scrollArea = new QScrollArea(centralWidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setGeometry(QRect(50, 200, 281, 431));
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea->setWidgetResizable(false);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 800, 12000));
        outputLabel = new QLabel(scrollAreaWidgetContents);
        outputLabel->setObjectName(QStringLiteral("outputLabel"));
        outputLabel->setGeometry(QRect(20, 0, 800, 12000));
        outputLabel->setStyleSheet(QLatin1String("#outputLabel{\n"
"	background:white;\n"
"}"));
        outputLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        scrollArea->setWidget(scrollAreaWidgetContents);
        scrollArea_2 = new QScrollArea(centralWidget);
        scrollArea_2->setObjectName(QStringLiteral("scrollArea_2"));
        scrollArea_2->setGeometry(QRect(380, 200, 281, 431));
        scrollArea_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea_2->setWidgetResizable(false);
        scrollAreaWidgetContents2 = new QWidget();
        scrollAreaWidgetContents2->setObjectName(QStringLiteral("scrollAreaWidgetContents2"));
        scrollAreaWidgetContents2->setGeometry(QRect(0, 0, 800, 12000));
        outputLabel2 = new QLabel(scrollAreaWidgetContents2);
        outputLabel2->setObjectName(QStringLiteral("outputLabel2"));
        outputLabel2->setGeometry(QRect(0, 0, 800, 12000));
        outputLabel2->setStyleSheet(QLatin1String("#outputLabel2{\n"
"	background:white;\n"
"}"));
        outputLabel2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        scrollArea_2->setWidget(scrollAreaWidgetContents2);
        Start = new QPushButton(centralWidget);
        Start->setObjectName(QStringLiteral("Start"));
        Start->setGeometry(QRect(920, 20, 101, 31));
        Close = new QPushButton(centralWidget);
        Close->setObjectName(QStringLiteral("Close"));
        Close->setGeometry(QRect(920, 60, 101, 31));
        scrollArea3 = new QScrollArea(centralWidget);
        scrollArea3->setObjectName(QStringLiteral("scrollArea3"));
        scrollArea3->setGeometry(QRect(710, 200, 281, 431));
        scrollArea3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea3->setWidgetResizable(false);
        scrollAreaWidgetContents2_3 = new QWidget();
        scrollAreaWidgetContents2_3->setObjectName(QStringLiteral("scrollAreaWidgetContents2_3"));
        scrollAreaWidgetContents2_3->setGeometry(QRect(0, 0, 800, 12000));
        outputLabel3 = new QLabel(scrollAreaWidgetContents2_3);
        outputLabel3->setObjectName(QStringLiteral("outputLabel3"));
        outputLabel3->setGeometry(QRect(0, 0, 800, 12000));
        outputLabel3->setStyleSheet(QLatin1String("#outputLabel3{\n"
"	background:white;\n"
"}"));
        outputLabel3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        scrollArea3->setWidget(scrollAreaWidgetContents2_3);
        scrollArea4 = new QScrollArea(centralWidget);
        scrollArea4->setObjectName(QStringLiteral("scrollArea4"));
        scrollArea4->setGeometry(QRect(49, 669, 941, 441));
        scrollArea4->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea4->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea4->setWidgetResizable(false);
        scrollArea4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        scrollAreaWidgetContents4 = new QWidget();
        scrollAreaWidgetContents4->setObjectName(QStringLiteral("scrollAreaWidgetContents4"));
        scrollAreaWidgetContents4->setGeometry(QRect(0, 0, 1200, 24000));
        outputLabel4 = new QLabel(scrollAreaWidgetContents4);
        outputLabel4->setObjectName(QStringLiteral("outputLabel4"));
        outputLabel4->setGeometry(QRect(0, 2, 1200, 24000));
        outputLabel4->setStyleSheet(QLatin1String("#outputLabel4{\n"
"	background:white;\n"
"}"));
        outputLabel4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        scrollArea4->setWidget(scrollAreaWidgetContents4);
        QtGuiApplication1Class->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(QtGuiApplication1Class);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QtGuiApplication1Class->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QtGuiApplication1Class);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QtGuiApplication1Class->setStatusBar(statusBar);

        retranslateUi(QtGuiApplication1Class);

        QMetaObject::connectSlotsByName(QtGuiApplication1Class);
    } // setupUi

    void retranslateUi(QMainWindow *QtGuiApplication1Class)
    {
        QtGuiApplication1Class->setWindowTitle(QApplication::translate("QtGuiApplication1Class", "QtGuiApplication1", nullptr));
        start->setText(QApplication::translate("QtGuiApplication1Class", "Start Server", nullptr));
        close->setText(QApplication::translate("QtGuiApplication1Class", "Close Server", nullptr));
        actionQuit->setText(QApplication::translate("QtGuiApplication1Class", "Quit", nullptr));
        OKbutton->setText(QApplication::translate("QtGuiApplication1Class", "Config", nullptr));
        ip1->setText(QApplication::translate("QtGuiApplication1Class", "127", nullptr));
        label_2->setText(QApplication::translate("QtGuiApplication1Class", ".", nullptr));
        label_3->setText(QApplication::translate("QtGuiApplication1Class", ".", nullptr));
        ip2->setText(QApplication::translate("QtGuiApplication1Class", "0", nullptr));
        label_4->setText(QApplication::translate("QtGuiApplication1Class", ".", nullptr));
        ip3->setText(QApplication::translate("QtGuiApplication1Class", "0", nullptr));
        ip4->setText(QApplication::translate("QtGuiApplication1Class", "1", nullptr));
        label_5->setText(QApplication::translate("QtGuiApplication1Class", "IP:", nullptr));
        label_6->setText(QApplication::translate("QtGuiApplication1Class", "PORT:", nullptr));
        port1->setText(QApplication::translate("QtGuiApplication1Class", "5050", nullptr));
        label_7->setText(QApplication::translate("QtGuiApplication1Class", "HomePage:", nullptr));
        homepage->setText(QString());
        outputLabel->setText(QString());
        outputLabel2->setText(QString());
        Start->setText(QApplication::translate("QtGuiApplication1Class", "Start", nullptr));
        Close->setText(QApplication::translate("QtGuiApplication1Class", "Close", nullptr));
        outputLabel3->setText(QString());
        outputLabel4->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class QtGuiApplication1Class: public Ui_QtGuiApplication1Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTGUIAPPLICATION1_H
