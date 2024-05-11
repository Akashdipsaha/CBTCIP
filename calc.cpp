#include <iostream>
#include <limits>
#include <string>

void printMenu();
bool isValidNumber(const std::string& str);
template <typename T>
T getNumber(const std::string& prompt);
void performOperation(char operation);

int main() {
    char choice;

    do {
        printMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice != 'Q' && choice != 'q') {
            performOperation(choice);
        }
    } while (choice != 'Q' && choice != 'q');

    std::cout << "Thank you for using Digital Calculator. Goodbye!\n";
    return 0;
}

void printMenu() {
    std::cout << "================ Digital Calculator ================\n";
    std::cout << "[A] Add\n";
    std::cout << "[S] Subtract\n";
    std::cout << "[M] Multiply\n";
    std::cout << "[D] Divide\n";
    std::cout << "[Q] Quit\n";
    std::cout << "Select an operation: ";
}

bool isValidNumber(const std::string& str) {
    return str.find_first_not_of("0123456789.-") == std::string::npos;
}

template <typename T>
T getNumber(const std::string& prompt) {
    std::string input;
    T value;

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        if (isValidNumber(input)) {
            try {
                value = std::stod(input);
                break;
            } catch (const std::exception&) {
                std::cout << "Invalid input. Please enter a numeric value.\n";
            }
        } else {
            std::cout << "Invalid input. Please enter a numeric value.\n";
        }
    }
    return value;
}

void performOperation(char operation) {
    double num1, num2, result;

    switch (operation) {
        case 'A':
        case 'a':
            num1 = getNumber<double>("Enter the first number: ");
            num2 = getNumber<double>("Enter the second number: ");
            result = num1 + num2;
            std::cout << "Result: " << num1 << " + " << num2 << " = " << result << std::endl;
            break;
        case 'S':
        case 's':
            num1 = getNumber<double>("Enter the first number: ");
            num2 = getNumber<double>("Enter the second number: ");
            result = num1 - num2;
            std::cout << "Result: " << num1 << " - " << num2 << " = " << result << std::endl;
            break;
        case 'M':
        case 'm':
            num1 = getNumber<double>("Enter the first number: ");
            num2 = getNumber<double>("Enter the second number: ");
            result = num1 * num2;
            std::cout << "Result: " << num1 << " * " << num2 << " = " << result << std::endl;
            break;
        case 'D':
        case 'd':
            num1 = getNumber<double>("Enter the first number: ");
            num2 = getNumber<double>("Enter the second number: ");
            if (num2 != 0) {
                result = num1 / num2;
                std::cout << "Result: " << num1 << " / " << num2 << " = " << result << std::endl;
            } else {
                std::cout << "Division by zero error!\n";
            }
            break;
        default:
            std::cout << "Invalid choice. Please select a valid operation.\n";
            break;
    }
}
