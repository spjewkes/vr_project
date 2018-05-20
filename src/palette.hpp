#ifndef __PALETTE_HPP__
#define __PALETTE_HPP__

class Palette
{
public:
	Palette();
	~Palette();

	int red(int i) const;
	int green(int i) const;
	int blue(int i) const;
	void check_palette(void);

private:
	void create_palette(void);
	void spread_col(int start_col, int end_col, int palette[][3]);
	void set_palette(int palette[][3]);
	void setrgbpalette(int index, int r, int g, int b);

	int palette[256][3];
};

#endif // __PALETTE_HPP__
