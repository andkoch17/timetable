#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <clocale>
#include <fstream>
//#include <windows.h>			//Библиотека для отображения на windows

#define MAX_PAIRS 4				//Максимальное количество пар в день
#define MAX_DAYS 5				//Количество учебных дней в неделе 
#define MAX_DAYS_IN_YEAR 80		//Количество учебных дней в семестре
#define CLEAR_WORD "clear"		//Слово для очистки экрана консоли, для linux-"clear", для WINDOWS - cls

/*
	Планы:
		1.Структура базы данных
			1.1 Subject +
			1.2	Teacher + 
			1.3 Group + 
			1.4 Classroom +
			1.5 Попробовать подогнать структуру базы данных под алгоритм + 
		2.Логика базы данных
			2.1 Нормальная запись словаря, отвечающего за количество часов предмета в семестр +
			2.2 Если введенного предмета не существует - предложить сделать новый +
			2.3 Главное меню +
			2.4 Редактирование (
									Группы + Добавить проверку на дупликат в добавлении нового проедмета в план
									Аудитории +
									Учителя +
									Предметы +
								)
			2.5 Удаление пустых объектов (А они вообще есть?)
			2.6 Нормальный вывод +
			2.7 Проблема: При ненаохде учителей вылет без сохранения (?)
			2.8 Автоматическая смена ID
		3.Сохранение в текстовый файл(позже в SQL) +
		4.Алгоритм составления расписания +
			4.1 Работа с памятью расписания +
			4.2 Вывод расписания +
			4.3 Сохранение расписания
			4.4 Сделать обнуление расписания +
			4.5 Сделать проверку на большем количестве данных

*/

using namespace std;

struct shedule
{
	/*
		subj_id - Предмет
		teacher_id - Преподаватель
		clsrm_id - аудитория
	*/
	bool exist;
	int group_id;
	int subj_id;
	int teacher_id;
	int clsrm_id;
};

class Teacher{
	public:
			
		Teacher(){
            /*В случае пустого объявления id=0 и нужно удалить*/
            cout << "Ошибка, вылет" << endl;
            teacher_id = 0;
			return;
		}
		
		Teacher(int _teacher_id, string _name, vector<int> _subj_ids){
			teacher_id = _teacher_id;
			name = _name;
			for(int i = 0; i < _subj_ids.size(); i++)
				subj_ids.push_back(_subj_ids[i]);
		}
		
		void add_subj(int subj_id){
			subj_ids.push_back(subj_id);
			return;
		}

		void delete_subj(int subj_id){
			for(int i = 0; i < subj_ids.size(); i++){
				if(subj_ids[i] == subj_id){
					subj_ids.erase(subj_ids.begin()+i);
					return;
				}
			}
		}

		void set_id(int _teacher_id){
			teacher_id = _teacher_id;
			return;
		}

		void set_name(string _name){
			name = _name;
			return;
		}
		
		int get_id(){
			return teacher_id;
		}

		string get_name(){
			return name;
		}

		vector<int> get_subj_ids(){
			return subj_ids;
		}

	private:
		/*
			teacher_id - уникальный id
			name - Имя преподавателя
			subj_ids - вектор id предметов, которые ведет преподаватель
		*/
		int teacher_id;
		string name;
		vector<int> subj_ids;
};

class Group{
	public:
	
		Group(){
			/*В случае пустого объявления id=0 и нужно удалить*/
			cout << "Ошибка, вылет" << endl;
			group_id = 0;
			return;
		}

		Group(int _group_id, string _name, map<int, int> _plan){
			group_id = _group_id;
			name = _name;
			priority = 0;

			map<int, int>::iterator _it = _plan.begin();
			
			for (int i = 0; _it != _plan.end(); _it++, i++){
				plan[_it->first] = _it->second;
			}


			return;
		}

		void set_id(int _group_id){
			group_id = _group_id;
			return;
		}

		void set_name(string _name){
			name = _name;
			return;
		}

		/*void set_plan(map<int, int> _plan){
			map<int, int>::iterator _it = _plan.begin();

			for (int i = 0; _it != _plan.end(); _it++, i++)
			{
				plan[_it->first] = _it->second;
			}
		}*/

		void add_to_plan(int plan_key, int plan_value){
			plan[plan_key] = plan_value;
			return;
		}

		void delete_from_plan(int plan_key){
			plan.erase(plan_key);
			return;
		}

		int get_id(){
			return group_id;
		}

		string get_name(){
			return name;
		}

		map<int, int> get_plan(){
			return plan;
		}

		int get_priority(){
			return priority;
		}

		void add_priority(int val){
			priority += val;
		}

	private:
		/*
			group_id - уникальный id
			name - Название группы
			plan - словарь {id предмета: кол-во часов в неделю}
		*/
		int group_id;
		string name;
		int priority;
		map<int, int> plan;
		shedule self_shedule;
};

class Subject{
	public:

		Subject(){
			/*В случае пустого объявления id=0 и нужно удалить*/
			cout << "Ошибка, вылет" << endl;
			subj_id = 0;
			return;
		}

		Subject(int _subj_id, string _name, int _hour_amount, int _complexity){
			subj_id = _subj_id;
			name = _name;
			hour_amount = _hour_amount;
			complexity = _complexity;
			return;
		}

		void set_id(int _subj_id){
			subj_id = _subj_id;
			return;
		}

		void set_name(string _name){
			name = _name;
			return;
		}

		void set_hour_amount(int _hour_amount){
			hour_amount = _hour_amount;
			return;
		}

		void set_complexity(int _complexity){
			complexity = _complexity;
			return;
		}

		int get_id(){
			return subj_id;
		}

		string get_name(){
			return name;
		}

		int get_hour_amount(){
			return hour_amount;
		}

		int get_complexity(){
			return complexity;
		}

	private:
		/*
			subj_id - уникальный id
			name - Название предмета
			hour_amout - Кол-во часов в семестре
		*/
		int subj_id;
		string name;
		int hour_amount;
		int complexity;
};

class Classroom{
	public:
		Classroom()
		{
			/*В случае пустого объявления id=0 и нужно удалить*/
			cout << "Ошибка, вылет" << endl;
			clsrm_id = 0;
			name = "";
			return;
		}

		Classroom(int _clsrm_id, string _name)
		{
			clsrm_id = _clsrm_id;
			name = _name;
			return;
		}

		void set_id(int _clsrm_id)
		{
			clsrm_id = _clsrm_id;
			return;
		}

		void set_name(string _name)
		{
			name = _name;
			return;
		}

		int get_id()
		{
			return clsrm_id;
		}

		string get_name()
		{
			return name;
		}

	private:
		/*
			clsrm_id - уникальный id
			name - Название аудитории
		*/
		int clsrm_id;
		string name;
};

class DataBase{
	public:

		/*
			Как происходит добаление:
			1.Генерируется id на основе последней записи в БД
			2.Вводится имя или название
			3.Вводится доп информация, если нужно связать записи разных структур, то сначала
			вводится название или имя того, что ищут, находят соответсвующую запись в векторе,
			после чего берут id для последующего поиска.

			Как происходит редактирование:
			1.Вводится название или имя того, что ищут
			2.Берется указатель на соответствующую запись
			3.Происходит редактирование
			4.Алгоритм изменений похож на добавление новой записи, работа ведется только с
			определенным пунктом(например: изменение предметов, который ведет учитель анологичен
			с добавлением их в новую запись)


		*/


		DataBase(){

			//Загрузка данныг БД
			
			ifstream fin;
			fin.open("database/subjects.txt");
			if(fin.is_open()){
				while(!fin.eof()){
					string buff;
					getline(fin, buff);
					if(buff != ""){
						int subj_id;
						string name;
						int hour_amount;
						int complexity = 0;
						int slash_count = 0;
						string ibuff = "";
						for (int i = 0; i < buff.length(); i++) {
							if (buff[i] == '/') {
								slash_count++;
								switch (slash_count)
								{
								case 1:
									subj_id = stoi(ibuff);
									i++;
									ibuff = buff[i];
									break;

								case 2:
									name = ibuff;
									i++;
									ibuff = buff[i];
									break;

								case 3:
									hour_amount = stoi(ibuff);
									i++;
									ibuff = buff[i];
									break;
								}
							}else{
								ibuff += buff[i];
							}
						}
						complexity = stoi(ibuff);

						Subject new_subj(subj_id, name, hour_amount, complexity);
						subjects.push_back(new_subj);
					}
				}
			}
			fin.close();

			fin.open("database/classrooms.txt");
			if(fin.is_open()){
				while(!fin.eof()){
					string buff;
					getline(fin, buff);
					if(buff != ""){
						string name;
						int clsrm_id;
						string ibuff = "";
						for(int i = 0; i < buff.length(); i++){
							if(buff[i] == '/'){
								clsrm_id = stoi(ibuff);
								i++;
								ibuff = buff[i];
							}else{
								ibuff += buff[i];
							}
						}
						name = ibuff;

						Classroom new_clsrm(clsrm_id, name);
						classrooms.push_back(new_clsrm);
					}
				}
			}
			fin.close();

			fin.open("database/groups.txt");
			if(fin.is_open()){
				while(!fin.eof()){
					string buff;
					getline(fin, buff);
					if(buff != ""){
						int group_id;
						string name;
						map<int, int> plan;
						int plan_key;
						int plan_value;
						string ibuff = "";
						int slash_count = 0;
						for(int i = 0; i < buff.length(); i++){
							if(buff[i] == '/'){
								slash_count++;
								switch (slash_count)
								{
								case 1:
									group_id = stoi(ibuff);
									i++;
									ibuff = buff[i];
									break;
								
								case 2:
									name = ibuff;
									i++;
									ibuff = buff[i];
									break;
								}
							}else{
								if(buff[i] == ','){
									plan_value = stoi(ibuff);
									plan[plan_key] = plan_value;
									i++;
									ibuff = buff[i];
								}else{
									if(buff[i] == '='){
										plan_key = stoi(ibuff);
										i++;
										ibuff = buff[i];
									}else{
										ibuff += buff[i];
									}
								}
							}
						}
						
						Group new_group(group_id, name, plan);
						groups.push_back(new_group);
					}
				}
			}
			fin.close();

			fin.open("database/teachers.txt");
			if(fin.is_open()){
				while(!fin.eof()){
					string buff;
					getline(fin, buff);
					if(buff != ""){
						int teacher_id;
						string name;
						vector<int> subj_ids;
						string ibuff = "";
						int slash_count = 0;
						for(int i = 0; i < buff.length(); i++){
							if(buff[i] == '/'){
								slash_count++;
								switch (slash_count)
								{
								case 1:
									teacher_id = stoi(ibuff);
									i++;
									ibuff = buff[i];
									break;

								case 2:
									name = ibuff;
									i++;
									ibuff = buff[i];
								}
							}else{
								if(buff[i] == ','){
									subj_ids.push_back(stoi(ibuff));
									i++;
									ibuff = buff[i];
								}else{
									ibuff += buff[i];
								}
							}
						}

						Teacher new_teacher(teacher_id, name, subj_ids);
						teachers.push_back(new_teacher);
					}
				}
			}
			fin.close();

			active_shedule = (shedule***)malloc(MAX_DAYS*sizeof(shedule**));
			for (int day = 0; day < MAX_DAYS; day++){
				active_shedule[day] = (shedule**)malloc(MAX_PAIRS*sizeof(shedule*));
				for (int pair = 0; pair < MAX_PAIRS; pair++){
					active_shedule[day][pair] = (shedule*)malloc(groups.size()*sizeof(shedule));
					for (int group = 0; group < groups.size(); group++){
						active_shedule[day][pair][group].exist = false;
					}
				}
			}

		}

		void menu(){

			//Главное меню, где осуществляется выбор дальнейших действий с базой данных

			system(CLEAR_WORD);
			char key;

			while(true){
				system(CLEAR_WORD);

				cout << "=================" << endl;
				show_shedule(active_shedule);
				cout << "=================" << endl;

				cout << "Выберите следующее действие:" << endl
				<< "1.Добавить" << endl
				<< "2.Редактировать базу данных" << endl
				<< "3.Показать" << endl
				<< "4.Сохранить изменения" << endl
				<< "5.Составить расписание" << endl
				<< "0.Выход" << endl;
				cin >> key;

				bool exit = false;

				switch (key)
				{
				case '1':
					while(!exit){
						system(CLEAR_WORD);

						cout << "1.Добавть нового учителя" << endl;
						cout << "2.Добавть новую группу" << endl;
						cout << "3.Добавть новый предмет" << endl;
						cout << "4.Добавть новый кабинет" << endl;
						cout << "0.Вернуться" << endl;

						cin >> key;

						switch (key)
						{
						case '1':
							system(CLEAR_WORD);
							add_new_teacher();
							break;
						
						case '2':
							system(CLEAR_WORD);
							add_new_group();
							break;

						case '3':
							system(CLEAR_WORD);
							add_new_subject();
							break;

						case '4':
							system(CLEAR_WORD);
							add_new_classroom();
							break;

						case '0':
							exit = true;
							break;

						default:
							system(CLEAR_WORD);
							cout << "Вы введи неправильное число" << endl;;
							break;
						}
					}

					break;
					
				case '2':
					while(!exit){
						system(CLEAR_WORD);

						cout << "Выберите следующее действие" << endl;
						cout << "1.Редактировать список учителей" << endl;
						cout << "2.Редактировать список груп" << endl;
						cout << "3.Редактировать список предметов" << endl;
						cout << "4.Редактировать список кабинетов" << endl;
						cout << "0.Вернуться" << endl;

						cin >> key;

						switch (key)
						{
						case '1':
							system(CLEAR_WORD);
							redact_teachers();
							break;
						
						case '2':
							system(CLEAR_WORD);
							redact_groups();
							break;

						case '3':
							system(CLEAR_WORD);
							redact_subjects();
							break;

						case '4':
							system(CLEAR_WORD);
							redact_classrooms();
							break;

						case '0':
							exit = true;
							break;

						default:
							break;
						} 
					}

				case '3':
					while(!exit){
						system(CLEAR_WORD);

						cout << "Выберите следующее действие" << endl;
						cout << "1.Показать список учителей" << endl;
						cout << "2.Показать список груп" << endl;
						cout << "3.Показать список предметов" << endl;
						cout << "4.Показать список кабинетов" << endl;
						cout << "0.Вернуться" << endl;

						cin >> key;

						switch (key)
						{
						case '1':
							system(CLEAR_WORD);
							show_teachers();
							cout << "Введите |e| что бы выйти\n";
							cin >> key;
							break;
						
						case '2':
							system(CLEAR_WORD);
							show_groups();
							cout << "Введите |e| что бы выйти\n";
							cin >> key;
							break;

						case '3':
							system(CLEAR_WORD);
							show_subjects();
							cout << "Введите |e| что бы выйти\n";
							cin >> key;
							break;

						case '4':
							system(CLEAR_WORD);
							show_classrooms();
							cout << "Введите |e| что бы выйти\n";
							cin >> key;
							break;

						case '0':
							exit = true;
							break;

						default:
							system(CLEAR_WORD);
							cout << "Вы ввели неправильное число" << endl;
							break;
						}
					}
					break;

				case '4':
					save_changes();
					break;

					case '5':
						system(CLEAR_WORD);
						change_shedule();
						break;
				
				case '0':
					return;
					break;

				default:
					system(CLEAR_WORD);
					cout << "Вы ввели неправильное число";
					break;
				}
				
			
			}
		}

		void add_new_teacher(){
			
			//Добавление нового преподавателя в базу данных

			int id;
			string name;
			vector<int> subj_ids;

			if (teachers.size() == 0)
				id = 1;
			else
				id = teachers[teachers.size()-1].get_id() + 1;
			
			cout << "Введите имя преподавателя" << endl;
			getline(cin, name);
			getline(cin ,name);

			while (true)
			{
				string subj_name;
				bool found = false;
				cout << "Введите название предмета или |выход| для выхода: ";
				getline(cin, name);
				getline(cin, name);


				if (subj_name == "выход"){
					break;
				}

				for(int i = 0; i < subjects.size(); i++){
					if (subjects[i].get_name() == subj_name){
						subj_ids.push_back(subjects[i].get_id());
						found = true;
					}
				}

				if (!found) {
					cout << "Предмет не найден, хотите добавить?  (Д/Н)" << endl;
					
					string accept_key;
					cin >> accept_key;

					if (accept_key == "д" || accept_key == "Д"){
						add_new_subject();
					}
				}
			}

			Teacher new_teacher = Teacher(id, name, subj_ids);
			teachers.push_back(new_teacher);
		}

		void show_teachers(){

			//вывод всех учителей на экран

			for(int i = 0; i < teachers.size(); i++){
				cout << "ID: " << teachers[i].get_id() << endl <<
				"Имя: " << teachers[i].get_name() << endl;

				vector<int> subj_ids = teachers[i].get_subj_ids();
				cout << "Предметы: ";
				for(int id : subj_ids)
					for(int j = 0; j < subjects.size(); j++)
						if(subjects[j].get_id() == id){
							cout << subjects[j].get_name() << ", ";
							break;
						}
				cout << endl;
			}
		}

		void redact_teachers(){	

			//редактирование списка учителей

			show_teachers();

			string name;

			cout << "Введите учителя, которого хотите редактировать" << endl;
			getline(cin, name);
			getline(cin ,name);

			Teacher* choosed_teacher;

			for(int i = 0; i < teachers.size(); i++){
				if(teachers[i].get_name() == name){
					choosed_teacher = &teachers[i];
					break;
				}
			}

			if(choosed_teacher != NULL){
				system(CLEAR_WORD);

				char key;

				cout << "ID: " << choosed_teacher -> get_id() << endl
				<< "Имя: " <<  choosed_teacher -> get_name() << endl
				<< "Предметы:" << endl;

				vector<int> subj_ids = choosed_teacher -> get_subj_ids();

				for(int i = 0; i < subj_ids.size(); i++){
					for(int j = 0; j < subjects.size(); j++){
						if(subjects[j].get_id() == subj_ids[i]){
							cout << i+1 << "." << subjects[j].get_name() << endl;
							break;
						}
					}
				}

				cout << endl;
				cout << "1.Поменять имя" << endl
				<< "2.Изменить список предметов" << endl
				<< "3.Удалить учителя" << endl
				<< "0.Выход" << endl;

				cin >> key;

				switch (key)
				{
				case '1':
					{	
						system(CLEAR_WORD);
						string teacher_name;
						cout << "Введите новое имя" << endl;
						getline(cin, name);
						getline(cin, name);
						choosed_teacher -> set_name(teacher_name);
						break;
					}
				
				case '2':
					{
						system(CLEAR_WORD);
						bool exit = false;
						while(!exit){
							cout << "1.Добавить предмет" << endl
							<< "2.Убрать предмет" << endl
							<< "0.Выход" << endl;
							cin >> key;

							switch (key)
							{
							case '1':
								{
									string subj_name;
									
									cout << "Введите название предмета, который хотите добавить" << endl;
									getline(cin, name);
									getline(cin, name);
									bool found = false;

									for (int i = 0; i < subjects.size(); i++){
										if (subjects[i].get_name() == subj_name){
											found = true;
											bool duplicate = false;
											for(int j = 0; j < subj_ids.size(); j++){
												if(subj_ids[j] == subjects[i].get_id())
													duplicate = true;
											}
											if(!duplicate)
												choosed_teacher -> add_subj(subjects[i].get_id());
											break;
										}											
									}

									if (!found) {
										cout << "Предмет не найден, хотите добавить?  (Д/Н)" << endl;
										
										string accept_key;
										cin >> accept_key;

										if (accept_key == "д" || accept_key == "Д"){
											add_new_subject();
										}
									}

									break;
								}

							case '2':
							{
									string subj_name;

									cout << "Введите название предмета, который хотите удалить" << endl;
									getline(cin, name);
									getline(cin, name);

									for(int i = 0; i < subjects.size(); i++){
										if(subjects[i].get_name() == subj_name)
											choosed_teacher -> delete_subj(subjects[i].get_id());
									}
									break;
								}

							case '0':
								exit = true;
								break;
							}
						}
						break;
					}

				case '3':
					for(int i = 0; i < teachers.size(); i++){
						if(teachers[i].get_id() == choosed_teacher -> get_id()){
							teachers.erase(teachers.begin()+i);
							break;
						}
					}
					break;

				case '0':
					return;
					break;
				}
				

			}
			
		}

		void add_new_group(){

			//Добавление новой группы

			int id;
			string name;
			map <int, int> plan;


			if (groups.size() == 0)
				id = 1;
			else
				id = groups[groups.size()-1].get_id() + 1;
			
			cout << "Введите название группы: ";
			getline(cin, name);
			getline(cin ,name);
			
			while (true)
			{
				string key;
				int plan_key = 0;
				int plan_value;
				cout << "Введите название предмета или |выход| для выхода: ";
				cin >> key;
				
				if (key == "выход")
					break;
				
				bool found = false;

				for(int i = 0; i < subjects.size(); i++){
					if (subjects[i].get_name() == key){
						found = true;
						plan_key = subjects[i].get_id();
						plan_value = subjects[i].get_hour_amount() / MAX_DAYS_IN_YEAR;
						plan[plan_key] = plan_value;
						break;
					}
				}

				if (!found) {
					cout << "Предмет не найден, хотите добавить?  (Д/Н)" << endl;
					
					string accept_key;
					cin >> accept_key;

					if (accept_key == "д" || accept_key == "Д"){
						add_new_subject();
					}
				}

			}

			Group new_group = Group(id, name, plan);
			groups.push_back(new_group);

		}

		void show_groups(){

			//Вывод всех групп на экран
			
			for (int i = 0; i < groups.size(); i++){
				cout << "ID: " << groups[i].get_id() << endl <<
				"Название: " <<groups[i].get_name() << endl;

				map<int, int> map_plan = groups[i].get_plan();

				auto it = map_plan.begin();
				
				cout << "Предметы и их часы в неделю: ";

				for (int i = 0; it != map_plan.end(); it++, i++)
				{
					for (int j = 0; j < subjects.size(); j++)
						if (subjects[j].get_id() == it->first){
							cout << subjects[j].get_name() << " - " << it->second << " ч., ";
						}
				}
				cout << endl;
			}

			return;
		}

		void redact_groups(){

			//редактирование групп

			show_groups();

			string name;

			cout << "Введите название группы, которую хотите редктировать" << endl;
			getline(cin, name);
			getline(cin ,name);

			Group* choosed_group;

			for(int i = 0; i < groups.size(); i ++){
				if(groups[i].get_name() == name)
					choosed_group = &groups[i];
			}

			if(choosed_group != NULL){
				system(CLEAR_WORD);

				char key;

				cout << "ID - " << choosed_group->get_id()
				<< "\tНазвание - " << choosed_group->get_name() << endl
				<< "План группы:" << endl;

				map<int, int> plan = choosed_group->get_plan();

				map<int, int>::iterator it = plan.begin();

				for(int i = 1; it != plan.end(); it++, i++){
					for(int j = 0; j != subjects.size(); j++){
						if(subjects[j].get_id() == it -> first)
							cout << i << "." << subjects[j].get_name() << " - "
							<< it -> second << endl;
					}
				}

				cout << endl;
				cout << "1.Поменять название" << endl
				<< "2.Изменить план" << endl
				<< "3.Удалить группу" << endl
				<< "0.Вернуться" << endl;
				cin >> key;
				
				switch (key)
				{
				case '1':
					{
						system(CLEAR_WORD);
						string group_name;
						cout << "Введите новое название группы" << endl;
						cin >> group_name;
						choosed_group->set_name(group_name);
						break;
					}
				
				case '2':
					{
						system(CLEAR_WORD);
						bool exit = false;
						while(!exit){
							cout << "1.Добавить предмет" << endl;
							cout << "2.Убрать предмет" << endl;
							cout << "0.Выход" << endl;
							cin >> key;

							switch(key)
							{
								case '2':
									{
										string subj_name;
										cout << "Введите название предмета, который хотите убрать: ";
										getline(cin, name);
										getline(cin, name);

										for(int i=0; i < subjects.size(); i++){
											if(subjects[i].get_name() == subj_name){
												choosed_group->delete_from_plan(subjects[i].get_id());
											}
										}
									}
									break;

								case '1':
									{
										string subj_name;
										cout << "Введите название предмета, который хотите добавить: ";
										getline(cin, subj_name);
										getline(cin, subj_name);

										bool found = false;

										for(int i = 0; i < subjects.size(); i++){
											if(subjects[i].get_name() == subj_name){
												found = true;
												choosed_group->add_to_plan(
																			subjects[i].get_id(),
																			subjects[i].get_hour_amount() / MAX_DAYS_IN_YEAR
																			);
												break;	
											}
										}

										if (!found) {
											cout << "Предмет не найден, хотите добавить?  (Д/Н)" << endl;
											
											string accept_key;
											cin >> accept_key;

											if (accept_key == "д" || accept_key == "Д"){
												add_new_subject();
											}
										}

									}									
									break;
								
								case '0':
									exit = true;
									break;
							}
						}
					}
					break;

				case '3':
					for(int i = 0; i < groups.size(); i++){
						if(groups[i].get_id() == choosed_group->get_id())
							groups.erase(groups.begin()+i);
							break;
					}
					break;

				case '0':
					return;
					break;
				}
			}
		}

		void add_new_subject(){

			//Добавление нового предмета

			string name;
			int hour_amount;
			int id;
			int complexity;

			cout << "Введите название предмета: ";


			getline(cin, name);
			getline(cin ,name);

			cout << "Введите ко-во часов в семестре: ";
			cin >> hour_amount;

			cout << "Введите сложность предмета от 1 до 10(в случае пропуска поставьте 1)";
			cin >> complexity;


			if (subjects.size() == 0)
				id = 1;
			else
				id = subjects[subjects.size()-1].get_id() + 1;

			Subject new_subject = Subject(id, name, hour_amount, complexity);
			
			subjects.push_back(new_subject);

			return; 
		}

		void show_subjects(){

			//Вывод всех предметов на экран

			for (int i = 0; i < subjects.size(); i++)
				cout << "ID: " << subjects[i].get_id() << endl
				<< "Название: " << subjects[i].get_name() << endl
				<< "Количество часов за полгода: " << subjects[i].get_hour_amount() << endl
				<< "Сложность: " << subjects[i].get_complexity() << endl;
			return;
		}

		void redact_subjects(){

			//Редактирование предметов

			show_subjects();
			while(true){

				string name;
				cout << "Введите название предмета, который хотите отредактировать" << endl;

				getline(cin, name);
			getline(cin ,name);

				Subject* choosed_subject;

				for(int i = 0; i < subjects.size(); i ++){
					if(subjects[i].get_name() == name){
						choosed_subject = &subjects[i];
						break;
					}
				}

				if(choosed_subject != NULL){
					while(true){
						system(CLEAR_WORD);
						
						char key;
	
						cout << "ID - " << choosed_subject->get_id() << endl
						<< "Название - " << choosed_subject->get_name() << endl
						<< "Количество часов в семестре - " << choosed_subject->get_hour_amount() << endl;

						cout << "1.Изменить название предмета" << endl
						<< "2.Изменить количество часов в семестре" << endl
						<< "3.Изменить сложность предмета" << endl
						<< "0.Вернуться" << endl;

						cin >> key;

						switch (key)
						{
						case '1':
						{
								system(CLEAR_WORD);
								string subject_name;
								cout << "Введите новое название" << endl;
								cin >> subject_name;
								choosed_subject->set_name(subject_name);
								break;
						}

						case '2':
							{
								system(CLEAR_WORD);
								int hour_amount;
								cout << "Введите новое колличество часов" << endl;
								cin >> hour_amount;
								choosed_subject->set_hour_amount(hour_amount);
								break;
							}

						case '0':
							return;
							break;
						}
					}
				}else{
					cout << "Такой группы нет" << endl;
				}
			}
		}

		void add_new_classroom(){

			//Добавление новой аудитории

			int id;
			string name;
		
			if (classrooms.size() == 0)
				id = 1;
			else
				id = classrooms[classrooms.size()-1].get_id() + 1;

			cout << "Введите название аудитории: ";
			getline(cin, name);
			getline(cin ,name);

			Classroom new_classroom = Classroom(id, name) ;
			
			classrooms.push_back(new_classroom);
		
		}

		void show_classrooms(){

			//Вывод всех аудиторий на экран

			for (int i = 0; i < classrooms.size(); i++){
				cout << "ID: " << classrooms[i].get_id() << endl
				<< "Название: " << classrooms[i].get_name() << endl;
			}
		}

		void redact_classrooms(){

			//Редактирование списка аудиторий

			show_classrooms();

			while (true){
				string name;

				cout << "Введите название аудитории, которую хотите редактировать" << endl;
				getline(cin, name);
				getline(cin ,name);

				Classroom* choosed_classroom;

				for(int i = 0; i < classrooms.size(); i++){
					if(classrooms[i].get_name() == name){
						choosed_classroom = &classrooms[i];
						break;
					}

					if(choosed_classroom != NULL){
						while(true){
							system(CLEAR_WORD);

							char key;

							cout << "ID - " << choosed_classroom->get_id() << endl
							<< "Название - " << choosed_classroom->get_name() << endl;
							
							cout << "1.Изменить название аудитории" << endl
							<< "2.Удалить аудиторию" << endl
							<< "0.Вернуться" << endl;

							cin >> key;

							switch (key)
							{
							case '1':
								{
									system(CLEAR_WORD);
									string clasroom_name;
									cout << "Введите новое название аудиотрии" << endl;
									cin >> clasroom_name;
									break;
								}

							case '2':
								for(int i = 0; i < subjects.size(); i++){
									if(subjects[i].get_id() == choosed_classroom -> get_id())
										subjects.erase(subjects.begin()+i);
										break;
								}
								break;

							}
						}
					}
				}
			}
			
		}

		void save_changes(){

			//Сохранение изменений

			ofstream fout;
			
			fout.open("database/teachers.txt");
			for(int i = 0; i < teachers.size(); i++){
				fout << teachers[i].get_id() << '/' << teachers[i].get_name() << '/';
				vector<int> subj_ids = teachers[i].get_subj_ids();
				for(int j = 0; j < subj_ids.size(); j++){
					fout << subj_ids[j] << ',';
				}
				fout << endl;
			}
			fout.close();

			fout.open("database/groups.txt");
			for(int i = 0; i < groups.size(); i++){
				fout << groups[i].get_id() << '/' << groups[i].get_name() << '/';
				map<int, int> plan = groups[i].get_plan();

				map<int, int>::iterator it = plan.begin();

				for(int j = 0; it != plan.end(); it++, j++){
					fout << it -> first << '=' << it -> second << ',';
				}
				fout << endl;
			}
			fout.close();

			fout.open("database/subjects.txt");
			for(int i = 0; i < subjects.size(); i++){
				fout << subjects[i].get_id() << '/' << subjects[i].get_name() << '/' 
				<< subjects[i].get_hour_amount() << '/' << subjects[i].get_complexity() << endl;
			}
			fout.close();

			fout.open("database/classrooms.txt");
			for(int i = 0; i < classrooms.size(); i++){
				fout << classrooms[i].get_id() << '/' << classrooms[i].get_name() << endl;
			}
			fout.close();

			return;
		}

		void change_shedule(){
			
			
			active_shedule = (shedule***)realloc(NULL, MAX_DAYS*sizeof(shedule**));
			for (int day = 0; day < MAX_DAYS; day++){
				active_shedule[day] = (shedule**)realloc(NULL, MAX_PAIRS*sizeof(shedule*));
				for (int pair = 0; pair < MAX_PAIRS; pair++){
					active_shedule[day][pair] = (shedule*)realloc(NULL, groups.size()*sizeof(shedule));
				}
			}

			active_shedule = generate_shedule();


		}


		shedule*** generate_shedule(){
			shedule*** new_shedule = new shedule**;
			new_shedule = (shedule***)malloc(MAX_DAYS*sizeof(shedule**));
			for (int day = 0; day < MAX_DAYS; day++){
				new_shedule[day] = (shedule**)malloc(MAX_PAIRS*sizeof(shedule*));
				for (int pair = 0; pair < MAX_PAIRS; pair++){
					new_shedule[day][pair] = (shedule*)malloc(groups.size()*sizeof(shedule));
					for (int group = 0; group < groups.size(); group++){
						new_shedule[day][pair][group].exist = false;
					}
				}
			}


			//Сортировка предметов по их сложности
			for (int i = 0; i < subjects.size(); i++){
				for (int j = subjects.size()-1; j > i; j--){
					if (subjects[j].get_complexity() < subjects[j-1].get_complexity()){
						Subject tmp = subjects[j];
						subjects[j] = subjects[j-1];
						subjects[j-1] = tmp;
					}
				}
			}


			bool ready_shedule = false;

			while (!ready_shedule){

				//Сортировка групп по их приоритету
				for (int i = 0; i < groups.size(); i++){
					for (int j = groups.size()-1; j < i; j++){
						if (groups[j].get_priority() < groups[j-1].get_priority()){
						Group tmp = groups[j];
						groups[j] = groups[j-1];
						groups[j-1] = tmp;
					}
					}
				}


				for (int group = 0; group < groups.size(); group++){
					int additional_complexity = 0;
					auto plan = groups[group].get_plan();
					while (additional_complexity < 10){
						for (int day = 0; day < MAX_DAYS; day++){
							int day_max_compexity = day + 5 + additional_complexity;
							for (int pair = 0; pair < MAX_PAIRS; pair++){
								if (!new_shedule[day][pair][group].exist){
									auto iter = plan.begin();
									
									bool new_pair = false;

									while (iter != plan.end() && !new_pair){
										
										for( int i = 0; i < subjects.size(); i++){
											if (subjects[i].get_complexity() <= day_max_compexity){
												if(subjects[i].get_id() == iter->first){
													int hour_amount = 0;
													//Сбор сведений о поставленных предметах, что бы не было слишком много часов
													for (int j = 0; j < MAX_DAYS; j++){
														for (int z = 0; z < MAX_PAIRS; z++){
															if (new_shedule[j][z][group].subj_id == iter->first)
																hour_amount++;
														}
													}
													if (hour_amount < iter->second){
														//Поиск учителей, которые могут вести данный предмет

														bool teacher_found = false;
														int teacher_id = 0;
														bool clsrm_found = false;
														int clsrm_id = 0;

														//Поиск учителя
														for (int teacher = 0; teacher < teachers.size(); teacher++){
															vector<int> subj_ids = teachers[teacher].get_subj_ids();
															for (int subject = 0; subject < subj_ids.size(); subject++){
																if (subj_ids[subject] == iter->first){
																	
																	bool teacher_free = true;
																	//Проверка на занятость учителя в это время
																	for (int j = 0; j < groups.size(); j++){
																		if (new_shedule[day][pair][j].teacher_id == teachers[teacher].get_id()){
																			teacher_free = false;
																		} 
																	}
																	if (teacher_free){
																		teacher_id = teachers[teacher].get_id();
																		teacher_found = true;
																		break;
																	}
																}
															}
														}

														//Поиск аудитории
														for (int clsrm = 0; clsrm < classrooms.size(); clsrm++){
															bool clsrm_free = true;
															for (int j = 0; j < groups.size(); j++){
																if (new_shedule[day][pair][j].clsrm_id == classrooms[clsrm].get_id()){
																	clsrm_free = false;
																}
															}
															if (clsrm_free){
																clsrm_found = true;
																clsrm_id = classrooms[clsrm].get_id();
																break;
															}
														}


														if (teacher_found && clsrm_found){
															new_shedule[day][pair][group].clsrm_id = clsrm_id;
															new_shedule[day][pair][group].exist = true;
															new_shedule[day][pair][group].subj_id = iter->first;
															new_shedule[day][pair][group].teacher_id = teacher_id;
															new_shedule[day][pair][group].group_id = groups[group].get_id();
															new_pair = true;
														}

													}
												}
											}else{
												break;
											}	
										}
										
										iter++;
									}
								}
							}
						}
					
						int hour_amount = 0;

						auto plan = groups[group].get_plan();

						bool end = true;

						for (auto iter = plan.begin(); iter != plan.end(); iter ++) {

							for (int i = 0; i < MAX_DAYS; i++){
								for (int j = 0; j < MAX_PAIRS; j++){
									if (new_shedule[i][j][group].subj_id == iter->first)
										hour_amount++;
								}
							}
							if (hour_amount < iter->second){
								additional_complexity++;
								end = false;
							}
						}

						if (end)
							break;
					}
					
					int additional_prority = 0;

					for (auto iter = plan.begin(); iter != plan.end(); iter++){
						int hour_counter = 0;
						for (int day = 0; day < MAX_DAYS; day++){
							for (int pair = 0; pair < MAX_PAIRS; pair++){
								if (new_shedule[group][pair][group].subj_id == iter->first){
									hour_counter++;
								}
							}
						}
						if (hour_counter < iter->second){
							additional_complexity += iter->second - hour_counter;
							groups[group].add_priority(additional_prority);
						}
					}
				}
				

				//Проверка на заполненность часов
				bool fit_hours = true;

				for (int group = 0; group < groups.size(); group++){
					auto plan = groups[group].get_plan();

					for (auto iter = plan.begin(); iter != plan.end(); iter++){
						int hour_counter = 0;					
						for (int day = 0; day < MAX_DAYS; day++){
							for (int pair = 0; pair < MAX_PAIRS; pair++){
								if (new_shedule[day][pair][group].subj_id == iter -> first){
									hour_counter++;
								}
							}
						}
						if (hour_counter < iter->second){
							fit_hours = false;
							break;
						}
					}
					if(!fit_hours)
						break;
				}

				if (fit_hours)
					ready_shedule = true;
			}
			return new_shedule;
		}

		void show_shedule(shedule*** new_shedule){
			
			for (int i = 0; i < groups.size(); i++){

				cout << "Группа " << groups[i].get_name() << endl;

				for (int day = 0; day < MAX_DAYS; day++){
					switch (day)
					{
					case 0:
						cout << "\tПонедельник" << endl;
						break;
					
					case 1:
						cout << "\tВторник" << endl;
						break;
					
					case 2:
						cout << "\tСреда" << endl;
						break;

					case 3:
						cout << "\tЧетверг" << endl;
						break;

					case 4:
						cout << "\tПятница" << endl;
						break;
					}
					
					for (int pair = 0; pair < MAX_PAIRS; pair++){
						cout << pair+1 << ". ";
						for (int group = 0; group < groups.size(); group++){
							if (new_shedule[day][pair][group].exist){
								if (new_shedule[day][pair][group].group_id == groups[i].get_id()){
									for (int subj = 0; subj < subjects.size(); subj++){
										if (subjects[subj].get_id() == new_shedule[day][pair][group].subj_id){
											cout << subjects[subj].get_name() << endl;
											break;
										}
									}
									
									for (int teacher = 0; teacher < teachers.size(); teacher++){
										if (teachers[teacher].get_id() == new_shedule[day][pair][group].teacher_id){
											cout << teachers[teacher].get_name() << endl;
											break;
										}
									}

									for (int clsrm = 0; clsrm < classrooms.size(); clsrm++){
										if (classrooms[clsrm].get_id() == new_shedule[day][pair][group].clsrm_id){
											cout << "Аудитория: " << classrooms[clsrm].get_name() << endl;
											break;
										}
									}
									cout << "\n--------" << endl;
								}
							}
						}
					}
					cout << "\n==========" << endl;
				}
			}
			
		}

	private:
		vector<Teacher> teachers;
		vector<Group> groups;
		vector<Subject> subjects;
		vector<Classroom> classrooms;

		shedule*** active_shedule;
};

int main(){

	//Команды для смены кодировки
	//SetConsoleCP(1251);
	//SetConsoleOutputCP(1251);

	DataBase db;


	db.menu();


	return 0;
}
