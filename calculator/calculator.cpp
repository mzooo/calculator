#include <iostream>
#include <stack>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <cctype>

struct oper {
	char op;		//연산자
	size_t pre;		//연산자의 우선순위
};

//Model
class Calculator {
public:
	size_t precedence(const char op) {	//연산자 우선 순위 함수
		if (op == '+' || op == '-')
			return 1;
		if (op == '*' || op == '/')
			return 2;
		throw std::runtime_error{ std::string("연산이 불가능한 연산자가 포함되어 에러 발생") };
	}
	double execute(std::stack<oper>& ope, std::stack<double>& operand) {	//좌항과 우항 연산 수행
		double result{};
		double right{ operand.top() };
		operand.pop();
		double left{ operand.top() };
		operand.pop();

		switch (ope.top().op) {
		case '+':
			result = left + right;
			break;
		case '-':
			result = left - right;
			break;
		case '*':
			result = left * right;
			break;
		case '/':
			result = left / right;
			break;
		default:
			throw std::runtime_error{ std::string("연산이 불가능한 연산자가 포함되어 에러 발생") };
		}
		ope.pop();
		operand.push(result);
		return result;
	}	
	std::string hTD(int length, std::string num){	//16진수를 10진수로 변환
		std::string dec;
		for (int i = 0; i < length; i++) {
			dec += num[i] * pow(16, length - i - 1);
		}
		return dec;
	}
	std::string bTD(int length, std::string num) {	//2진수를 10진수로 변환
		std::string dec;
		for (int i = 0; i < length; i++) {
			dec += num[i] * pow(2, length - i - 1);
		}
		return dec;
	}
};

//View
class CalculatorView {
public:
	std::string input(){
		std::string in;
		std::cout << "계산식 입력(빈줄 또는 잘못된 식 입력 시 계산기 종료) : " << std::endl;
		std::getline(std::cin, in);
		return in;
	}
};

//Controller
class CalculatorController {
private:
	Calculator* model;
	CalculatorView* view;
public:
	CalculatorController(Calculator* model, CalculatorView* view) {
		this->model = model;
		this->view = view;
	}
	void calculate() {
		try {
			while (1) {
				std::stack<oper> operators;
				std::stack<double> operands;
				std::string input;
				int bracketcount{};

				input = view->input();
				if (input.empty()) break;

				input.erase(std::remove_if(input.begin(), input.end(), isspace), input.end());

				for (auto c : input) {
					if (c == '(')
						bracketcount++;
					else if (c == ')')
						bracketcount--;			
				}
	
				if (bracketcount != 0)
					throw std::runtime_error{ std::string("괄호로 개수가 올바르지 않아 에러 발생") };

				size_t index{}, i{};	//index : 문자열의 인덱스, i : 부분문자열의 인덱스
				for (; input[index] == '('; index++, bracketcount++);
				operands.push(stod(input.substr(index), &i));		//왼쪽항
				index += i;

				while (1) {
					for (; input[index] == ')'; index++, bracketcount--);
					if (index != input.length())
						operators.push(oper{ input[index], model->precedence(input[index++]) + 3 * bracketcount });

					i = 0;
					for (; input[index] == '('; index++, bracketcount++);
					if (index != input.length())
						operands.push(stod(input.substr(index), &i));//오른쪽항
					index += i;

					if (index == input.length()) {
						while (operators.size())
							model->execute(operators, operands);
						break;
					}
					//연산자 우선순위 비교, 처리할 연산 남은 경우
					while (operators.size()) {
						for (; input[index] == ')'; index++, bracketcount--);						
						if (index == input.length()) {
							break;
						}
						//현재 시점 연산자의 순위가 더 낮은 경우, 이전 연산 수행
						if (model->precedence(input[index]) + 3 * bracketcount <= operators.top().pre)
							model->execute(operators, operands);
						else
							break;
					}
				}
				std::cout << "result = " << operands.top() << std::endl;
			}
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}
};

int main() {
	Calculator* model = new Calculator();
	CalculatorView* view = new CalculatorView();
	CalculatorController* controller = new CalculatorController(model, view);

	controller->calculate();

	return 0;
}