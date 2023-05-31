#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <sstream>
#include <fstream>
#include <cmath>
#include <cstdint>
#include <windows.h>


using namespace std;

struct Parameter {
    int numbers_count;
    string output_file;

    Parameter() : 
        numbers_count(10000),
        output_file("rnd.dat") {
    }
};

int pow2(int d) {
    return 1 << d;
}

void lc(const Parameter& parameters, const vector<int>& args) {
	int m = args[0];
    int a = args[1];
    int c = args[2];
    int x = args[3];

	if (m <= 0 || a > m || a < 0 || c > m || c < 0 || x > m || x < 0) {
		cout << endl << "Ошибка!" << endl;
		return;
	}

	int xi;
	vector<int> res;
	double per = 0.1;
	for (int i = 0; i < parameters.numbers_count; i++) {
		xi = (a * x + c) % m;
		res.push_back(xi % 1001);
		x = xi;

		if (static_cast<double>(i) / parameters.numbers_count >= per) {
			cout << endl << per * 100 << "%\n";
			per += 0.1;
		}
	}

	cout << endl << "100%" << endl;

	ofstream wr(parameters.output_file, ios::out);
	for (int i = 0; i < size(res); i++) {
		wr << res[i] << ",";
	}
	wr.close();
}


void add(const Parameter& parameters, const vector<int>& args) {
	double per = 0.1;
    
	int m = args[0];
	int x0 = args[1];
	int x1 = args[2];

	if (m <= 0 || x0 >= x1 || x0 < 1 || x1 + 3 > size(args)) {
		cout << endl << "Ошибка!" << endl;
		return;
	}

	vector<int> res(begin(args) + 3, end(args));

	int xi;
	int num = size(res);

	for (int i = num; i < parameters.numbers_count + num; i++) {
		res.push_back((res[i - x0] + res[i - x1]) % m);
		if (static_cast<double>(i) / parameters.numbers_count >= per) {
			cout << endl << per * 100 << "%\n";
			per += 0.1;
		}
	}
	cout << endl << "100%" << endl;

	ofstream wr(parameters.output_file, ios::out);
	for (int i = x1; i < size(res); i++) {
		wr << res[i] % 1001 << ",";
	}
	wr.close();
}


void p5(const Parameter& parameters, const vector<int>& args) {
	double per = 0.1;
	int p = args[0];
	int q1 = args[1];
	int q2 = args[2];
	int q3 = args[3];
	int w = args[4];

	if (q1 >= p || q2 >= p || q3 >= p) {
		cout << endl << "Ошибка!" << endl;
		return;
	}

	vector<int> reg(p);
	for (int i = 0; i < p; i++) {
		reg[i] = rand() % 2;
    }

	ofstream wr(parameters.output_file, ios::out);

	for (int i = 0; i < parameters.numbers_count; i++) {
		int x_np = (reg[q1 - 1] ^ reg[q2 - 1] ^ reg[q3 - 1]);
		reg.push_back(x_np);
		reg.erase(begin(reg));

		int new_num = 0;
		int d = 1;
		for (int i = 0; i < w; i++) {
			new_num = (new_num + reg[i] * d) % 1001;
			d *= 2;
		}

		wr << new_num % 1001 << ",";

		if (static_cast<double>(i) / parameters.numbers_count >= per) {
			cout << endl << per * 100 << "%\n";
			per += 0.1;
		}

	}
	cout << endl << "100%" << endl;

	wr.close();
}


int lfsr_func(const vector<int>& a_i, const vector<int>& reg_st) {
	int res = 0;
	
    for (int i = 0; i < size(a_i); i++) {
		res += a_i[i] * reg_st[i] % 2;
	}
	res += reg_st.back() % 2;

	return res;
}


int lfsr_vec_to_num(const vector<int>& reg_st) {
	int res = 0;

	for (int i = size(reg_st) - 1; i >= 0; i--) {
		res += reg_st[i] * pow2(i);
	}

	return res;
}

int lfsr_str_to_num(const string& reg_st) {
	int res = 0;

	for (int i = size(reg_st) - 1; i >= 0; i--) {
		res += reg_st[i] * pow2(i);
	}

	return res;
}

string num_to_str(int num, int n) {
	string res;

	while (num > 0) {
		res = to_string(num % 2) + res;
		num /= 2;
	}

	while (size(res) < n) {
		res = "0" + res;
	}

	return res;
}

string sdvig(const string& str, const string& new_x) {
	return str.substr(1) + new_x;
}

void lfsr(const Parameter& parameters, const vector<string>& args) {
	double per = 0.1;

	int x0 = stoi(args[0]);
	if (x0 < 0) {
		cout << endl << "Ошибка!" << endl;
		return;
	}

	ofstream wr(parameters.output_file, ios::out);
	
    string reg = args[1];
    for (int i = 0; i < parameters.numbers_count; i++) {
		string bin_x0 = num_to_str(x0, size(reg));
		int new_x = 0;

		for (int j = 0; j < size(reg); j++) {
			if (reg[size(reg) - j - 1] == '1' && bin_x0[size(bin_x0) - j - 1] == '1') {
				new_x++;
			}
		}

		x0 = x0 >> 1;

		if (new_x % 2 == 1) {
			int bits = size(reg);
			x0 = ((int)x0 + pow2(bits - 1)) % pow2(bits);
		}

		wr << x0 % 1001 << ",";

		if (static_cast<double>(i) / parameters.numbers_count >= per) {
			cout << endl << per * 100 << "%\n";
			per += 0.1;
		}

	}
    
	cout << endl << "100%" << endl;
	wr.close();
}

vector<int> lfsr2(int n, const string& vec, int x0) {
	string reg(vec);
	vector<int> res;

	for (int i = 0; i < n; i++) {
		string bin_x0 = num_to_str(x0, size(reg));
		int new_x = 0;
		
        for (int j = 0; j < size(reg); j++) {
			if (reg[size(reg) - j - 1] == '1' && bin_x0[size(bin_x0) - j - 1] == '1') {
				new_x++;
			}
		}
		x0 = x0 >> 1;

		if (new_x % 2 == 1) {
			int bits = size(reg);
			x0 = ((int)x0 + pow2(bits - 1)) % pow2(bits);
		}

		res.push_back(x0 % 1001);

	}

	return res;
}

void nfsr(const Parameter& parameters, const vector<string>& args) {
	double per = 0.1;

	vector<int> R1 = lfsr2(parameters.numbers_count, args[0], lfsr_str_to_num(args[0]));
	vector<int> R2 = lfsr2(parameters.numbers_count, args[1], lfsr_str_to_num(args[1]));
	vector<int> R3 = lfsr2(parameters.numbers_count, args[2], lfsr_str_to_num(args[2]));

	vector<int> res;

	ofstream wr(parameters.output_file, ios::out);

	for (int i = 0; i < parameters.numbers_count; i++) {
		wr << ((R1[i] & R2[i]) ^ (R2[i] & R3[i]) ^ R3[i]) << ",";

		if (static_cast<double>(i) / parameters.numbers_count >= per) {
			cout << endl << per * 100 << "%\n";
			per += 0.1;
		}
	}

	cout << endl << "100%" << endl;

	wr.close();
}



int mt_str_to_num(const vector<int>& reg_st) {
	int res = 0;

	for (int i = size(reg_st) - 1; i >= 0; i--) {
		res += reg_st[i] * pow2(i);
	}

	return res;
}


void mt(const Parameter& parameters, const vector<int>& args) {
	double per = 0.1;

	int p = 624, w = 32, r = 31, q = 397, a = 2567483615, u = 11, s = 7, t = 15, l = 18, b = 2636928640, c = 4022730752;
	int64_t u_v = 2147483648;
	int u_n = 11;
	int64_t h_v = 2147483647;
	int mod = args[0];
	int x0 = args[1];

	if (x0 < 0 || mod <= 0) {
		cout << endl << "Ошибка!" << endl;
		return;
	}

	vector<int64_t> X;
	X.push_back(x0);

	for (int i = 1; i < p; i++) {
		X.push_back(abs(1812433253 * (X[i - 1] ^ (X[i - 1] >> 30)) + i));
	}

	ofstream wr(parameters.output_file, ios::out);
	
    int it = 0;
	for (int i = 0; i < parameters.numbers_count; i++) {
		int y_n = ((X[it] & u_n) | (X[it] & h_v));

		if (y_n % 2 == 1)
			X[it] = X[(it + q) % p] ^ (y_n >> 1) ^ a;
		else
			X[it] = X[(it + q) % p] ^ (y_n >> 1) ^ 0;

		y_n = X[it];
		y_n = y_n ^ (y_n >> u);
		y_n = y_n ^ ((y_n << s) & b);
		y_n = y_n ^ ((y_n << t) & c);
		int Z = y_n ^ (y_n >> l);

		wr << Z % mod % 1001 << ",";

		it = (it + 1) % p;

		if (static_cast<double>(i) / parameters.numbers_count >= per) {
			cout << endl << per * 100 << "%" << endl;
			per += 0.1;
		}
	}
	cout << endl << "100%" << endl;

	wr.close();
}


void rc4(const Parameter& parameters, const vector<int>& args) {
	double per = 0.1;
	
    vector<int> K(256), S(256);
	
    for (int i = 0; i < 256; i++) {
		K[i] = args[i];
		S[i] = i;
		
        if (K[i] < 0) {
			cout << endl << "Ошибка!" << endl;
			return;
		}
	}

	int j = 0;
	
    for (int i = 0; i < 256; i++) {
		j = (j + S[i] + K[i]) % 256;
		swap(S[i], S[j]);
	}

	ofstream wr(parameters.output_file, ios::out);

	int i = 0;
	j = 0;
	
    for (int k = 0; k < parameters.numbers_count; k++) {
		i = (i + 1) % 256;
		j = (j + S[i]) % 256;
		
        swap(S[i], S[j]);
		
        int t = (S[i] + S[j]) % 256;
		int R = S[t];
		
		wr << R % 1001 << ",";

		if (static_cast<double>(i) / parameters.numbers_count >= per) {
			cout << endl << per * 100 << "%" << endl;
			per += 0.1;
		}
	}

	cout << endl << "100%" << endl;

	wr.close();
}

int rsa_2_to_10(const vector<int>& reg_st) {
	int res = 0;

	for (int i = size(reg_st) - 1; i >= 0; i--) {
		res += reg_st[i] * pow2(i);
	}

	return res;
}

int rsa_pow(int x, int y, int m) {
	int res = 1;
	
    for (int i = 0; i < y; i++) {
		res = res * x % m;
	}
	
    return res;
}

void rsa(const Parameter& parameters, const vector<int>& args) {
	double per = 0.1;

	int n = args[0];
    int e = args[1];
    int x1 = args[2];
    int l = 10;

	if (e <= 1 || x1 < 1 || x1 > n - 1) {
		cout << endl << "Ошибка!" << endl;
		return;
	}

	ofstream wr(parameters.output_file, ios::out);

	for (int k = 0; k < parameters.numbers_count; k++) {
		int x2;
		vector<int> z(l);

		for (int i = 0; i < l; i++) {
			x2 = rsa_pow(x1, e, n);
			z[i] = x2 % 2;
			x1 = x2;
		}

		int res_z = rsa_2_to_10(z);

		wr << (res_z + 1000) % 1001 << ",";

		if (static_cast<double>(k) / parameters.numbers_count >= per) {
			cout << endl << per * 100 << "%\n";
			per += 0.1;
		}
	}

	cout << endl << "100%" << endl;

	wr.close();
}



void bbs(const Parameter& parameters, const vector<int>& args) {
	double per = 0.1;
	int p = 127, q = 131, n = 16637, x0 = args[0], l = 11;

	if (x0 <= 1 || n % x0 == 0 || x0 % n == 0) {
		cout << endl << "Ошибка!" << endl;
		return;
	}

	ofstream wr(parameters.output_file, ios::out);

	for (int k = 0; k < parameters.numbers_count; k++) {
		int x2 = (x0 * x0) % n;

		vector<int> z;
		for (int i = 0; i < l; i++) {
			x2 = (x0 * x0) % n;
			z.push_back(x2 % 2);
			x0 = x2;
		}

		int res_z = rsa_2_to_10(z);

		wr << res_z % 1001 << ",";

		if (static_cast<double>(k) / parameters.numbers_count >= per) {
			cout << endl << per * 100 << "%\n";
			per += 0.1;
		}
	}

	cout << endl << "100%" << endl;
	
    wr.close();
}


template <class T>
vector<T> splitStr(const string& str) {
	vector<T> elems;
	T elem;
	stringstream ss(str);

	while (ss >> elem) {
		elems.push_back(elem);

		if (ss.peek() == ',') {
			ss.ignore();
		}
	}

	return elems;
}


string to_str(char* s) {
	string res(s);
	res = res.substr(3);

	return res;
}



int main(int argc, char* argv[]) {

	SetConsoleOutputCP(CP_UTF8);

	int it = 0;
	int ch = 0;
	vector<int> int_args;
	vector<string> string_args;

    Parameter parameters;
	int nn = 10000;
	string f = "rnd.dat";


	for (int i = 1; argv[i]; i++) {
		string str_arg = string(argv[i]);
		if (str_arg[1] == 'g') {

			string ggg = str_arg.substr(3, 2);
			cout << endl << ggg << endl;
			if (ggg == "lc") {
				ch = 1;
				int_args = { 31104,625,6571,33 };
			}
			else if (ggg == "ad") {
				ch = 2;
				int_args = { 8001,29,49,816,159,798,290,168,441,691,655,874,220,125,977,586,381,868,294,948,437,581,181,701,536,11,672,103,601,794,189,12,130,386,828,288,183,117,456,624,807,110,498,27,234,474,613,615,341,906,562,778,486,155,276,894,441,226,762,234,762,98,458,399,445,765,223,879 };
			}
			else if (ggg == "5p") {
				ch = 3;
				int_args = { 4253,1093,2254,3297,16 };
			}
			else if (ggg == "lfsr") {
				ch = 4;
				string_args = { "123","101101101101011" };
			}
			else if (ggg == "nfsr") {
				ch = 5;
				string_args = { "101101101101011","101101101101011","101101101101011" };
			}
			else if (ggg == "mt") {
				ch = 6;
				int_args = { 10001,8191 };
			}
			else if (ggg == "rc") {
				ch = 7;
				int_args = { 802,720,341,337,961,882,417,785,198,727,899,372,374,425,556,615,813,768,840,183,893,568,73,387,18,436,182,125,806,899,485,607,619,825,944,579,707,360,363,904,87,262,276,460,687,831,75,499,599,915,681,492,483,754,878,500,189,60,624,994,959,109,600,577,934,544,156,640,903,519,544,990,781,819,449,468,650,524,967,248,438,647,739,920,400,617,419,588,676,43,581,634,151,181,211,84,724,367,723,627,886,267,617,667,85,65,134,735,589,100,983,26,747,721,945,147,337,364,734,13,406,315,647,556,496,858,640,220,224,362,847,110,629,463,776,713,528,909,448,116,9,430,141,755,151,86,901,488,449,635,500,855,950,147,410,446,4,49,665,227,411,511,336,39,974,112,752,501,21,200,617,29,629,757,784,779,843,684,266,292,319,766,146,269,912,556,714,916,605,378,142,15,889,478,54,862,590,806,363,610,5,979,638,634,736,421,413,578,105,679,869,424,444,14,692,356,569,405,271,173,783,413,188,671,891,242,533,480,48,895,89,53,873,727,686,608,147,98,185,252,776,54,675,220,67,366,576,636,771,846,808,553,259,996,224,149 };
			}
			else if (ggg == "rs") {
				ch = 8;
				int_args = { 7191817,151,69 };
			}
			else if (ggg == "bb") {
				ch = 9;
				int_args = { 8627 };
			}

		}
		else if (str_arg[1] == 'i' && (ch == 4 || ch == 5)) {
			string_args = splitStr<string>(to_str(argv[i]));
		}
		else if (str_arg[1] == 'i') {
			int_args = splitStr<int>(to_str(argv[i]));
		}
		else if (str_arg[1] == 'n') {
			parameters.numbers_count = stoi(to_str(argv[i]));
		}
		else if (str_arg[1] == 'f') {
			parameters.output_file = to_str(argv[i]);
		}
		else if (str_arg[1] == 'h') {
			cout << "Введена команда c /h. Допустимые параметры:";
			cout << "\n\n/g:<код_метода> - параметр указывает на метод генерации ПСЧ, при этом код_метода может быть одним из следующих:\n";
			cout << "\n  lc – линейный конгруэнтный метод (Вход: модуль, множитель, приращение, начальное значение)"
				<< "\n  add – аддитивный метод(Вход: модуль, младший индекс, старший индекс, последовательность начальных значений)"
				<< "\n  5p – пятипараметрический метод(Вход: p, q1, q2, q3, w)"
				<< "\n  lfsr – регистр сдвига с обратной связью(РСЛОС) (Вход: двоичное представление вектора коэффициентов, начальное значение регистра)"
				<< "\n  nfsr – нелинейная комбинация РСЛОС(Вход: двоичное представление векторов коэффициентов для R1, R2, R3)"
				<< "\n  mt – вихрь Мерсенна(Вход: модуль, начальное значение x)"
				<< "\n  rc4 – RC4(Вход: 256 начальных значений)"
				<< "\n  rsa – ГПСЧ на основе RSA(Вход: модуль n, число e, начальное значение x; e удовлетворяет условиям : 1 < e < (p - 1)(q - 1), НОД(e, (p - 1)(q - 1)) = 1, где p * q = n.x из интервала[1, n])"
				<< "\n  bbs – алгоритм Блюма - Блюма - Шуба(Вход: Начальное значение x(взаимно простое с n)); \n";
			cout << "\n\n/i:<число> - инициализационный вектор генератора.";
			cout << "\n\n/n:<длина> - количество генерируемых чисел. Если параметр не указан, - генерируется 10000 чисел.";
			cout << "\n\n/f:<полное_имя_файла> - полное имя файла, в который будут выводиться данные. Если параметр не указан, данные должны записываться в файл с именем rnd.dat.";
			cout << "\n\n/h – информация о допустимых параметрах командной строки программы.\n";
		}
	}

	switch (ch) {
	case 1:
		lc(parameters, int_args);
		return 0;
	case 2:
		add(parameters, int_args);
		return 0;
	case 3:
		p5(parameters, int_args);
		return 0;
	case 4:
		lfsr(parameters, string_args);
		return 0;
	case 5:
		nfsr(parameters, string_args);
		return 0;
	case 6:
		mt(parameters, int_args);
		return 0;
	case 7:
		rc4(parameters, int_args);
		return 0;
	case 8:
		rsa(parameters, int_args);
		return 0;
	case 9:
		bbs(parameters, int_args);
		return 0;
	default:
		return 0;
	}

	return 0;
}