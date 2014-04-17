/********************************************************************************
** Form generated from reading UI file 'imagesearch.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGESEARCH_H
#define UI_IMAGESEARCH_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_ImageSearchClass
{
public:

    void setupUi(QDialog *ImageSearchClass)
    {
        if (ImageSearchClass->objectName().isEmpty())
            ImageSearchClass->setObjectName(QString::fromUtf8("ImageSearchClass"));
        ImageSearchClass->resize(600, 400);

        retranslateUi(ImageSearchClass);

        QMetaObject::connectSlotsByName(ImageSearchClass);
    } // setupUi

    void retranslateUi(QDialog *ImageSearchClass)
    {
        ImageSearchClass->setWindowTitle(QApplication::translate("ImageSearchClass", "ImageSearch", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ImageSearchClass: public Ui_ImageSearchClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGESEARCH_H
