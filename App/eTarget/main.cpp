#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QObject>

#include <Bluetooth/DeviceHandler.h>

#include <Business/TargetIndicator.h>
#include <Business/TargetIndicatorModel.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    Bluetooth::DeviceHandler deviceHandler;
    deviceHandler.tryToConnect();

    Business::TargetIndicator targetIndicator;
    Business::TargetIndicatorModel targetIndicatorModel(targetIndicator);

    QObject::connect(&deviceHandler, &Bluetooth::DeviceHandler::strikeDetected, &targetIndicator, &Business::TargetIndicator::onStrikeDetected);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("device", &deviceHandler);
    engine.rootContext()->setContextProperty("targetIndicatorModel", &targetIndicatorModel);
    engine.load(QUrl(QStringLiteral("qrc:/QML/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
