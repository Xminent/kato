#include <QApplication>
#include <mozart/api/win_dark.hpp>
#include <mozart/views/main_window.hpp>

int main(int argc, char *argv[])
{
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication app(argc, argv);

	// winDark::setDark_qApp();

	mozart::MainWindow window;
	window.show();

	return QApplication::exec();
}
