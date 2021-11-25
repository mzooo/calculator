#include <iostream>
#include <stack>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <cctype>

struct oper {
	char op;		//������
	size_t pre;		//�������� �켱����
};

//Model
class Calculator {
public:
	size_t precedence(const char op) {	//������ �켱 ���� �Լ�
		if (op == '+' || op == '-')
			return 1;
		if (op == '*' || op == '/')
			return 2;
		throw std::runtime_error{ std::string("������ �Ұ����� �����ڰ� ���ԵǾ� ���� �߻�") };
	}
	double execute(std::stack<oper>& ope, std::stack<double>& operand) {	//���װ� ���� ���� ����
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
			throw std::runtime_error{ std::string("������ �Ұ����� �����ڰ� ���ԵǾ� ���� �߻�") };
		}
		ope.pop();
		operand.push(result);
		return result;
	}	
	std::string hTD(int length, std::string num){	//16������ 10������ ��ȯ
		std::string dec;
		for (int i = 0; i < length; i++) {
			dec += num[i] * pow(16, length - i - 1);
		}
		return dec;
	}
	std::string bTD(int length, std::string num) {	//2������ 10������ ��ȯ
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
		std::cout << "���� �Է�(���� �Ǵ� �߸��� �� �Է� �� ���� ����) : " << std::endl;
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
					throw std::runtime_error{ std::string("��ȣ�� ������ �ùٸ��� �ʾ� ���� �߻�") };

				size_t index{}, i{};	//index : ���ڿ��� �ε���, i : �κй��ڿ��� �ε���
				for (; input[index] == '('; index++, bracketcount++);
				operands.push(stod(input.substr(index), &i));		//������
				index += i;

				while (1) {
					for (; input[index] == ')'; index++, bracketcount--);
					if (index != input.length())
						operators.push(oper{ input[index], model->precedence(input[index++]) + 3 * bracketcount });

					i = 0;
					for (; input[index] == '('; index++, bracketcount++);
					if (index != input.length())
						operands.push(stod(input.substr(index), &i));//��������
					index += i;

					if (index == input.length()) {
						while (operators.size())
							model->execute(operators, operands);
						break;
					}
					//������ �켱���� ��, ó���� ���� ���� ���
					while (operators.size()) {
						for (; input[index] == ')'; index++, bracketcount--);						
						if (index == input.length()) {
							break;
						}
						//���� ���� �������� ������ �� ���� ���, ���� ���� ����
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