#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <string>			// hab ich noch eingefügt um den Winkel erstmal auszugeben, kann man dann auch grundsätzlich benutzen text auszugeben :)
#include "Objekte_deklarieren.h"
#include <vector>
#include <cmath>
#include <iostream>  //ich hatte mal was damit versucht, hat aber nicht funktioniert. Insofern kann das auch eigentlich wieder weg
using namespace std;

bool Fenster = false;		// true = Vollbild, false = Fenster
int x_breite = 1920;
int y_hoehe = 1080;

int speed_drehen_ente = 1;
float scale_Ente = 0.1;
float scale_Baum = 0.15;
float scale_Stein = 0.1;
float scale_Karte_x = 1.3;
float scale_Karte_y = 1.1;

int FPS = 30;						//30, weil die Zeitmessung immer nur alle 500ms aktualisiert wird
int FPS_counter = 0;
unsigned long long  dzeit_start = 0;
unsigned long long  dzeit = 0;
int updates_per_frame = 4;			// wie oft die Update Funktion pro Frame aufgerufen wird, damit die Bewegung der Objekte nicht zu ruckelig wirkt



void draw_bäume(vector<Baum>& vector_baum, Gosu::Image& baum);
void ueberprüfe_kollision_baum_laser(vector<Baum>& vector_baum, Laser& laser);
void ueberprüfe_kollision_baum_character(vector<Baum>& vector_baum, Charakter& ente);


void draw_steine(vector<Stein>& vector_stein, Gosu::Image& stein);
void ueberprüfe_kollision_stein_laser(vector<Stein>& vector_stein, Laser& laser);
void ueberprüfe_kollision_stein_character(vector<Stein>& vector_stein, Charakter& ente);



class GameWindow : public Gosu::Window
{	
	Gosu::Image Karte;
	Gosu::Image Ente;
	Gosu::Image baum;
	vector<Baum> vector_baum;
	Gosu::Image stein;
	vector<Stein> vector_stein;
	Charakter cha;																													
	Laser laser;
	Gosu::Font font;				// erzeugt ein Text, der im Gamewindow ausgegeben werden kann
	
public:

	GameWindow()
		: Window(x_breite, y_hoehe, Fenster),
		Karte("Karte.png"),
		Ente("ente.png"),
		cha(50, 380, 90, 3, 35, 35, true),
		baum ("Baum.png"),
		stein("Stein.png"),														
		laser(502, 693, 0, 100, 100, false, false),
		font(20)							// 20 gibt die Textgroesse an
	{
		set_caption("Gamewindow");
		vector_baum.push_back(Baum(200, 150, 0, 20, 70));
		vector_baum.push_back(Baum(300, 100, 0, 20, 70));
		vector_baum.push_back(Baum(225, 800, 0, 20, 70));
		vector_baum.push_back(Baum(800, 900, 0, 20, 70));
		vector_baum.push_back(Baum(600, 400, 0, 20, 70));
		vector_baum.push_back(Baum(1700, 900, 0, 20, 70));
		vector_baum.push_back(Baum(1800, 800, 0, 20, 70));
		vector_baum.push_back(Baum(1830, 930, 0, 20, 70));
		vector_baum.push_back(Baum(1750, 150, 0, 20, 70));
		vector_stein.push_back(Stein(600, 100, 0, 35, 35));
		vector_stein.push_back(Stein(800, 450, 0, 35, 35));
		vector_stein.push_back(Stein(250, 930, 0, 35, 35));
		vector_stein.push_back(Stein(240, 985, 0, 35, 35));
		vector_stein.push_back(Stein(1300, 380, 0, 35, 35));
		vector_stein.push_back(Stein(1220, 400, 0, 35, 35));
		vector_stein.push_back(Stein(1100, 600, 0, 35, 35));
		vector_stein.push_back(Stein(1650, 610, 0, 35, 35));
		vector_stein.push_back(Stein(1800, 400, 0, 35, 35));
		vector_stein.push_back(Stein(1500, 950, 0, 35, 35));
	}

																				// Wird bis zu 60x pro Sekunde aufgerufen.
																				// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
																				// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
		Karte.draw_rot(x_breite/2, y_hoehe/2, 0, 0, 0.5, 0.5, scale_Karte_x, scale_Karte_y);
		FPS_counter++;													//FPS Anzeige
		if(Gosu::milliseconds() - dzeit_start >= 500) {
			dzeit_start = Gosu::milliseconds();
			FPS = FPS_counter;
			FPS_counter = 0;
		}
		font.draw_text(to_string(FPS * 2) + "  FPS", 10, 10, 0);
		font.draw_text(to_string(Gosu::milliseconds() - dzeit) + "  ms" + to_string(y_hoehe), 10, 25, 0);				// Zeigt die Zeit zwischen 2 Frames an
		dzeit = Gosu::milliseconds();
		


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
				font.draw_text(to_string(laser.get_winkel()) + "  Winkel Laser", 150, 10, 0);    // muss man dann noch rausmachen, hatte ich ursprünglich drin, weil etwas nicht ganz funktioniert hat und ich rausfinden wollte warum
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
				font.draw_text(to_string(laser.get_winkel()) + "  Winkel Laser", 150, 10, 0);   //muss man dann noch rausmachen 
			}
		}
		font.draw_text(to_string(cha.winkel_zu_stein) + ", " + to_string(cha.get_winkel()) + ", " + to_string(cha.winkeldiff_zum_stein) + ", " + to_string(cha.abstand_stein), 150, 20, 0);  //ist nur zur Fehlersuche drin. kann also auch wieder raus
		font.draw_text(to_string(cha.test), 150, 40, 0); //hier genauso
		draw_bäume(vector_baum, baum);			// zeichnet alle Bäume aus dem Vektor
		draw_steine(vector_stein, stein);		// zeichnet alle Steine aus dem Vektor
		Ente.draw_rot(cha.get_x(), cha.get_y(), 0, cha.get_winkel(), 0.5, 0.5, scale_Ente, scale_Ente);				// Ente nach Laser, sodass die Ente über dem laser liegt, so sieht es aus als schiesst sie aus ihrem Schnabel
	}

															// Wird 60x pro Sekunde aufgerufen
	void update() override
	{ 
		for (int i = 0; i < updates_per_frame; i++) {		//läuft durch die Schleife, damit die Eingaben öfter pro Frame überprüft werden
			if(input().down(Gosu::KB_LEFT)){				// Bewege die Ente nach links
		
				cha.drehen(-(speed_drehen_ente));
				laser.set_schiesst(false);					// sorgt dafür, dass der Laser weggeht, wenn die Ente sich weiterdreht
			}
			if(input().down(Gosu::KB_RIGHT)){			// Bewege die Ente nach rechts													ich hab hier das else if zu einem if gemacht. ich finds besser wenn sich die Ente auch während dem Bewegen drehen kann
				cha.drehen(speed_drehen_ente);
				laser.set_schiesst(false);					// sorgt dafür, dass der Laser weggeht, wenn die Ente sich weiterdreht
			}
			if (input().down(Gosu::KB_UP)) {
				ueberprüfe_kollision_stein_character(vector_stein, cha);							// hier auch das else if mit if ausgetauscht, gleiche Begründung wie oben
				ueberprüfe_kollision_baum_character(vector_baum, cha);
				if (cha.get_bewegen()) {
					double speed = 5.0 / updates_per_frame;
					cha.bewegen_x(Gosu::offset_x(cha.get_winkel(), speed));
					cha.bewegen_y(Gosu::offset_y(cha.get_winkel(), speed));
				}
				
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
				//laser.bewegen_y(Gosu::offset_y(cha.get_winkel(), laserspeed));		funktioniert nicht so gut
				//laser.bewegen_x(Gosu::offset_x(cha.get_winkel(), laserspeed));		funktioniert nicht so gut
			
				for (int laserspeed = 1000; laserspeed > 0; laserspeed--) {										// Schleife, damtit der Laser nicht durch Ojekte glitcht
				ueberprüfe_kollision_baum_laser(vector_baum, laser);													// überprüft ob der Laser ein Obejrkt (Baum) trifft
				ueberprüfe_kollision_stein_laser(vector_stein, laser);												// überprüft ob der Laser ein Obejrkt (Stein) trifft
				laser.bewegen(Gosu::offset_x(cha.get_winkel(), 1), Gosu::offset_y(cha.get_winkel(), 1));		// Senden des Lasers bis zum Rand
				}
			}

			else if (!input().down(Gosu::KB_SPACE)) {
			laser.set_schiesst(false);
			laser.set_ende_erreicht(false);
			}
		}
	}
};

						// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
}

																						//Definitioon der Funktionen
void draw_bäume(vector<Baum>& vector_baum, Gosu::Image& baum) {
	for (auto it = vector_baum.begin(); it != vector_baum.end(); ++it) {
		baum.draw_rot(it->get_x(), it->get_y(), 0, it->get_winkel(), 0.5, 0.5, scale_Baum, scale_Baum);
	}
}

void ueberprüfe_kollision_baum_laser(vector<Baum>& vector_baum, Laser& laser) {																			// Diese Funktion soll überprüfen, ob der Laser einen Baum oder Stein trifft
	for (auto it = vector_baum.begin(); it != vector_baum.end(); ++it) {
		if (it->get_x() - it->get_groesse_x() <= laser.get_x() && laser.get_x() <= it->get_x() + it->get_groesse_x() &&
			it->get_y() - it->get_groesse_y() <= laser.get_y() && laser.get_y() <= it->get_y() + it->get_groesse_y()) {		        // wenn der Laser den Baum trifft, hört er auf zu schießen
			laser.set_ende_erreicht(true);
		}
	}
}

void ueberprüfe_kollision_baum_character(vector<Baum>& vector_baum, Charakter& ente) {
	/*float xabstand;
	float xabstandwinkel;
	float yabstand;
	float yabstandwinkel;
	for (auto it = vector_baum.begin(); it != vector_baum.end(); ++it) {
		xabstand = abs(ente.get_x() - it->get_x());
		xabstandwinkel = abs(ente.get_x() - 5 * cos(ente.get_winkel() * 3.14159 / 180) - it->get_x());
		yabstand = abs(ente.get_y() - it->get_y());
		yabstandwinkel = abs(ente.get_y() + ente.get_groesse_y() * sin(ente.get_winkel() * 3.14159 / 180) - it->get_y());
		if ((xabstand < 70) && (yabstand < 70) && xabstandwinkel <= xabstand && yabstandwinkel <= yabstand) {
			return false;
		}
	}
	return true;*/

	float xabstand;
	float yabstand;
	float abstand;
	int winkel;
	int winkeldifferenz = 0;
	for (auto it = vector_baum.begin(); it != vector_baum.end(); ++it) {
		xabstand = fabs(ente.get_x() - it->get_x());
		yabstand = fabs(ente.get_y() - it->get_y());
		abstand = sqrtf(xabstand * xabstand + yabstand * yabstand);
		if (abstand < 85) {
			winkel = atan2(it->get_y() - ente.get_y(), it->get_x() - ente.get_x()) * (180.0 / M_PI);
			winkel = winkel + 90;
			if (winkel < 0) {
				winkel = winkel + 360;
			}
			winkeldifferenz = abs(ente.get_winkel() - winkel);
			if (winkeldifferenz > 180) {
				winkeldifferenz = 360 - winkeldifferenz;
			}
			if (winkeldifferenz > 45) {
				ente.set_bewegen(true);
				ente.test = 1;
			}
			else {
				ente.set_bewegen(false);
				ente.test = 0;
			}

			ente.winkel_zu_stein = winkel;
			ente.winkeldiff_zum_stein = winkeldifferenz;
			ente.abstand_stein = abstand;
		}
	}

}

void draw_steine(vector<Stein>& vector_stein, Gosu::Image& stein) {
	for (auto it = vector_stein.begin(); it != vector_stein.end(); ++it) {
		stein.draw_rot(it->get_x(), it->get_y(), 0, it->get_winkel(), 0.5, 0.5, scale_Stein, scale_Stein);
	}
}

void ueberprüfe_kollision_stein_laser(vector<Stein>& vector_stein, Laser& laser) {																			// Diese Funktion soll überprüfen, ob der Laser einen Baum oder Stein trifft
	for (auto it = vector_stein.begin(); it != vector_stein.end(); ++it) {
		if (it->get_x() - it->get_groesse_x() <= laser.get_x() && laser.get_x() <= it->get_x() + it->get_groesse_x() &&
			it->get_y() - it->get_groesse_y() <= laser.get_y() && laser.get_y() <= it->get_y() + it->get_groesse_y()) {		        // wenn der Laser den Stein trifft, hört er auf zu schießen
			laser.set_ende_erreicht(true);
		}
	}
}

void ueberprüfe_kollision_stein_character(vector<Stein>& vector_stein, Charakter& ente) {
	float xabstand;
	float yabstand;
	float abstand;
	int winkel;
	int winkeldifferenz=0;
	for (auto it = vector_stein.begin(); it != vector_stein.end(); ++it) {
		xabstand = fabs(ente.get_x() - it->get_x());
		yabstand = fabs(ente.get_y() - it->get_y());
		abstand = sqrtf(xabstand * xabstand + yabstand * yabstand);
		if (abstand < 70) {
			winkel = atan2(it->get_y() - ente.get_y(), it->get_x() - ente.get_x()) * (180.0 / M_PI);
			winkel = winkel + 90;
			if (winkel < 0) {
				winkel = winkel + 360;
			}
			winkeldifferenz = abs(ente.get_winkel() - winkel);
			if (winkeldifferenz > 180) {
				winkeldifferenz = 360 - winkeldifferenz;
			}
			if (winkeldifferenz > 45) {
				ente.set_bewegen(true);
				ente.test = 1;
			}
			else {
				ente.set_bewegen(false);
				ente.test = 0;
			}
			
			ente.winkel_zu_stein = winkel;
			ente.winkeldiff_zum_stein = winkeldifferenz;
			ente.abstand_stein = abstand;
		}
	}
	
}