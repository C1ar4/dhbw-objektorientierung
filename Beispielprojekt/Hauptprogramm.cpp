#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <string>			// hab ich noch eingefügt um den Winkel erstmal auszugeben, kann man dann auch grundsätzlich benutzen text auszugeben :)
#include "Objekte_deklarieren.h"
#include <vector>
#include <cmath>
#include <fstream>
#include <cstdlib>
using namespace std;

bool Fenster = false;		// true = Vollbild, false = Fenster
int x_breite = 1920;
int y_hoehe = 1080;

bool startbildschirm = true;
bool anfang_spiel = true;
int Laser_delay = 12;					//Delay, damit Laser nicht zu lang bleibt, nachdem geschossen wurde
int Laser_dalay_cpy = Laser_delay;;
int ges_highscore = 0;

int speed_drehen_ente = 1;
float scale_Ente = 0.1;
float scale_fisch = 0.08;
float scale_Baum = 0.15;
float scale_Stein = 0.1;
float scale_Karte_x = 1.3;
float scale_Karte_y = 1.1;

int FPS = 30;						//30, weil die Zeitmessung immer nur alle 500ms aktualisiert wird
int FPS_counter = 0;
unsigned long long  dzeit_start = 0;
unsigned long long  dzeit = 0;
int updates_per_frame = 4;			// wie oft die Update Funktion pro Frame aufgerufen wird, damit die Bewegung der Objekte nicht zu ruckelig wirkt

vector<vector<float>> startpunkte_gegner = { {1550, 100}, {390, 1000}, {700, 670}, {1070, 370}, {1100, 300}, {1200, 200},  {400, 900},  {550, 850}};
int counter = 60;
int counter2 = 120;


void ueberprüfe_kollision_character_gegner(Charakter& gegner, Charakter& ente);
void überprüfe_kollision_character_laser(Charakter& gegner, Laser& laser, Spieldaten& spieldaten);

void draw_bäume(vector<Baum>& vector_baum, Gosu::Image& baum);
void ueberprüfe_kollision_baum_laser(vector<Baum>& vector_baum, Laser& laser);
void ueberprüfe_kollision_baum_character(vector<Baum>& vector_baum, Charakter& ente);


void draw_steine(vector<Stein>& vector_stein, Gosu::Image& stein);
void ueberprüfe_kollision_stein_laser(vector<Stein>& vector_stein, Laser& laser);
void ueberprüfe_kollision_stein_character(vector<Stein>& vector_stein, Charakter& ente);



class GameWindow : public Gosu::Window
{	
	Gosu::Image Karte;
	Gosu::Image Startbildschirm;
	Gosu::Image Laserduck;
	Gosu::Image Druecke_Return_um;
	Gosu::Image Untertitel;
	Gosu::Image Ente;
	Gosu::Image baum;
	vector<Baum> vector_baum;
	Gosu::Image stein;
	vector<Stein> vector_stein;
	Charakter cha;
	Gosu::Image Gegner;
	vector<Charakter> vector_fisch;
	Laser laser;
	Gosu::Font font;				// erzeugt ein Text, der im Gamewindow ausgegeben werden kann
	Gosu::Font font_groß;
	Spieldaten spieldaten;
	Gosu::Image herz;
	Gosu::Sample laser_schuss_ton;
	Gosu::Sample game_over_ton;
	Gosu::Image game_over_bild;
	
public:

	GameWindow()
		: Window(x_breite, y_hoehe, Fenster),
		Karte("Karte.png"),
		Startbildschirm("Startbildschirm.png"),
		Laserduck("Laserduck.png"),
		Druecke_Return_um("Druecke_Return_um.png"),
		Untertitel("Untertitel.png"),
		Ente("Ente.png"),
		cha(50, 380, 90, 3, 35, 35, true, true),
		baum("Baum.png"),
		stein("Stein.png"),
		Gegner("Gegner_links.png"),
		laser(502, 693, 0, 100, 100, false, false),
		spieldaten(0, 1, 0, 0),
		herz("Herz.png"),
		font(20),						// 20 gibt die Textgroesse an
		font_groß(35),					// 50 gibt die Textgroesse an
		laser_schuss_ton("Laser_schuss.mp3"),
		game_over_ton("Game_over.mp3"),
		game_over_bild("Game_over_bild.png")
	{
		set_caption("Gamewindow");
		vector_baum.push_back(Baum(200, 150, 0, 20, 70));
		vector_baum.push_back(Baum(300, 100, 0, 20, 70));
		vector_baum.push_back(Baum(225, 800, 0, 20, 70));
		vector_baum.push_back(Baum(800, 900, 0, 20, 70));
		vector_baum.push_back(Baum(600, 400, 0, 20, 70));
		vector_baum.push_back(Baum(1850, 900, 0, 20, 70));
		vector_baum.push_back(Baum(1600, 700, 0, 20, 70));
		vector_baum.push_back(Baum(1650, 930, 0, 20, 70));
		vector_baum.push_back(Baum(1850, 150, 0, 20, 70));
		vector_baum.push_back(Baum(1400, 400, 0, 20, 70));
		vector_baum.push_back(Baum(1200, 600, 0, 20, 70));
		vector_stein.push_back(Stein(600, 100, 0, 35, 35));
		vector_stein.push_back(Stein(800, 450, 0, 35, 35));
		vector_stein.push_back(Stein(250, 930, 0, 35, 35));
		vector_stein.push_back(Stein(500, 985, 0, 35, 35));
		vector_stein.push_back(Stein(1100, 100, 0, 35, 35));
		vector_stein.push_back(Stein(1000, 500, 0, 35, 35));
		vector_stein.push_back(Stein(1100, 350, 0, 35, 35));
		vector_stein.push_back(Stein(1720, 610, 0, 35, 35));
		vector_stein.push_back(Stein(1800, 300, 0, 35, 35));
		vector_stein.push_back(Stein(1500, 950, 0, 35, 35));
		vector_fisch.push_back(Charakter(1000, 200, 0, 3, 35, 35, true, false));
	}

																				// Wird bis zu 60x pro Sekunde aufgerufen.
																				// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
																				// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{	
		if (startbildschirm) {
			if (anfang_spiel) {
				spieldaten.set_ges_highscore(ges_highscore);
				Startbildschirm.draw(0, 0, 0, (double)x_breite / Startbildschirm.width(), (double)y_hoehe / Startbildschirm.height());
				Laserduck.draw(x_breite / 2 -400, -150, 0, 0.8, 0.6);
				Druecke_Return_um.draw(1470, 750, 0, 0.3, 0.3);
				Untertitel.draw(50, 250, 0, 0.35, 0.35);
				if (input().down(Gosu::KB_RETURN)) {
					startbildschirm = false;
					anfang_spiel = false;
				}
			}
			else {
				game_over_bild.draw(0, 0, 0, (double)x_breite / Startbildschirm.width(), (double)y_hoehe / game_over_bild.height());
				Druecke_Return_um.draw(50, 750, 0, 0.2, 0.2);
				font_groß.draw_text("Aktueller Score:", 1475, 680, 0, 0.8, 0.8, Gosu::Color::BLACK);
				font_groß.draw_text(to_string(spieldaten.get_punkte()), 1500, 730, 0, 0.8, 0.8, Gosu::Color::BLACK);
				font_groß.draw_text("Aktueller Highscore:", 1475, 780, 0, 0.8, 0.8, Gosu::Color::BLACK);
				font_groß.draw_text(to_string(spieldaten.get_highscore()), 1500, 830, 0, 0.8, 0.8, Gosu::Color::BLACK);
				font_groß.draw_text("Insgesamter Highscore:", 1475, 880, 0, 0.8, 0.8, Gosu::Color::BLACK);
				font_groß.draw_text(to_string(spieldaten.get_ges_highscore()), 1500, 930, 0, 0.8, 0.8, Gosu::Color::BLACK);


				if (counter2 <= 0) {
					if (input().down(Gosu:: KB_RETURN)) {

						spieldaten.set_punkte(0);
						spieldaten.set_level(1);
						if (vector_fisch.size() > 1) {
							vector_fisch.erase(vector_fisch.begin() + 1, vector_fisch.end());
						}
						startbildschirm = false;

					}
				}
				else {
					counter2 = counter2 - 1;
				}
				
			}
		}

		if (input().down(Gosu::KB_ESCAPE)) {	
			ges_highscore = spieldaten.get_ges_highscore();
			close();
		}

		if (!startbildschirm && cha.get_leben() <= 0) {						// wenn die Ente keine Leben mehr hat, wird die Runde beendet
			if (spieldaten.get_punkte() > spieldaten.get_highscore()) {
				spieldaten.set_highscore(spieldaten.get_punkte());
			}
			if (spieldaten.get_highscore() > spieldaten.get_ges_highscore()) {
				spieldaten.set_ges_highscore(spieldaten.get_highscore());
			}
			counter2 = 120;
			cha.set_leben(3);
			startbildschirm = true;
			game_over_ton.play();
		}

		if(!startbildschirm) {
		Karte.draw_rot(x_breite/2, y_hoehe/2, 0, 0, 0.5, 0.5, scale_Karte_x, scale_Karte_y);
		}

		FPS_counter++;			//FPS Anzeige
		if(Gosu::milliseconds() - dzeit_start >= 500) {
			dzeit_start = Gosu::milliseconds();
			FPS = FPS_counter;
			FPS_counter = 0;
		}
		font.draw_text(to_string(FPS * 2) + "  FPS", 10, 10, 0);
		font.draw_text(to_string(Gosu::milliseconds() - dzeit) + "  ms  Aufloesung:  " + to_string(y_hoehe), 10, 25, 0);		// Zeigt die Zeit zwischen 2 Frames an
		dzeit = Gosu::milliseconds();


		if(!startbildschirm) {

			if (laser.get_schiesst() && Laser_delay >= 0) {										// wenn der Laser schießen soll, werden 3 parallele linien als Laser ausgegeben
				Laser_delay--;
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
					//font.draw_text(to_string(laser.get_winkel()) + "  Winkel Laser", 150, 10, 0);   //muss man dann noch rausmachen 
				}
			}
			if (!laser.get_schiesst()) {
				Laser_delay = Laser_dalay_cpy;
			}
		
			font_groß.draw_text("Punkte:   " + to_string(spieldaten.get_punkte()), 850, 20, 0);		//zeigt die Punkte an
			font_groß.draw_text("Level:   " + to_string(spieldaten.get_level()/4), 1700, 20, 0);	//zeigt das Level an
			

			draw_bäume(vector_baum, baum);			// zeichnet alle Bäume aus dem Vektor
			draw_steine(vector_stein, stein);		// zeichnet alle Steine aus dem Vektor
			for (auto it = vector_fisch.begin(); it < vector_fisch.end(); it++) {	// läuft durch den Vektor mit den Fischen
				if (it->get_existiert()) {
					Gegner.draw_rot(it->get_x(), it->get_y(), 0, it->get_winkel()+90, 0.5, 0.5, scale_fisch, scale_fisch);
					if (it->get_leben() == 3) {	
						herz.draw(it->get_x() - 40, it->get_y() - 70, 1, 0.03, 0.03);
						herz.draw(it->get_x() - 15, it->get_y() - 70, 1, 0.03, 0.03);
						herz.draw(it->get_x() + 10, it->get_y() - 70, 1, 0.03, 0.03);
					}
					else if (it->get_leben() == 2) {
						herz.draw(it->get_x() - 30, it->get_y() - 70, 1, 0.03, 0.03);
						herz.draw(it->get_x(), it->get_y() - 70, 1, 0.03, 0.03);
					}
					else if (it->get_leben() == 1) {
						herz.draw(it->get_x() - 15, it->get_y() - 70, 1, 0.03, 0.03);
					}
				}
			}
			Ente.draw_rot(cha.get_x(), cha.get_y(), 0, cha.get_winkel(), 0.5, 0.5, scale_Ente, scale_Ente);				// Ente nach Laser, sodass die Ente über dem laser liegt, so sieht es aus als schiesst sie aus ihrem Schnabel
			if (cha.get_leben() == 3) {	
				herz.draw(cha.get_x() - 40, cha.get_y() - 75, 1, 0.03, 0.03);
				herz.draw(cha.get_x() - 15, cha.get_y() - 75, 1, 0.03, 0.03);
				herz.draw(cha.get_x() + 10, cha.get_y() - 75, 1, 0.03, 0.03);
			}
			else if (cha.get_leben() == 2) {
				herz.draw(cha.get_x() - 30, cha.get_y() - 75, 1, 0.03, 0.03);
				herz.draw(cha.get_x(), cha.get_y() - 75, 1, 0.03, 0.03);
			}
			else if (cha.get_leben() == 1) {
				herz.draw(cha.get_x() - 15, cha.get_y() - 75, 1, 0.03, 0.03);
			}
		}
	}

																// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		if (!startbildschirm) {
			for (int i = 0; i < updates_per_frame; i++) {		//läuft durch die Schleife, damit die Eingaben öfter pro Frame überprüft werden
				if(input().down(Gosu::KB_LEFT)){				// Bewege die Ente nach links
		
					cha.drehen(-(speed_drehen_ente));
					laser.set_schiesst(false);					// sorgt dafür, dass der Laser weggeht, wenn die Ente sich weiterdreht
				}
				if(input().down(Gosu::KB_RIGHT)){				// Bewege die Ente nach rechts												
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
						laser_schuss_ton.play();
					}

					laser.set_schiesst(true);
					laser.set_winkel(cha.get_winkel());
			
					for (int laserspeed = 1000; laserspeed > 0; laserspeed--) {											// Schleife, damtit der Laser nicht durch Ojekte glitcht
						ueberprüfe_kollision_baum_laser(vector_baum, laser);											// überprüft ob der Laser ein Obejrkt (Baum) trifft
						ueberprüfe_kollision_stein_laser(vector_stein, laser);											// überprüft ob der Laser ein Obejrkt (Stein) trifft
						if(!laser.get_ende_erreicht()) {
							for (auto it = vector_fisch.begin(); it < vector_fisch.end(); it++) {
							überprüfe_kollision_character_laser(*it, laser, spieldaten);								// überprüft ob der Laser den Gegner trifft	
							}
						}
						laser.bewegen(Gosu::offset_x(cha.get_winkel(), 1), Gosu::offset_y(cha.get_winkel(), 1));		// Senden des Lasers bis zum Rand
						if(laser.get_ende_erreicht()) {
							break;																						 // wenn der Laser ein Objekt trifft, wird die Schleife abgebrochen
						}
					}
				}

				else if (!input().down(Gosu::KB_SPACE)) {
				laser.set_schiesst(false);
				laser.set_ende_erreicht(false);
				}
				if (spieldaten.get_punkte() >= spieldaten.get_level()) {			// pro 4 Punkte kommt ein neuer Fisch hinzu
					vector_fisch.push_back(Charakter(1000, 200, 0, 5, 35, 35, true, false));
					spieldaten.set_level(spieldaten.get_level() + 4);
				}
				for (auto fisch = vector_fisch.begin(); fisch < vector_fisch.end(); fisch++) {
					if (fisch->get_existiert()) {
						if (counter == 0) {
							ueberprüfe_kollision_stein_character(vector_stein, *fisch);
							ueberprüfe_kollision_baum_character(vector_baum, *fisch);
							int winkel;
							winkel = atan2(cha.get_y() - fisch->get_y(), cha.get_x() - fisch->get_x()) * (180.0 / M_PI);
							winkel = winkel + 90;

							if (winkel < 0) {
								winkel = winkel + 360;
							}
							fisch->set_winkel(winkel);

							if (fisch->get_bewegen()) {
								double speed = 2.0 / updates_per_frame;
								fisch->bewegen_x(Gosu::offset_x(fisch->get_winkel(), speed));
								fisch->bewegen_y(Gosu::offset_y(fisch->get_winkel(), speed));
							}

							ueberprüfe_kollision_character_gegner(*fisch, cha);
						}
						else {
							counter = counter - 1;
						}
				
					}
					else {
						int i = rand() % 8;
						fisch->set_x(startpunkte_gegner.at(i).at(0));
						fisch->set_y(startpunkte_gegner.at(i).at(1));
						fisch->set_existiert(true);
						fisch->set_leben(3);
						counter = 60;
					}
				}
			}
		}
	}
};

						// C++ Hauptprogramm
int main()
{
	ifstream infile("highscore.txt");
	if (infile.is_open()) {
		if (infile >> ges_highscore) {

		}
	}
	

	GameWindow window;
	window.show();

	ofstream outfile("highscore.txt");
	if (outfile.is_open()) { // Überprüft, ob die Datei erfolgreich geöffnet wurde
		outfile << ges_highscore;
		outfile.close(); // Schließt die Datei
	}
}


void ueberprüfe_kollision_character_gegner(Charakter& gegner, Charakter& ente) {
	float xabstand;
	float yabstand;
	float abstand;
	
	xabstand = fabs(ente.get_x() - gegner.get_x());
	yabstand = fabs(ente.get_y() - gegner.get_y());
	abstand = sqrtf(xabstand * xabstand + yabstand * yabstand);
	if (abstand < 70) {
		ente.schaden(1);
		gegner.set_existiert(false);
		gegner.set_x(0);
		gegner.set_y(0);
	}
	
}

void überprüfe_kollision_character_laser(Charakter& gegner, Laser& laser, Spieldaten& spieldaten) {
	if (gegner.get_x() - gegner.get_groesse_x() <= laser.get_x() && laser.get_x() <= gegner.get_x() + gegner.get_groesse_x() &&
		gegner.get_y() - gegner.get_groesse_y() <= laser.get_y() && laser.get_y() <= gegner.get_y() + gegner.get_groesse_y()) {		        // wenn der Laser den Gegner trifft, hört er auf zu schießen
		gegner.set_leben(gegner.get_leben() - 1);
		laser.set_ende_erreicht(true);
		if (gegner.get_leben() <= 0) {
			gegner.set_existiert(false);
			gegner.set_x(0);
			gegner.set_y(0);
			spieldaten.set_punkte(spieldaten.get_punkte() + 1);
		}
	}
}
																						
void draw_bäume(vector<Baum>& vector_baum, Gosu::Image& baum) {
	for (auto it = vector_baum.begin(); it != vector_baum.end(); ++it) {
		baum.draw_rot(it->get_x(), it->get_y(), 0, it->get_winkel(), 0.5, 0.5, scale_Baum, scale_Baum);
	}
}

void ueberprüfe_kollision_baum_laser(vector<Baum>& vector_baum, Laser& laser) {														// Diese Funktion soll überprüfen, ob der Laser einen Baum oder Stein trifft
	for (auto it = vector_baum.begin(); it != vector_baum.end(); ++it) {
		if (it->get_x() - it->get_groesse_x() <= laser.get_x() && laser.get_x() <= it->get_x() + it->get_groesse_x() &&
			it->get_y() - it->get_groesse_y() <= laser.get_y() && laser.get_y() <= it->get_y() + it->get_groesse_y()) {		        // wenn der Laser den Baum trifft, hört er auf zu schießen
			laser.set_ende_erreicht(true);
		}
	}
}

void ueberprüfe_kollision_baum_character(vector<Baum>& vector_baum, Charakter& ente) {
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

void ueberprüfe_kollision_stein_laser(vector<Stein>& vector_stein, Laser& laser) {													// Diese Funktion soll überprüfen, ob der Laser einen Baum oder Stein trifft
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