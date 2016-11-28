#pragma once

namespace SystemGPU
{
	namespace Draw
	{
		void DrawLine(int x1, int y1, int x2, int y2, char color[4]);
		void DrawRectangle(int x1, int y1, int x2, int y2, char color[4]);
		void DrawFilledRectangle(int x1, int y1, int x2, int y2, char color[4]);
	}
	
	void Init();
	
	void Display();
	void Clear();
	void Clear(char color[4]);
};