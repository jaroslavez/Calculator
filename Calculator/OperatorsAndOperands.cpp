#include "Calculator.h"

/*--------------interface of operators and operands--------------*/
Part_of_expression::~Part_of_expression()
{}
/*--------------interface of Operator--------------*/
Operator::Operator(Priority priority_of_this_operator)
{
    priority = priority_of_this_operator;
}

void Operator::setLeft(Part_of_expression* left_branch)
{
    Left = left_branch;
}

void Operator::setRight(Part_of_expression* right_branch)
{
    Right = right_branch;
}

Priority Operator::getPriority() const
{
    return priority;
}

Operator::~Operator()
{
    delete Left;
    delete Right;
}

/*--------------interface of Operand--------------*/
Operand::Operand(bool this_is_neg)
{
    this_is_negative = this_is_neg;
}

/*--------------Inner Expression--------------*/
InnerExpression::InnerExpression(InnerExpression* previous_expression, bool isNegative): Operand(isNegative)
{
    previous_exp = previous_expression;
}

void InnerExpression::push_operator(Operator* new_operator)
{
    if(Operators_is_empty())
    {
        set_first_operator(new_operator);
    }
    else
    {
        if(starts_operator->getPriority() >= new_operator->getPriority())
        {
            set_operator_of_same_or_less_priority_when_there_are_already_others(new_operator);
        }
        else
        {
            set_a_more_priority_operator_when_there_are_others_already(new_operator);
        }
    }
}

void InnerExpression::set_first_operator(Operator* new_operator)
{
    new_operator->setLeft(operand);
    operand = nullptr;
    operators.push_back(new_operator);
    starts_operator = new_operator;
}

void InnerExpression::set_operator_of_same_or_less_priority_when_there_are_already_others(Operator *new_operator)
{
    new_operator->setLeft(starts_operator);
    operand = nullptr;
    operators.push_back(new_operator);
    starts_operator = new_operator;
}

void InnerExpression::set_a_more_priority_operator_when_there_are_others_already(Operator *new_operator)
{
    if(operand == nullptr)
    {
            delete new_operator;
            throw "there is no left-standing value";
    }
    starts_operator->setRight(new_operator);
    new_operator->setLeft(operand);
    operand = nullptr;
    operators.push_back(new_operator);
}

void InnerExpression::set_operand(Operand *new_operand)
{
    operand = new_operand;
}

void InnerExpression::set_Right_to_the_last_operator(Operand* new_operand)
{
    operators.last()->setRight(new_operand);
}

float InnerExpression::calculate()
{
    if((!this_is_negative) && (operators.isEmpty()) && (operand != nullptr))
    {
        return operand->calculate();
    }
    else if((this_is_negative) && (operators.isEmpty()) && (operand != nullptr))
    {
        return (0.0 - operand->calculate());
    }
    else if((this_is_negative) && (!operators.isEmpty()))
    {
        return (0.0 - starts_operator->calculate());
    }
    else if((!this_is_negative) && (!operators.isEmpty()))
    {
        return starts_operator->calculate();
    }
    else
    {
        throw "Error in module InnerExpression::calculate()";
    }
}

InnerExpression* InnerExpression::getPrevious_exp() const
{
    return previous_exp;
}

bool InnerExpression::Operators_is_empty() const
{
    if(operators.isEmpty())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool InnerExpression::Operand_is_empty() const
{
    if(operand == nullptr)
    {
        return true;
    }
    else
    {
        return false;
    }
}

InnerExpression::~InnerExpression()
{
    if((starts_operator == nullptr) && (operand != nullptr))
    {
        delete operand;
    }
    else if(starts_operator != nullptr)
    {
        delete starts_operator;
    }
}

/*--------------Number--------------*/

Number::Number(bool this_is_neg): Operand(this_is_neg)
{}

float Number::calculate()
{
    if(this_is_negative)
    {
        return (0.0 - value.toFloat());
    }
    else
    {
        return value.toFloat();
    }
}

void Number::puch_numeral(QString numeral)
{
    value += numeral;
}

bool Number::first_numeral_is_null() const
{
    if(value.contains(QRegExp("^0")))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Number::last_character_is_point() const
{
    if(value.contains(QRegExp("(\\.)$")))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Number::contains_point() const
{
    if(value.contains(QRegExp("\\.")))
    {
        return true;
    }
    else
    {
        return false;
    }
}

Number::~Number()
{}

/*--------------Pluse--------------*/
Plus::Plus(): Operator(Priority::PLUS)
{}

float Plus::calculate()
{
    return (Plus::Left->calculate() + Plus::Right->calculate());
}

Plus::~Plus()
{}

/*--------------Multiplication--------------*/
Multiplication::Multiplication(): Operator(Priority::MULT)
{}

float Multiplication::calculate()
{
    return (Multiplication::Left->calculate() * Multiplication::Right->calculate());
}

Multiplication::~Multiplication()
{}
/*--------------Division--------------*/
Division::Division(): Operator(Priority::DIV)
{}

float Division::calculate()
{
    return (Division::Left->calculate() / Division::Right->calculate());
}

Division::~Division()
{}
