#ifndef IMAGESEARCH_H
#define IMAGESEARCH_H

#include <QtGui/QDialog>
#include "ui_imagesearch.h"

class ImageSearch : public QDialog
{
	Q_OBJECT

public:
	ImageSearch(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ImageSearch();

private:
	Ui::ImageSearchClass ui;
};

#endif // IMAGESEARCH_H
