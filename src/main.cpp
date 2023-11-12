#include <QApplication>
#include <kato/api/win_dark.hpp>
#include <kato/views/main_window.hpp>

int main(int argc, char *argv[])
{
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication app(argc, argv);

	kato::MainWindow window;
	window.show();

	return QApplication::exec();
}
