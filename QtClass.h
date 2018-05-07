#pragma once

#include <QWidget>
#include <QImage>
#include <QColor>
#include <QtWidgets>
#include <QMessageBox>

class QtClass : public QWidget
{
	Q_OBJECT

protected:
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
public:
	QtClass(QWidget *parent = Q_NULLPTR);
	~QtClass();
	void newImage(int x, int y);
	QImage *img;
	int row;
	QRgb* data;
	QVector<QPoint> Point;
	QColor colorB, colorC;


	void DDA(QPoint A, QPoint B, QColor color);
	void vyber_farby();
	void initImage();
	void setPixel(int x, int y, QColor color);
	void BezierovaK();
	void CoonsovaK(); 

	void vymaz();
	
};

