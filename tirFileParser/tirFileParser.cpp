#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <tchar.h>

#include "console.h"

using namespace std;

int main(void)
{
	SetConsoleTitle(_T(".tir file parser"));

	OPENFILENAME ofn;
	char szFile[100];

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "TIR Files (*.tir)\0*.tir;*.tir\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		ifstream tir(szFile);

		string line;
		vector<string> css(137);
		string fnom;
		while (getline(tir, line))
		{
			if (line.find("[VERTICAL]") != string::npos)
			{
				for (int i = 0; i < 5; i++)
					getline(tir, line);

				getline(tir, fnom);

				fnom.erase(0, 27);
				fnom.erase(8, fnom.length());
			}
			if (line.find("[LATERAL_COEFFICIENTS]") != string::npos)
			{
				for (int i = 0; i < 18; i++)
				{
					getline(tir, css[i]);

					css[i].erase(0, 27);
					css[i].erase(15, css[i].length());
					css[i].erase(find(css[i].begin(), css[i].end(), ' '));
				}
				tir.close();
				break;
			}
		}
		vector<double> cssDouble(20);

		cout.precision(8);

		SetConsoleTextAttribute(hConsole, Console_Color::Yellow);

		for (int i = 0; i < 18; i++)
			cssDouble[i] = atof(css[i].c_str());

		for (int i = 0; i < 18; i++)
			cout << fixed << cssDouble[i] << endl;

		cout << fnom << endl;

		string fileWrite(szFile);
		fileWrite.erase(fileWrite.size() - 4);

		ofstream matlab(fileWrite + ".m");

		matlab << "pCy1 = " << cssDouble[0] << ";" << endl;
		matlab << "pDy1 = " << cssDouble[1] << ";" << endl;
		matlab << "pDy2 = " << cssDouble[2] << ";" << endl;
		matlab << "pDy3 = " << cssDouble[3] << ";" << endl;
		matlab << "pEy1 = " << cssDouble[4] << ";" << endl;
		matlab << "pEy2 = " << cssDouble[5] << ";" << endl;
		matlab << "pEy3 = " << cssDouble[6] << ";" << endl;
		matlab << "pEy4 = " << cssDouble[7] << ";" << endl;
		matlab << "pKy1 = " << cssDouble[8] << ";" << endl;
		matlab << "pKy2 = " << cssDouble[9] << ";" << endl;
		matlab << "pKy3 = " << cssDouble[10] << ";" << endl;
		matlab << "pHy1 = " << cssDouble[11] << ";" << endl;
		matlab << "pHy2 = " << cssDouble[12] << ";" << endl;
		matlab << "pHy3 = " << cssDouble[13] << ";" << endl;
		matlab << "pVy1 = " << cssDouble[14] << ";" << endl;
		matlab << "pVy2 = " << cssDouble[15] << ";" << endl;
		matlab << "pVy3 = " << cssDouble[16] << ";" << endl;
		matlab << "pVy4 = " << cssDouble[17] << ";" << endl;

		matlab << endl;

		matlab << "Fz0 = " << fnom << ";" << endl;

		matlab << "lCy = 1;\nlFz0 = 1;\nlKy = 1;\nlHy = 1;\nlVy = 1;\nlgy = 1;\nluy = 1;\nlEy = 1;" << endl;

		matlab << "z0 = 1;\nz2 = 1;\nz3 = 1;\nz4 = 1;" << endl;

		matlab << "a = -4:0.05:4;\naRad = deg2rad(a);\ng = deg2rad(0);\nFz = 62.5 * 9.81;" << endl;

		matlab << "dfz = (Fz - Fz0) / Fz0;\ngy = g * lgy;\nSHy = (pHy1 + pHy2 * dfz) * lHy + pHy3 * gy * z0 + z4 - 1;\nay = aRad + SHy;\nCy = pCy1 * lCy;\nuy = (pDy1 + pDy2 * dfz) * (1 - pDy3 * gy * gy) * luy;\nDy = uy * Fz * z2;\nEy = (pEy1 + pEy2 * dfz) * (1 - (pEy3 + pEy4 * gy) * sign(ay)) * lEy;\nKy0 = pKy1 * Fz0 * sin(2 * atan(Fz / (pKy2 * Fz0 * lFz0))) * lFz0 * lKy;\nKy = Ky0 * (1 - pKy3 * abs(gy)) * z3;\nBy = Ky / (Cy * Dy);\nSVy = Fz * ((pVy1 + pVy2 * dfz) * lVy + (pVy3 + pVy4 * dfz) * gy) * luy * z4;\nKyg0 = pHy3 * Ky0 + Fz * (pVy3 + pVy4 * dfz);\nFy = Dy * sin(Cy * atan(By.*ay - Ey .* (By.*ay - atan(By.*ay)))) + SVy;\nplot(a, Fy)\n grid on" << endl;
	}

	else
	{
		SetConsoleTextAttribute(hConsole, Console_Color::Red);
		cout << "error opening file" << endl;
		getchar();
		return 0;
	}

	SetConsoleTextAttribute(hConsole, Console_Color::Green);
	cout << endl << "complete" << endl;

	getchar();
	return 0;
}