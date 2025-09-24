#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include "Objekt.h"

int x_breite = 800;
int y_hoehe = 600;

Objekt::Objekt(float x, float y, int winkel) {
	this->x = x;
	this->y = y;
	this->winkel = winkel;
}
void Charakter::bewegen_x(float dx) {					
	if(0 < x && x < x_breite){																											//Begrenzung in x-Richtung
	x += dx; 
	}
	else if (x >= x_breite) {
		if (dx < 0) {
			x += dx;
		}
	}
	else if (x <= 0) {
		if (dx > 0) {
			x += dx;
		}
	}
}
void Charakter::bewegen_y(float dy) {
	if (0 < y && y < y_hoehe) {																											//Begrenzung in y-Richtung
		y += dy;
	}
	else if (y >= y_hoehe) {
		if (dy < 0) {
			y += dy;
		}
	}
	else if (y <= 0) {
		if (dy > 0) {
			y += dy;
		}
	}
}
void Charakter::drehen(int dwinkel) {
		winkel += dwinkel;
}
float Charakter::get_x() const {
	return x; 
}
float Charakter::get_y() const {
	return y; 
}
int Charakter::get_winkel() const {
	return winkel; 
}
void Charakter::schaden(int dmg) {
	leben -= dmg;
}
float Charakter::get_leben() const {
	return leben;
}
Charakter::Charakter(float x, float y, int winkel, int leben): Objekt(x, y, winkel), leben(leben) {}




class GameWindow : public Gosu::Window
{
	Gosu::Image Ente;
	Charakter cha;																													// Erzeuge ein Objekt der Klasse Objekt
public:

	GameWindow()
		: Window(x_breite+40, y_hoehe+40),
		Ente("rakete.png"),
		cha(100, 100, 0, 3)																											// Initialisiere das Objekt
	{
		set_caption("Gamewindow");
	}

																																	// Wird bis zu 60x pro Sekunde aufgerufen.
																																	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
																																	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
		Ente.draw_rot(cha.get_x() + Ente.width() / 2, cha.get_y() + Ente.height() / 2, 0, cha.get_winkel());
		
	}

																																	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		if(input().down(Gosu::KB_LEFT)){																							// Bewege die Ente nach links
		
			cha.drehen(-5);
		}
		else if(input().down(Gosu::KB_RIGHT)){																						// Bewege die Ente nach rechts
			cha.drehen(5);
		}
		else if (input().down(Gosu::KB_UP)) {
			double speed = 5.0;
			cha.bewegen_x(Gosu::offset_x(cha.get_winkel(), speed));
			cha.bewegen_y(Gosu::offset_y(cha.get_winkel(), speed));
		}

	}
};

													// C++ Hauptprogramm
int main()
{
	
	GameWindow window;
	window.show();
}
