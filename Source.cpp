#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

// Struct to store test result information
struct TestResult {
    string testName;
    double score;
    int totalQuestions;
};

// Struct to store user information
struct User {
    string name;
    string address;
    string phone_number;
    string login;
    string password;
    vector<TestResult> testResults;
};

// Struct to store test information
struct Test {
    string name;
    vector<string> questions;
    vector<string> correct_answers;
    vector<string> incorrect_answers;
};

// Function to encrypt a string
string encrypt(string text) {
    string encrypted_text = text;
    int shift = 3;

    for (char& c : encrypted_text) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            c = static_cast<char>((c - base + shift) % 26 + base);
        }
    }
    return encrypted_text;
}

// Function to decrypt a string
string decrypt(string text) {
    string decrypted_text = text;
    int shift = 3;

    for (char& c : decrypted_text) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            c = static_cast<char>((c - base - shift + 26) % 26 + base);
        }
    }

    return decrypted_text;
}

// Function to load users from a file
vector<User> load_users(string filename) {
    vector<User> users;
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: file " << filename << " not found" << endl;
        return users;
    }
    User user;
    while (file >> user.name >> user.address >> user.phone_number >> user.login >> user.password) {
        user.login = decrypt(user.login);
        user.password = decrypt(user.password);
        users.push_back(user);
    }
    file.close();
    return users;
}

// Function to save users to a file
void save_users(const vector<User>& users, string filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: file " << filename << " not found\n";
        return;
    }
    for (const User& user : users)
        file << user.name << " " << user.address << " " << user.phone_number << " " << encrypt(user.login) << " " << encrypt(user.password) << "\n";
    file.close();
}

// Function to load tests from a file
vector<Test> load_tests(string filename) {
    vector<Test> tests;
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: file " << filename << " not found\n";
        return tests;
    }
    Test test;
    while (file >> test.name) {
        test.questions.clear();
        test.correct_answers.clear();
        test.incorrect_answers.clear();

        int num_questions;
        file >> num_questions;

        for (int i = 0; i < num_questions; i++) {
            string question, correct_answer, incorrect_answer1, incorrect_answer2;
            file >> question >> correct_answer >> incorrect_answer1 >> incorrect_answer2;

            test.questions.push_back(question);
            test.correct_answers.push_back(correct_answer);
            test.incorrect_answers.push_back(incorrect_answer1);
            test.incorrect_answers.push_back(incorrect_answer2);
        }

        tests.push_back(test);
    }
    file.close();
    return tests;
}

// Function to save tests to a file
void save_tests(const vector<Test>& tests, string filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: file " << filename << " not found\n";
        return;
    }
    for (const Test& test : tests) {
        file << test.name << "\n";
        file << test.questions.size() << "\n";
        for (int i = 0; i < test.questions.size(); i++)
            file << test.questions[i] << " " << test.correct_answers[i] << " " << test.incorrect_answers[2 * i] << " " << test.incorrect_answers[2 * i + 1] << "\n";
    }
    file.close();
}

// Function to save tests' results to a file
void save_test_results(const vector<User>& users, string filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: file " << filename << " not found\n";
        return;
    }

    for (const User& user : users) {
        file << "User: " << user.login << "\n";
        for (const TestResult& result : user.testResults)
            file << result.testName << " " << result.score << " " << result.totalQuestions << "\n";
        file << "\n";
    }

    file.close();
}

// Function to load tests' results from a file
void load_test_results(vector<User>& users, string filename) {

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: file " << filename << " not found\n";
        return;
    }

    User* currentUser = nullptr;
    string line;

    while (getline(file, line)) {
        if (line.find("User: ") == 0) {
            string login = line.substr(6);
            for (User& user : users) {
                if (user.login == login) {
                    currentUser = &user;
                    break;
                }
            }
        }
        else if (!line.empty() && currentUser != nullptr) {
            stringstream ss(line);
            string testName;
            int score, totalQuestions;
            ss >> testName >> score >> totalQuestions;

            TestResult result;
            result.testName = testName;
            result.score = score;
            result.totalQuestions = totalQuestions;

            currentUser->testResults.push_back(result);
        }
    }

    file.close();
}

// Function to add predefined tests
void add_tests(vector<Test>& tests) {
    Test psychology_test;
    psychology_test.name = "Psychology Test";

    psychology_test.questions.push_back("What is the definition of psychology?");
    psychology_test.correct_answers.push_back("The scientific study of the mind and behavior");
    psychology_test.incorrect_answers.push_back("The study of physical health");
    psychology_test.incorrect_answers.push_back("The study of plants and animals");

    psychology_test.questions.push_back("Who is considered the founder of psychoanalysis?");
    psychology_test.correct_answers.push_back("Sigmund Freud");
    psychology_test.incorrect_answers.push_back("Carl Jung");
    psychology_test.incorrect_answers.push_back("Ivan Pavlov");

    tests.push_back(psychology_test);

    Test math_test;
    math_test.name = "Math Test";

    math_test.questions.push_back("What is the value of pi (π)?");
    math_test.correct_answers.push_back("3.14159");
    math_test.incorrect_answers.push_back("2.71828");
    math_test.incorrect_answers.push_back("1.61803");

    math_test.questions.push_back("What is the Pythagorean theorem?");
    math_test.correct_answers.push_back("a^2 + b^2 = c^2");
    math_test.incorrect_answers.push_back("E = mc^2");
    math_test.incorrect_answers.push_back("F = ma");

    tests.push_back(math_test);

    Test quantum_physics_test;
    quantum_physics_test.name = "Quantum Physics Test";

    quantum_physics_test.questions.push_back("What is a quantum?");
    quantum_physics_test.correct_answers.push_back("The smallest indivisible unit of a physical quantity");
    quantum_physics_test.incorrect_answers.push_back("A subatomic particle");
    quantum_physics_test.incorrect_answers.push_back("A unit of energy");

    quantum_physics_test.questions.push_back("What is the uncertainty principle?");
    quantum_physics_test.correct_answers.push_back("The principle stating that the position and momentum of a particle cannot both be precisely determined");
    quantum_physics_test.incorrect_answers.push_back("The principle stating that energy is quantized");
    quantum_physics_test.incorrect_answers.push_back("The principle stating that particles can be in multiple states simultaneously");

    tests.push_back(quantum_physics_test);
}

int main() {
    vector<User> users;
    vector<Test> tests;
    users = load_users("users.txt");
    //tests = load_tests("tests.txt");
    add_tests(tests);
    load_test_results(users, "test_results.txt");

    while (true) {
        cout << "------------------------------\n";
        cout << "1. Register\n"
            << "2. Login\n"
            << "3. Exit\n"
            << "Your choice: ";

        int choice;
        cin >> choice;

        cout << "------------------------------\n";
        string login, password;
        User* currentUser = nullptr;
        User new_user;

        switch (choice) {
        case 1:
            // Register a new user
            cout << "Enter your name: ";
            cin.ignore();
            getline(cin, new_user.name);
            cout << "Enter your address: ";
            getline(cin, new_user.address);
            cout << "Enter your phone number: ";
            cin >> new_user.phone_number;
            cout << "Enter your login: ";
            cin >> new_user.login;
            cout << "Enter your password: ";
            cin >> new_user.password;
            users.push_back(new_user);
            save_users(users, "users.txt");
            break;
        case 2:
            // Login an existing user
            cout << "Enter your login: ";
            cin >> login;
            cout << "Enter your password: ";
            cin >> password;

            // Find user in the list
            for (User& user : users) {
                if (user.login == login && user.password == password) {
                    currentUser = &user;
                    break;
                }
            }

            // If user is found
            if (currentUser != nullptr) {
                while (true) {
                    cout << "------------------------------\n";
                    cout << "1. View test results\n"
                        << "2. Take a test\n"
                        << "3. Logout\n"
                        << "Your choice: ";
                    int choice;
                    cin >> choice;
                    cout << "------------------------------\n";

                    switch (choice) {
                    case 1:
                        // View test results
                        cout << "Test Results:\n";
                        for (const TestResult& result : currentUser->testResults) {
                            cout << "Test: " << result.testName << "\n";
                            cout << "Score: " << result.score << " out of " << result.totalQuestions << "\n";
                            cout << "Percentage: " << (static_cast<double>(result.score) / result.totalQuestions) * 100 << "%\n";
                            //cout << "------------------------------\n";
                        }
                        break;
                    case 2:
                        // Take a test
                        for (int i = 0; i < tests.size(); i++)
                            cout << i + 1 << ". " << tests[i].name << "\n";
                        cout << "Your choice: ";

                        int test_choice;
                        cin >> test_choice;

                        if (test_choice >= 1 && test_choice <= tests.size()) {
                            Test& selected_test = tests[test_choice - 1];
                            int score = 0;
                            cout << "------------------------------\n";
                            cout << "Taking " << selected_test.name << ":\n";
                            for (int i = 0; i < selected_test.questions.size(); i++) {
                                cout << "Question " << i + 1 << ": " << selected_test.questions[i] << "\n";
                                cout << "Options:\n";
                                cout << "a. " << selected_test.correct_answers[i] << "\n";
                                cout << "b. " << selected_test.incorrect_answers[i * 2] << "\n";
                                cout << "c. " << selected_test.incorrect_answers[i * 2 + 1] << "\n";

                                cout << "Your answer (a, b, c): ";
                                char user_answer;
                                cin >> user_answer;
                                cout << "------------------------------\n";

                                if (user_answer == 'a') {
                                    cout << "Correct!\n" << "------------------------------\n";
                                    score++;
                                }
                                else cout << "Incorrect!\n" << "------------------------------\n";
                            }

                            cout << "Test completed!\n";
                            cout << "Your score: " << score << " out of " << selected_test.questions.size() << "\n";
                            save_test_results(users, "test_results.txt");

                            TestResult result;
                            result.testName = selected_test.name;
                            result.score = score;
                            result.totalQuestions = selected_test.questions.size();
                            currentUser->testResults.push_back(result);
                            save_test_results(users, "test_results.txt");
                            save_users(users, "users.txt");
                        }
                        else cout << "Invalid test choice\n";
                        break;
                    case 3:
                        // Logout
                        currentUser = nullptr;
                        break;

                    default: cout << "Invalid choice\n"; break;
                    }
                    if (currentUser == nullptr) break;
                }
            }
            else cout << "Invalid login or password\n";
            break;
        case 3:
            // Exit the program
            save_tests(tests, "tests.txt");
            cout << "Goodbye!\n";
            return 0;
        default:
            cout << "Invalid choice\n";
            break;
        }
    }
    save_test_results(users, "test_results.txt");
    save_users(users, "users.txt");
    save_tests(tests, "tests.txt");
    return 0;
}
