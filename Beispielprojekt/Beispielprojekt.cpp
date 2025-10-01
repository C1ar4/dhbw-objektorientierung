#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <string>			// hab ich noch eingefügt um den Winkel erstmal auszugeben, kann man dann auch grundsätzlich benutzen text auszugeben :)
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
		if (winkel <= 0) {									// hab hier noch eingebaut, dass der Winkel immer zwischen 0 und 360 Grad bleibt
			winkel += 360;
		}
		else if (winkel > 360) {
			winkel -= 360;
		}
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
void Laser::drehen(int dwinkel) {						// wird jetzt so gar im Moment gar nicht für den Laser gebraucht
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
float Laser::get_x_start() const{						// returnt den x-Startwert des Lasers
	return start_x;
}
float Laser::get_y_start() const {						// returnt den y-Startwert des Lasers
	return start_y;
}
void Laser::bewegen(float dx, float dy) {														// funktioniert besser als das getrennte bewegen in x und y Richtung (man kann sich überlegen, ob man versucht es bei allen Objekten zu ändern...)
	if (!ende_erreicht) {
		if (0 <= x + dx && x + dx <= x_breite && 0 <= y + dy && y + dy <= y_hoehe) {																											
			x += dx;
			y += dy;
		}
		else {																		// diese ganzen schleifen sogen dafür, dass der laser nicht komisch über den Bildschirm zuckt, wenn die Ente zu nah an der Wand ist
			if (0 > x + dx) { //wenn 0>x+dx
				double anteil = 0;
				double anteil_x = -x / dx;
				double anteil_y = 1;
				if (0 > y + dy) {
					anteil_y = -y / dy;
				}
				else if (y + dy > y_hoehe) {
					anteil_y = (y_hoehe - y) / dy;
				}
				if (anteil_x <= anteil_y) {
					anteil = anteil_x;
				}
				else {
					anteil = anteil_y;
				}
				dx = dx * anteil;
				dy = dy * anteil;
				x += dx;
				y += dy;
			}
			else if (x + dx > x_breite) { //wenn x + dx > x_breite
				double anteil = 0;
				double anteil_x = (x_breite - x) / dx;
				double anteil_y = 1;
				if (0 > y + dy) {
					anteil_y = -y / dy;
				}
				else if (y + dy > y_hoehe) {
					anteil_y = (y_hoehe - y) / dy;
				}
				if (anteil_x <= anteil_y) {
					anteil = anteil_x;
				}
				else {
					anteil = anteil_y;
				}
				dx = dx * anteil;
				dy = dy * anteil;
				x += dx;
				y += dy;
			}
			else if (0 > y + dy) { //wenn 0 > y + dy
				double anteil = 0;
				double anteil_x = 1;
				double anteil_y = -y / dy;
				if (0 > x + dx) {
					anteil_x = -x / dy;
				}
				else if (x + dx > x_breite) {
					anteil_x = (x_breite - x) / dy;
				}
				if (anteil_x <= anteil_y) {
					anteil = anteil_x;
				}
				else {
					anteil = anteil_y;
				}
				dx = dx * anteil;
				dy = dy * anteil;
				x += dx;
				y += dy;
			}
			else if (y + dy > y_hoehe) { //wenn y + dy > y_hoehe
				double anteil = 0;
				double anteil_x = 1;
				double anteil_y = (y_hoehe - y) / dy;
				if (0 > x + dx) {
					anteil_x = -x / dy;
				}
				else if (x + dx > x_breite) {
					anteil_x = (x_breite - x) / dy;
				}
				if (anteil_x <= anteil_y) {
					anteil = anteil_x;
				}
				else {
					anteil = anteil_y;
				}
				dx = dx * anteil;
				dy = dy * anteil;
				x += dx;
				y += dy;
			}  
			ende_erreicht = true;
		}
	}
}
void Laser::set_winkel(float w) {					
	winkel = w;
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
void Laser::set_ende_erreicht(bool ende) {				// diese Variable ende_erreicht sagt an, wenn der Laser die Wand erreicht hat und kann mit dieser Funktion gesetzt werden
	ende_erreicht = ende;
}
bool Laser::get_schiesst() const{						// die Variable schiesst sagt an, wann der Laser schießt und kann mit dieser Funktion gesetzt werden
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
	Gosu::Font font;				// erzeugt ein Text, der im Gamewindow ausgegeben werden kann
	
public:

	GameWindow()
		: Window(Gosu::screen_width(), Gosu::screen_height(), true),
		Ente("ente.png"),
		cha(50, 380, 90, 3),																											// Initialisiere das Objekt
		laser(502, 693, 0, 100, 100, false, false),
		font(20)						// 20 gibt die Textgroesse an
	{
		set_caption("Gamewindow");
	}

																																	// Wird bis zu 60x pro Sekunde aufgerufen.
																																	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
																																	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
		if (laser.get_schiesst()) {										// wenn der Laser schießen soll, werden 3 parallele linien als Laser ausgegeben
			Gosu::Graphics::draw_line(
				laser.get_x_start(), laser.get_y_start(), Gosu::Color::WHITE,
				laser.get_x(), laser.get_y(), Gosu::Color::WHITE,
				0.0
			);
			if (((90 <= laser.get_winkel()) && (laser.get_winkel() < 180)) || ((270 <= laser.get_winkel()) && (laser.get_winkel() < 360))) {
				Gosu::Graphics::draw_line(
					laser.get_x_start() + 1, laser.get_y_start() - 1, Gosu::Color::RED,
					laser.get_x() + 1, laser.get_y() - 1, Gosu::Color::RED,
					0.0
				);
				Gosu::Graphics::draw_line(
					laser.get_x_start() - 1, laser.get_y_start() + 1, Gosu::Color::RED,
					laser.get_x() - 1, laser.get_y() + 1, Gosu::Color::RED,
					0.0
				);
				font.draw_text(std::to_string(laser.get_winkel()), 100, 100, 0);    // muss man dann noch rausmachen, hatte ich ursprünglich drin, weil etwas nicht ganz funktioniert hat und ich rausfinden wollte warum
			}
			else {
				Gosu::Graphics::draw_line(
					laser.get_x_start() + 1, laser.get_y_start() + 1, Gosu::Color::RED,
					laser.get_x() + 1, laser.get_y() + 1, Gosu::Color::RED,
					0.0
				);
				Gosu::Graphics::draw_line(
					laser.get_x_start() - 1, laser.get_y_start() - 1, Gosu::Color::RED,
					laser.get_x() - 1, laser.get_y() - 1, Gosu::Color::RED,
					0.0
				);
				font.draw_text(std::to_string(laser.get_winkel()), 100, 100, 0);   //muss man dann noch rausmachen 
			}
		}
		Ente.draw_rot(cha.get_x(), cha.get_y(), 0, cha.get_winkel(), 0.5, 0.5, scale_Ente, scale_Ente);				// Ente nach Laser, sodass die Ente über dem laser liegt, so sieht es aus als schiesst sie aus ihrem Schnabel
	}

																																	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		if(input().down(Gosu::KB_LEFT)){																							// Bewege die Ente nach links
		
			cha.drehen(-5);
			laser.set_schiesst(false);					// sorgt dafür, dass der Laser weggeht, wenn die Ente sich weiterdreht
		}
		else if(input().down(Gosu::KB_RIGHT)){																						// Bewege die Ente nach rechts
			cha.drehen(5);
			laser.set_schiesst(false);					// sorgt dafür, dass der Laser weggeht, wenn die Ente sich weiterdreht
		}
		else if (input().down(Gosu::KB_UP)) {
			double speed = 5.0;
			cha.bewegen_x(Gosu::offset_x(cha.get_winkel(), speed));
			cha.bewegen_y(Gosu::offset_y(cha.get_winkel(), speed));
			laser.set_schiesst(false);					// sorgt dafür, dass der Laser weggeht, wenn die Ente sich weiterbewegt
		}
		else if (input().down(Gosu::KB_SPACE)) {
			if (!laser.get_schiesst()) {				// beim ersten Durchgang, nach drücken, werden die koordinaten der Ente dem laser übergeben
				laser.set_x(cha.get_x());
				laser.set_x_start(cha.get_x());
				laser.set_y(cha.get_y());
				laser.set_y_start(cha.get_y());
				
			}
			laser.set_schiesst(true);
			laser.set_winkel(cha.get_winkel());
			double laserspeed = 1000.0;
			//laser.bewegen_y(Gosu::offset_y(cha.get_winkel(), laserspeed));		funktioniert nicht so gut
			//laser.bewegen_x(Gosu::offset_x(cha.get_winkel(), laserspeed));		funktioniert nicht so gut
			laser.bewegen(Gosu::offset_x(cha.get_winkel(), laserspeed), Gosu::offset_y(cha.get_winkel(), laserspeed));		// Senden des Lasers bis zum Rand
			
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
