#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <string>			// hab ich noch eingefügt um den Winkel erstmal auszugeben, kann man dann auch grundsätzlich benutzen text auszugeben :)
#include "Objekte_deklarieren.h"
#include <vector>
using namespace std;


int speed_drehen_ente = 1;
int x_breite = Gosu::screen_width() - 0;
int y_hoehe = Gosu::screen_height() - 0;
float scale_Ente = 0.1;
float scale_Baum = 0.15;
float scale_Stein = 0.1;
int FPS = 30;			//30, weil die Zeitmessung immer nur alle 500ms aktualisiert wird
int FPS_counter = 0;
unsigned long long  dzeit_start = 0;
unsigned long long  dzeit = 0;
int updates_per_frame = 4;			// wie oft die Update Funktion pro Frame aufgerufen wird, damit die Bewegung der Objekte nicht zu ruckelig wirkt



void draw_bäume(vector<Baum>& vector_baum, Gosu::Image& baum);
void ueberprüfe_kollision_baum(vector<Baum>& vector_baum, Laser& laser);

void draw_steine(vector<Stein>& vector_stein, Gosu::Image& stein);
void ueberprüfe_kollision_stein(vector<Stein>& vector_stein, Laser& laser);


class GameWindow : public Gosu::Window
{
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
		: Window(Gosu::screen_width(), Gosu::screen_height(), true),
		Ente("ente.png"),
		cha(50, 380, 90, 3),
		baum ("Baum.png"),
		stein("Stein.png"),														
		laser(502, 693, 0, 100, 100, false, false),
		font(20)							// 20 gibt die Textgroesse an
	{
		set_caption("Gamewindow");
		vector_baum.push_back(Baum(300, 300, 0, 40, 85));
		vector_stein.push_back(Stein(600, 300, 0, 50, 40));
	}

																																	// Wird bis zu 60x pro Sekunde aufgerufen.
																																	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
																																	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
		FPS_counter++;													//FPS Anzeige
		if(Gosu::milliseconds() - dzeit_start >= 500) {
			dzeit_start = Gosu::milliseconds();
			FPS = FPS_counter;
			FPS_counter = 0;
		}
		font.draw_text(std::to_string(FPS * 2) + "  FPS", 10, 10, 0);
		font.draw_text(std::to_string(Gosu::milliseconds() - dzeit) + "  ms", 10, 25, 0);				// Zeigt die Zeit zwischen 2 Frames an
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
				font.draw_text(std::to_string(laser.get_winkel()) + "  Winkel Laser", 150, 10, 0);    // muss man dann noch rausmachen, hatte ich ursprünglich drin, weil etwas nicht ganz funktioniert hat und ich rausfinden wollte warum
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
				font.draw_text(std::to_string(laser.get_winkel()) + "  Winkel Laser", 150, 10, 0);   //muss man dann noch rausmachen 
			}
		}

		draw_bäume(vector_baum, baum);			// zeichnet alle Bäume aus dem Vektor
		draw_steine(vector_stein, stein);		// zeichnet alle Steine aus dem Vektor
		Ente.draw_rot(cha.get_x(), cha.get_y(), 0, cha.get_winkel(), 0.5, 0.5, scale_Ente, scale_Ente);				// Ente nach Laser, sodass die Ente über dem laser liegt, so sieht es aus als schiesst sie aus ihrem Schnabel
	}

																																	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{ 
		for (int i = 0; i < updates_per_frame; i++) {
		if(input().down(Gosu::KB_LEFT)){																							// Bewege die Ente nach links
		
			cha.drehen(-(speed_drehen_ente));
			laser.set_schiesst(false);					// sorgt dafür, dass der Laser weggeht, wenn die Ente sich weiterdreht
		}
		else if(input().down(Gosu::KB_RIGHT)){																						// Bewege die Ente nach rechts
			cha.drehen(speed_drehen_ente);
			laser.set_schiesst(false);					// sorgt dafür, dass der Laser weggeht, wenn die Ente sich weiterdreht
		}
		else if (input().down(Gosu::KB_UP)) {
			double speed = 5.0/updates_per_frame;
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
			//laser.bewegen_y(Gosu::offset_y(cha.get_winkel(), laserspeed));		funktioniert nicht so gut
			//laser.bewegen_x(Gosu::offset_x(cha.get_winkel(), laserspeed));		funktioniert nicht so gut
			
			ueberprüfe_kollision_baum(vector_baum, laser);											// überprüft ob der Laser ein Obejrkt (Baum/Stein) trifft
			ueberprüfe_kollision_stein(vector_stein, laser);										// überprüft ob der Laser ein Obejrkt (Baum/Stein) trifft
			
			double laserspeed = 50.0 / updates_per_frame;		//muss aktuell so "langsam" sein, weil der Laser sonst teils durch die Objekte hindurch schießen kann
			laser.bewegen(Gosu::offset_x(cha.get_winkel(), laserspeed), Gosu::offset_y(cha.get_winkel(), laserspeed));		// Senden des Lasers bis zum Rand
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


void draw_bäume(vector<Baum>& vector_baum, Gosu::Image& baum) {
	for (auto it = vector_baum.begin(); it != vector_baum.end(); ++it) {
		baum.draw_rot(it->get_x(), it->get_y(), 0, it->get_winkel(), 0.5, 0.5, scale_Baum, scale_Baum);
	}
}

void ueberprüfe_kollision_baum(vector<Baum>& vector_baum, Laser& laser) {																			// Diese Funktion soll überprüfen, ob der Laser einen Baum oder Stein trifft
	for (auto it = vector_baum.begin(); it != vector_baum.end(); ++it) {
		if (it->get_x() - it->get_groesse_x() <= laser.get_x() && laser.get_x() <= it->get_x() + it->get_groesse_x() &&
			it->get_y() - it->get_groesse_y() <= laser.get_y() && laser.get_y() <= it->get_y() + it->get_groesse_y()) {		        // wenn der Laser den Baum trifft, hört er auf zu schießen
			laser.set_ende_erreicht(true);
		}
	}
}

void draw_steine(vector<Stein>& vector_stein, Gosu::Image& stein) {
	for (auto it = vector_stein.begin(); it != vector_stein.end(); ++it) {
		stein.draw_rot(it->get_x(), it->get_y(), 0, it->get_winkel(), 0.5, 0.5, scale_Stein, scale_Stein);
	}
}

void ueberprüfe_kollision_stein(vector<Stein>& vector_stein, Laser& laser) {																			// Diese Funktion soll überprüfen, ob der Laser einen Baum oder Stein trifft
	for (auto it = vector_stein.begin(); it != vector_stein.end(); ++it) {
		if (it->get_x() - it->get_groesse_x() <= laser.get_x() && laser.get_x() <= it->get_x() + it->get_groesse_x() &&
			it->get_y() - it->get_groesse_y() <= laser.get_y() && laser.get_y() <= it->get_y() + it->get_groesse_y()) {		        // wenn der Laser den Stein trifft, hört er auf zu schießen
			laser.set_ende_erreicht(true);
		}
	}
}