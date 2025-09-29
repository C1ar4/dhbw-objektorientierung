#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include "Objekt.h"

int x_breite = Gosu::screen_width() - 0;
int y_hoehe = Gosu::screen_height() - 0;
float scale_Ente = 0.1;

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

void Laser::bewegen_x(float dx) {
	if (!ende_erreicht) {
		if (0 <= x + dx && x + dx <= x_breite) {																											//nicht benutzt, ist auch nicht ganz zielführend
			x += dx;
		}
		else {
			ende_erreicht = true;
		}
	}
}
void Laser::bewegen_y(float dy) {
		
	if (!ende_erreicht) {
		if (0 <= y+dy && y+dy <= y_hoehe) {																											//nicht benutzt, ist auch nicht ganz zielführend
			y += dy;
		} 
		else {
			ende_erreicht = true;
		}
	}
}
void Laser::drehen(int dwinkel) {
	winkel += dwinkel;
}
float Laser::get_x() const {
	return x;
}
float Laser::get_y() const {
	return y;
}
int Laser::get_winkel() const {
	return winkel;
}
float Laser::get_x_start() const{
	return start_x;
}
float Laser::get_y_start() const {
	return start_y;
}
void Laser::bewegen(float dx, float dy) {
	if (!ende_erreicht) {
		if (0 <= x + dx && x + dx <= x_breite && 0 <= y + dy && y + dy <= y_hoehe) {																											
			x += dx;
			y += dy;
		}
		else {
			if (0 > x + dx && x + dx <= x_breite && 0 <= y + dy && y + dy <= y_hoehe) { //wenn 0>x+dx, alles andere so wie oben
				double anteil = -x / dx;
				dx = dx * anteil;
				dy = dy * anteil;
				x += dx;
				y += dy;
			}
			if (0 <= x + dx && x + dx > x_breite && 0 <= y + dy && y + dy <= y_hoehe) { //wenn x + dx > x_breite, alles andere so wie oben
				double anteil = (x_breite - x) / dx;
				dx = dx * anteil;
				dy = dy * anteil;
				x += dx;
				y += dy;
			}
			if (0 <= x + dx && x + dx <= x_breite && 0 > y + dy && y + dy <= y_hoehe) { //wenn 0 > y + dy, alles andere so wie oben
				double anteil = -y / dy;
				dx = dx * anteil;
				dy = dy * anteil;
				x += dx;
				y += dy;
			}
			if (0 <= x + dx && x + dx <= x_breite && 0 <= y + dy && y + dy > y_hoehe) { //wenn y + dy > y_hoehe, alles andere so wie oben
				double anteil = (y_hoehe - y) / dy;
				dx = dx * anteil;
				dy = dy * anteil;
				x += dx;
				y += dy;
			}
			ende_erreicht = true;
		}
	}
}
void Laser::set_x(float xgeg) {
	x = xgeg;
}
void Laser::set_y(float ygeg) {
	y = ygeg;
}
void Laser::set_x_start(float xgeg) {
	start_x = xgeg;
}
void Laser::set_y_start(float ygeg) {
	start_y = ygeg;
}
void Laser::set_schiesst(bool sch) {
	schiesst = sch;
}
void Laser::set_ende_erreicht(bool ende) {
	ende_erreicht = ende;
}
bool Laser::get_schiesst() const{
	return schiesst;
}
bool Laser::get_ende_erreicht() const {
	return ende_erreicht;
}
Laser::Laser(float x, float y, int winkel, float start_x, float start_y, bool schiesst, bool ende_erreicht): Objekt(x, y, winkel), start_x(start_x), start_y(start_y), schiesst(schiesst), ende_erreicht(ende_erreicht) {}



class GameWindow : public Gosu::Window
{
	Gosu::Image Ente;
	Charakter cha;																													// Erzeuge ein Objekt der Klasse Objekt
	Laser laser;
	
public:

	GameWindow()
		: Window(Gosu::screen_width(), Gosu::screen_height(), true),
		Ente("ente.png"),
		cha(100, 700, 0, 3),																											// Initialisiere das Objekt
		laser(502, 693, 0, 100, 100, false, false)
	{
		set_caption("Gamewindow");
	}

																																	// Wird bis zu 60x pro Sekunde aufgerufen.
																																	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
																																	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
		if (laser.get_schiesst()) {
			Gosu::Graphics::draw_line(
				laser.get_x_start(), laser.get_y_start(), Gosu::Color::WHITE,
				laser.get_x(), laser.get_y(), Gosu::Color::WHITE,
				0.0
			);
		}
		Ente.draw_rot(cha.get_x(), cha.get_y(), 0, cha.get_winkel(), 0.5, 0.5, scale_Ente, scale_Ente);
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
		else if (input().down(Gosu::KB_SPACE)) {
			if (!laser.get_schiesst()) {
				laser.set_x(cha.get_x());
				laser.set_x_start(cha.get_x());
				laser.set_y(cha.get_y());
				laser.set_y_start(cha.get_y());
				
			}
			laser.set_schiesst(true);
			double laserspeed = 1000.0;
			//laser.bewegen_y(Gosu::offset_y(cha.get_winkel(), laserspeed));		funktioniert nicht so gut
			//laser.bewegen_x(Gosu::offset_x(cha.get_winkel(), laserspeed));		funktioniert nicht so gut
			laser.bewegen(Gosu::offset_x(cha.get_winkel(), laserspeed), Gosu::offset_y(cha.get_winkel(), laserspeed));
			
		}
		else if (!input().down(Gosu::KB_SPACE)) {
			laser.set_schiesst(false);
			laser.set_ende_erreicht(false);
		}
	}
};

																																	// C++ Hauptprogramm
int main()
{
	
	GameWindow window;
	window.show();
}
