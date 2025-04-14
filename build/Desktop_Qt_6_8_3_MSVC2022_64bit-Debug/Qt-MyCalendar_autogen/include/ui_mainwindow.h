/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *monthNavigationLayout;
    QPushButton *shift_left;
    QLabel *monthLabel;
    QPushButton *shift_right;
    QWidget *calendarWidget;
    QGridLayout *gridLayout;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        monthNavigationLayout = new QHBoxLayout();
        monthNavigationLayout->setObjectName("monthNavigationLayout");
        shift_left = new QPushButton(centralwidget);
        shift_left->setObjectName("shift_left");

        monthNavigationLayout->addWidget(shift_left);

        monthLabel = new QLabel(centralwidget);
        monthLabel->setObjectName("monthLabel");
        monthLabel->setAlignment(Qt::AlignCenter);

        monthNavigationLayout->addWidget(monthLabel);

        shift_right = new QPushButton(centralwidget);
        shift_right->setObjectName("shift_right");

        monthNavigationLayout->addWidget(shift_right);


        verticalLayout->addLayout(monthNavigationLayout);

        calendarWidget = new QWidget(centralwidget);
        calendarWidget->setObjectName("calendarWidget");
        gridLayout = new QGridLayout(calendarWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);

        verticalLayout->addWidget(calendarWidget);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\354\235\274\354\240\225\352\264\200\353\246\254 \355\224\204\353\241\234\352\267\270\353\236\250", nullptr));
        shift_left->setText(QCoreApplication::translate("MainWindow", "<", nullptr));
        monthLabel->setText(QCoreApplication::translate("MainWindow", "2025-04", nullptr));
        shift_right->setText(QCoreApplication::translate("MainWindow", ">", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
