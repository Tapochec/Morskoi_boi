#pragma once
#include "Bot.h" 

namespace Morskoiboi {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Сводка для MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			this->Text = "Морской бой";
			myMap = gcnew array<int, 2>(mapSize, mapSize);
			enemyMap = gcnew array<int, 2>(mapSize, mapSize);
			Init();
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(794, 526);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->ResumeLayout(false);

		}
#pragma endregion

	public: const int mapSize = 10;
	public: int cellSize = 30;
	public: String^ alphabet = "АБВГДЕЖЗИК";

	array<int, 2>^ myMap;
	array<int, 2>^ enemyMap;

	public: array<Button^, 2>^ myButton = gcnew array<Button^, 2>(mapSize, mapSize);
	public: array<Button^, 2>^ enemyButton = gcnew array<Button^, 2>(mapSize, mapSize);

	public: bool isPlaying = false;
	public: Bot^ bot;

	public: void Init()
	{
		isPlaying = false;
		CreateMap();
		bot = gcnew Bot(enemyMap, myMap, enemyButton, myButton);
		enemyMap = bot->ConfigureShips();
	}

	public: void CreateMap()
	{
		this->Width = mapSize * 2 * cellSize + 80;
		this->Height = (mapSize + 1) * cellSize + 50;

		for (int i = 0; i < mapSize; i++) {

			for (int j = 0; j < mapSize; j++) {
				myMap[i, j] = 0;

				Button^ button = gcnew Button();
				button->Location = System::Drawing::Point(j * cellSize, i * cellSize);
				button->Size = System::Drawing::Size(cellSize, cellSize);

				if (j == 0 || i == 0) {
					button->BackColor = Color::Gray;
					if (i == 0 && j > 0) {
						button->Text = alphabet[j - 1].ToString();
					}
					if (j == 0 && i > 0) {
						button->Text = i.ToString();
					}

				}
				else {
					button->Click += gcnew EventHandler(this, &MyForm::ConfigureShips);
				}

				myButton[i, j] = button;
				this->Controls->Add(button);
			}
		}
		for (int i = 0; i < mapSize; i++) {

			for (int j = 0; j < mapSize; j++) {

				myMap[i, j] = 0;
				enemyMap[i, j] = 0;

				Button^ button = gcnew Button();
				button->Location = System::Drawing::Point(320 + j * cellSize, i * cellSize);
				button->Size = System::Drawing::Size(cellSize, cellSize);


				if (j == 0 || i == 0) {
					button->BackColor = Color::Gray;
					if (i == 0 && j > 0) {
						button->Text = alphabet[j - 1].ToString();
					}
					if (j == 0 && i > 0) {
						button->Text = i.ToString();
					}
				}
				button->Click += gcnew EventHandler(this, &MyForm::PlayerShoot);
				enemyButton[i, j] = button;
				this->Controls->Add(button);
			}
		}
		Label^ map1 = gcnew Label();
		map1->Text = "Карта игрока";
		map1->Location = System::Drawing::Point(mapSize * cellSize / 2, mapSize * cellSize + 10);
		this->Controls->Add(map1);

		Label^ map2 = gcnew Label();
		map2->Text = "Карта противника";
		map2->Location = System::Drawing::Point(350 + mapSize * cellSize / 2, mapSize * cellSize + 10);
		this->Controls->Add(map2);

		Button^ startButton = gcnew Button();
		startButton->Text = "Начать игру";
		startButton->Click += gcnew EventHandler(this, &MyForm::Start);
		startButton->Location = System::Drawing::Point(0, mapSize * cellSize + 20);
		this->Controls->Add(startButton);
	}

	public: void Start(Object^ sender, EventArgs^ e)
	{
		isPlaying = true;
	}


			public: bool CheckIfMapNotEmpty() {

				bool isEmpty1 = true;
				bool isEmpty2 = true;

				for (int i = 1; i < mapSize; i++) {
					for (int j = 1; j < mapSize; j++) {

						if (myMap[i, j] != 0)
							isEmpty1 = false;
						if (enemyMap[i, j] != 0)
							isEmpty2 = false;

					}
				}
				if (isEmpty1 || isEmpty2)
					return false;
				else return true;
			}

	public: void ConfigureShips(Object^ sender, EventArgs^ e) {

		Button^ pressedButton = dynamic_cast<Button^>(sender);
		if (!isPlaying) {
			if (myMap[pressedButton->Location.Y / cellSize, pressedButton->Location.X / cellSize] == 0) {

				pressedButton->BackColor = Color::Red;
				myMap[pressedButton->Location.Y / cellSize, pressedButton->Location.X / cellSize] = 1;
			}
			else {
				pressedButton->BackColor = Color::White;
				myMap[pressedButton->Location.Y / cellSize, pressedButton->Location.X / cellSize] = 0;
			}

		}
	}

		  void PlayerShoot(Object^ sender, EventArgs^ e)
		  {
			  Button^ pressedButton = dynamic_cast<Button^>(sender);
			  bool playerTurn = Shoot(enemyMap, pressedButton);
			  if (!playerTurn)
				  bot->Shoot();

			  if (CheckIfMapNotEmpty()) {
				  this->Controls->Clear();
				  Init();
			  }
		  }

		  public: bool Shoot(array<int, 2>^ map, Button^ pressedButton) {
			  bool hit = false;
			  if (isPlaying) {
				  int delta = 0;
				  if (pressedButton->Location.X > 320) 
					  delta = 320;
				  if (map[pressedButton->Location.Y / cellSize, (pressedButton->Location.X - delta) / cellSize] != 0) {

					  hit = true;
					  map[pressedButton->Location.Y / cellSize, (pressedButton->Location.X - delta) / cellSize] = 0;
					  pressedButton->BackColor = Color::Blue;
					  pressedButton->Text = "X";
				  }
				  else {
					  hit = false;

					  pressedButton->BackColor = Color::Black;

				  }
			  }
			  return hit;
		  }


	};
}
