#include "Objekte_deklarieren.h"


extern int x_breite;
extern int y_hoehe;



Objekt::Objekt(float x, float y, int winkel) {
	this->x = x;
	this->y = y;
	this->winkel = winkel;
}
void Charakter::bewegen_x(float dx) {
	if (0 < x && x < x_breite) {																											//Begrenzung in x-Richtung
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
float Charakter::get_groesse_x() const {
	return groesse_x;
}
float Charakter::get_groesse_y() const {
	return groesse_y;
}
void Charakter::set_bewegen(bool bewegtsich) {
	bewegen = bewegtsich;
}
bool Charakter::get_bewegen() const {
	return bewegen;
}
Charakter::Charakter(float x, float y, int winkel, int leben, float groesse_x, float groesse_y, bool bewegen) : Objekt(x, y, winkel), leben(leben), groesse_x(groesse_x), groesse_y(groesse_y), bewegen(bewegen) {}



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
		if (0 <= y + dy && y + dy <= y_hoehe) {																											//nicht benutzt, ist auch nicht ganz zielführend
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
float Laser::get_x_start() const {						// returnt den x-Startwert des Lasers
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
bool Laser::get_schiesst() const {						// die Variable schiesst sagt an, wann der Laser schießt und kann mit dieser Funktion gesetzt werden
	return schiesst;
}
bool Laser::get_ende_erreicht() const {
	return ende_erreicht;
}
Laser::Laser(float x, float y, int winkel, float start_x, float start_y, bool schiesst, bool ende_erreicht) : Objekt(x, y, winkel), start_x(start_x), start_y(start_y), schiesst(schiesst), ende_erreicht(ende_erreicht) {}



void Baum::bewegen_x(float dx) {
	if (0 < x && x < x_breite) {																											//Begrenzung in x-Richtung
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
void Baum::bewegen_y(float dy) {
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
void Baum::drehen(int dwinkel) {
	winkel += dwinkel;
	if (winkel <= 0) {									// hab hier noch eingebaut, dass der Winkel immer zwischen 0 und 360 Grad bleibt
		winkel += 360;
	}
	else if (winkel > 360) {
		winkel -= 360;
	}
}
float Baum::get_x() const {
	return x;
}
float Baum::get_y() const {
	return y;
}
int Baum::get_winkel() const {
	return winkel;
}
float Baum::get_groesse_x() const {
	return groesse_x;
}
float Baum::get_groesse_y() const {
	return groesse_y;
}



void Stein::bewegen_x(float dx) {
	if (0 < x && x < x_breite) {																											//Begrenzung in x-Richtung
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
void Stein::bewegen_y(float dy) {
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
void Stein::drehen(int dwinkel) {
	winkel += dwinkel;
	if (winkel <= 0) {									// hab hier noch eingebaut, dass der Winkel immer zwischen 0 und 360 Grad bleibt
		winkel += 360;
	}
	else if (winkel > 360) {
		winkel -= 360;
	}
}
float Stein::get_x() const {
	return x;
}
float Stein::get_y() const {
	return y;
}
int Stein::get_winkel() const {
	return winkel;
}
float Stein:: get_groesse_x() const {
	return groesse_x;
}
float Stein:: get_groesse_y() const {
	return groesse_y;
}