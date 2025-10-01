#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <string>			// hab ich noch eingefügt um den Winkel erstmal auszugeben, kann man dann auch grundsätzlich benutzen text auszugeben :)
#include "Objekte_deklarieren.h"



int x_breite = Gosu::screen_width() - 0;
int y_hoehe = Gosu::screen_height() - 0;
float scale_Ente = 0.1;



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