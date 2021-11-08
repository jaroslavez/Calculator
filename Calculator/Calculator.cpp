#include "Calculator.h"

Calculator::Calculator(QWidget* pwg) : QWidget(pwg)
{
    /*--------------Creating an initial expression--------------*/
    InnerExpression* first_exp = new InnerExpression();
    current_exp = first_exp;

    /*--------------Body of the calculator widget--------------*/
    Layout= new QGridLayout;
    setDisplay();
    setButton();
    setInfo("Created by jaroslavez. v.3.1");
    
    setLayout(Layout);
}

void Calculator::setButton()
{
    QString Buttons[5][4]= {  {"(", ")", "", "CE"},
                              {"7", "8", "9", "/"},
                              {"4", "5", "6", "*"},
                              {"1", "2", "3", "-"},
                              {"0", ".", "=", "+"}
                            }; 
    for(int i = 0; i < 5; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            if(Buttons[i][j].isEmpty())
            {
                continue;
            }
            Layout->addWidget(CreateButton(Buttons[i][j]), i+2, j);
        }
    }
}

QPushButton* Calculator::CreateButton(const QString& name)
{
    QPushButton* pcmd = new QPushButton(name);
    pcmd->setMinimumSize(40,40);
    connect(pcmd, SIGNAL(clicked()), SLOT(slotButtonClicked()));
    if(name=="CE")
    {
        pcmd->setStyleSheet(QString::fromUtf8("background-color: rgb(178,34,34);"));
    }
    return pcmd;
}

void Calculator::setDisplay()
{
    display = new QLabel;
    display->setAlignment(Qt::AlignRight);

    QPalette pall;
    pall.setColor(display->backgroundRole(), Qt::white);
    display->setPalette(pall);
    display->setAutoFillBackground(true);

    QFont font = display->font();
    font.setPixelSize(15);
    display->setFont(font);

    display->setText("0");

    Layout->addWidget(display, 0, 0, 1, 4);
}

void Calculator::setInfo(const QString text_info)
{
    info = new QLabel(this);
    info->setFixedSize(230, 15);
    
    info->setAlignment(Qt::AlignRight);

    QPalette pall;
    pall.setColor(info->backgroundRole(), Qt::gray);
    info->setPalette(pall);
    info->setAutoFillBackground(true);

    QFont font = info->font();
    font.setPixelSize(10);
    info->setFont(font);
    info->setStyleSheet("background-color: rgb(245, 245, 245);"
                        "color: rgb(117, 117, 117);");

    info->setText(text_info);
    info->move(0, 257);
    
    info->show();
}

void Calculator::slotButtonClicked()
{
    QString str = ((QPushButton*)sender())->text();


    if((str == "CE") || (stringDisplay.length() >= 26))
    {
        try
        {
            while(current_exp->getPrevious_exp() != nullptr)
            {
                current_exp = current_exp->getPrevious_exp();
            }
            delete current_exp;
            current_exp = new InnerExpression();
            current_numb = nullptr;
            following_operand_is_negatie = false;
            stringDisplay = "";
            display->setText("0");
            return;
        }
        catch (...)
        {
            qDebug() << "Error in the module Calculator::slotButtonClicked(CE)";
            return;
        }
    }
    else if(str.contains(QRegExp("[0-9]")))
    {
        try
        {
            if(current_numb == nullptr)
            {
                create_number();
            }

            if(current_numb->first_numeral_is_null() && !current_numb->contains_point())
            {
                throw "After a number starting from zero, try to enter a number";
            }
        }
        catch (const char* error)
        {
            qDebug() << error;
            return;
        }

        current_numb->puch_numeral(str);
        stringDisplay += str;
        display->setText(stringDisplay);
        return;
    }
    else if(str == ".")
    {
        try
        {
            if(current_numb == nullptr)
            {
                throw "Attempt to put a point to the absence of a number";
            }
            else if(current_numb->contains_point())
            {
                throw "Attempt to put a point in the presence of a point";
            }
        }
        catch (const char* error)
        {
           qDebug() << error;
           return;
        }

        current_numb->puch_numeral(str);
        stringDisplay += str;
        display->setText(stringDisplay);
        return;
    }
    else if(str == "+")
    {
        try
        {
            Plus* new_plus;
            create_operator(new_plus);
        }
        catch (const char* error)
        {
            qDebug() << error;
            return;
        }
       stringDisplay += str;
       display->setText(stringDisplay);
       return;
    }
    else if(str == "-")
    {
        try
        {
            if(current_exp->Operand_is_empty())
            {
                if(!current_exp->Operators_is_empty())
                {
                    throw "After the operator, the operator is entered";
                }

            }
            else
            {
                Plus* new_plus;
                create_operator(new_plus);
            }
            following_operand_is_negatie = true;
            stringDisplay += str;
            display->setText(stringDisplay);
            return;
        }
        catch (const char* error)
        {
            qDebug() << error;
            return;
        }
    }
    else if(str == "*")
    {
        try
        {
            Multiplication* new_multiplication;
            create_operator(new_multiplication);
        }
        catch (const char* error)
        {
            qDebug() << error;
            return;
        }
       stringDisplay += str;
       display->setText(stringDisplay);
       return;
    }
    else if(str == "/")
    {
        try
        {
            Division* new_division;
            create_operator(new_division);
        }
        catch (const char* error)
        {
            qDebug() << error;
            return;
        }
       stringDisplay += str;
       display->setText(stringDisplay);
       return;
    }
    else if(str == "(")
    {
        try
        {
            create_expression();
        }
        catch (const char* error)
        {
            qDebug() << error;
            return;
        }
       stringDisplay += str;
       display->setText(stringDisplay);
       return;
    }
    else if(str == ")")
    {
        try
        {
            exit_the_current_expression();
        }
        catch (const char* error)
        {
            qDebug() << error;
            return;
        }
       stringDisplay += str;
       display->setText(stringDisplay);
       return;
    }
    else
    {
        try
        {
            result();
        }
        catch (const char* error)
        {
            qDebug() << error;
            return;
        }
        delete current_exp;
        current_exp = new InnerExpression();
        current_numb = nullptr;
        following_operand_is_negatie = false;
        stringDisplay = "";
        return;
    }
}

void Calculator::create_number()
{

    Number* new_numb = new Number(type_following_expression_is_negative());

    if(current_exp->Operators_is_empty() && current_exp->Operand_is_empty())
    {
        current_exp->set_operand(new_numb);
        current_numb = new_numb;
    }
    else if(!current_exp->Operators_is_empty() && current_exp->Operand_is_empty())
    {
        current_exp->set_Right_to_the_last_operator(new_numb);
        current_exp->set_operand(new_numb);
        current_numb = new_numb;
    }
    else if(!current_exp->Operand_is_empty())
    {
        Multiplication* new_multiplication;
        create_operator(new_multiplication);
        current_exp->set_Right_to_the_last_operator(new_numb);
        current_exp->set_operand(new_numb);
        current_numb = new_numb;
    }
    else
    {
        throw "Error in the module Calculator::create_number()";
    }
}

void Calculator::create_expression()
{
    if(current_numb != nullptr)
    {
        if(current_numb->last_character_is_point())
        {
            throw "Error in the module Calculator::create_expression():Attempt to put a parenthesis after the point";
        }
    }

    InnerExpression* new_exp = new InnerExpression(current_exp, type_following_expression_is_negative());

    if(current_exp->Operators_is_empty() && current_exp->Operand_is_empty())
    {
        current_exp->set_operand(new_exp);
        current_exp = new_exp;
    }
    else if(!current_exp->Operators_is_empty() && current_exp->Operand_is_empty())
    {
        current_exp->set_Right_to_the_last_operator(new_exp);
        current_exp->set_operand(new_exp);
        current_exp = new_exp;
    }
    else if(!current_exp->Operand_is_empty())
    {
        Multiplication* new_multiplication;
        create_operator(new_multiplication);
        current_exp->set_Right_to_the_last_operator(new_exp);
        current_exp->set_operand(new_exp);
        current_exp = new_exp;
    }
    else
    {
        throw "Error in the module Calculator::create_expression()";
    }
}

void Calculator::exit_the_current_expression()
{
    if(current_exp->getPrevious_exp() == nullptr)
    {
        throw "Error in the module Calculator::exit_the_current_expression(): Attempt to exit the last expression";
    }
    else if(current_numb != nullptr)
    {
        if(current_numb->last_character_is_point())
        {
            throw "Error in the module Calculator::exit_the_current_expression():Attempt to put a parenthesis after the point";
        }
    }

    current_exp = current_exp->getPrevious_exp();
    current_numb = nullptr;
}

bool Calculator::type_following_expression_is_negative()
{
    if(following_operand_is_negatie)
    {
        following_operand_is_negatie = false;
        return true;
    }
    else
    {
        return false;
    }

}

template<class T>
void Calculator::create_operator(T* new_operator)
{
    if(current_exp->Operand_is_empty())
    {
        throw "Error in the module Calculator::create_operator():there is no left-standing value";
    }
    new_operator = new T();
    current_exp->push_operator(new_operator);
    current_numb = nullptr;
}

void Calculator::result()
{
    if(current_exp->getPrevious_exp() != nullptr)
    {
        throw "Error in the module Calculator::result(): Attempt to get the result without closing all parentheses";
    }
    else if(current_numb != nullptr)
    {
        if(current_numb->last_character_is_point())
        {
            throw "Error in the module Calculator::result():Attempt to get the result after the point";
        }
    }

    display->setNum(current_exp->calculate());
}
