/********************************************************************************
** Form generated from reading UI file 'scheduledialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCHEDULEDIALOG_H
#define UI_SCHEDULEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ScheduleDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *labelTitle;
    QLineEdit *lineEditTitle;
    QLabel *labelLocation;
    QLineEdit *lineEditLocation;
    QLabel *labelTime;
    QTimeEdit *timeEdit;
    QPushButton *input_button;

    void setupUi(QDialog *ScheduleDialog)
    {
        if (ScheduleDialog->objectName().isEmpty())
            ScheduleDialog->setObjectName("ScheduleDialog");
        verticalLayout = new QVBoxLayout(ScheduleDialog);
        verticalLayout->setObjectName("verticalLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        labelTitle = new QLabel(ScheduleDialog);
        labelTitle->setObjectName("labelTitle");

        formLayout->setWidget(0, QFormLayout::LabelRole, labelTitle);

        lineEditTitle = new QLineEdit(ScheduleDialog);
        lineEditTitle->setObjectName("lineEditTitle");

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEditTitle);

        labelLocation = new QLabel(ScheduleDialog);
        labelLocation->setObjectName("labelLocation");

        formLayout->setWidget(1, QFormLayout::LabelRole, labelLocation);

        lineEditLocation = new QLineEdit(ScheduleDialog);
        lineEditLocation->setObjectName("lineEditLocation");

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEditLocation);

        labelTime = new QLabel(ScheduleDialog);
        labelTime->setObjectName("labelTime");

        formLayout->setWidget(2, QFormLayout::LabelRole, labelTime);

        timeEdit = new QTimeEdit(ScheduleDialog);
        timeEdit->setObjectName("timeEdit");

        formLayout->setWidget(2, QFormLayout::FieldRole, timeEdit);


        verticalLayout->addLayout(formLayout);

        input_button = new QPushButton(ScheduleDialog);
        input_button->setObjectName("input_button");

        verticalLayout->addWidget(input_button);


        retranslateUi(ScheduleDialog);

        QMetaObject::connectSlotsByName(ScheduleDialog);
    } // setupUi

    void retranslateUi(QDialog *ScheduleDialog)
    {
        ScheduleDialog->setWindowTitle(QCoreApplication::translate("ScheduleDialog", "\354\235\274\354\240\225 \354\266\224\352\260\200", nullptr));
        labelTitle->setText(QCoreApplication::translate("ScheduleDialog", "\354\240\234\353\252\251:", nullptr));
        labelLocation->setText(QCoreApplication::translate("ScheduleDialog", "\354\236\245\354\206\214:", nullptr));
        labelTime->setText(QCoreApplication::translate("ScheduleDialog", "\354\213\234\352\260\204:", nullptr));
        input_button->setText(QCoreApplication::translate("ScheduleDialog", "\354\266\224\352\260\200", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScheduleDialog: public Ui_ScheduleDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCHEDULEDIALOG_H
