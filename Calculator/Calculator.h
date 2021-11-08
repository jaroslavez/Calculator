#pragma once
#include <QtWidgets>
#include <QLabel>

enum Priority
{
    PLUS = 1,
    MULT = 2,
    DIV = 2,
};

/*--------------interface of operators and operands--------------*/
class Part_of_expression
{
public:
    virtual float calculate() = 0;
    virtual ~Part_of_expression();
};

////////////////////////////////////////////////////////////////////////////////////////

/*--------------interface of operators--------------*/
class Operator: public Part_of_expression
{
protected:
    Part_of_expression* Left;
    Part_of_expression* Right;
    Priority priority;
public:
    Operator(Priority priority_of_this_operator);
    void setLeft (Part_of_expression* left_branch);
    void setRight (Part_of_expression* right_branch);
    Priority getPriority() const;
    ~Operator();
};

////////////////////////////////////////////////////////////////////////////////////////

class Plus: public Operator
{
public:
    Plus();
    float calculate() override;
    ~Plus();
};

////////////////////////////////////////////////////////////////////////////////////////

class Multiplication: public Operator
{
public:
    Multiplication();
    float calculate() override;
    ~Multiplication();
};

////////////////////////////////////////////////////////////////////////////////////////

class Division: public Operator
{
public:
    Division();
    float calculate() override;
    ~Division();
};

////////////////////////////////////////////////////////////////////////////////////////

/*--------------interface of operand--------------*/
class Operand: public Part_of_expression
{
protected:
    bool this_is_negative;
public:
    Operand(bool this_is_neg);
};

////////////////////////////////////////////////////////////////////////////////////////

class Number: public Operand
{
    QString value;
public:
    Number(bool this_is_neg = false);
    float calculate() override;
    void puch_numeral(QString numeral);
    bool first_numeral_is_null() const;
    bool last_character_is_point() const;
    bool contains_point() const;
    ~Number();
};

////////////////////////////////////////////////////////////////////////////////////////

class InnerExpression: public Operand
{
    QList<Operator*> operators; //All operators and expressions of this expression are stored here.
    Operand* operand = nullptr;

    InnerExpression* previous_exp = nullptr; //This is a temporary variable for storing a previous expression.

    Operator* starts_operator = nullptr;

public:
    InnerExpression(InnerExpression* previous_expression = nullptr, bool isNegative = false);

    float calculate() override;

    void push_operator(Operator* new_operator);
    void set_first_operator(Operator* new_operator);
    void set_a_more_priority_operator_when_there_are_others_already(Operator* new_operator);
    void set_operator_of_same_or_less_priority_when_there_are_already_others(Operator* new_operator);
    void set_operand(Operand* new_operand);
    void set_Right_to_the_last_operator(Operand* new_operand);

    InnerExpression* getPrevious_exp() const;
    bool Operators_is_empty() const;
    bool Operand_is_empty() const;
    ~InnerExpression();
};

/*--------------Body of the calculator widget--------------*/
class Calculator : public QWidget
{
    Q_OBJECT
private:
    QGridLayout* Layout;
    QLabel* display; //This is a widget for output and input expression.
    QLabel* info;
    
    QString stringDisplay; //The whole expression is stored here as a string.
    
    InnerExpression* current_exp = nullptr;
    Number* current_numb = nullptr;

    bool following_operand_is_negatie = false;

public:
    Calculator (QWidget* pwg = 0);
    
    void setButton();
    QPushButton* CreateButton (const QString& name);

    void setDisplay();
    void setInfo(const QString text_info);
    //////////////////////////////////////
    void create_number();
    void create_expression();

    template<class T>
    void create_operator(T* new_operator);
    //////////////////////////////////////
    bool type_following_expression_is_negative();
    //////////////////////////////////////
    void exit_the_current_expression();
    //////////////////////////////////////
    void result();
public slots:
    void slotButtonClicked();
};
