#include "Calculator.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Calculator calculator;

    calculator.setWindowTitle("Calculator");
    calculator.setFixedSize(230, 270);

    calculator.show();

    return app.exec();
}
