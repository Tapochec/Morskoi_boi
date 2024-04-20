#pragma once
#include "MyForm.h"
namespace Morskoiboi {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace System::Collections::Generic;
	using namespace System::Text;
	using namespace System::Threading::Tasks;

	public ref class Bot : public System::Windows::Forms::Form
	{
	public: const int mapSize = 10;
		array<int, 2>^ myMap;
		array<int, 2>^ enemyMap;

	public: array<Button^, 2>^ myButton = gcnew array<Button^, 2>(mapSize, mapSize);
	public: array<Button^, 2>^ enemyButton = gcnew array<Button^, 2>(mapSize, mapSize);

	public: Bot(array<int, 2>^ myMap, array<int, 2>^ enemyMap, array<Button^, 2>^ myButton, array<Button^, 2>^ enemyButton) {
		this->myMap = myMap;
		this->enemyMap = enemyMap;
		this->myButton = myButton;
		this->enemyButton = enemyButton;
		  }

	public: bool IsInsideMap(int i, int j) {
		if (i < 0 || j < 0 || i >= mapSize || j >= mapSize) {
			return false;
		}
		return true;
	}

	public: bool IsEmpty(int i, int j, int length) {
		bool isEmpty = true;
		for (int k = j; k < j + length; k++) {

			if (myMap[i, k] != 0) {
				isEmpty = false;
				break;
			}
			
		}
		return isEmpty;
	}

	public: array<int, 2>^ ConfigureShips() {
		int lengthShip = 4;
		int cycleValue = 4;
		int shipsCount = 10;
		Random^ r = gcnew Random();
		int posX = 0;
		int posY = 0;


		while (shipsCount > 0) {
			for (int i = 0; i < cycleValue / 4; i++) {
				posX = r->Next(0, mapSize);
				posY = r->Next(0, mapSize);

				while (!IsInsideMap(posX, posY+lengthShip-1) || !IsEmpty(posX, posY, lengthShip)) {
					posX = r->Next(0, mapSize);
					posY = r->Next(0, mapSize);
				}
				for (int k = posY; k < posY + lengthShip; k++) {
					myMap[posX, posY] = 1;
				}
				lengthShip--;
				cycleValue *= 2;
				shipsCount--;
				if (shipsCount <= 0)
					break;
			}
		}
		return myMap;

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

	public: bool Shoot() {
		bool hit = false;
		int delta = 0;
		Random^ r = gcnew Random();

		int posX = r->Next(1, mapSize);
		int posY = r->Next(1, mapSize);

		while (enemyButton[posX, posY]->BackColor == Color::Blue || enemyButton[posX, posY]->BackColor == Color::Black) {
			posX = r->Next(1, mapSize);
			posY = r->Next(1, mapSize);
		}

		if (enemyMap[posX, posY] != 0) {
			hit = true;
			enemyMap[posX, posY] = 0;
			enemyButton[posX, posY]->BackColor = Color::Blue;
			enemyButton[posX, posY]->Text = "X";
		}
		else {
			hit = false;
			enemyButton[posX, posY]->BackColor = Color::Black;
		}
		if (hit)
			Shoot();

		return hit;
	}
	};
}