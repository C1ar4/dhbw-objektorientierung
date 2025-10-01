#pragma once

class Objekt {
protected:
	float x;
	float y;
	int winkel;

public:
	virtual void bewegen_x(float dx) = 0;
	virtual void bewegen_y(float dy) = 0;
	virtual void drehen(int dwinkel) = 0;
	virtual float get_x() const = 0;
	virtual float get_y() const = 0;
	virtual int get_winkel() const = 0;
	Objekt(float x, float y, int winkel);
};

class Charakter : public Objekt {
private:
		int leben;
public:
		void bewegen_x(float dx) override;
		void bewegen_y(float dy) override;
		void drehen(int dwinkel) override;	
		float get_x() const override;
		float get_y() const override;
		int get_winkel() const override;
		void schaden(int dmg);
		float get_leben() const;
		Charakter(float x, float y, int winkel, int leben);
};

class Laser : public Objekt {
private:
	float start_x;
	float start_y;
	bool schiesst;
	bool ende_erreicht;
public:
	void bewegen_x(float dx) override;
	void bewegen_y(float dy) override;	
	void drehen(int dwinkel) override;
	float get_x() const override;
	float get_y() const override;
	int get_winkel() const override;
	void bewegen(float dx, float dy);
	void set_winkel(float w);
	float get_x_start() const;
	float get_y_start() const;
	void set_x(float x);
	void set_y(float y);
	void set_x_start(float x);
	void set_y_start(float y);
	void set_schiesst(bool sch);
	void set_ende_erreicht(bool ende);
	bool get_schiesst() const;
	bool get_ende_erreicht() const;
	Laser(float x, float y, int winkel, float start_x, float start_y, bool schiesst, bool ende_erreicht);

};