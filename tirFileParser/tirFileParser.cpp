#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <Windows.h>
#include <tchar.h>

#include "console.h"

std::vector<std::string> tokenize(const std::string& str, const char delim = ' ')
{
	std::vector<std::string> out;

	std::stringstream ss(str);

	std::string s;
	while (std::getline(ss, s, delim))
	{
		if (!s.empty())
		{
			out.push_back(s);
		}
	}

	return out;
}

int main()
{
	SetConsoleTitle(_T(".tir file parser"));

	OPENFILENAME ofn{};
	char szFile[MAX_PATH];

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "TIR Files (*.tir)\0*.tir;*.tir\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = nullptr;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		std::ifstream tir(szFile);

		std::string line;
		std::vector<std::string> css(18);
		std::string fNom;
		while (std::getline(tir, line))
		{
			if (line.find("[VERTICAL]") != std::string::npos)
			{
				for (int i = 0; i < 5; i++)
					std::getline(tir, line);

				std::getline(tir, fNom);

				fNom = tokenize(fNom)[2];
			}
			if (line.find("[LATERAL_COEFFICIENTS]") != std::string::npos)
			{
				for (int i = 0; i < css.size(); i++)
				{
					std::getline(tir, css[i]);

					css[i] = tokenize(css[i])[2];
				}
				tir.close();
				break;
			}
		}
		std::vector<double> cssDouble(css.size());

		std::cout.precision(8);

		SetConsoleTextAttribute(hConsole, Yellow);

		for (int i = 0; i < css.size(); i++)
			cssDouble[i] = std::atof(css[i].c_str());

		for (int i = 0; i < css.size(); i++)
			std::cout << std::fixed << cssDouble[i] << std::endl;

		std::cout << fNom << std::endl;

		std::string fileWrite(szFile);
		fileWrite.erase(fileWrite.size() - 4);

		std::ofstream matlab(fileWrite + ".m");

		matlab << "pCy1 = " << cssDouble[0] << ";" << std::endl;
		matlab << "pDy1 = " << cssDouble[1] << ";" << std::endl;
		matlab << "pDy2 = " << cssDouble[2] << ";" << std::endl;
		matlab << "pDy3 = " << cssDouble[3] << ";" << std::endl;
		matlab << "pEy1 = " << cssDouble[4] << ";" << std::endl;
		matlab << "pEy2 = " << cssDouble[5] << ";" << std::endl;
		matlab << "pEy3 = " << cssDouble[6] << ";" << std::endl;
		matlab << "pEy4 = " << cssDouble[7] << ";" << std::endl;
		matlab << "pKy1 = " << cssDouble[8] << ";" << std::endl;
		matlab << "pKy2 = " << cssDouble[9] << ";" << std::endl;
		matlab << "pKy3 = " << cssDouble[10] << ";" << std::endl;
		matlab << "pHy1 = " << cssDouble[11] << ";" << std::endl;
		matlab << "pHy2 = " << cssDouble[12] << ";" << std::endl;
		matlab << "pHy3 = " << cssDouble[13] << ";" << std::endl;
		matlab << "pVy1 = " << cssDouble[14] << ";" << std::endl;
		matlab << "pVy2 = " << cssDouble[15] << ";" << std::endl;
		matlab << "pVy3 = " << cssDouble[16] << ";" << std::endl;
		matlab << "pVy4 = " << cssDouble[17] << ";" << std::endl;

		matlab << std::endl;

		matlab << "Fz0 = " << fNom << ";" << std::endl;

		matlab << "lCy = 1;\nlFz0 = 1;\nlKy = 1;\nlHy = 1;\nlVy = 1;\nlgy = 1;\nluy = 1;\nlEy = 1;" << std::endl;

		matlab << "z0 = 1;\nz2 = 1;\nz3 = 1;\nz4 = 1;" << std::endl;

		matlab << "a = -4:0.05:4;\naRad = deg2rad(a);\ng = deg2rad(0);\nFz = 62.5 * 9.81;" << std::endl;

		matlab <<
			"dfz = (Fz - Fz0) / Fz0;\ngy = g * lgy;\nSHy = (pHy1 + pHy2 * dfz) * lHy + pHy3 * gy * z0 + z4 - 1;\nay = aRad + SHy;\nCy = pCy1 * lCy;\nuy = (pDy1 + pDy2 * dfz) * (1 - pDy3 * gy * gy) * luy;\nDy = uy * Fz * z2;\nEy = (pEy1 + pEy2 * dfz) * (1 - (pEy3 + pEy4 * gy) * sign(ay)) * lEy;\nKy0 = pKy1 * Fz0 * sin(2 * atan(Fz / (pKy2 * Fz0 * lFz0))) * lFz0 * lKy;\nKy = Ky0 * (1 - pKy3 * abs(gy)) * z3;\nBy = Ky / (Cy * Dy);\nSVy = Fz * ((pVy1 + pVy2 * dfz) * lVy + (pVy3 + pVy4 * dfz) * gy) * luy * z4;\nKyg0 = pHy3 * Ky0 + Fz * (pVy3 + pVy4 * dfz);\nFy = Dy * sin(Cy * atan(By.*ay - Ey .* (By.*ay - atan(By.*ay)))) + SVy;\nplot(a, Fy)\ngrid on"
			<< std::endl;
	}
	else
	{
		SetConsoleTextAttribute(hConsole, Red);
		std::cout << "error opening file" << std::endl;
		std::getchar();
		return 0;
	}

	SetConsoleTextAttribute(hConsole, Green);
	std::cout << std::endl << "complete" << std::endl;

	std::getchar();
	return 0;
}
