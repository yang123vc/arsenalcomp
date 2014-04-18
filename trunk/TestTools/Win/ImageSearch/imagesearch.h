#ifndef IMAGESEARCH_H
#define IMAGESEARCH_H


#include "stdhdr.h"

#include "image_hash.h"


#include "ui_imagesearch.h"


class ImageSearch : public QDialog
{
		Q_OBJECT

private slots:
		void on_check_foreground_window();
		void on_icon_activated(QSystemTrayIcon::ActivationReason reason);
protected:
		void check_foreground_window();
		void on_matched(DWORD pid, const CByteImage_t &img, const std::wstring &matched_sample_name, size_t distance);
public:
		ImageSearch(QWidget *parent = 0, Qt::WFlags flags = 0);
		~ImageSearch();

public:
		void LoadSamples(const std::wstring &path);

private:
		Ui::ImageSearchClass	ui;

		QAction *m_showAction;
		QAction *m_hideAction;
		QAction *m_quitAction;

		QMenu					*m_trayIconMenu;
		QSystemTrayIcon			*m_tray;
private:
		typedef std::map<std::wstring, std::string>		SampleMap_t;
		SampleMap_t		m_sample_map;

private:
		QTimer *m_check_foreground_timer;

private:
		QPixmap		m_matched_pic;
};




#endif // IMAGESEARCH_H
