#include "QtClass.h"


QtClass::QtClass(QWidget *parent)
	: QWidget(parent)
{
	vyber_farby();
}

QtClass::~QtClass(){
}

void QtClass::vyber_farby() {
	QMessageBox msgBox;
	msgBox.setText(QStringLiteral("Zadaj farbu pre Bézierovu kubiku"));
	msgBox.exec();
	colorB = QColorDialog::getColor(Qt::red, this);
	

	msgBox.setText(QStringLiteral("Zadaj farbu pre Coonsovu kubiku"));
	msgBox.exec();
	colorC = QColorDialog::getColor(Qt::blue, this);

}

void QtClass::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QRect area = event->rect();
	painter.drawImage(area, *img, area);
}

void QtClass::vymaz() {
	Point.clear();
}


void QtClass::newImage(int x, int y){
	img = new QImage(x, y, QImage::Format_ARGB32);
	img->fill(Qt::white);
	this->setMinimumSize(img->size()); 

	update(); 
}

void QtClass::setPixel(int x, int y, QColor color) {
	data[x + y * row] = color.rgb();
}


void QtClass::DDA(QPoint A, QPoint B, QColor color) {
	double m, x, y;
	QPoint C;

	m = (B.y() - A.y()) / (double)(B.x() - A.x());

	if ((fabs(m)) <= 1) {
		if (A.x() > B.x()) {
			C = B;
			B = A;
			A = C;
		}
		x = A.x();
		y = A.y();
		for (int i = A.x(); i < B.x(); i++) {
			x = x + 1;
			y = y + m;
			setPixel(x, y, color);
		}

	}
	else {
		if (A.y() > B.y()) {
			C = B;
			B = A;
			A = C;
		}
		x = A.x();
		y = A.y();
		for (int i = A.y(); i < B.y(); i++) {
			x = x + 1 / m;
			y = y + 1;
			setPixel(x, y, color);
		}

	}
}

void QtClass::mousePressEvent(QMouseEvent *event) {

	if (event->button() == Qt::LeftButton) {
		Point.push_back(event->pos());
		//qDebug() << "Point "<< event->pos().x() << event->pos().y();
		setPixel(event->pos().x(), event->pos().y(), Qt::black);
		setPixel(event->pos().x() + 1, event->pos().y(), Qt::black);
		setPixel(event->pos().x(), event->pos().y() + 1, Qt::black);
		setPixel(event->pos().x(), event->pos().y() - 1, Qt::black);
		setPixel(event->pos().x() - 1, event->pos().y(), Qt::black);
		setPixel(event->pos().x() + 1, event->pos().y() + 1, Qt::black);
		setPixel(event->pos().x() - 1, event->pos().y() - 1, Qt::black);
		setPixel(event->pos().x() + 1, event->pos().y() - 1, Qt::black);
		setPixel(event->pos().x() - 1, event->pos().y() + 1, Qt::black);

	}
	update();
}

void QtClass::initImage(){
	this->resize(img->size());
	this->setMinimumSize(img->size());

	row = img->bytesPerLine() / 4;
	data = (QRgb *)img->bits();
}


void QtClass::BezierovaK() {
	QVector<QVector<QPoint>> B;
	QVector<QPoint> spoj;
	QVector<QPoint> buf;
	QPoint P;

	spoj.push_back(Point[0]);
	
	for (double t = 0; t < 1; t += 0.05) {

		for (int j = 0; j < Point.size(); j++) {
			buf.push_back(Point[j]);
		}

		B.push_back(buf);
		buf.clear();

		for (int i = 1; i < Point.size(); i++) {
			for (int j =0; j < Point.size()-i ; j++) {
				P = (1 - t)*B[i - 1][j]+ t * B[i - 1][j+ 1];
				buf.push_back(P);
			}
			B.push_back(buf);
			buf.clear();
		}
		spoj.push_back(P);
		B.clear();
	}

	spoj.push_back(*(Point.end() - 1));

	for (int i = 0; i < spoj.size() - 1; i++) {
		DDA(spoj[i], spoj[i + 1], colorB);
	}

	update();

}


void QtClass::CoonsovaK() {
	double x, y;
	QPoint p;
	QVector<QPoint> P;
	QPoint *B;
	double C0, C1, C2, C3;

	for (int i = 0; i < Point.size()-3; i++) {
		
		x =(( Point[i].x()*1 / 6 )+ (Point[i + 1].x()*2/3) +( Point[i + 2].x()*1 / 6));
		y = ((Point[i].y()*1 / 6 )+ (Point[i + 1].y()*2/3) +( Point[i + 2].y()*1 / 6));
		setPixel(x, y, colorC);

		for (double t = 0; t < 1; t += 0.0001) {
			x =((Point[i].x()*(-1)/6*t*t*t+ Point[i].x()*t*t/2- Point[i].x()*t/2+ Point[i].x()/6) +( Point[i + 1].x()/2*t*t*t- Point[i+1].x()*t*t+ Point[i + 1].x()*2/3) +( Point[i + 2].x()*(-1) / 2*t*t*t) + (Point[i + 2].x() / 2*t*t) + Point[i + 2].x() / 2*t + Point[i + 2].x() / 6) + (Point[i + 3].x() / 6*t*t*t);
			y = ((Point[i].y()*(-1) / 6 * t*t*t + Point[i].y()*t*t / 2 - Point[i].y()*t / 2 + Point[i].y() / 6) + (Point[i + 1].y() / 2 * t*t*t - Point[i + 1].y()*t*t + Point[i + 1].y() * 2 / 3) + (Point[i + 2].y()*(-1) / 2 * t*t*t) + (Point[i + 2].y() / 2 * t*t) + Point[i + 2].y() / 2 * t + Point[i + 2].y() / 6) + (Point[i + 3].y() / 6 * t*t*t);
			setPixel(x, y, colorC);
		}


		x = ((Point[i + 1].x()*(1 / 6)) + ((2 / 3)*Point[i + 2].x()) + (Point[i + 3].x()*(1 / 6)));
		y = ((Point[i + 1].y()*(1 / 6) )+( (2 / 3)*Point[i + 2].y()) +( Point[i + 3].y()*(1 / 6)));
		setPixel(round(x), round(y), colorC);
		
	}
	update();
}
