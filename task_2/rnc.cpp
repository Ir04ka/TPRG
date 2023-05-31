#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <sstream>
#include <fstream>
#include <cmath>
#include <windows.h>
#include <ranges>
#include <algorithm>


using namespace std;

struct Parameter {
    int numbers_count;
    string output_file;

    Parameter() : 
        numbers_count(10000),
        output_file("result.txt") {
    }
};

vector<int> convert_file_to_ints(const string& path) {
    ifstream input(path);

    if (!input.is_open()) {
        cout << "ERROR" << endl;
        return {};
    }

    vector<int> res;

    string temp;
    getline(input, temp);
    stringstream splitter(temp);
    while (getline(splitter, temp, ',')) {
        res.push_back(stoi(temp));
    }

    input.close();

    return res;
}

string to_str(char* s) {
    string res(s);
    res = res.substr(3);

    return res;
}

string to_str_p(char* s) {
    string res(s);
    res = res.substr(4);

    return res;
}

vector<double> U(const vector<int>& arr) {

    auto max_it = max_element(std::begin(arr), std::end(arr));
    double max_value = *max_it; 

    max_value++;

    vector<double> res;
    
    for (auto elem : arr) {
        res.push_back(elem / max_value);
    }

    return res;
}

void st(const Parameter& parameters, const vector<int>& arr, int a, int b) {
    ofstream output(parameters.output_file, ios::out);

    auto max_it = max_element(std::begin(arr), std::end(arr));
    int max_value = *max_it;
    max_value++;

    vector<int> res;
    for (auto elem : arr) {
        res.push_back(b * elem / max_value + a);
        output << res.back() << ",";
    }

    output.close();
}

void tr(const Parameter& parameters, const vector<int>& v, int a, int b) {
    ofstream output(parameters.output_file, ios::out);

    vector<double> u = U(v);

    vector<int> res;
    for (int i = 0; i < size(v) - 1; i++) {
        double h = (static_cast<double>(a) + static_cast<double>(b) * (u[i] + u[i + 1] - 1));
        res.push_back(h);

        output << (int)res[i] << ",";
    }

    output.close();
}

void ex(const Parameter& parameters, const vector<int>& v, int a, int b) {
    ofstream output(parameters.output_file, ios::out);

    vector<double> u = U(v);
    
    vector<int> res;
    for (auto elem : u) {
        if (elem == 0) {
            elem += 0.001;
        }

        if (elem == 1) {
            elem -= 0.001;
        }

        double h = (static_cast<double>(a) - b * log(elem));

        res.push_back(h);
        
        output << res.back() << ",";
    }

    output.close();
}

void nr(const Parameter& parameters, const vector<int>& v, int a, int b) {
    ofstream output(parameters.output_file, ios::out);

    vector<double> u = U(v);

    vector<int> res;
    for (int i = 0; i < size(v); i += 2) {
        if (u[i] == 0) {
            u[i] += 0.001;
        }

        if (u[i] == 1) {
            u[i] -= 0.001;
        }

        double h1 = a + (double)b * sqrt(-2 * log(1 - u[i])) * cos(2 * M_PI * u[(i + 1) % size(v)]);
        res.push_back(h1);
        output << res[i] << ",";

        double h2 = (a + (double)b * sqrt(-2 * log(1 - u[i])) * sin(2 * M_PI * u[(i + 1) % size(v)]));
        res.push_back(h2);
        output << res[i + 1] << ",";
    }

    output.close();
}

void gm(const Parameter& parameters, const vector<int>& v, int a, int b) {
    ofstream output(parameters.output_file, ios::out);

    vector<double> u = U(v);

    vector<int> res;

    for (int i = 0; i < size(v); i++) {
        if (u[i] == 0) {
            u[i] += 0.001;
        }

        if (u[i] == 1) {
            u[i] -= 0.001;
        }

        double h = (static_cast<double>(a) - static_cast<double>(b) * log(1 - u[i]));
        res.push_back(h);
        output << res[i] << ",";
    }

    output.close();
}

vector<double> nr_for_ln(const vector<int>& v, int a, double b) {
    vector<double> u = U(v);

    vector<double> res;
    for (int i = 0; i < size(v); i += 2) {
        if (u[i] == 0) {
            u[i] += 0.001;
        }

        if (u[i] == 1) {
            u[i] -= 0.001;
        }

        double h1 = a + b * sqrt(-2 * log(1 - u[i])) * cos(2 * M_PI * u[(i + 1) % size(v)]);
        h1 += 0.5;
        res.push_back(h1);
        
        double h2 = (a + b * sqrt(-2 * log(1 - u[i])) * sin(2 * M_PI * u[(i + 1) % size(v)]));
        h2 += 0.5;
        res.push_back(h2);
    }

    return res;
}

void ln(const Parameter& parameters, const vector<int>& v, int a, int b) {
    ofstream output(parameters.output_file, ios::out);
    
    vector<double> res;
    if (b <= 42) {
        vector<double> u = nr_for_ln(v, 0, 0.1);
        for (auto elem : u) {
            if (elem == 0) {
                elem += 0.001;
            }

            if (elem == 1) {
                elem -= 0.001;
            }
            
            double h = (a + exp(b - elem));
            
            res.push_back(h);

            output << res.back() << ",";
        }
    }
    else {
        cout << "Error!" << endl;
    }

    output.close();
}

void ls(const Parameter& parameters, const vector<int>& v, int a, int b) {
    ofstream output(parameters.output_file, ios::out);

    vector<double> u = U(v);

    vector<int> res;
    for (auto elem : u) {
        if (elem == 0) {
            elem += 0.001;
        }

        if (elem == 1) {
            elem -= 0.001;
        }

        double h = a + (double)b * log(elem / (1 - elem));
        res.push_back(h);
        output << res.back() << ",";
    }

    output.close();
}

double fact(double x) {
    return (x == 0 || x == 1) ? 1 : x * fact(x - 1);
}


double Cnk(double n, double k) {
    return fact(n) / (fact(n - k) * fact(k));
}

void bi(const Parameter& parameters, const vector<int>& v, int n, double p) {
    ofstream output(parameters.output_file, ios::out);

    vector<int> res;
    vector<double> u = U(v);
    for (int i = 0; i < size(v); i++) {
        double temp_res = 0;
        for (int k = 0; k <= n; k++) {
            temp_res += Cnk(n, k) * powf(p, k) * powf(1 - p, n - k);

            if (temp_res >= u[i]) {
                res.push_back(k);
                break;
            }
        }

        output << res[i] << ",";
    }

    output.close();
}

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(CP_UTF8);

    string f = "rnd.dat";
    int programm_number = 0;
    vector<int> numbers;
    vector<string> string_numbers;
    int p1 = 0, p2 = 0;
    double p3 = 0;

    Parameter parameters;

    for (int i = 1; argv[i]; i++) {
        string str_arg = string(argv[i]);
        if (str_arg[1] == 'd') {

            string ggg = str_arg.substr(3, 2);

            if (ggg == "st") {
                programm_number = 1;
                numbers = { 31460, 48498, 57017, 28508, 47022, 56279, 60907, 30453, 15226, 7613, 3806, 34671, 17335, 8667, 4333, 34934, 17467 };
            }
            else if (ggg == "tr") {
                programm_number = 2;
                numbers = { 937, 1284, 445, 444, 1335, 1132, 881, 1636, 454, 887, 1075, 1044, 780, 1313, 1059 };
            }
            else if (ggg == "ex") {
                programm_number = 3;
                numbers = { 4253, 1093, 2254, 3297, 16 };
            }
            else if (ggg == "nr") {
                programm_number = 4;
                string_numbers = { "123", "101101101101011" };
            }
            else if (ggg == "gm") {
                programm_number = 5;
                string_numbers = { "101101101101011", "101101101101011", "101101101101011" };

            }
            else if (ggg == "ln") {
                programm_number = 6;
                numbers = { 10001, 8191 };
            }
            else if (ggg == "ls") {
                programm_number = 7;
                numbers = { 802, 720, 341, 337, 961, 882, 417, 785, 198, 727, 899, 372, 374, 425, 556, 615, 813, 768, 840, 183, 893, 568, 73, 387, 18, 436, 182, 125, 806, 899, 485, 607, 619, 825, 944, 579, 707, 360, 363, 904, 87, 262, 276, 460, 687, 831, 75, 499, 599, 915, 681, 492, 483, 754, 878, 500, 189, 60, 624, 994, 959, 109, 600, 577, 934, 544, 156, 640, 903, 519, 544, 990, 781, 819, 449, 468, 650, 524, 967, 248, 438, 647, 739, 920, 400, 617, 419, 588, 676, 43, 581, 634, 151, 181, 211, 84, 724, 367, 723, 627, 886, 267, 617, 667, 85, 65, 134, 735, 589, 100, 983, 26, 747, 721, 945, 147, 337, 364, 734, 13, 406, 315, 647, 556, 496, 858, 640, 220, 224, 362, 847, 110, 629, 463, 776, 713, 528, 909, 448, 116, 9,430, 141, 755, 151, 86, 901, 488, 449, 635, 500, 855, 950, 147, 410, 446, 4,49, 665, 227, 411, 511, 336, 39, 974, 112, 752, 501, 21, 200, 617, 29, 629, 757, 784, 779, 843, 684, 266, 292, 319, 766, 146, 269, 912, 556, 714, 916, 605, 378, 142, 15, 889, 478, 54, 862, 590, 806, 363, 610, 5,979, 638, 634, 736, 421, 413, 578, 105, 679, 869, 424, 444, 14, 692, 356, 569, 405, 271, 173, 783, 413, 188, 671, 891, 242, 533, 480, 48, 895, 89, 53, 873, 727, 686, 608, 147, 98, 185, 252, 776, 54, 675, 220, 67, 366, 576, 636, 771, 846, 808, 553, 259, 996, 224, 149 };
            }
            else if (ggg == "bi") {
                programm_number = 8;
                numbers = { 7191817, 151, 69 };
            }
        }
        else if (str_arg[1] == 'p') {
            
            if (str_arg[2] == '1') {
                p1 = stoi(to_str_p(argv[i]));
            }
            else if (str_arg[2] == '2') {
                if (programm_number == 8) {
                    p3 = stof(to_str_p(argv[i]));
                }
                else {
                    p2 = stoi(to_str_p(argv[i]));
                }
            }
        }
        else if (str_arg[1] == 'f') {
			string path = to_str(argv[i]);
            numbers = convert_file_to_ints(path);
        } else if (str_arg[1] == 'o') {
			string path = to_str(argv[i]);;
			parameters.output_file = path;
		}
    }

    switch (programm_number) {
    case 1:
        st(parameters, numbers, p1, p2);
        return 0;
    case 2:
        tr(parameters, numbers, p1, p2);
        return 0;
    case 3:
        ex(parameters, numbers, p1, p2);
        return 0;
    case 4:
        nr(parameters, numbers, p1, p2);
        return 0;
    case 5:
        gm(parameters, numbers, p1, p2);
        return 0;
    case 6:
        ln(parameters, numbers, p1, p2);
        return 0;
    case 7:
        ls(parameters, numbers, p1, p2);
        return 0;
    case 8:
        bi(parameters, numbers, p1, p3);
        return 0;
    default:
        return 0;
    }

    return 0;
}