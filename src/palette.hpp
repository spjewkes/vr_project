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

private:
	void create_palette(void);
	void spread_col(int start_col, int end_col, int palette[][3]);

	int palette[256][3];
};

#endif // __PALETTE_HPP__
