//
// Created by vojta on 26.3.19.
//

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/bind.hpp>
#include <string>
#include <iostream>

template<typename T>
struct Value {
    T val;
    Value() {
        val={};

    }

    Value(const Value &other) {
    }

    Value &operator=(const Value &other) {
        val=other.val;
        return *this;
    }

    Value &operator+=(const Value &rhs) {
        val+=rhs.val;
        return *this;
    }

    Value &operator-=(const Value &rhs) {
        val-=rhs.val;
        return *this;
    }

    Value &operator*=(const Value &rhs) {
        val*=rhs.val;
        return *this;
    }

    Value &operator/=(const Value &rhs) {
        val/=rhs.val;
        return *this;
    }

    Value &operator=(const double &d) {
        val=d;
        return *this;
    }

    //Value(double const& i, boost::spirit::qi::unused_type, boost::spirit::qi::unused_type)
    //{
    //}
//    Value(const char x, boost::spirit::qi::unused_type, boost::spirit::qi::unused_type) {
//        std::cout << "const char ctor\n";
//    }

    ~Value() {
    }

};

//std::ostream &operator<<(std::ostream &os, const Value &obj) {
//    return os;
//}

    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;


    ///////////////////////////////////////////////////////////////////////////////
    //  Our calculator grammar
    ///////////////////////////////////////////////////////////////////////////////
    template<typename Iterator, typename value_type >
    struct calculator : qi::grammar<Iterator, ascii::space_type, value_type> {

        calculator() : calculator::base_type(expression) {
            using qi::lit;
            qi::double_type double_;
            qi::_val_type _val;
            qi::_1_type _1;


            expression = additive_expression[_val =  _1];

            additive_expression =
                    multiplicative_expression[_val = _1] >> *(("+" >> multiplicative_expression)[_val += _1] |
                                                              ("-" >> multiplicative_expression)[_val -= _1]);

            multiplicative_expression =
                    primary_expression[_val = _1]
                            >> *(("*" >> primary_expression[_val *= _1]) | ("/" >> primary_expression)[_val /= _1]);

            primary_expression =
                    double_[_val = _1]
                    //                    | nameOfFunction >> lit('(') >> parameter_list >> lit(')')
                    | '(' >> expression[_val = _1] >> ')'
                // unary minus
                //| ('-' >> primary_expression)
                // unary plus
                //| ('+' >> primary_expression)
                    ;

//            parameter_list =
//                    expression >> *(lit(',') >> expression)
//                    | boost::spirit::eps;
//            nameOfFunction = (qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9"));



        }

        qi::rule<Iterator, ascii::space_type, value_type> expression, additive_expression, multiplicative_expression, primary_expression, parameter_list;

        qi::rule<Iterator, ascii::space_type, std::string()> nameOfFunction;
    };


///////////////////////////////////////////////////////////////////////////////
//  Main program
///////////////////////////////////////////////////////////////////////////////
int
main() {
    std::cout << "Arithmetic expression parser...\n\n";
    std::cout << "Type an expression...or [q or Q] to quit\n\n";

    typedef std::string::const_iterator iterator_type;
    typedef calculator<iterator_type,Value<double> > calculator;

    boost::spirit::ascii::space_type space; // Our skipper
    calculator calc; // Our grammar

    std::string str;

    while (std::getline(std::cin, str)) {
        if (str.empty() || str[0] == 'q' || str[0] == 'Q')
            break;

        std::string::const_iterator iter = str.begin();
        std::string::const_iterator end = str.end();
        Value<double> result;
        bool r = boost::spirit::qi::phrase_parse(iter, end, calc, space, result);

        if (r && iter == end) {
            std::cout << "-------------------------\n";
            std::cout << "Parsing succeeded\n";
            std::cout << "RESULT: " << result.val << '\n';
            std::cout << "-------------------------\n";
        } else {
            std::string rest(iter, end);
            std::cout << "-------------------------\n";
            std::cout << "Parsing failed\n";
            std::cout << "stopped at: \" " << rest << "\"\n";
            std::cout << "-------------------------\n";
        }
    }


    std::cout << "Bye... :-) \n\n";
    return 0;
}