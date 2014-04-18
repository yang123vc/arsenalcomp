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
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_ImageSearchClass
{
public:
    QLabel *matched_image;

    void setupUi(QDialog *ImageSearchClass)
    {
        if (ImageSearchClass->objectName().isEmpty())
            ImageSearchClass->setObjectName(QString::fromUtf8("ImageSearchClass"));
        ImageSearchClass->resize(640, 480);
        ImageSearchClass->setMinimumSize(QSize(640, 480));
        ImageSearchClass->setMaximumSize(QSize(800, 600));
        matched_image = new QLabel(ImageSearchClass);
        matched_image->setObjectName(QString::fromUtf8("matched_image"));
        matched_image->setGeometry(QRect(10, 10, 300, 300));
        matched_image->setAlignment(Qt::AlignCenter);

        retranslateUi(ImageSearchClass);

        QMetaObject::connectSlotsByName(ImageSearchClass);
    } // setupUi

    void retranslateUi(QDialog *ImageSearchClass)
    {
        ImageSearchClass->setWindowTitle(QApplication::translate("ImageSearchClass", "ImageSearch", 0, QApplication::UnicodeUTF8));
        matched_image->setText(QApplication::translate("ImageSearchClass", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ImageSearchClass: public Ui_ImageSearchClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGESEARCH_H
